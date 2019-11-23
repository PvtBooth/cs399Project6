/******************************************************************************/
/*!
	\file   Physics_BroadPhase.c
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Physics_BroadPhase.h"
#include "RigidbodyComponent.h"
#include <stdlib.h>
#include "Vector2.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include <math.h>
#include "Physics_NarrowPhase.h"
#include <AEEngine.h>
#include "Graphics.h"
#include "Camera.h"
#include "Primitive.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define PHYSICS_GRID_NODE_NUM_INITIAL_POINTERS_TO_CONTENTS 4

#define PHYSICS_GRID_NODE_LIST_INITIAL_POINTERS_TO_NODES 32

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct GridNode
{
  RigidbodyComponent **rigidbodies; //!<Array of pointers to RigidbodyComponents, starting at 20 and doubling whenever its limit is reached
  int rigidbodyContainerSize;   //!<How many RigidbodyCompoent pointers there are allocated to this node
  int rigidbodiesContained; //!<How many RigidbodyComponents overlap this node in any given frame
  ColliderComponent **colliders; //!<Array of pointers to ColliderCompoents, with no rigidbody, starting at 20 and doubling whenever its limit is reached
  int colliderContainerSize;   //!<How many ColliderCompoent pointers there are allocated to this node
  int collidersContained; //!<How many ColliderComponents overlap this node in any given frame
  int debugCount;
  int x, y;
  GridNode *next;
  GridNode *prev;
} GridNode;

typedef struct Grid
{
  int totalNodes;
  int xCount;
  int yCount;
  float nodeWidth;
  float nodeHeight;
  GridNode *nodes;
  GridNode *activeNodesHead;
  int numActiveNodes;
  GridNode** nodeList;
  int nodeListCur;
  int nodeListMax;
} Grid;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static Grid grid;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void AddGridNodeToActiveList(GridNode *node);

static void RemoveGridNodeFromActiveList(GridNode *node);

static GridNode* GetGridNode(int x, int y);

static void WorldToGrid(Vector2 position, int *gridX, int *gridY);

static Vector2 GridToWorld(int gridX, int gridY);

static bool BoundingCircleOverlapsGridNode(ColliderComponent *collider, int nodeX, int nodeY);

static bool RectContains(int minX, int minY, int maxX, int maxY, int x, int y);

static void GridNodeAddRigidbody(GridNode *node, RigidbodyComponent *rigidbody);

static void GridNodeRemoveRigidbody(GridNode *node, RigidbodyComponent *rigidbody);

static void GridNodeAddCollider(GridNode *node, ColliderComponent *collider);

static void GridNodeRemoveCollider(GridNode *node, ColliderComponent *collider);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
void Physics_BroadPhase_CreateGrid(float levelWidth, float levelHeight, float nodeWidth, float nodeHeight)
{
  if (grid.nodes)
  {
    Physics_BroadPhase_FreeGrid();
  }

  int i;
  grid.xCount = (int)(levelWidth / nodeWidth) + 1;
  grid.yCount = (int)(levelHeight / nodeHeight) + 1;
  grid.totalNodes = grid.xCount * grid.yCount;
  grid.nodeWidth = nodeWidth;
  grid.nodeHeight = nodeHeight;

  grid.nodes = (GridNode*)calloc(grid.totalNodes, sizeof(GridNode));
  for (i = 0; i < grid.totalNodes; i++)
  {
    grid.nodes[i].rigidbodies = (RigidbodyComponent**)malloc(sizeof(RigidbodyComponent*) * PHYSICS_GRID_NODE_NUM_INITIAL_POINTERS_TO_CONTENTS);
    grid.nodes[i].colliders  = (ColliderComponent**)malloc(sizeof(ColliderComponent*)  * PHYSICS_GRID_NODE_NUM_INITIAL_POINTERS_TO_CONTENTS);
    grid.nodes[i].rigidbodiesContained = 0;
    grid.nodes[i].collidersContained  = 0;
    grid.nodes[i].rigidbodyContainerSize = PHYSICS_GRID_NODE_NUM_INITIAL_POINTERS_TO_CONTENTS;
    grid.nodes[i].colliderContainerSize  = PHYSICS_GRID_NODE_NUM_INITIAL_POINTERS_TO_CONTENTS;
    grid.nodes[i].debugCount = 0;
    grid.nodes[i].x = i % grid.xCount;
    grid.nodes[i].y = i / grid.xCount;
    grid.nodes[i].next = NULL;
    grid.nodes[i].prev = NULL;
  }
  grid.activeNodesHead = NULL;//(GridNode**)calloc(grid.totalNodes, sizeof(GridNode*));

  grid.nodeList = (GridNode**)calloc(PHYSICS_GRID_NODE_LIST_INITIAL_POINTERS_TO_NODES, sizeof(GridNode*));
  grid.nodeListMax = PHYSICS_GRID_NODE_LIST_INITIAL_POINTERS_TO_NODES;

  grid.numActiveNodes = 0;
}

void Physics_BroadPhase_RegisterRigidbody(RigidbodyComponent *rigidbody)
{
  //TransformComponent *transform = RigidbodyComponent_GetTransform(rigidbody);
  ColliderComponent *collider = RigidbodyComponent_GetCollider(rigidbody);
  Vector2 position = ColliderComponent_GetBoundingCircleWorldPos(collider);
  int gridX = 0, gridY = 0;
  int maxNodeRadius; //How many nodes this rigidbody can overlap in a single direction from the center of itself
  Vector2 *oldMin = ColliderComponent_GetBoundingCircleMin(collider);
  Vector2 *oldMax = ColliderComponent_GetBoundingCircleMax(collider);
  int minX, minY, maxX, maxY;
  int x, y;
  Vector2 minPos;
  minPos.x = position.x - ColliderComponent_GetBoundingCircleRadius(collider);
  minPos.y = position.y - ColliderComponent_GetBoundingCircleRadius(collider);
  Vector2 maxPos;
  maxPos.x = position.x + ColliderComponent_GetBoundingCircleRadius(collider);
  maxPos.y = position.y + ColliderComponent_GetBoundingCircleRadius(collider);

  WorldToGrid(position, &gridX, &gridY);
  WorldToGrid(minPos, &minX, &minY);
  WorldToGrid(maxPos, &maxX, &maxY);
  
  maxNodeRadius = 1 + (int)((ColliderComponent_GetBoundingCircleRadius(collider)
                    - (float)fmod(ColliderComponent_GetBoundingCircleRadius(collider), min(grid.nodeWidth, grid.nodeHeight))) / min(grid.nodeWidth, grid.nodeHeight));

  minX = max(minX, 0);
  maxX = min(maxX + 1, grid.xCount);
  minY = max(minY, 0);
  maxY = min(maxY + 1, grid.yCount);

  for (x = (int)min(minX, oldMin->x); x < max(maxX, oldMax->x); x++)
  {
    for (y = (int)min(minY, oldMin->y); y < max(maxY, oldMax->y); y++)
    {
      if (RectContains(minX, minY, maxX, maxY, x, y))
      {
        if (!RectContains((int)oldMin->x, (int)oldMin->y, (int)oldMax->x, (int)oldMax->y, x, y))
        {
          GridNodeAddRigidbody(GetGridNode(x, y), rigidbody);
        }
      }
      else if (RectContains((int)oldMin->x, (int)oldMin->y, (int)oldMax->x, (int)oldMax->y, x, y))
      {
        GridNodeRemoveRigidbody(GetGridNode(x, y), rigidbody);
      }
    }
  }

  /*for (x = min(min.x, oldMin->x); x < max(min.x, oldMin->x); x++)
  {
    for (y = min(min.y, oldMin->y); y < max(min.y, oldMin->y); y++)
    {
      if (x < min.x)
      {
        
      }
    }
    for (y = min(max.y, oldMax->y); y < max(max.y, oldMax->y); y++)
    {

    }
  }*/

  oldMin->x = (float)minX;
  oldMin->y = (float)minY;
  oldMax->x = (float)maxX;
  oldMax->y = (float)maxY;
}

