/******************************************************************************/
/*!
	\file   AISwarmAgent.h
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Vector2.h"

typedef enum AISWARM_STATE AISWARM_STATE;

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct AISwarmAgent AISwarmAgent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AISwarmAgent_Create(float authority, float sightDistance);

void AISwarmAgent_SetToTargetVector(AISwarmAgent *agent, Vector2 toTargetVector);

void AISwarmAgent_SetToCenterVector(AISwarmAgent *agent, Vector2 toCenterVector);

void AISwarmAgent_AddToAgentRepulsionVector(AISwarmAgent *agent, Vector2 repulsionVector);

void AISwarmAgent_AddToObjectRepulsionVector(AISwarmAgent *agent, Vector2 repulsionVector);

void AISwarmAgent_SetDistanceToTarget(AISwarmAgent *agent, float distance);

Vector2 AISwarmAgent_GetToTargetVector(AISwarmAgent *agent);

Vector2 AISwarmAgent_GetToCenterVector(AISwarmAgent *agent);

Vector2 AISwarmAgent_GetAgentRepulsionVector(AISwarmAgent *agent);

Vector2 AISwarmAgent_GetObjectRepulsionVector(AISwarmAgent *agent);

float AISwarmAgent_GetDistanceToTarget(AISwarmAgent *agent);

void AISwarmAgent_Clear(AISwarmAgent *agent);

Vector2 AISwarmAgent_GetPosition(AISwarmAgent *agent);

Vector2 AISwarmAgent_GetVelocity(AISwarmAgent *agent);

AISWARM_STATE AISwarmAgent_GetState(AISwarmAgent *agent);

void AISwarmAgent_SetNext(AISwarmAgent *agent, AISwarmAgent *next);

void AISwarmAgent_SetPrev(AISwarmAgent *agent, AISwarmAgent *prev);

AISwarmAgent* AISwarmAgent_GetNext(AISwarmAgent *agent);

AISwarmAgent* AISwarmAgent_GetPrev(AISwarmAgent *agent);

Entity* AISwarmAgent_GetEntity(AISwarmAgent *agent);

void AISwarmAgent_Debug(AISwarmAgent *agent);