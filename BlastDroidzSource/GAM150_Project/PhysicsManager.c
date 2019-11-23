/******************************************************************************/
/*!
	\file   PhysicsManager.c
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PhysicsManager.h"
#include <stddef.h>
#include "Physics_BroadPhase.h"
#include "Physics_Resolver.h"
#include <string.h>
#include "Physics_Layers.h"
#include <stdlib.h>
#include "Physics_NarrowPhase.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define PHYSICS_CAST_COLLIDER_LIST_INITIAL 64

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

//From ColliderComponent, in order to make the ColliderComponent work in here
typedef struct BoundingCircle
{
  float radius;
  Vector2 offset;
  Vector2 gridMin;
  Vector2 gridMax;
} BoundingCircle;

//From ColliderComponent, in order to make the ColliderComponent work in here
typedef struct PhysicsMaterial
{
  float restitution;
} PhysicsMaterial;

//From ColliderComponent, in order to work with physics casting
typedef struct ColliderComponent
{
  unsigned int id;
  COLLIDER_TYPE type;
  RigidbodyComponent *rigidbody;
  void *collider;
  BoundingCircle boundingCircle;
  TransformComponent *transform;
  unsigned int *collidingWith;
  int collidingWithMax;
  int collidingWithCount;
  PhysicsMaterial physicsMaterial;
  PhysicsLayer layer;
  bool isTrigger;
  bool proxy;

  Component *component;
} ColliderComponent;

//From TransformComponent, in order to work with physics casting
typedef struct TransformComponent
{
  TransformComponent *parent;
  TransformComponent *children;
  TransformComponent *nextBrother;
  TransformComponent *prevBrother;
  Vector2 position;
  Vector2 rotation;
  Vector2 scale;
  Matrix3x3 transformMatrix;
  Matrix3x3 cameraTransformMatrix;
  ColliderComponent *collider;
  bool transformMatrixDirty;
} TransformComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

//Radius of the arena. Used to prevent objects from leaving.
static float arenaRadius;

static RigidbodyComponent *rigidbodyListHead;
static RigidbodyComponent *unusedRigidbodyListHead;
static unsigned int rigidbodyID = 0;

//For box/circle/polygon casts
static ColliderComponent **castColliderList;
static int castColliderCur = 0;
static int castColliderMax = PHYSICS_CAST_COLLIDER_LIST_INITIAL;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void RaycastAgainstGridNode(RaycastData *raycastData, GridNode *node, LayerMask layerMask);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void PhysicsManager_Init()
{
  Physics_Layers_Init();
  Physics_Resolver_Init();
  //Physics_BroadPhase_CreateGrid(2160, 1440, 80, 80);
  //Hank MVP test
  //Physics_BroadPhase_CreateGrid(6000, 6000, 80, 80);
  //END MVP test

  if (castColliderList)
  {
    free(castColliderList);
  }
  castColliderList = (ColliderComponent**)calloc(PHYSICS_CAST_COLLIDER_LIST_INITIAL, sizeof(ColliderComponent*));
  castColliderCur = 0;
  castColliderMax = PHYSICS_CAST_COLLIDER_LIST_INITIAL;
}

/*!
* \brief
*   Creates a new grid. Use this whenever the play area is resized. If a grid
*   already exists, it will have its data freed.
* \param width
*   The width of the new grid.
* \param height
*   The height of the new grid.
* \param xCount
*   The number of nodes along the X Axis.
* \param yCount
*   The number of nodes along the Y Axis.
*/
void PhysicsManager_NewGridFromCount(float width, float height, int xCount, int yCount)
{
  Physics_BroadPhase_CreateGrid(width, height, width / xCount, height / yCount);
  arenaRadius = max(width, height) / 2;
}

/*!
* \brief
*   Creates a new grid. Use this whenever the play area is resized. If a grid
*   already exists, it will have its data freed.
* \param width
*   The width of the new grid.
* \param height
*   The height of the new grid.
* \param nodeWidth
*   The width of each node.
* \param nodeHeight
*   The height of each node.
*/
void PhysicsManager_NewGridFromSize(float width, float height, float nodeWidth, int nodeHeight)
{
  Physics_BroadPhase_CreateGrid(width, height, nodeWidth, (float)nodeHeight);
  arenaRadius = max(width, height) / 2;
}