void Physics_BroadPhase_RemoveRigidbody(RigidbodyComponent *rigidbody)
{
  ColliderComponent *collider = RigidbodyComponent_GetCollider(rigidbody);
  Vector2 *oldMin = ColliderComponent_GetBoundingCircleMin(collider);
  Vector2 *oldMax = ColliderComponent_GetBoundingCircleMax(collider);
  int x, y;
  for (x = (int)oldMin->x; x < oldMax->x; x++)
  {
    for (y = (int)oldMin->y; y < oldMax->y; y++)
    {
      GridNodeRemoveRigidbody(GetGridNode(x, y), rigidbody);
    }
  }
}

void Physics_BroadPhase_RegisterCollider(ColliderComponent *collider)
{
  //TransformComponent *transform = ColliderComponent_GetTransform(collider);
  Vector2 position = ColliderComponent_GetBoundingCircleWorldPos(collider);
  int gridX = 0, gridY = 0;
  int maxNodeRadius; //How many nodes this rigidbody can overlap in a single direction from the center of itself
  Vector2 *oldMin = ColliderComponent_GetBoundingCircleMin(collider);
  Vector2 *oldMax = ColliderComponent_GetBoundingCircleMax(collider);
  int minX, minY, maxX, maxY;
  int x, y;

  float minPosX = position.x - ColliderComponent_GetBoundingCircleRadius(collider);
  float minPosY = position.y - ColliderComponent_GetBoundingCircleRadius(collider);

  float maxPosX = position.x + ColliderComponent_GetBoundingCircleRadius(collider);
  float maxPosY = position.y + ColliderComponent_GetBoundingCircleRadius(collider);
  
  Vector2 minPos;
  minPos.x = minPosX;
  minPos.y = minPosY;

  Vector2 maxPos;
  maxPos.x = maxPosX;
  maxPos.y = maxPosY;

  WorldToGrid(position, &gridX, &gridY);
  WorldToGrid(minPos, &minX, &minY);
  WorldToGrid(maxPos, &maxX, &maxY);

  maxNodeRadius = 1 + (int)((ColliderComponent_GetBoundingCircleRadius(collider)
    - (float)fmod(ColliderComponent_GetBoundingCircleRadius(collider), min(grid.nodeWidth, grid.nodeHeight))) / min(grid.nodeWidth, grid.nodeHeight));

  minX = max(minX, 0);
  maxX = min(maxX + 1, grid.xCount);
  minY = max(minY, 0);
  maxY = min(maxY + 1, grid.yCount);

  for (x = (int)min(minX, oldMin->x); x < max(maxX, oldMax->x); x++)
  {
    for (y = (int)min(minY, oldMin->y); y < max(maxY, oldMax->y); y++)
    {
      if (RectContains(minX, minY, maxX, maxY, x, y))
      {
        if (!RectContains((int)oldMin->x, (int)oldMin->y, (int)oldMax->x, (int)oldMax->y, x, y))
        {
          GridNodeAddCollider(GetGridNode(x, y), collider);
        }
      }
      else if (RectContains((int)oldMin->x, (int)oldMin->y, (int)oldMax->x, (int)oldMax->y, x, y))
      {
        GridNodeRemoveCollider(GetGridNode(x, y), collider);
      }
    }
  }

  /*for (x = min(min.x, oldMin->x); x < max(min.x, oldMin->x); x++)
  {
  for (y = min(min.y, oldMin->y); y < max(min.y, oldMin->y); y++)
  {
  if (x < min.x)
  {

  }
  }
  for (y = min(max.y, oldMax->y); y < max(max.y, oldMax->y); y++)
  {

  }
  }*/

  oldMin->x = (float)minX;
  oldMin->y = (float)minY;
  oldMax->x = (float)maxX;
  oldMax->y = (float)maxY;
}


