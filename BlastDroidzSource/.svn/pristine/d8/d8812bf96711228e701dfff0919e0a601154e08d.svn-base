/******************************************************************************/
/*!
	\file   Physics_Layers.h
	\author Evan Kau
	\date   2/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <stdbool.h>

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef unsigned int LayerMask;

typedef enum PhysicsLayer
{
  PHYSICS_LAYER_DEFAULT = 0,
  PHYSICS_LAYER_PLAYER,
  PHYSICS_LAYER_ENEMY,
  PHYSICS_LAYER_NEUTRAL,

  PHYSICS_LAYER_MAX
} PhysicsLayer;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Physics_Layers_Init(void);

bool Physics_Layers_Check(PhysicsLayer layer1, PhysicsLayer layer2);

bool Physics_Layers_CheckLayerMask(LayerMask layerMask, PhysicsLayer layer);

LayerMask Physics_Layers_GetLayerMask(PhysicsLayer layer);

LayerMask Physics_Layers_CreateLayerMask(int collisionCount, ...);