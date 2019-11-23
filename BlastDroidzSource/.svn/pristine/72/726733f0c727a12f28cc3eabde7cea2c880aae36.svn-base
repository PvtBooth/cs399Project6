/******************************************************************************/
/*!
	\file   InputEvent.c
	\author Cole Astaire
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma warning(disable: 4244)

#include "InputEvent.h"
#include <Windows.h>
#include <Xinput.h>
#include "GameStateStack.h"
#include <AEEngine.h>
#include "Camera.h"

#define LEFT_STICK_DEAD_ZONE 0.40f
#define RIGHT_STICK_DEAD_ZONE 0.40f
#define LEFT_TRIGGER_DEAD_ZONE 0.20f
#define RIGHT_TRIGGER_DEAD_ZONE 0.20f
#define KEYBOARD_TO_LEFTSTICK_NORMALIZED_MAGNITUDE 0.85f
#define PLAYER_1_ID 0


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct InputData
{
  INPUT_EVENT_TYPE playerState[4];
 
} InputData;

typedef struct VIBRATE {
    XINPUT_VIBRATION controllerVibrate;
    float duration;
} VIBRATE;


/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

//array of input data up to the total number of possible input types
static InputData lastFrameData[INPUT_TYPE_MAX];
static VIBRATE playerVibrates[INPUT_TYPE_MAX];

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void InputEvent_GetLeftStick(int id, XINPUT_STATE state);
static void InputEvent_GetWASD();
static void InputEvent_GetQERFS();
static void InputEvent_GetRightStick(int id, XINPUT_STATE state);
static void InputEvent_GetMouse();
static void InputEvent_GetLeftTrigger(int id, XINPUT_STATE state);
static void InputEvent_GetRightTrigger(int id, XINPUT_STATE state);
static void InputEvent_GetBAXY(int id, XINPUT_STATE state);
static void InputEvent_Get1234();
static void InputEvent_GetBumpers(int id, XINPUT_STATE state);
static void InputEvent_GetStartBack(int id, XINPUT_STATE state);
static void InputEvent_SendEvent(int playerID, INPUT_TYPE inputType, INPUT_EVENT_TYPE eventType, float magnitude, float angle, Vector2 direction);
static void InputEvent_GetDPad(int id, XINPUT_STATE state);
static void InputEvent_Vibrate(int id, VIBRATE* state, float dt);

/*------------------------------------------------------------------------------
// Private Helper Function Declarations:
//----------------------------------------------------------------------------*/


static bool wButton_IsPressed(XINPUT_STATE state, int xinput_gamepad_define);
static bool WASD_isPressed();
static Vector2 WASD_GetDirection();

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

//TODO: replace all gamepad buton conditionals with the new ispressed fuction
//TODO: decmopose the conditionals that determine which input event type is to be sent

/*!
 \brief
 */
void InputEvent_Init(void)
{
  //for each input data, set each playerState to released
  for(int i = 0; i < INPUT_TYPE_MAX;  ++i)
  {
    for(int j = 0; j < 4; ++j)
    {
      lastFrameData[i].playerState[j] = INPUT_EVENT_TYPE_RELEASED;
      playerVibrates[i].controllerVibrate.wLeftMotorSpeed = 0;
      playerVibrates[i].controllerVibrate.wRightMotorSpeed = 0;
      playerVibrates[i].duration = 0.0f;

    }
  }
}

/*!
 \brief this function updates input
 \param	dt Change in time (in seconds) since last game loop.
 */