void Physics_BroadPhase_RemoveCollider(ColliderComponent *collider)
{
  Vector2 *oldMin = ColliderComponent_GetBoundingCircleMin(collider);
  Vector2 *oldMax = ColliderComponent_GetBoundingCircleMax(collider);
  int x, y;
  for (x = (int)oldMin->x; x < oldMax->x; x++)
  {
    for (y = (int)oldMin->y; y < oldMax->y; y++)
    {
      GridNodeRemoveCollider(GetGridNode(x, y), collider);
    }
  }
}

void Physics_BroadPhase_Update()
{
  GridNode *iterator = grid.activeNodesHead;
  int j, k;
  while (iterator)
  {
    for (j = 0; j < iterator->rigidbodiesContained; j++)
    {
      for (k = j + 1; k < iterator->rigidbodiesContained; k++)
      {
        Physics_NarrowPhase_CheckCollision(RigidbodyComponent_GetCollider(iterator->rigidbodies[j]), RigidbodyComponent_GetCollider(iterator->rigidbodies[k]));
      }
      for (k = 0; k < iterator->collidersContained; k++)
      {
        Physics_NarrowPhase_CheckCollision(RigidbodyComponent_GetCollider(iterator->rigidbodies[j]), iterator->colliders[k]);
      }
    }
    //grid.activeNodes[i]->rigidbodiesContained = 0;
    //grid.activeNodes[i]->collidersContained = 0;
    iterator = iterator->next;
  }
  //grid.numActiveNodes = 0;
}

