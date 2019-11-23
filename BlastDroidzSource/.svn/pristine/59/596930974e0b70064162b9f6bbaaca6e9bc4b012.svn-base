/******************************************************************************/
/*!
	\file   MeshGroup.h
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <AEEngine.h>
#include "Matrix3x3.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct MeshNode MeshNode;
typedef struct MeshGroup MeshGroup;
struct MeshNode
{
  //!The Mesh this node represents
  AEGfxVertexList *mesh;

  float r, g, b, a;

  //!The next MeshNode in the MeshGroup Linked List
  MeshNode *next;
};
struct MeshGroup
{
  //!Head of the MeshNode linked list
  MeshNode *head;

  //!MeshGroup thats next in the full inked list
  MeshGroup *next;
};
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void MeshGroup_Render(MeshGroup* group, Matrix3x3 *transform);

MeshGroup *MeshGroup_Create(void);

void MeshGroup_AddRect(MeshGroup *group, float width, float height, u32 color);

void MeshGroup_AddRectRGBA(MeshGroup *group, float width, float height, float r, float g, float b, float a);

void MeshGroup_AddTriangle(MeshGroup *group, float x1, float y1, float x2, float y2, float x3, float y3, u32 color);

void MeshGroup_AddTriangleRGBA(MeshGroup *group, float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a);

void MeshGroup_AddMesh(MeshGroup *group, AEGfxVertexList *mesh);

void MeshGroup_AddMeshRGBA(MeshGroup *group, AEGfxVertexList *mesh, float r, float g, float b, float a);

void MeshGroup_SetRGBA(MeshGroup *group, float r, float g, float b, float a);

void MeshGroup_DestroyAll();

/*----------------------------------------------------------------------------*/


