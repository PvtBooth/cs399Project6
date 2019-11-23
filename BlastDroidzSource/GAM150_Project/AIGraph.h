/******************************************************************************/
/*!
	\file   AIGraph.h
	\author Evan Kau
	\date   3/5/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "AIMarkerComponent.h"

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

void AIGraph_Init(void);

//void AIGraph_Update(float dt);

void AIGraph_RegisterMarker(Entity *entity, AIMARKER_TYPE type);

void AIGraph_RemoveMarker(AIMarker *marker);

AIMarker* AIGraph_GetMarkerHead();

AIMarker* AIGraph_GetNearestMarker(Vector2 position, bool notPlayer);

void AIGraph_Exit(void);