Vector2 Physics_BroadPhase_GetNearestPointWithinGrid(Vector2 point)
{
  if (point.x < -grid.nodeWidth * (grid.xCount / 2.0f))
    point.x = -grid.nodeWidth * (grid.xCount / 2.0f);
  else if (point.x > grid.nodeWidth * (grid.xCount / 2.0f))
    point.x = grid.nodeWidth * (grid.xCount / 2.0f);
  if (point.y < -grid.nodeHeight * (grid.yCount / 2.0f))
    point.y = -grid.nodeHeight * (grid.yCount / 2.0f);
  else if (point.y > grid.nodeHeight * (grid.yCount / 2.0f))
    point.y = grid.nodeHeight * (grid.yCount / 2.0f);
  return point;
}

GridNode* Physics_BroadPhase_GetNodeAtPoint(Vector2 position)
{
  GridNode* node;
  int x, y;
  WorldToGrid(position, &x, &y);
  if (x < 0 || x >= grid.xCount || y < 0 || y >= grid.yCount)
    return NULL;
  node = GetGridNode(x, y);
  node->debugCount++;
  return node;
}

/*!
 * \brief Will modify position to be in the next grid node.
 * \param position
 *   The position that the ray is currently at, which will be changed in the function.
 * \param direction
 *   The direction of the ray.
 * \return Void.
 */
Vector2 Physics_BroadPhase_GetNextNodePositionFromPoint(Vector2 position, Vector2 direction)
{
  int x, y;
  Vector2 currentNodePos;
  float nextX;
  float nextY;
  float distX;
  float distY;
  Vector2 normalizedDirection = Vector2_Normalize(direction);

  WorldToGrid(position, &x, &y);

  currentNodePos = GridToWorld(x, y);
  if (normalizedDirection.x > 0)
    nextX = currentNodePos.x + grid.nodeWidth / 2;
  else
    nextX = currentNodePos.x - grid.nodeWidth / 2;
  if (normalizedDirection.y > 0)
    nextY = currentNodePos.y + grid.nodeHeight / 2;
  else
    nextY = currentNodePos.y - grid.nodeHeight / 2;

  distX = (float)fabs((nextX - position.x) / normalizedDirection.x) + 0.1f;
  distY = (float)fabs((nextY - position.y) / normalizedDirection.y) + 0.1f;

  if (distX < distY)
  {
    position = Vector2_AddFloats(position, distX * normalizedDirection.x, distX * normalizedDirection.y);
  }
  else if (distY < distX)
  {
    position = Vector2_AddFloats(position, distY * normalizedDirection.x, distY * normalizedDirection.y);
  }
  else
  {
    position = Vector2_AddFloats(position, distX * normalizedDirection.x, distY * normalizedDirection.y);
  }

  return position;
}

GridNode* Physics_BroadPhase_GetGridNodeFromNode(GridNode *node, int xOffset, int yOffset)
{
  return GetGridNode(node->x + xOffset, node->y + yOffset);
}

