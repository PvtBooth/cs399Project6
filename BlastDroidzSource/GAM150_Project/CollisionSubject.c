/******************************************************************************/
/*!
	\file   CollisionSubject.c
	\author Henry Brobeck
	\date   1/30/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "CollisionSubject.h"
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


/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/


void CollisionSubject_Notify(Entity* first, Entity* second, Vector2 position, Vector2 normal, float penetration)
{
  CollisionData data;
  data.position = position;
  data.normal = normal;
  data.penetration = penetration;
  data.first = first;
  data.second = second;

  Entity_LocalEvent(first, EVENT_COLLISION, &data);
  Entity_LocalEvent(second, EVENT_COLLISION, &data);

}

CollisionData* CollisionSubject_GetData(void* collisionData)
{
  return (CollisionData*)collisionData;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
