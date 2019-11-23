/******************************************************************************/
/*!
	\file   AIMarkerComponent.c
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "AIMarkerComponent.h"
#include "TransformComponent.h"
#include <stdlib.h>
#include "AISwarm.h"
#include "AIGraph.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct AIMarker
{
  Entity *targetEntity;
  AIMARKER_TYPE type;

  AISwarm *swarms;
  int swarmCount;

  AIMarker *next;
  AIMarker *prev;
} AIMarker;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt);
static void Destroy(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *AIMarkerComponent_Create(Entity *targetEntity, AIMARKER_TYPE type)
{
  Component *component = Component_Create();
  AIMarker *marker = calloc(1, sizeof(AIMarker));

  marker->targetEntity = targetEntity;
  marker->type = type;

  component->data = marker;
  component->update = &Update;
  component->destroy = &Destroy;
  component->type = COMPONENT_TYPE_AIMARKER;

  return component;
}

void AIMarkerComponent_AddSwarm(AIMarker *marker, AISwarm *swarm)
{
  AISwarm *swarmMarkerNext = AISwarm_GetMarkerNext(swarm);
  AISwarm *swarmMarkerPrev = AISwarm_GetMarkerPrev(swarm);
  
  if (swarmMarkerNext)
  {
    AISwarm_SetMarkerPrev(swarmMarkerNext, swarmMarkerPrev);
  }
  if (swarmMarkerPrev)
  {
    AISwarm_SetMarkerNext(swarmMarkerPrev, swarmMarkerNext);
  }
  else if (AISwarm_GetTargetMarker(swarm))
  {
    AISwarm_GetTargetMarker(swarm)->swarms = swarmMarkerNext;
  }

  AISwarm_SetMarkerNext(swarm, marker->swarms);
  AISwarm_SetMarkerPrev(swarm, NULL);
  if (marker->swarms)
  {
    AISwarm_SetMarkerPrev(marker->swarms, swarm);
  }
  marker->swarms = swarm;
  AISwarm_SetTargetMarker(swarm, marker);
  marker->swarmCount++;
}

void AIMarkerComponent_RemoveSwarm(AIMarker *marker, AISwarm *swarm)
{
  AISwarm *swarmMarkerNext = AISwarm_GetMarkerNext(swarm);
  AISwarm *swarmMarkerPrev = AISwarm_GetMarkerPrev(swarm);

  if (swarmMarkerPrev)
  {
    AISwarm_SetMarkerNext(swarmMarkerPrev, swarmMarkerNext);
  }
  else
  {
    marker->swarms = swarmMarkerNext;
  }
  if (swarmMarkerNext)
  {
    AISwarm_SetMarkerPrev(swarmMarkerNext, swarmMarkerPrev);
  }
  marker->swarmCount--;
}

Entity* AIMarker_GetEntity(AIMarker *marker)
{
  return marker->targetEntity;
}

Vector2 AIMarker_GetPosition(AIMarker *marker)
{
  return TransformComponent_GetPosition(marker->targetEntity->transformComponent);
}

AIMARKER_TYPE AIGraph_GetType(AIMarker *marker)
{
  return marker->type;
}

void AIMarker_SetNext(AIMarker *marker, AIMarker *next)
{
  marker->next = next;
}

void AIMarker_SetPrev(AIMarker *marker, AIMarker *prev)
{
  marker->prev = prev;
}

AIMarker* AIMarker_GetNext(AIMarker *marker)
{
  return marker->next;
}

AIMarker* AIMarker_GetPrev(AIMarker *marker)
{
  return marker->prev;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(dt);
}

static void Destroy(Component *self)
{
  AIMarker *marker = (AIMarker*)self->data;
  if (marker->prev)
  {
    marker->prev->next = marker->next;
  }
  if (marker->next)
  {
    marker->next->prev = marker->prev;
  }
  AIGraph_RemoveMarker(marker);

  AISwarm *iterator = marker->swarms;
  while (iterator)
  {
    AISwarm *next = AISwarm_GetMarkerNext(iterator);
    AISwarm_MarkerLost(iterator);
    iterator = next;
  }

  free(self->data);
  free(self);
}