GridNode** Physics_BroadPhase_GetNodeInRadius(Vector2 position, float radius, int *nodeCount)
{
  int centerX, centerY;
  WorldToGrid(position, &centerX, &centerY);
  GridNode *centerNode = GetGridNode(centerX, centerY);
  if (!centerNode)
  {
    *nodeCount = 0;
    return NULL;
  }
  grid.nodeListCur = 0;
  int halfNodesWide = (int)(radius / grid.nodeWidth + 1);
  int halfNodesTall = (int)(radius / grid.nodeHeight + 1);
  int x, y;

  for (x = -halfNodesWide; x <= halfNodesWide; x++)
  {
    for (y = -halfNodesTall; y <= halfNodesTall; y++)
    {
      GridNode *node = Physics_BroadPhase_GetGridNodeFromNode(centerNode, x, y);
      if (node)
      {
        if (grid.nodeListCur == grid.nodeListMax)
        {
          grid.nodeList = (GridNode**)realloc(grid.nodeList, sizeof(GridNode*) * grid.nodeListMax * 2);
          grid.nodeListMax *= 2;
        }
        grid.nodeList[grid.nodeListCur] = node;
        grid.nodeListCur++;
      }
    }
  }

  *nodeCount = grid.nodeListCur;
  return grid.nodeList;
}

RigidbodyComponent** Physics_BroadPhase_GetGridNodeRigidbodies(GridNode *node)
{
  return node->rigidbodies;
}

int Physics_BroadPhase_GetGridNodeRigidbodyCount(GridNode *node)
{
  return node->rigidbodiesContained;
}

ColliderComponent** Physics_BroadPhase_GetGridNodeColliders(GridNode *node)
{
  return node->colliders;
}

int Physics_BroadPhase_GetGridNodeColliderCount(GridNode *node)
{
  return node->collidersContained;
}

Vector2 Physics_BroadPhase_GetGridNodeWorldPosition(GridNode *node)
{
  return GridToWorld(node->x, node->y);
}

void Physics_BroadPhase_SanitizeGrid()
{
  GridNode *iterator = grid.activeNodesHead;
  int i;
  while (iterator)
  {
    for (i = 0; i < iterator->rigidbodiesContained; i++)
    {
      ColliderComponent *collider = RigidbodyComponent_GetCollider(iterator->rigidbodies[i]);
      Vector2 *value = ColliderComponent_GetBoundingCircleMin(collider);
      *value = Vector2_Zero();
      value = ColliderComponent_GetBoundingCircleMax(collider);
      *value = Vector2_Zero();
    }
    for (i = 0; i < iterator->collidersContained; i++)
    {
      Vector2 *value = ColliderComponent_GetBoundingCircleMin(iterator->colliders[i]);
      *value = Vector2_Zero();
      value = ColliderComponent_GetBoundingCircleMax(iterator->colliders[i]);
      *value = Vector2_Zero();
    }
    iterator->collidersContained = 0;
    iterator->rigidbodiesContained = 0;
    RemoveGridNodeFromActiveList(iterator);
    iterator = grid.activeNodesHead;
  }
}

void Physics_BroadPhase_FreeGrid()
{
  int i;
  GridNode *node = grid.nodes;
  for (i = 0; i < grid.totalNodes; i++)
  {
    free(node->rigidbodies);
    free(node->colliders);
    node++;
  }
  grid.totalNodes = 0;
  free(grid.nodeList);
  grid.nodeList = NULL;
  //free(grid.activeNodes);
  free(grid.nodes);
  grid.nodes = NULL;
}

