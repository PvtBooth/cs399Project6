/******************************************************************************/
/*!
	\file   InputGatherComponent.c
	\author Evan Kau
	\date   3/11/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "InputGatherComponent.h"
#include "Component.h"
#include "Entity.h"
#include <stdlib.h>
#include "InputEvent.h"
#include "TransformComponent.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct InputGatherData
{
  //InputEvent buttonB;
  //InputEvent buttonA;
  //InputEvent buttonX;
  //InputEvent buttonY;
  //InputEvent buttonLB;
  //InputEvent buttonLT;
  //InputEvent buttonRB;
  //InputEvent buttonRT;
  //InputEvent stickLeft;
  //InputEvent stickRight;
  //InputEvent triggerLeft;
  //InputEvent triggerRight;
  //InputEvent mousePosition;
  //InputEvent mouseLeft;
  //InputEvent mouseRight;
  InputEvent input[INPUT_TYPE_MAX];
} InputGatherData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void EventHandle(Component *self, EVENT_TYPE eventType, void *data, bool isLocal);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* InputGatherComponent_Create()
{
  Component *component = Component_Create();
  InputGatherData *data = (InputGatherData*)calloc(1, sizeof(InputGatherData));

  component->data = data;
  component->eventHandle = &EventHandle;

  return component;
}

InputEvent* InputGatherComponent_GetInput(InputGatherData *data, INPUT_TYPE type)
{
  if(!data)
  {
    return NULL;
  }
  return &data->input[type];
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void EventHandle(Component *self, EVENT_TYPE eventType, void *data, bool isLocal)
{
    UNREFERENCED_PARAMETER(isLocal);
  InputGatherData *gatherData = self->data;
  if (eventType == EVENT_INPUT)
  {
    InputEvent *input = (InputEvent*)data;

    gatherData->input[input->inputType] = *input;

    if (input->inputType == INPUT_TYPE_MOUSEPOSITION)
    {
      gatherData->input[INPUT_TYPE_STICK_R].angle = Vector2_GetSignedAngle(Vector2_Sub(input->direction, TransformComponent_GetPosition(self->parent->transformComponent)), ((Vector2) { 1, 0 })) * -1;
    }
    if (input->inputType == INPUT_TYPE_MOUSE_LEFT)
    {
      gatherData->input[INPUT_TYPE_STICK_R].magnitude = (float)((input->inputEventType == INPUT_EVENT_TYPE_HELD) ? 1 : 0);
    }

    //switch (input->inputType)
    //{
    //case INPUT_TYPE_BUTTON_B:
    //  gatherData->buttonB = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_A:
    //  gatherData->buttonA = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_X:
    //  gatherData->buttonX = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_Y:
    //  gatherData->buttonY = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_LB:
    //  gatherData->buttonLB = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_LT:
    //  gatherData->buttonLT = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_RB:
    //  gatherData->buttonRB = *input;
    //  break;
    //case INPUT_TYPE_BUTTON_RT:
    //  gatherData->buttonRT = *input;
    //  break;
    //case INPUT_TYPE_STICK_L:
    //  gatherData->stickLeft= *input;
    //  break;
    //case INPUT_TYPE_STICK_R:
    //  gatherData->stickRight = *input;
    //  break;
    //case INPUT_TYPE_TRIGGER_L:
    //  gatherData->triggerLeft = *input;
    //  break;
    //case INPUT_TYPE_TRIGGER_R:
    //  gatherData->triggerRight = *input;
    //  break;
    //case INPUT_TYPE_MOUSEPOSITION:
    //  gatherData->mousePosition = *input;
    //  break;
    //case INPUT_TYPE_MOUSE_LEFT:
    //  gatherData->mouseLeft = *input;
    //  break;
    //case INPUT_TYPE_MOUSE_RIGHT:
    //  gatherData->mouseRight = *input;
    //  break;
    //default:
    //  break;
    //}
  }
}