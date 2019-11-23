/******************************************************************************/
/*!
	\file   InputEvent.h
	\author Cole Astaire
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief  
 */
/******************************************************************************/

#pragma once

#include "Vector2.h"
#include <math.h>


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

//enumerator to determine which playerState of action will be taken
typedef enum INPUT_EVENT_TYPE
{
  INPUT_EVENT_TYPE_RELEASED, //not pressed this frame, pressed last frame, works as "disconnected
  INPUT_EVENT_TYPE_TRIGGERED, //pressed this frame, not last, works as "connected
  INPUT_EVENT_TYPE_HELD, //pressed this frame
  //INPUT_EVENT_TYPE_CONTINUOUS, //any analog input, no press, but continuous output

}INPUT_EVENT_TYPE;

//enumerator to determine which part of the controller is being used
typedef enum INPUT_TYPE
{
  //all input methods included
  INPUT_TYPE_BUTTON_B,
  INPUT_TYPE_BUTTON_A,
  INPUT_TYPE_BUTTON_X,
  INPUT_TYPE_BUTTON_Y,
  INPUT_TYPE_BUTTON_RT,
  INPUT_TYPE_BUTTON_LT,
  INPUT_TYPE_BUTTON_RB,
  INPUT_TYPE_BUTTON_LB,
  INPUT_TYPE_BUTTON_START,
  INPUT_TYPE_BUTTON_BACK,
  INPUT_TYPE_STICK_R,
  INPUT_TYPE_STICK_L,
  INPUT_TYPE_TRIGGER_R,
  INPUT_TYPE_TRIGGER_L,
  INPUT_TYPE_DPAD_UP,
  INPUT_TYPE_DPAD_LEFT,
  INPUT_TYPE_DPAD_DOWN,
  INPUT_TYPE_DPAD_RIGHT,
  INPUT_TYPE_MOUSE_LEFT,
  INPUT_TYPE_MOUSE_RIGHT,
  INPUT_TYPE_CONTROLLERSTATE,
  INPUT_TYPE_MOUSEPOSITION,

  //the total number of input types
  INPUT_TYPE_MAX

}INPUT_TYPE;

//TODO: refactor identifier
//input events will hold all of the relevant information for the gamepad in the current frame
typedef struct InputEvent
{
  int playerID; //controller's id from Xinput
  INPUT_TYPE inputType; //what button/input method
  INPUT_EVENT_TYPE inputEventType; //what playerState of event will occur
  float magnitude; //magnitude of analog input
  float angle; //angle, used for sticks
  Vector2 direction; //direction, used for sticks

} InputEvent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void InputEvent_Init(void);

void InputEvent_Update(float dt);

void InputEvent_Exit(void);

void InputEvent_ControllerVibrate(int id, float damage, float duration);

bool InputEvent_ControllerIsConnected(int id);

/*----------------------------------------------------------------------------*/


