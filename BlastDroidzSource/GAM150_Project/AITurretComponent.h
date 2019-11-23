/******************************************************************************/
/*!
	\file   AITurretComponent.h
	\author Evan Kau
	\date   4/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "Physics_layers.h"
#include "Component.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct AITurretData AITurretData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AITurret_Create(Vector2 positionOffset, float rate, float randomDelay, float range, float fireSpread, float projectileSpeed,
  float damage, float force, LayerMask checkMask, Vector2 projectileSize);

void AITurret_SetEnabled(AITurretData *shooter, bool enabled);