bool PhysicsManager_GetUnusedRigidbody(Component** component)
{
  //If there are any unused rigidbodies,
  if (unusedRigidbodyListHead)
  {
    //Get the one from the front of the list
    *component = RigidbodyComponent_GetComponent(unusedRigidbodyListHead);
    unusedRigidbodyListHead = RigidbodyComponent_GetNext(unusedRigidbodyListHead);
    return true;
  }
  return false;
}

void PhysicsManager_RegisterRigidbody(RigidbodyComponent *rigidbody)
{
  if (rigidbodyListHead)
  {
    RigidbodyComponent_SetPrev(rigidbodyListHead, rigidbody);
    RigidbodyComponent_SetNext(rigidbody, rigidbodyListHead);
  }
  else
  {
    RigidbodyComponent_SetNext(rigidbody, NULL);
  }
  rigidbodyListHead = rigidbody;
  RigidbodyComponent_SetID(rigidbody, rigidbodyID++);
}

void PhysicsManager_RemoveRigidbody(RigidbodyComponent *rigidbody)
{
  //Detach this rigidbody from the used list
  if (RigidbodyComponent_GetNext(rigidbody))
  {
    RigidbodyComponent_SetPrev(RigidbodyComponent_GetNext(rigidbody), RigidbodyComponent_GetPrev(rigidbody));
  }
  if (RigidbodyComponent_GetPrev(rigidbody))
  {
    RigidbodyComponent_SetNext(RigidbodyComponent_GetPrev(rigidbody), RigidbodyComponent_GetNext(rigidbody));
  }
  if (rigidbody == rigidbodyListHead)
  {
    rigidbodyListHead = RigidbodyComponent_GetNext(rigidbody);
  }

  //Add this rigidbody to the unused list
  if (unusedRigidbodyListHead)
  {
    RigidbodyComponent_SetNext(rigidbody, unusedRigidbodyListHead);
  }
  else
  {
    RigidbodyComponent_SetNext(rigidbody, NULL);
  }
  RigidbodyComponent_SetPrev(rigidbody, NULL);
  unusedRigidbodyListHead = rigidbody;
}

/*!
 * \brief
 *   Runs a Physics update. This will update all rigidbodies, check for any
 *   collisions, and resolve any collisions.
 * \param dt
 *   The delta time.
 */
void PhysicsManager_Update(float dt)
{
  RigidbodyComponent *rigidbody = rigidbodyListHead;

  while (rigidbody)
  {
    RigidbodyComponent_Update(rigidbody, dt);
    rigidbody = RigidbodyComponent_GetNext(rigidbody);
  }

  Physics_BroadPhase_Update();

  Physics_Resolver_Update();
}

/*!
 * \brief
 *   Raycasts against any objects that collide with the default layer. A raycast
 *   checks against all entities with colliders to determine if they intersect a
 *   given line or not.
 * \param raycastData
 *   [OUT] The pointer to a struct that will contain information about the raycast
 *   after the fact.
 * \param origin
 *   The starting point of the raycast.
 * \param direction
 *   The direction the raycast is going in.
 * \param distance
 *   How far the raycast should go.
 * \return
 *   Returns true if the raycast hit anything. Returns false if no objects were hit
 *   within the given distance.
 */
bool PhysicsManager_Raycast(RaycastData *raycastData, Vector2 origin, Vector2 direction, float distance)
{
  return PhysicsManager_RaycastWithMask(raycastData, origin, direction, distance, Physics_Layers_GetLayerMask(PHYSICS_LAYER_DEFAULT));
}

/*!
 * \brief
 *   Raycasts against a given layer. A raycast checks against all entities with
 *   colliders to determine if they intersect a given line or not.
 * \param raycastData
 *   [OUT] The pointer to a struct that will contain information about the raycast
 *   after the fact.
 * \param origin
 *   The starting point of the raycast.
 * \param direction
 *   The direction the raycast is going in.
 * \param distance
 *   How far the raycast should go.
 * \param layerMask
 *   The layer(s) to check against, i.e. if you passed in the enemy layer, it would
 *   check against all enemies.
 * \return
 *   Returns true if the raycast hit anything. Returns false if no objects were hit
 *   within the given distance.
 */