void InputEvent_Update(float dt)
{
  UNREFERENCED_PARAMETER(dt);

  //number of controllers connected
  int controllerCount = 0;

  //new DWORD, which is defined in Xinput to store controller booleans
  DWORD dwResult;

  //mouse input function, outside of the controller checks, run every frame regardless.
  InputEvent_GetMouse();

  //for each pad up to the max number of pads
  for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
  {

    //new gamepad state
    XINPUT_STATE state;

    //zero out the memory required to store state
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    //get the state of the controller from XInput.
    dwResult = XInputGetState(i, &state);

    if (dwResult == ERROR_SUCCESS)
    {
      //controller is connected

      //increment the count
      ++controllerCount;

      //if controller ws not plugged in last frame, send input event
      if(lastFrameData[INPUT_TYPE_CONTROLLERSTATE].playerState[i] == INPUT_EVENT_TYPE_RELEASED)
      {
        //telling the game taht the controller is plugged in
        InputEvent_SendEvent(i, INPUT_TYPE_CONTROLLERSTATE, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
      }

      //send a held event, signifying that te controller is still plugged in
      InputEvent_SendEvent(i, INPUT_TYPE_CONTROLLERSTATE, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());

      InputEvent_GetLeftStick(i, state);
      InputEvent_GetRightStick(i, state);
      InputEvent_GetLeftTrigger(i, state);
      InputEvent_GetRightTrigger(i, state);
      InputEvent_GetStartBack(i, state);
      InputEvent_GetBAXY(i, state);
      InputEvent_GetBumpers(i, state);
      InputEvent_GetDPad(i, state);
      InputEvent_Vibrate(i, &playerVibrates[i], dt);
    }
    else
    {
      //controller is not connected

      //if controller ws not plugged in last frame, send input event
      if (lastFrameData[INPUT_TYPE_CONTROLLERSTATE].playerState[i] == INPUT_EVENT_TYPE_HELD)
      {
        //telling the game that the controller is not  plugged in
        InputEvent_SendEvent(i, INPUT_TYPE_CONTROLLERSTATE, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
      }
    }

  }

  //if no controllers are connected, get keyboard input
  if (controllerCount == 0)
  {
    //mouse input functions, outside of the controller checks, run every frame regardless.
    //InputEvent_Get1234();
    InputEvent_GetWASD();
    InputEvent_GetQERFS();
  }
}

/*!
 \brief
 */
void InputEvent_Exit(void)
{
}

void InputEvent_ControllerVibrate(int id, float damage, float duration)
{
    playerVibrates[id].controllerVibrate.wLeftMotorSpeed = 65535.0f * (1.0f / damage);
    playerVibrates[id].controllerVibrate.wRightMotorSpeed = 65535.0f * (1.0f / damage);
    playerVibrates[id].duration = duration;
}

/*!
\brief This function determines if the controller of the given id is connected.
*/
bool InputEvent_ControllerIsConnected(int id)
{
  //if controller state is released, return false
  if(lastFrameData[INPUT_TYPE_CONTROLLERSTATE].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
  {
    return false;
  }

  return true;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

/*!
\brief This functin checks input from the left anamlog stick, then sends the appropriate input events
*/
static void InputEvent_GetLeftStick(int id, XINPUT_STATE state)
{
  //get relevant values from the state of the pad
  float LX = state.Gamepad.sThumbLX;
  float LY = state.Gamepad.sThumbLY;

  //find magnitude
  float magnitude = sqrtf(LX*LX + LY*LY);

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  float normalizedMagnitude = magnitude / 32767.0f;

  //find current angle
  float curAngle = atan2f(LY, LX);

  //find direction
  Vector2 normalizedDirection;
  normalizedDirection.x = LX / 32767.0f;
  normalizedDirection.y = LY / 32767.0f;

  //check if the magnitude is outside o the deadzone
  if(normalizedMagnitude > LEFT_STICK_DEAD_ZONE)
  {
    if (lastFrameData[INPUT_TYPE_STICK_L].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_STICK_L, INPUT_EVENT_TYPE_TRIGGERED, normalizedMagnitude, curAngle, normalizedDirection);
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_STICK_L, INPUT_EVENT_TYPE_HELD, normalizedMagnitude, curAngle, normalizedDirection);
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_STICK_L].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_STICK_L, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief This functin checks input from the WASD Buttons, then sends the appropriate input events
       as if it were the gamepad's left analog stick
*/
static void InputEvent_GetWASD()
{
  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  float normalizedMagnitude = KEYBOARD_TO_LEFTSTICK_NORMALIZED_MAGNITUDE;

  //find the normalized direcction using the vector pointer returned from get drection
  Vector2 normalizedDirection = Vector2_Normalize(WASD_GetDirection());

  //find current angle using a based dummy
  Vector2 dummy = { 1,0 };
  float curAngle = Vector2_GetSignedAngle(normalizedDirection, dummy);

  //check if any of the keys are pressed
  if (WASD_isPressed())
  {
    if (lastFrameData[INPUT_TYPE_STICK_L].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_STICK_L, INPUT_EVENT_TYPE_TRIGGERED, normalizedMagnitude, curAngle, normalizedDirection);
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_STICK_L, INPUT_EVENT_TYPE_HELD, normalizedMagnitude, curAngle, normalizedDirection);
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_STICK_L].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_STICK_L, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief This functin checks input from the right anamlog stick, then sends the appropriate input events
*/
static void InputEvent_GetQERFS()
{
  //check Q, send the proper events
  if (AEInputCheckCurr('E')) //E and Q are swapped because that's what we wanted
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_A].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_A].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_A].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check R, send the proper events
  if (AEInputCheckCurr('R'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_B].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_B].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_B].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check E, and send the proper events
  if (AEInputCheckCurr('Q'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_X].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_X].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_X].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check f, and send the proper events
  if (AEInputCheckCurr('F'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_RB].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_RB, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_RB, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_RB].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_RB, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check left shift, and send the proper events
  if (AEInputCheckCurr(VK_LSHIFT))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief This functin checks input from the right anamlog stick, then sends the appropriate input events
