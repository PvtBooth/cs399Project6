/******************************************************************************/
/*!
	\file   LevelState.h
	\author Evan Kau
	\date   3/11/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "GameState.h"
#include <stdbool.h>
#include "StackState.h"

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

StackState* LevelState_Create();

void LevelState_NextLevel();

void LevelStackState_SetGameplayBool(StackState* below, bool active);

void LevelState_SetPlayerShipID(int playerID, int shipID);

void LevelStackState_CombatText(int value, Vector2 position);

/*----------------------------------------------------------------------------*/