bool PhysicsManager_RaycastWithMask(RaycastData *raycastData, Vector2 origin, Vector2 direction, float distance, LayerMask layerMask)
{
  Vector2 currentPosition = origin;
  GridNode *currentNode = NULL;
  Vector2 normalizedDirection = Vector2_Normalize(direction);

  memset(raycastData, 0, sizeof(RaycastData));

  raycastData->origin = origin;
  raycastData->distance = distance;
  raycastData->direciton = normalizedDirection;
  //Set the point to the end of the ray to start with
  raycastData->point = Vector2_Scale(normalizedDirection, distance);
  raycastData->point = Vector2_Add(raycastData->point, origin);

  //Get the node we're currently in
  currentNode = Physics_BroadPhase_GetNodeAtPoint(currentPosition);
  
  if (currentNode == NULL)
  {
    raycastData->distance = distance;
    return false;
  }
  
  while (raycastData->transform == NULL)
  {
    //Get the node we're currently in
    currentNode = Physics_BroadPhase_GetNodeAtPoint(currentPosition);
    //If we're outside the grid,
    if (currentNode == NULL)
      return false; //We haven't hit anything

    if (Vector2_Distance(currentPosition, origin) > distance + 1) //Too far away
    {
      raycastData->distance = distance;
      return false;
    }

    RaycastAgainstGridNode(raycastData, currentNode, layerMask);
    //Get the position of the next node
    currentPosition = Physics_BroadPhase_GetNextNodePositionFromPoint(currentPosition, direction);
  }
  if (Physics_BroadPhase_GetNodeAtPoint(raycastData->point) != currentNode)
  {
    do
    {
      //Get the node we're currently in
      currentNode = Physics_BroadPhase_GetNodeAtPoint(currentPosition);
      //If we're outside the grid,
      if (currentNode == NULL)
        break; //Return whatever we hit preivously
      //Get the position of the next node
      currentPosition = Physics_BroadPhase_GetNextNodePositionFromPoint(currentPosition, direction);
      RaycastAgainstGridNode(raycastData, currentNode, layerMask);
    } while (Physics_BroadPhase_GetNodeAtPoint(raycastData->point) != currentNode);
  }

  if (raycastData->distance < distance)
  {
    return true;
  }
  else
  {
    raycastData->distance = distance;
    return false;
  }
}

