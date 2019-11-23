/******************************************************************************/
/*!
	\file   EnemyTestComponent.h
	\author Evan Kau
	\date   2/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "Component.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct EnemyTestComponent EnemyTestComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* EnemyTestComponent_Create(Vector2 target, float speed, float rotationSpeed);

void EnemyTestComponent_SetTargetPosition(EnemyTestComponent *enemyTestComponent, Vector2 target);
