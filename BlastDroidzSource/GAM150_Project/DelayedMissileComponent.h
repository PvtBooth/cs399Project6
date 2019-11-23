/******************************************************************************/
/*!
	\file   DelayedMissileComponent.h
	\author Evan Kau
	\date   4/13/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Colors.h"
#include "Vector2.h"
#include "Component.h"
#include "Physics_layers.h"

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

Component* DelayedMissileComponent_Create(float speed, float range, float damage, float force, float radius, Vector2 floatPosition, float floatDelay, float floatRate,
                                          float direction, Color color, LayerMask targetLayer);