/*!
* \brief
*   Gets all colliders that intersect with a given box. Returns the array, and passes back an int
*   for how many were hit.
* \param origin
*   The center of the box cast.
* \param halfSize
*   Half of the width and height of the box cast.
* \param collidersHit
*   [OUT] The number of colliders that intersect with the box. Will be 0 if the cast starts outside
*   of the grid.
* \return
*   The array of colliders hit by the box cast. Returns NULL if the cast starts outside of the grid.
*/
const ColliderComponent** PhysicsManager_BoxCast(Vector2 origin, Vector2 halfSize, int *collidersHit)
{
  int nodeCount;
  GridNode** searchNodes = Physics_BroadPhase_GetNodeInRadius(origin, max(halfSize.x, halfSize.y), &nodeCount);
  int i, j;

  castColliderCur = 0;

  if (nodeCount == 0)
  {
    *collidersHit = 0;
    return NULL;
  }

  TransformComponent proxyTransform = { 0 };
  ColliderComponent proxyCollider;
  BoxCollider boxCollider;

  ColliderComponent_CreateProxyBox(&proxyCollider, &boxCollider, &proxyTransform, halfSize, Vector2_Zero());

  TransformComponent_SetPosition(&proxyTransform, origin);
  TransformComponent_SetScale(&proxyTransform, ((Vector2){1, 1}));

  for (i = 0; i < nodeCount; i++)
  {
    for (j = 0; j < Physics_BroadPhase_GetGridNodeRigidbodyCount(searchNodes[i]); j++)
    {
      ColliderComponent *collider = RigidbodyComponent_GetCollider(Physics_BroadPhase_GetGridNodeRigidbodies(searchNodes[i])[j]);
      if (Physics_NarrowPhase_CheckCollision(&proxyCollider, collider))
      {
        if (castColliderCur == castColliderMax)
        {
          castColliderList = (ColliderComponent**)realloc(castColliderList, sizeof(ColliderComponent*) * castColliderMax * 2);
          castColliderMax *= 2;
        }
        castColliderList[castColliderCur] = collider;
        castColliderCur++;
      }
    }
    for (j = 0; j < Physics_BroadPhase_GetGridNodeColliderCount(searchNodes[i]); j++)
    {
      ColliderComponent *collider = Physics_BroadPhase_GetGridNodeColliders(searchNodes[i])[j];
      if (Physics_NarrowPhase_CheckCollision(&proxyCollider, collider))
      {
        if (castColliderCur == castColliderMax)
        {
          castColliderList = (ColliderComponent**)realloc(castColliderList, sizeof(ColliderComponent*) * castColliderMax * 2);
          castColliderMax *= 2;
        }
        castColliderList[castColliderCur] = collider;
        castColliderCur++;
      }
    }
  }

  Camera_DrawLine(origin.x - halfSize.x, origin.y - halfSize.y, 1, 1, 0, 1, origin.x - halfSize.x, origin.y + halfSize.y, 1, 1, 0, 1, true);
  Camera_DrawLine(origin.x + halfSize.x, origin.y - halfSize.y, 1, 1, 0, 1, origin.x + halfSize.x, origin.y + halfSize.y, 1, 1, 0, 1, true);
  Camera_DrawLine(origin.x - halfSize.x, origin.y - halfSize.y, 1, 1, 0, 1, origin.x + halfSize.x, origin.y - halfSize.y, 1, 1, 0, 1, true);
  Camera_DrawLine(origin.x - halfSize.x, origin.y + halfSize.y, 1, 1, 0, 1, origin.x + halfSize.x, origin.y + halfSize.y, 1, 1, 0, 1, true);
  //
  //for (i = 0; i < nodeCount; i++)
  //{
  //  Vector2 n = Physics_BroadPhase_GetGridNodeWorldPosition(searchNodes[i]);
  //  Camera_DrawLine(n.x - 40, n.y - 40, 0.25f, 1, 0, 1, n.x - 40, n.y + 40, 0.25f, 1, 0, 1, true);
  //  Camera_DrawLine(n.x + 40, n.y - 40, 0.25f, 1, 0, 1, n.x + 40, n.y + 40, 0.25f, 1, 0, 1, true);
  //  Camera_DrawLine(n.x - 40, n.y - 40, 0.25f, 1, 0, 1, n.x + 40, n.y - 40, 0.25f, 1, 0, 1, true);
  //  Camera_DrawLine(n.x - 40, n.y + 40, 0.25f, 1, 0, 1, n.x + 40, n.y + 40, 0.25f, 1, 0, 1, true);
  //}

  *collidersHit = castColliderCur;
  return castColliderList;
}

/*!
 * \brief
 *   Gets all colliders that intersect with a given circle. Returns the array, and passes back an int
 *   for how many were hit.
 * \param origin
 *   The center of the circle cast.
 * \param radius
 *   The radius of the circle cast.
 * \param collidersHit
 *   [OUT] The number of colliders that intersect with the circle. Will be 0 if the cast starts outside
 *   of the grid.
 * \return
 *   The array of colliders hit by the circle cast. Returns NULL if the cast starts outside of the grid.
 */
