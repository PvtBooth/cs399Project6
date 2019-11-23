/******************************************************************************/
/*!
	\file   BombProjectileComponent.h
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Physics_Layers.h"
#include "Component.h"
#include "ShotLogData.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct BombProjectileData BombProjectileData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* BombProjectileComponent_Create(float speed, float timer, LayerMask targetLayer);

Component* BombProjectileComponent_Copy(BombProjectileData *data);

void BombProjectileComponent_SetShotLog(BombProjectileData *data, ShotLogData *logTemplate);
