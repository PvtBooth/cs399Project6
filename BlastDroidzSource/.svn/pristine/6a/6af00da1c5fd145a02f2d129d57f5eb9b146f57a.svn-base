/******************************************************************************/
/*!
	\file   Primitive.h
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <AEEngine.h>
#include "TransformComponent.h"
typedef struct Primitive Primitive;
struct Primitive
{
  //!Mesh representing a primitve
  AEGfxVertexList *mesh;

  Primitive *next;

};


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Primitive *Primitive_CreateRect(float width, float height, u32 color);

void Primitive_Destroy(Primitive *primitive);

void Primitive_Render(Primitive *primitive, Vector2 position);

void Primitive_RenderTransform(Primitive *primitive, TransformComponent *transform);

/*----------------------------------------------------------------------------*/


