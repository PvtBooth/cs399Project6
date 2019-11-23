/******************************************************************************/
/*!
	\file   AbilityComponent.c
	\author Evan Kau
	\date   3/3/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AbilityComponent.h"
#include <stdbool.h>
#include <stdlib.h>
#include "Vector2.h"

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

static void Update(Component *self, float dt);
static void Destroy(Component *self);
static void EventHandle(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AbilityComponent_Create(EVENT_TYPE triggerEvent)
{
  Component *component = Component_Create();
  AbilityData *data = (AbilityData*)calloc(1, sizeof(AbilityData));

  data->triggerEvent = triggerEvent;

  component->data = data;
  component->update = &Update;
  component->destroy = &Destroy;
  component->eventHandle = &EventHandle;

  return component;
}

void AbilityComponent_SetPlayer(Component *self, Player *player)
{
  AbilityData *data = self->data;
  data->player = player;
  Player_AddAbility(player, self);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  AbilityData *data = (AbilityData*)self->data;
  int i;

  for (i = INPUT_TYPE_BUTTON_B; i < INPUT_TYPE_BUTTON_Y + 1; i++)
  {
    InputEvent *inputEvent = InputGatherComponent_GetInput(data->player->inputGatherComponent, i);
    if(!inputEvent)
    {
      return;
    }
    if (i + EVENT_ABILITY_1 == data->triggerEvent)
    {
      if (inputEvent && inputEvent->inputEventType == INPUT_EVENT_TYPE_TRIGGERED)
      {
        if (data->state == ABILITY_STATE_IDLE)
        {
          data->state = ABILITY_STATE_PRIMED;
          data->player->abilityActive = true;
        }
        else if (data->state == ABILITY_STATE_PRIMED || data->state == ABILITY_STATE_ACTIVE) //TODO: Remove/alter? Option, perhaps?
        {
          data->state = ABILITY_STATE_IDLE;
          data->player->abilityActive = false;
        }
      }
    }
    else
    {
      if (inputEvent->inputEventType == INPUT_EVENT_TYPE_TRIGGERED)
      {
        data->state = ABILITY_STATE_IDLE;
      }
    }
  }

  //If this is a default ability,
  if (data->triggerEvent == EVENT_ABILITY_0)
  {
    //If another ability is in use,
    if (data->player->abilityActive)
    {
      data->state = ABILITY_STATE_IDLE;
    }
    //If no other abilities are in use,
    else
    {
      data->state = ABILITY_STATE_PRIMED;
    }
  }
  data->abilityUpdate(self, dt);
}

static void Destroy(Component *self)
{
  AbilityData *data = (AbilityData*)self->data;
  data->destroy(self);
  free(data);
  free(self);
}

static void EventHandle(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(eventType);
    UNREFERENCED_PARAMETER(eventData);
    UNREFERENCED_PARAMETER(isLocal);
  //AbilityData *data = (AbilityData*)self->data;
  //if (data->triggerEvent != EVENT_ABILITY_0)
  //{
  //  INPUT_TYPE inputType = eventType - (EVENT_ABILITY_1 - INPUT_TYPE_BUTTON_B);
  //  if (Event_TypeIsAbility(eventType))
  //  {
  //    if (eventType == data->triggerEvent)
  //    {
  //      if (data->state == ABILITY_STATE_IDLE)
  //      {
  //        data->state = ABILITY_STATE_PRIMED;
  //        data->player->abilityActive = true;
  //      }
  //      else if (data->state == ABILITY_STATE_PRIMED || data->state == ABILITY_STATE_ACTIVE) //TODO: Remove/alter? Option, perhaps?
  //      {
  //        data->state = ABILITY_STATE_IDLE;
  //        data->player->abilityActive = false;
  //      }
  //    }
  //    else
  //    {
  //      data->state = ABILITY_STATE_IDLE;
  //    }
  //    //data->triggered(self, eventType);
  //  }
  //}
}