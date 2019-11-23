/******************************************************************************/
/*!
	\file   AISwarm.h
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "AISwarmAgent.h"
#include "AIMarkerComponent.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct AIMarker AIMarker;
typedef struct AISwarm AISwarm;

typedef enum AISWARM_STATE
{
  AISWARM_STATE_IDLE = 0,
  AISWARM_STATE_MOVE,
  AISWARM_STATE_ATTACK
} AISWARM_STATE;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void AISwarm_Init(void);

AISwarm* AISwarm_Create(float targetAttraction, float centerAttraction, float agentRepulsion, float agentRepulsionFalloff,
  float objectRepulsion, float objectRepulsionFalloff, float objectPadding /*, float matchVelocities, float matchVelocitiesFalloff*/);

void AISwarm_Update(AISwarm *swarm, float dt);

void AISwarm_AddAgent(AISwarm *swarm, AISwarmAgent *agent);

void AISwarm_RemoveAgent(AISwarm *swarm, AISwarmAgent *agent);

void AISwarm_DetectPlayer(AISwarm *swarm, int playerSeen);

void AISwarm_MarkerLost(AISwarm *swarm);

Vector2 AISwarm_GetCenter(AISwarm *swarm);

float AISwarm_GetRadius(AISwarm *swarm);

void AISwarm_SetNext(AISwarm *swarm, AISwarm *next);

void AISwarm_SetPrev(AISwarm *swarm, AISwarm *prev);

AISwarm* AISwarm_GetNext(AISwarm *swarm);

AISwarm* AISwarm_GetPrev(AISwarm *swarm);

void AISwarm_SetMarkerNext(AISwarm *swarm, AISwarm *next);

void AISwarm_SetMarkerPrev(AISwarm *swarm, AISwarm *prev);

AISwarm* AISwarm_GetMarkerNext(AISwarm *swarm);

AISwarm* AISwarm_GetMarkerPrev(AISwarm *swarm);

void AISwarm_SetTargetMarker(AISwarm *swarm, AIMarker *marker);

AIMarker* AISwarm_GetTargetMarker(AISwarm *swarm);

AISWARM_STATE AISwarm_GetState(AISwarm *swarm);

void AISwarm_Free(AISwarm *swarm);

void AISwarm_Exit();

void AISwarm_Debug(AISwarm *swarm);