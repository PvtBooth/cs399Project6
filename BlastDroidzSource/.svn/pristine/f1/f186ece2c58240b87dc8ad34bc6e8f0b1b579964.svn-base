/******************************************************************************/
/*!
	\file   AIForwardShoot.h
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Physics_layers.h"
#include "Component.h"
#include "Vector2.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct AIForwardShootData AIForwardShootData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AIForwardShoot_Create(float rate, float range, float fireAngle, float projectileSpeed, float damage, float force, LayerMask targetMask, Vector2 projectileSize);

void AIForwardShoot_SetEnabled(AIForwardShootData *shooter, bool enabled);
