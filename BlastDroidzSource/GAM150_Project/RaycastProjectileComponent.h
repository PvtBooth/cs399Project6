/******************************************************************************/
/*!
	\file   RaycastProjectileComponent.h
	\author Evan Kau
	\date   3/2/2017
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

typedef struct RaycastProjectileData RaycastProjectileData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* RaycastProjectileComponent_Create(float speed, float maxDistance, float damage, float force, LayerMask targetLayer);

Component* RaycastProjectileComponent_Copy(RaycastProjectileData *data);

void RaycastProjectileComponent_SetShotLog(RaycastProjectileData *data, ShotLogData *logTemplate);
