/******************************************************************************/
/*!
	\file   CollisionDamageComponent.c
	\author Henry Brobeck
	\date   2/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "CollisionDamageComponent.h"
#include <stdlib.h>
#include "CollisionSubject.h"
#include "HealthSubject.h"


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

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt);
static void Destroy(Component * self);
static void OnAttach(Component * self);
static void OnOtherAttach(Component * self, Component * component);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* CollisionDamageComponent_Create(int damage)
{
  Component* newComponent = Component_Create();

  newComponent->update = Update;
  newComponent->destroy = Destroy;
  newComponent->eventHandle = EventHandle;
  newComponent->data = calloc(1, sizeof(int));
  int *data = newComponent->data;
  *data = damage;
  return newComponent;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/


static void Update(Component* self, float dt)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(dt);
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

static void OnAttach(Component* self)
{
    UNREFERENCED_PARAMETER(self);
}

static void OnOtherAttach(Component* self, Component* component)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(component);
}
static void EventHandle(Component *self, int type, void *eventData, bool isLocal)
{
  if (isLocal && type == EVENT_COLLISION)
  {
    CollisionData *data = eventData;
    int *damage = self->data;
    if(data->first == self->parent)
    {
      HealthSubject_DamageNotify(self->parent, data->second, (float)*damage);
    }
    else
    {
      HealthSubject_DamageNotify(self->parent, data->first, (float)*damage);
    }
  }
}