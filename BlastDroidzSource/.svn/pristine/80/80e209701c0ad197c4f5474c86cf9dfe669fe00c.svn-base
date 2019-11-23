/******************************************************************************/
/*!
	\file   Component.c
	\author Henry Brobeck
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "Component.h"
#include <stdlib.h>
#include <stdbool.h>


/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt);
static void Destroy(Component *self);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static Component* Copy(Component *self);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief Creates a new basic component, with default callbacks
 * \return Pointer to new component
 */
Component* Component_Create()
{
  //Allocate memory for new component
  Component *newComponent = malloc(sizeof(Component));

  //fill new component with default fields
  newComponent->update = Update;
  newComponent->destroy = Destroy;
  newComponent->onAttach = OnAttach;
  newComponent->onOtherAttach = OnOtherAttach;
  newComponent->copy = &Copy; //Hi Hank
  newComponent->eventHandle = EventHandle;
  newComponent->next = NULL;
  newComponent->prev = NULL;
  newComponent->parent = NULL;
  newComponent->data = NULL;

  newComponent->type = COMPONENT_TYPE_DEFAULT;

  return newComponent;
}

/**
 * \brief Frees a component and all its data
 * \param component Component to destroy
 */
void Component_Destroy(Component* component)
{
  component->destroy(component);
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
  free(self->data);
  free(self);
}
static void OnAttach(Component *self)
{
    UNREFERENCED_PARAMETER(self);
}
static void OnOtherAttach(Component *self, Component *component)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(component);
}

static Component* Copy(Component *self)
{
    UNREFERENCED_PARAMETER(self);
  return NULL; //Breakpoint set up because we should never use this function.
}

static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(eventData);
    UNREFERENCED_PARAMETER(isLocal);
}