void Physics_BroadPhase_Debug()
{
  Vector2 cameraPos = Camera_GetPosition();

  float scaleX = Camera_GetRenderScaleFactor();
  float scaleY = Camera_GetRenderScaleFactor();

  Vector2 scale;
  scale.x = scaleX;
  scale.y = scaleY;

  int x, y;
  Primitive *p = Primitive_CreateRect(grid.nodeWidth, grid.nodeHeight, 0XFFFFFF00);
  AEGfxSetPosition(0, 0);
  AEGfxSetTintColor(1, 1, 1, 1);
  for (x = -1; x < grid.xCount; x++)
  {
    Vector2 start = GridToWorld(x, -1);
    Vector2 end = GridToWorld(x, grid.yCount);
    start = Vector2_AddFloats(start, grid.nodeWidth / 2, grid.nodeHeight / 2);
    end = Vector2_AddFloats(end, grid.nodeWidth / 2, -grid.nodeHeight / 2);
    Camera_DrawLine(start.x, start.y, 1, 1, 0, 1, end.x, end.y, 1, 0, 0, 1, true);
  }
  for (y = -1; y < grid.yCount; y++)
  {
    Vector2 start = GridToWorld(-1, y);
    Vector2 end = GridToWorld(grid.xCount, y);
    start = Vector2_AddFloats(start, grid.nodeWidth / 2, grid.nodeHeight / 2);
    end = Vector2_AddFloats(end, -grid.nodeWidth / 2, grid.nodeHeight / 2);
    Camera_DrawLine(start.x, start.y, 1, 1, 0, 1, end.x, end.y, 1, 0, 0, 1, true);
  }
  for (x = 0; x < grid.xCount; x++)
  {
    for (y = 0; y < grid.yCount; y++)
    {
      int contained = GetGridNode(x, y)->rigidbodiesContained + GetGridNode(x, y)->collidersContained;
      if (contained > 0)
      {
        Vector2 pos = GridToWorld(x, y);
        Matrix3x3 m;
        pos = Vector2_Sub(pos, cameraPos);
        pos.x *= scale.x;
        pos.y *= scale.y;
        m = Matrix3x3_BuildTransform(pos, 0, scale);
        //Primitive_Render(p, &pos);
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency((float)contained / GetGridNode(x, y)->rigidbodyContainerSize / 2.0f + 0.15f);
        AEGfxSetTintColor((float)contained / GetGridNode(x, y)->rigidbodyContainerSize, 1 - (float)contained / GetGridNode(x, y)->rigidbodyContainerSize, 0, 1);
        //AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
        // Set poisition for object 1
        //AEGfxSetPosition(obj1X, obj1Y);
        AEGfxSetTransform((float(*)[3])(&m));
        // No texture for object 1
        AEGfxTextureSet(NULL, 0, 0);
        // Drawing the mesh (list of triangles)
        //AEGfxSetTransparency(1.0);

        AEGfxMeshDraw(p->mesh, AE_GFX_MDM_TRIANGLES);
        GetGridNode(x, y)->debugCount = 0;
      }
    }
  }
  //AEGfxSetBlendMode(AE_GFX_BM_NONE);
  AEGfxSetTintColor(1, 1, 1, 1);
  AEGfxMeshFree(p->mesh);
  free(p);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void AddGridNodeToActiveList(GridNode *node)
{
  if (grid.activeNodesHead != NULL)
  {
    node->next = grid.activeNodesHead;
    grid.activeNodesHead->prev = node;
  }
  else
  {
    node->next = NULL;
  }
  node->prev = NULL;
  grid.activeNodesHead = node;
}

static void RemoveGridNodeFromActiveList(GridNode *node)
{
  if (grid.activeNodesHead == node)
  {
    grid.activeNodesHead = node->next;
  }
  if (node->next)
  {
    node->next->prev = node->prev;
  }
  if (node->prev)
  {
    node->prev->next = node->next;
  }
}

static GridNode* GetGridNode(int x, int y)
{
  //If out of bounds,
  if (x < 0 || x >= grid.xCount || y < 0 || y >= grid.yCount)
  {
    return NULL;
  }
  return &grid.nodes[x + y * grid.xCount];
}

static void WorldToGrid(Vector2 position, int *gridX, int *gridY)
{
  Vector2 gridPos = position;
  //Move by half the grid size
  gridPos.x += (float)(grid.xCount - 1) * grid.nodeWidth  / 2;
  gridPos.y += (float)(grid.yCount - 1) * grid.nodeHeight / 2;
  *gridX = (int)((gridPos.x + grid.nodeWidth / 2) / grid.nodeWidth);
  *gridY = (int)((gridPos.y + grid.nodeWidth / 2) / grid.nodeHeight);
}

static Vector2 GridToWorld(int gridX, int gridY)
{
  Vector2 result;

  result.x = gridX * grid.nodeWidth;
  result.y = gridY * grid.nodeHeight;
  //Move back by half of the grid size
  result.x -= (float)(grid.xCount - 1) * grid.nodeWidth / 2;
  result.y -= (float)(grid.yCount - 1) * grid.nodeHeight / 2;

  return result;
}

static bool BoundingCircleOverlapsGridNode(ColliderComponent *collider, int nodeX, int nodeY)
{
  bool result = false;
  Vector2 gridNodePosition = GridToWorld(nodeX, nodeY);
  Vector2 boundingCirclePosition = ColliderComponent_GetBoundingCircleWorldPos(collider);
  Vector2 closestCirclePoint = Vector2_Sub(gridNodePosition, boundingCirclePosition);
  closestCirclePoint = Vector2_Normalize(closestCirclePoint);
  closestCirclePoint = Vector2_Scale(closestCirclePoint, min(ColliderComponent_GetBoundingCircleRadius(collider), Vector2_Distance(boundingCirclePosition, gridNodePosition)));
  
  if (closestCirclePoint.x + boundingCirclePosition.x > gridNodePosition.x - grid.nodeWidth / 2 && closestCirclePoint.x + boundingCirclePosition.x < gridNodePosition.x + grid.nodeWidth / 2)
  {
    if (closestCirclePoint.y + boundingCirclePosition.y > gridNodePosition.y - grid.nodeHeight / 2 && closestCirclePoint.y + boundingCirclePosition.y < gridNodePosition.y + grid.nodeHeight / 2)
    {
      result = true;
    }
  }
  return result;
}

static bool RectContains(int minX, int minY, int maxX, int maxY, int x, int y)
{
  if (x >= minX && x < maxX)
  {
    if (y >= minY && y < maxY)
    {
      return true;
    }
  }
  return false;
}

static void GridNodeAddRigidbody(GridNode *node, RigidbodyComponent *rigidbody)
{
  //If this node is full,
  if (node->rigidbodiesContained == node->rigidbodyContainerSize)
  {
    //Allocate double the previous space
    node->rigidbodies = (RigidbodyComponent**)realloc(node->rigidbodies, sizeof(RigidbodyComponent*) * node->rigidbodyContainerSize * 2);
    node->rigidbodyContainerSize *= 2;
  }
  node->rigidbodies[node->rigidbodiesContained] = rigidbody;
  node->rigidbodiesContained++;

  if (node->rigidbodiesContained + node->collidersContained == 1)
  {
    AddGridNodeToActiveList(node);
    grid.numActiveNodes++;
  }
  //node->debugCount = node->rigidbodiesContained + node->collidersContained;
}

static void GridNodeRemoveRigidbody(GridNode *node, RigidbodyComponent *rigidbody)
{
  int i;
  //Find the rigidbody in the array
  for (i = 0; i < node->rigidbodiesContained; i++)
  {
    if (RigidbodyComponent_GetID(node->rigidbodies[i]) == RigidbodyComponent_GetID(rigidbody))
    {
      break;
    }
  }
  //For the remaineder of the elements of the array, scoot the over left by one
  for (; i < node->rigidbodiesContained - 1; i++)
  {
    node->rigidbodies[i] = node->rigidbodies[i + 1];
  }
  node->rigidbodiesContained--;

  if (node->rigidbodiesContained + node->collidersContained == 0)
  {
    RemoveGridNodeFromActiveList(node);
    grid.numActiveNodes--;
  }
  //node->debugCount = node->rigidbodiesContained + node->collidersContained;
}

static void GridNodeAddCollider(GridNode *node, ColliderComponent *collider)
{
  //If this node is full,
  if (node->collidersContained == node->colliderContainerSize)
  {
    //Allocate double the previous space
    node->colliders = (ColliderComponent**)realloc(node->colliders, sizeof(ColliderComponent*) * node->colliderContainerSize * 2);
    node->colliderContainerSize *= 2;
  }
  node->colliders[node->collidersContained] = collider;
  node->collidersContained++;

  if (node->rigidbodiesContained + node->collidersContained == 1)
  {
    AddGridNodeToActiveList(node);
    grid.numActiveNodes++;
  }
  //node->debugCount = node->rigidbodiesContained + node->collidersContained;
}

static void GridNodeRemoveCollider(GridNode *node, ColliderComponent *collider)
{
  int i;
  //Find the rigidbody in the array
  for (i = 0; i < node->collidersContained; i++)
  {
    if (ColliderComponent_GetID(node->colliders[i]) == ColliderComponent_GetID(collider))
    {
      break;
    }
  }
  //For the remaineder of the elements of the array, scoot the over left by one
  for (; i < node->collidersContained - 1; i++)
  {
    node->colliders[i] = node->colliders[i + 1];
  }
  node->collidersContained--;

  if (node->rigidbodiesContained + node->collidersContained == 0)
  {
    RemoveGridNodeFromActiveList(node);
    grid.numActiveNodes--;
  }
  //node->debugCount = node->rigidbodiesContained + node->collidersContained;
}