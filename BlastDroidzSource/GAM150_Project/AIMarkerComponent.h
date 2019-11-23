/******************************************************************************/
/*!
	\file   AIMarkerComponent.h
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Entity.h"
#include "AISwarm.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum AIMARKER_TYPE
{
  AIMARKER_TYPE_POINT,
  AIMARKER_TYPE_STATION,
  AIMARKER_TYPE_PLAYER
} AIMARKER_TYPE;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct AISwarm AISwarm;

typedef struct AIMarker AIMarker;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *AIMarkerComponent_Create(Entity *targetEntity, AIMARKER_TYPE type);

void AIMarkerComponent_AddSwarm(AIMarker *marker, AISwarm *swarm);

void AIMarkerComponent_RemoveSwarm(AIMarker *marker, AISwarm *swarm);

Entity* AIMarker_GetEntity(AIMarker *marker);

Vector2 AIMarker_GetPosition(AIMarker *marker);

AIMARKER_TYPE AIGraph_GetType(AIMarker *marker);

void AIMarker_SetNext(AIMarker *marker, AIMarker *next);

void AIMarker_SetPrev(AIMarker *marker, AIMarker *prev);

AIMarker* AIMarker_GetNext(AIMarker *marker);

AIMarker* AIMarker_GetPrev(AIMarker *marker);
