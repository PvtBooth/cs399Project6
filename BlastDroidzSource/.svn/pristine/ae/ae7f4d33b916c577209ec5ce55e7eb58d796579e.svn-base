/******************************************************************************/
/*!
	\file   CollisionDestroyComponent.c
	\author Henry Brobeck
	\date   1/30/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "CollisionDestroyComponent.h"
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

static void EventHandle(Component *self, EVENT_TYPE type, void *data, bool isLocal);


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* CollisionDestroyComponent_Create(void)
{
	Component* newComponent = Component_Create();
  newComponent->eventHandle = EventHandle;
  return newComponent;
	
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static void EventHandle(Component *self, int type, void *data, bool isLocal)
{
    UNREFERENCED_PARAMETER(data);
  if (isLocal && type == EVENT_COLLISION)
  {
    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
  }
}