/******************************************************************************/
/*!
	\file   AIManager.h
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "AISwarm.h"

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

void AIManager_Init(void);

void AIManager_Update(float dt);

void AIManager_AddSwarm(AISwarm *swarm);

void AIManager_RemoveSwarm(AISwarm *swarm);

AISwarm* AIManager_GetNearestSwarm(Vector2 position);

void AIManager_Exit(void);

void AIManager_Clean(void);

void AIManager_Debug(void);

void AIManager_Render(void);