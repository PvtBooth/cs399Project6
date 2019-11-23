/******************************************************************************/
/*!
	\file   MeshGroup.c
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "MeshGroup.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static MeshGroup *meshGroupHead = NULL;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void AddGroupToList(MeshGroup *group);
static void AddNodeToGroup(MeshGroup *group, MeshNode *node);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/



void MeshGroup_Render(MeshGroup* group, Matrix3x3 *transform)
{
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  AEGfxSetTransform((float(*)[3])transform);
  AEGfxTextureSet(NULL, 0, 0);
  AEGfxSetTransparency(1.0);
  MeshNode *iterator = group->head;
  while (iterator)
  {
    AEGfxSetTintColor(iterator->r, iterator->g, iterator->b, iterator->a);
    AEGfxMeshDraw(iterator->mesh, AE_GFX_MDM_TRIANGLES);
    iterator = iterator->next;
  }
}

/**
* \brief Create a new Empty mesh group
* \return the new mesh group
*/
MeshGroup* MeshGroup_Create()
{
  MeshGroup *group = calloc(1, sizeof(MeshGroup));

  AddGroupToList(group);

  return group;
}

void MeshGroup_AddRect(MeshGroup *group, float width, float height, u32 color)
{

  MeshNode *node = calloc(1, sizeof(MeshNode));


  AEGfxMeshStart();

  // This shape has 2 triangles
  AEGfxTriAdd(
    -width / 2, -height / 2, color, 0.0f, 1.0f,
    width / 2, -height / 2, color, 1.0f, 1.0f,
    -width / 2, height / 2, color, 0.0f, 0.0f);



  AEGfxTriAdd(
    width / 2, -height / 2, color, 1.0f, 1.0f,
    width / 2, height / 2, color, 1.0f, 0.0f,
    -width / 2, height / 2, color, 0.0f, 0.0f);

  node->mesh = AEGfxMeshEnd();

  node->r = 1.0f;
  node->g = 1.0f;
  node->b = 1.0f;
  node->a = 1.0f;

  AddNodeToGroup(group, node);
}

void MeshGroup_AddRectRGBA(MeshGroup* group, float width, float height, float r, float g, float b, float a)
{
  MeshNode *node = calloc(1, sizeof(MeshNode));


  AEGfxMeshStart();

  // This shape has 2 triangles
  AEGfxTriAdd(
    -width / 2, -height / 2, 0xFFFFFFFF, 0.0f, 1.0f,
    width / 2, -height / 2, 0xFFFFFFFF, 1.0f, 1.0f,
    -width / 2, height / 2, 0xFFFFFFFF, 0.0f, 0.0f);



  AEGfxTriAdd(
    width / 2, -height / 2, 0xFFFFFFFF, 1.0f, 1.0f,
    width / 2, height / 2, 0xFFFFFFFF, 1.0f, 0.0f,
    -width / 2, height / 2, 0xFFFFFFFF, 0.0f, 0.0f);

  node->mesh = AEGfxMeshEnd();

  node->r = r;
  node->g = g;
  node->b = b;
  node->a = a;

  AddNodeToGroup(group, node);
}


/**
* \brief Adds a new triangle to a Mesh Group
* \param group the group to add to
* \param x1 x1
* \param y1 y1
* \param x2 x2
* \param y2 y2
* \param x3 x3
* \param y3 y3
* \param color Color of the triangle
*/
void MeshGroup_AddTriangle(MeshGroup *group, float x1, float y1, float x2, float y2, float x3, float y3, u32 color)
{
  MeshNode *node = calloc(1, sizeof(MeshNode));
  AEGfxMeshStart();
  AEGfxTriAdd(
    x1, y1, color, 0.0f, 1.0f,
    x2, y2, color, 1.0f, 1.0f,
    x3, y3, color, 0.0f, 0.0f);
  node->mesh = AEGfxMeshEnd();
  node->r = 1.0f;
  node->g = 1.0f;
  node->b = 1.0f;
  node->a = 1.0f;
  AddNodeToGroup(group, node);

}

void MeshGroup_AddTriangleRGBA(MeshGroup* group, float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a)
{
  MeshNode *node = calloc(1, sizeof(MeshNode));
  AEGfxMeshStart();
  AEGfxTriAdd(
    x1, y1, 0xFFFFFFFF, 0.0f, 1.0f,
    x2, y2, 0xFFFFFFFF, 1.0f, 1.0f,
    x3, y3, 0xFFFFFFFF, 0.0f, 0.0f);
  node->mesh = AEGfxMeshEnd();
  node->r = r;
  node->g = g;
  node->b = b;
  node->a = a;
  AddNodeToGroup(group, node);
}

void MeshGroup_AddMesh(MeshGroup* group, AEGfxVertexList* mesh)
{
  MeshNode *node = calloc(1, sizeof(MeshNode));
  node->mesh = mesh;
  node->r = 1.0f;
  node->g = 1.0f;
  node->b = 1.0f;
  node->a = 1.0f;
  AddNodeToGroup(group, node);
}

void MeshGroup_AddMeshRGBA(MeshGroup* group, AEGfxVertexList* mesh, float r, float g, float b, float a)
{
  MeshNode *node = calloc(1, sizeof(MeshNode));
  node->mesh = mesh;
  node->r = r;
  node->g = g;
  node->b = b;
  node->a = a;
  AddNodeToGroup(group, node);
}

void MeshGroup_SetRGBA(MeshGroup* group, float r, float g, float b, float a)
{
  MeshNode *node = group->head;
  while(node)
  {
    node->r = r;
    node->g = g;
    node->b = b;
    node->a = a;
    node = node->next;
  }
}


/**
* \brief Frees all mesh groups, for use on scene transitions
*/
void MeshGroup_DestroyAll()
{
  MeshGroup *groupIterator = meshGroupHead;
  MeshGroup *prevGroupIterator;
  MeshNode *nodeIterator;
  MeshNode *prevNodeIterator;
  while (groupIterator)
  {
    prevGroupIterator = groupIterator;
    nodeIterator = groupIterator->head;
    while (nodeIterator)
    {
      prevNodeIterator = nodeIterator;
      AEGfxMeshFree(nodeIterator->mesh);
      nodeIterator = nodeIterator->next;
      free(prevNodeIterator);
    }
    groupIterator = groupIterator->next;
    free(prevGroupIterator);
  }
  meshGroupHead = NULL;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void AddGroupToList(MeshGroup *group)
{
  if (meshGroupHead)
  {
    group->next = meshGroupHead;
  }
  meshGroupHead = group;
}
static void AddNodeToGroup(MeshGroup *group, MeshNode *node)
{
  if (group->head)
  {
    node->next = group->head;
  }
  group->head = node;
}