const ColliderComponent** PhysicsManager_CircleCast(Vector2 origin, float radius, int *collidersHit)
{
  int nodeCount;
  GridNode** searchNodes = Physics_BroadPhase_GetNodeInRadius(origin, radius, &nodeCount);
  int i, j;

  castColliderCur = 0;

  if (nodeCount == 0)
  {
    *collidersHit = 0;
    return NULL;
  }

  TransformComponent proxyTransform = {0};
  ColliderComponent proxyCollider;
  CircleCollider circleCollider;

  ColliderComponent_CreateProxyCircle(&proxyCollider, &circleCollider, &proxyTransform, radius, Vector2_Zero());

  TransformComponent_SetPosition(&proxyTransform, origin);
  TransformComponent_SetScale(&proxyTransform, ((Vector2){1, 1}));

  for (i = 0; i < nodeCount; i++)
  {
    for (j = 0; j < Physics_BroadPhase_GetGridNodeRigidbodyCount(searchNodes[i]); j++)
    {
      ColliderComponent *collider = RigidbodyComponent_GetCollider(Physics_BroadPhase_GetGridNodeRigidbodies(searchNodes[i])[j]);
      if (Physics_NarrowPhase_CheckCollision(&proxyCollider, collider))
      {
        if (castColliderCur == castColliderMax)
        {
          castColliderList = (ColliderComponent**)realloc(castColliderList, sizeof(ColliderComponent*) * castColliderMax * 2);
          castColliderMax *= 2;
        }
        castColliderList[castColliderCur] = collider;
        castColliderCur++;
      }
    }
    for (j = 0; j < Physics_BroadPhase_GetGridNodeColliderCount(searchNodes[i]); j++)
    {
      ColliderComponent *collider = Physics_BroadPhase_GetGridNodeColliders(searchNodes[i])[j];
      if (Physics_NarrowPhase_CheckCollision(&proxyCollider, collider))
      {
        if (castColliderCur == castColliderMax)
        {
          castColliderList = (ColliderComponent**)realloc(castColliderList, sizeof(ColliderComponent*) * castColliderMax * 2);
          castColliderMax *= 2;
        }
        castColliderList[castColliderCur] = collider;
        castColliderCur++;
      }
    }
  }

  //Camera_DrawLine(origin.x - circleCollider.radius, origin.y - circleCollider.radius, 1, 1, 0, 1, origin.x - circleCollider.radius, origin.y + circleCollider.radius, 1, 1, 0, 1, true);
  //Camera_DrawLine(origin.x + circleCollider.radius, origin.y - circleCollider.radius, 1, 1, 0, 1, origin.x + circleCollider.radius, origin.y + circleCollider.radius, 1, 1, 0, 1, true);
  //Camera_DrawLine(origin.x - circleCollider.radius, origin.y - circleCollider.radius, 1, 1, 0, 1, origin.x + circleCollider.radius, origin.y - circleCollider.radius, 1, 1, 0, 1, true);
  //Camera_DrawLine(origin.x - circleCollider.radius, origin.y + circleCollider.radius, 1, 1, 0, 1, origin.x + circleCollider.radius, origin.y + circleCollider.radius, 1, 1, 0, 1, true);
  //
  //for (i = 0; i < nodeCount; i++)
  //{
  //  Vector2 n = Physics_BroadPhase_GetGridNodeWorldPosition(searchNodes[i]);
  //  Camera_DrawLine(n.x, n.y - 20, 0.25f, 1, 0, 1, n.x, n.y + 20, 0.25f, 1, 0, 1, true);
  //}

  *collidersHit = castColliderCur;
  return castColliderList;
}

/*!
 * \brief
 *   Gets the radius of the arena.
 * \return
 *   The radius of the arena.
 */
float PhysicsManager_GetArenaRadius()
{
  return arenaRadius;
}

void PhysicsManager_Exit()
{
  if (castColliderList)
  {
    free(castColliderList);
  }

  RigidbodyComponent *iterator = rigidbodyListHead;
  while (iterator)
  {
    RigidbodyComponent *temp = iterator;
    iterator = RigidbodyComponent_GetNext(iterator);
    free(RigidbodyComponent_GetComponent(temp));
    free(temp);
  }

  iterator = unusedRigidbodyListHead;
  while (iterator)
  {
    RigidbodyComponent *temp = iterator;
    iterator = RigidbodyComponent_GetNext(iterator);
    free(RigidbodyComponent_GetComponent(temp));
    free(temp);
  }

  Physics_Resolver_Exit();

  Physics_BroadPhase_FreeGrid();
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void RaycastAgainstGridNode(RaycastData *raycastData, GridNode *node, LayerMask layerMask)
{
  RigidbodyComponent **rigidbodies = Physics_BroadPhase_GetGridNodeRigidbodies(node);
  int rigidbodyCount = Physics_BroadPhase_GetGridNodeRigidbodyCount(node);
  ColliderComponent **colliders = Physics_BroadPhase_GetGridNodeColliders(node);
  int colliderCount = Physics_BroadPhase_GetGridNodeColliderCount(node);
  int i;

  //Check against each rigidbody and collider
  for (i = 0; i < rigidbodyCount; i++)
  {
    if (Physics_Layers_CheckLayerMask(layerMask, ColliderComponent_GetLayer(RigidbodyComponent_GetCollider(rigidbodies[i]))))
    {
      ColliderComponent_RaycastAgainstCollider(raycastData, RigidbodyComponent_GetCollider(rigidbodies[i]));
    }
  }
  for (i = 0; i < colliderCount; i++)
  {
    if (Physics_Layers_CheckLayerMask(layerMask, ColliderComponent_GetLayer(colliders[i])))
    {
      ColliderComponent_RaycastAgainstCollider(raycastData, colliders[i]);
    }
  }
}