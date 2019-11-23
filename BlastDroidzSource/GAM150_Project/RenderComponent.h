/******************************************************************************/
/*!
	\file   RenderComponent.h
	\author Henry Brobeck
	\date   1/21/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include "Graphics.h"
#include "Primitive.h"
#include "Sprite.h"
#include "GraphicsManager.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef enum RENDER_BEHAVIOR_TYPE
{
  RENDER_BEHAVIOR_PARALAX,
  RENDER_BEHAVIOR_GROUP,
  RENDER_BEHAVIOR_SPRITE,
  RENDER_BEHAVIOR_SPRITESHEET
}RENDER_BEHAVIOR;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *RenderComponent_CreatePrimitive(Primitive *primitive);

Component* RenderComponent_CreateMeshGroup(MeshGroup* group, RENDER_LAYER renderLayer);

Component *RenderComponent_CreateSprite(Sprite *sprite, RENDER_BEHAVIOR behavior, RENDER_LAYER renderLayer);

Component *RenderComponent_SetParalax(Component *component, Vector2 paralax);

void RenderComponent_UnRegister(Component *component);

/*----------------------------------------------------------------------------*/


