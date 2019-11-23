/******************************************************************************/
/*!
	\file   AIGraph.c
	\author Evan Kau
	\date   3/5/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AIGraph.h"
#include <stddef.h>
#include "Entity.h"
#include <stdlib.h>

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

AIMarker *markerListHead = NULL;

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void AIGraph_Init(void)
{
  markerListHead = NULL;
}

void AIGraph_RegisterMarker(Entity *entity, AIMARKER_TYPE type)
{
  Component *marker = AIMarkerComponent_Create(entity, type);

  Entity_AttachComponent(entity, marker);

  if (markerListHead)
  {
    AIMarker_SetPrev(markerListHead, marker->data);
  }

  AIMarker_SetNext(marker->data, markerListHead);

  markerListHead = marker->data;
}

void AIGraph_RemoveMarker(AIMarker *marker)
{
  if (marker == markerListHead)
  {
    markerListHead = AIMarker_GetNext(marker);
  }
}

AIMarker* AIGraph_GetMarkerHead()
{
  return markerListHead;
}

AIMarker* AIGraph_GetNearestMarker(Vector2 position, bool notPlayer)
{
  if (markerListHead == NULL)
    return NULL;
  AIMarker *nearestMarker = markerListHead;
  float minDistSqrd = Vector2_DistanceSquared(position, TransformComponent_GetPosition(AIMarker_GetEntity(nearestMarker)->transformComponent));
  AIMarker *iterator = markerListHead;
  while (AIMarker_GetNext(iterator))
  {
    iterator = AIMarker_GetNext(iterator);
    if (AIGraph_GetType(iterator) == AIMARKER_TYPE_PLAYER && notPlayer)
      continue;
    float distSqrd = Vector2_DistanceSquared(position, TransformComponent_GetPosition(AIMarker_GetEntity(iterator)->transformComponent));
    if (distSqrd < minDistSqrd || (AIGraph_GetType(nearestMarker) == AIMARKER_TYPE_PLAYER && notPlayer))
    {
      minDistSqrd = distSqrd;
      nearestMarker = iterator;
    }
  }
  return nearestMarker;
}

void AIGraph_Exit(void)
{

}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