*/
static void InputEvent_GetRightStick(int id, XINPUT_STATE state)
{
  //get relevant values from the state of the pad
  float RX = state.Gamepad.sThumbRX;
  float RY = state.Gamepad.sThumbRY;

  //find magnitude
  float magnitude = sqrtf(RX*RX + RY*RY);

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  float normalizedMagnitude = magnitude / 32767.0f;

  //find current angle
  float curAngle = atan2f(RY, RX);

  //find direction
  Vector2 normalizedDirection;
  normalizedDirection.x = RX / 32767.0f;
  normalizedDirection.y = RY / 32767.0f;

  //check if the magnitude is outside o the deadzone
  if (normalizedMagnitude > LEFT_STICK_DEAD_ZONE)
  {
    //if the stick was released last frame, send a triggered event
    if (lastFrameData[INPUT_TYPE_STICK_R].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_STICK_R, INPUT_EVENT_TYPE_TRIGGERED, normalizedMagnitude, curAngle, normalizedDirection);
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_STICK_R, INPUT_EVENT_TYPE_HELD, normalizedMagnitude, curAngle, normalizedDirection);
    }
  }
  else //otherwise, assuming the magnitude is indside the deadzone
  {
    //if the stick was held last frame, send a released event
    if (lastFrameData[INPUT_TYPE_STICK_R].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_STICK_R, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief This function checks input from the mouse, and sends events containing its position and its keypresses
*/
static void InputEvent_GetMouse()
{
  //get the position in world space of the mouse
  Vector2 mouseWorldPosition = Camera_MouseToWorld();

  //send the mouse position as an input event
  InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSEPOSITION, INPUT_EVENT_TYPE_HELD, 0, 0, mouseWorldPosition);

  //if the left mouse button is pressed, as opposed to the right analog sticck being outside of the deadzone
  if (AEInputCheckCurr(VK_LBUTTON))
  {
    //if the stick was released last frame, send a triggered event
    if (lastFrameData[INPUT_TYPE_MOUSE_LEFT].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSE_LEFT, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSE_LEFT, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else //otherwise, assuming the magnitude is indside the deadzone
  {
    //if the stick was held last frame, send a released event
    if (lastFrameData[INPUT_TYPE_MOUSE_LEFT].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSE_LEFT, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //if the left mouse button is pressed, as opposed to the right analog sticck being outside of the deadzone
  if (AEInputCheckCurr(VK_RBUTTON))
  {
    //if the stick was released last frame, send a triggered event
    if (lastFrameData[INPUT_TYPE_MOUSE_RIGHT].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSE_RIGHT, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSE_RIGHT, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else //otherwise, assuming the magnitude is indside the deadzone
  {
    //if the stick was held last frame, send a released event
    if (lastFrameData[INPUT_TYPE_MOUSE_RIGHT].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_MOUSE_RIGHT, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief this function determines the current magnitude of the left trigger and sends and event
*/
static void InputEvent_GetLeftTrigger(int id, XINPUT_STATE state)
{
  //get relevant values from the state of the pad
  float magnitude = state.Gamepad.bLeftTrigger;

  //normalized magnitude
  float normalizedMagnitude;

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  normalizedMagnitude = magnitude / 256;

  //check if the magnitude is outside of the deadzone
  if (normalizedMagnitude > LEFT_TRIGGER_DEAD_ZONE)
  {
    // if last frame the left trigger was released, send a triggered event
    if (lastFrameData[INPUT_TYPE_BUTTON_LT].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_LT, INPUT_EVENT_TYPE_TRIGGERED, normalizedMagnitude, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_LT, INPUT_EVENT_TYPE_HELD, normalizedMagnitude, 0, Vector2_Zero());
    }
  }
  else //otherwise, assuming the trigger is inside the deadzone
  {
    //if last frame, the trigger was held, send a released event
    if (lastFrameData[INPUT_TYPE_BUTTON_LT].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_LT, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief this function determines the current magnitude of the right trigger and sends an event
*/
static void InputEvent_GetRightTrigger(int id, XINPUT_STATE state)
{
  //get relevant values from the state of the pad
  float magnitude = state.Gamepad.bRightTrigger;

  //normalized magnitude
  float normalizedMagnitude;

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  normalizedMagnitude = magnitude / 256;

  //check if the magnitude is outside of the deadzone
  if (normalizedMagnitude > RIGHT_TRIGGER_DEAD_ZONE)
  {
    //if last frame the right trigger was released, send a triggered event
    if (lastFrameData[INPUT_TYPE_BUTTON_RT].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_RT, INPUT_EVENT_TYPE_TRIGGERED, normalizedMagnitude, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_RT, INPUT_EVENT_TYPE_HELD, normalizedMagnitude, 0, Vector2_Zero());
    }
  }
  else //otherwise, assuming that trigger is inside the deadzone
  {
    //if last frame the trigger was held, send a released event
    if (lastFrameData[INPUT_TYPE_BUTTON_RT].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_RT, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief this function checks for input from gamepad keys BAXY, then sends the appropriate input events
*/
static void InputEvent_GetBAXY(int id, XINPUT_STATE state)
{
  //check B, send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0)
  {

    if (lastFrameData[INPUT_TYPE_BUTTON_B].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_B].playerState[id] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_B].playerState[id] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check A, send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_A].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_A].playerState[id] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_A].playerState[id] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
  //check X, and send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_X].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_X].playerState[id] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_X].playerState[id] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
  //check Y, and send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[id] == INPUT_EVENT_TYPE_HELD || lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[id] == INPUT_EVENT_TYPE_TRIGGERED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief this function checks for input from the proper keys from keyboard controls, then sends input events as if 
       the corresponding gamepad key was pressed
*/
static void InputEvent_Get1234()
{
  //check 1, send the proper events as if B was pressed
  if (AEInputCheckCurr('1'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_B].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {

    if (lastFrameData[INPUT_TYPE_BUTTON_B].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_B, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check 2, send the proper events as if A was pressed
  if (AEInputCheckCurr('2'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_A].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_A].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_A, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check 3, and send the proper events as if x was pressed
  if (AEInputCheckCurr('3'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_X].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_X].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_X, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check 4, and send the proper events as if Y was pressed
  if (AEInputCheckCurr('4'))
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_Y].playerState[PLAYER_1_ID] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(PLAYER_1_ID, INPUT_TYPE_BUTTON_Y, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief this functions creates an input event and sends it down the state stack given input type
*/
static void InputEvent_GetBumpers(int id, XINPUT_STATE state)
{
  //check LB, send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_LB].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_LB, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_LB, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_LB].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_LB, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check RB, send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_RB].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_RB, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_RB, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_RB].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_RB, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

static void InputEvent_GetDPad(int id, XINPUT_STATE state)
{
  //check 1, send the proper events as if B was pressed
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
  {
    if (lastFrameData[INPUT_TYPE_DPAD_UP].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_UP, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_UP, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {

    if (lastFrameData[INPUT_TYPE_DPAD_UP].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_UP, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check 2, send the proper events as if A was pressed
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
  {
    if (lastFrameData[INPUT_TYPE_DPAD_LEFT].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_LEFT, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_LEFT, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_DPAD_LEFT].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_LEFT, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check 3, and send the proper events as if x was pressed
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
  {
    if (lastFrameData[INPUT_TYPE_DPAD_DOWN].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_DOWN, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_DOWN, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_DPAD_DOWN].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_DOWN, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check 4, and send the proper events as if Y was pressed
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
  {
    if (lastFrameData[INPUT_TYPE_DPAD_RIGHT].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_RIGHT, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_RIGHT, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_DPAD_RIGHT].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_DPAD_RIGHT, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}

/*!
\brief this functions sends input events for the start and back buttons of the given gamepad
*/
static void InputEvent_GetStartBack(int id, XINPUT_STATE state)
{
  //check start, send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_START].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_START, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_START, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_START].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_START, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }

  //check back, send the proper events
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0)
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_BACK].playerState[id] == INPUT_EVENT_TYPE_RELEASED)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_BACK, INPUT_EVENT_TYPE_TRIGGERED, 0, 0, Vector2_Zero());
    }
    else
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_BACK, INPUT_EVENT_TYPE_HELD, 0, 0, Vector2_Zero());
    }
  }
  else
  {
    if (lastFrameData[INPUT_TYPE_BUTTON_BACK].playerState[id] == INPUT_EVENT_TYPE_HELD)
    {
      InputEvent_SendEvent(id, INPUT_TYPE_BUTTON_BACK, INPUT_EVENT_TYPE_RELEASED, 0, 0, Vector2_Zero());
    }
  }
}


/*!
\brief this functions creates an input event and sends it down the state stack given input type
*/
static void InputEvent_SendEvent(int playerID, INPUT_TYPE inputType, INPUT_EVENT_TYPE eventType, float magnitude, float angle, Vector2 direction)
{
  //new input event
  InputEvent inputEvent;

  //assign parameters
  inputEvent.inputType = inputType;
  inputEvent.inputEventType = eventType;
  inputEvent.playerID = playerID;
  inputEvent.magnitude = magnitude;
  inputEvent.angle = angle;
  inputEvent.direction = direction;

  //pass the input event to the state stack
  GameStateStack_PassInputEvent(&inputEvent);

  // assign held input event type
  lastFrameData[inputType].playerState[playerID] = eventType;
}

/*------------------------------------------------------------------------------
// Private Helper Functions:
//----------------------------------------------------------------------------*/

/*!
\brief this function determines whether a Xinput "wButton" is pressed, given the xInputMacro.
       The macro syntas begins as follows: XINPUT_GAMEPAD_<BUTTONTITLE>
*/
static bool wButton_IsPressed(XINPUT_STATE state, int xinput_gamepad_define)
{
  if ((state.Gamepad.wButtons & xinput_gamepad_define) != 0)
  {
    return true;
  }

  return false;
}


/*!
\brief this function checks for wasd input
*/
static bool WASD_isPressed()
{
  if (AEInputCheckCurr('W') || AEInputCheckCurr('A') || AEInputCheckCurr('S') || AEInputCheckCurr('D'))
  {
    return true;
  }

  return false;
}

/*!
\brief this function determines the current direction, considering the wasd directions
*/
static Vector2 WASD_GetDirection()
{
  //start with a zeroed vector
  Vector2 direction = { 0,0 };

  //if w is pressed, add 1 to the y value
  if(AEInputCheckCurr('W'))
  {
    --direction.y;
  }

  //if a is pressed, subtract 1 from the x value
  if(AEInputCheckCurr('A'))
  {
    --direction.x;
  }

  //if s is pressed, subtract 1 from the y value
  if(AEInputCheckCurr('S'))
  {
    ++direction.y;
  }

  //if d is pressed, add 1 to the x value
  if(AEInputCheckCurr('D'))
  {
    ++direction.x;
  }

  return direction;
}

//this function causes the controller to vibrate
static void InputEvent_Vibrate(int id, VIBRATE* state, float dt)
{
    XInputSetState(id, &(state->controllerVibrate));

    if(playerVibrates[id].duration > 0.0f)
        playerVibrates[id].duration -= dt;

    if (playerVibrates[id].duration < 0.0f)
    {
        playerVibrates[id].duration = 0.0f;

        playerVibrates[id].controllerVibrate.wLeftMotorSpeed = 0;
        playerVibrates[id].controllerVibrate.wRightMotorSpeed = 0;
    }
}
