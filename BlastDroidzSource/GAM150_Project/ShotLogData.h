/******************************************************************************/
/*!
	\file   ShotLogData.h
	\author Evan Kau
	\date   3/12/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Entity.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct ShotLogData {
  Entity sourceEntity;
  bool isPlayer;
  //int typeID; //Type of ship, type of enemy
  COMPONENT_TYPE weaponType;
  bool initialized;
} ShotLogData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void ShotLogData_Init(ShotLogData *data, Entity *sourceEntity, bool isPlayer, COMPONENT_TYPE weaponType);

void ShotLogData_LogDamageDealt(ShotLogData *data, float damage, Vector2 position, Entity *hitEntity);
