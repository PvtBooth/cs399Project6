/******************************************************************************/
/*!
	\file   Input.c
	\author Cole Astaire
	\date   1/30/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief  This file converts all of the information about the current xbox
          controllers into useable data. This file also includes functions
          which convert keyboard input into pad variables. 
          All functions in this file must begin with "Input_".
 */
/******************************************************************************/

#include "Input.h"
#include <Windows.h>
#include <Xinput.h>
#include <math.h>
#include <AEEngine.h>
#include "RigidbodyComponent.h"
#include "Player.h"
#include "GameState.h"
#include "ColeTestState2TheReStatening.h"
#include "Camera.h"
#include "AbilitySubject.h"

#define PAD_B 1
#define PAD_A 2
#define PAD_X 3
#define PAD_Y 4

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

// TODO: place callback function pointers here, not defined, they are just variables
// initially set to null

static void(*LthumbAngleCallback)(int id, float angle, float magnitude) = NULL;
static void(*LthumbMagnitudeCallback)(int id, float normalizedMagnitude) = NULL;
static void(*RthumbAngleCallback)(int id, float angle) = NULL;
static void(*RthumbMagnitudeCallback)(int id, float normalizedMagnitude) = NULL;
static void(*BAXYCallback)(int id, int button) = NULL;
static void(*RTriggerMagnitudeCallback)(int id, float normalizedPressure) = NULL;
static void(*LTriggerMagnitudeCallback)(int id, float normalizedPressure) = NULL;
static void(*RBumperCallback)(int id, bool state) = NULL;

//TODO: migrate this functionality
static void Temp_Keyboard_Force_Propulsion(Player *player);
static void Temp_Keyboard_Force_PropulsionBoost(Player *player);
static void Temp_Keyboard_Force_PropulsionStrafe(Player *player, float direction);

//controller flag callbacks
static void(*OnControllerConnectCallback)(int id);
static void(*OnControllerDisconnectCallback)(int id);

static bool triggered = false;
static float time = 0;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Input_Lthumb_Angle(int id, XINPUT_STATE state);
static void Input_Lthumb_Magnitude(int id, XINPUT_STATE state);
static void Input_Rthumb_Angle(int id, XINPUT_STATE state);
static void Input_Rthumb_Magnitude(int id, XINPUT_STATE state);
static void Input_BAXY_Press(int id, XINPUT_STATE state);
static void Input_Rtrigger_Magnitude(int id, XINPUT_STATE state);
static void Input_Ltrigger_Magnitutde(int id, XINPUT_STATE state);
static void Input_RBumper(int id, XINPUT_STATE state);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/


/*======================POTENTIAL FUNTION LIST====================
 * Lthumb_Angle
 * Lthumb_Magnitude
 * Rthumb_Angle
 * Rthumb_Magnitude
 * BAXY_Press
 * Rtrigger_Pressure
 * Ltrigger_Pressure
 * Rbumper_Press
 * Lbumber_Press
 * Start_Press
 * Back_Press
 */

/*!
 \brief This function initializes input.
 */
void Input_Init(void)
{
}

/*!
 \brief This function gets controller states, and checks for input at every frame.
 \param	dt Change in time (in seconds) since last game loop.
 */
void Input_Update(float dt)
{
  //check state, bail if in any state other than play state
  if(GameState_GetCurrent() != ColeTestState2_GetState())
  {
    return;
  }

  //new DWORD, which is defined in Xinput to store controller booleans
  DWORD dwResult;

  //for each pad up to the max number of pads
  for (DWORD i = 0; i< XUSER_MAX_COUNT; i++)
  {
    //new gamepad state
    XINPUT_STATE state;

    //zero out the memory required to store state
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    //get the state of the controller from XInput.
    dwResult = XInputGetState(i, &state);

    //if the controller is connected
    if (dwResult == ERROR_SUCCESS)
    {

      //
      //if(Player_GetPlayer(i) == NULL)
      //{
      //  if (i == 1)
      //  {
      //    Player_Create("Player2", i + 1, 0xFFFFFF00, Player_GetPosition(0),);
      //  }
      //
      //  if (i == 2)
      //  {
      //    Player_Create("Player3", i + 1, 0XFFFF69B4, Player_GetPosition(0));
      //  }
      //
      //  if (i == 3)
      //  {
      //    Player_Create("Player4", i + 1, 0xFF89CFF0, Player_GetPosition(0));
      //  }
      //}

      //check for all input and call necessary controls
      Input_Lthumb_Angle(i, state);
      Input_Lthumb_Magnitude(i, state);
      Input_Rthumb_Angle(i, state);
      Input_Rthumb_Magnitude(i, state);
      Input_BAXY_Press(i, state);
      Input_Rtrigger_Magnitude(i, state);
      Input_Ltrigger_Magnitutde(i, state);
      Input_RBumper(i, state);
    }
    else
    {
      // Controller is not connected

      //keyboard input, for player 1, index 0
      if (i == 0)
      {
        Player *player = Player_GetPlayer(i);
        //Vector2 direction = { 0, 0 }; //Start with the assumption that we aren't changing direction

        Vector2 cameraPosition = Camera_MouseToWorld();

        Vector2 newDirection;
        newDirection.x = -(TransformComponent_GetPosition(player->transformComponent).x) + cameraPosition.x;
        newDirection.y = -(TransformComponent_GetPosition(player->transformComponent).y) + cameraPosition.y;

        Vector2 normal = { 0 };

        normal = Vector2_Normalize(newDirection);

        //float rotation = atan2f(normal.y, normal.x);


        if (AEInputCheckCurr('W'))
        {
          Temp_Keyboard_Force_Propulsion(player);
        }
        if (AEInputCheckCurr('A'))
        {

        }
        if (AEInputCheckCurr('D'))
        {

        }
        //direction = Vector2_Normalize(direction);
        if (player != NULL)
        {
          RigidbodyComponent_PIDLoopToPoint(player->rigidbodyComponent, Vector2_Add(normal, TransformComponent_GetPosition(player->transformComponent)), 100, 20);
        }

        if (AEInputCheckCurr(VK_LBUTTON))
        {
          //shoot
          //Component *shootComponent = player->shootComponent;
          //ShootComponent_Shoot(shootComponent);
          //Hank ability test
          AbilitySubject_Notify(player->playerEntity, EVENT_ABILITY_1);
        }

        if (AEInputCheckTriggered(VK_RBUTTON))
        {
            triggered = true;
        }
        if (triggered)
        {
            time += dt;
            Temp_Keyboard_Force_PropulsionBoost(player);
        }
        if (time >= 1)
        {
            triggered = false;
            time = 0;
        }
      }
    }
  }
}

/*!
 \brief this function is not required, because no memory is allocated for input.
 */
void Input_Exit(void)
{
}

/*------------------------------------------------------------------------------
// Input Calculators
//----------------------------------------------------------------------------*/

/*!
\brief This function determines the angle of the current pad's left thumbstick.
\param 
*/
static void Input_Lthumb_Angle(int id, XINPUT_STATE state)
{
  //bind check
  if (LthumbAngleCallback == NULL)
    return;

  //get relevant values from the state of the pad
  float LX = state.Gamepad.sThumbLX;
  float LY = state.Gamepad.sThumbLY;

  //set current angle
  float curAngle = atan2(LY, LX);

  /*------------------------------------------------------------------------------
  // Magnitude calculations for deadzone identification, to be moved
  //----------------------------------------------------------------------------*/

  //determine how far the controller is from center
  float magnitude = sqrt(LX*LX + LY*LY);

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  float normalizedMagnitude = magnitude / 32767;

  //----------------------------------------------------------------------------

  //callback, third parameter for test purposes
  LthumbAngleCallback(id, curAngle, normalizedMagnitude);
}

/*!
\brief this function determines the current magnitude of the left thumb stick
*/
static void Input_Lthumb_Magnitude(int id, XINPUT_STATE state)
{
  //bind check
  if (LthumbMagnitudeCallback == NULL)
    return;

  //get relevant values from the state of the pad
  float LX = state.Gamepad.sThumbLX;
  float LY = state.Gamepad.sThumbLY;

  //determine how far the controller is from center
  float magnitude = sqrt(LX*LX + LY*LY);

  //determine the direction the controller is pushed
  //float normalizedLX = LX / magnitude;
  //float normalizedLY = LY / magnitude;

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  float normalizedMagnitude = magnitude / 32767;

  //callback
  LthumbMagnitudeCallback(id, normalizedMagnitude);
}

/*!
\brief This function determines the current angle of the right thumb stick
*/
static void Input_Rthumb_Angle(int id, XINPUT_STATE state)
{
  //bind check
  if (RthumbAngleCallback == NULL)
    return;

  //get relevant values from the state of the pad
  float RX = state.Gamepad.sThumbRX;
  float RY = state.Gamepad.sThumbRY;

  //set current angle
  float curAngle;
  curAngle = atan2(RY, RX);

  //callback
  RthumbAngleCallback(id, curAngle);
}

/*!
\brief This function determines the current magnitude of the right thumb stick
*/
static void Input_Rthumb_Magnitude(int id, XINPUT_STATE state)
{
  //bind check
  if (RthumbMagnitudeCallback == NULL)
    return;

  //get relevant values from the state of the pad
  float RX = state.Gamepad.sThumbRX;
  float RY = state.Gamepad.sThumbRY;

  //determine how far the controller is from center
  float magnitude = sqrt(RX*RX + RY*RY);

  //normalized magnitude
  float normalizedMagnitude;

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  normalizedMagnitude = magnitude / 32767;

  //callback
  RthumbMagnitudeCallback(id, normalizedMagnitude);
}

/*!
\brief This function checks for the BAXY button presses
*/
static void Input_BAXY_Press(int id, XINPUT_STATE state)
{
  //bind check
  if (BAXYCallback == NULL)
    return;

  //check B, callback
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0)
  {
    BAXYCallback(id, PAD_B);
  }
  //check A, callback
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0)
  {
    BAXYCallback(id, PAD_A);
  }
  //check X, and callback
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0)
  {
    BAXYCallback(id, PAD_X);
  }
  //check Y, and callback
  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0)
  {
    BAXYCallback(id, PAD_Y);
  }
}

/*!
\brief This function determines the current pressure of the right trigger
*/
static void Input_Rtrigger_Magnitude(int id, XINPUT_STATE state)
{
  //bind check
  if (RTriggerMagnitudeCallback == NULL)
    return;

  //get relevant values from the state of the pad
  float magnitude = state.Gamepad.bRightTrigger;

  //normalized magnitude
  float normalizedMagnitude;

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  normalizedMagnitude = magnitude / 256;

  //callback
  RTriggerMagnitudeCallback(id, normalizedMagnitude);
}

/*!
\brief This function determines the current pressure of the left trigger
*/
static void Input_Ltrigger_Magnitutde(int id, XINPUT_STATE state)
{
  //bind check
  if (LTriggerMagnitudeCallback == NULL)
    return;

  //get relevant values from the state of the pad
  float magnitude = state.Gamepad.bLeftTrigger;

  //normalized magnitude
  float normalizedMagnitude;

  //normalize the magnitude giving a magnitude value of 0.0 to 1.0
  normalizedMagnitude = magnitude / 256;

  //callback
  LTriggerMagnitudeCallback(id, normalizedMagnitude);
}

static void Input_RBumper(int id, XINPUT_STATE state)
{
  if(RBumperCallback == NULL)
  {
    return;
  }

  if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
  {
    RBumperCallback(id, true);
  }
  else
  {
    RBumperCallback(id, false);
  }
}

/*------------------------------------------------------------------------------
// Input Setters (public)
//----------------------------------------------------------------------------*/

/*!
\brief This function sets the callback of the left thumb stick angle
*/
void Input_Set_Lthumb_Angle(void(*callback)(int id, float angle, float magnitude))
{
  LthumbAngleCallback = callback;
}

/*!
\brief This function sets the callback of the left thumb stick magnitude
*/
void Input_Set_Lthumb_Magnitude(void(*callback)(int id, float normalizedMagnitude))
{
  LthumbMagnitudeCallback = callback;
}

/*!
\brief This function sets the callback of the right thumb stick angle
*/
void Input_Set_Rthumb_Angle(void(*callback)(int id, float angle))
{
  RthumbAngleCallback = callback;
}

/*!
\brief This function sets the callback of the right thumb stick magnitude
*/
void Input_Set_Rthumb_Magnitude(void(*callback)(int id, float normalizedMagnitude))
{
  RthumbMagnitudeCallback = callback;
}

/*!
\brief This function sets the callback of the baxy buttons
*/
void Input_Set_BAXY(void(*callback)(int id, int button))
{
  BAXYCallback = callback;
}

/*!
\brief This function sets the callback of the right trigger magnitude
*/
void Input_Set_Rtrigger_Magnitude(void(*callback)(int id, float normalizedMagnitude))
{
  RTriggerMagnitudeCallback = callback;
}

/*!
\brief This function sets the call back of the left trigger magnitude
*/
void Input_Set_Ltrigger_Magnitude(void(*callback)(int id, float normalizedMagnitude))
{
  LTriggerMagnitudeCallback = callback;
}

/*!
 * \brief
 *   This function sets the callback of the right bumper
 */
void Input_Set_RBumper(void(*callback)(int id, bool state))
{
  RBumperCallback = callback;
}

/*!
\brief This function sets the call back of the controller connect
*/
void Input_SetOnConnectCallBack(void(*callback)(int id))
{
  OnControllerConnectCallback = callback;
}

/*!
\brief This function sets the call back of the controller disconnect
*/
void Input_SetOnDisconnectCallback(void(*callback)(int id))
{
  OnControllerDisconnectCallback = callback;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

//this function is temporary
static void Temp_Keyboard_Force_Propulsion(Player *player)
{
  //pull the transform
  TransformComponent *transform = Entity_GetTransform(player->playerEntity);

  //pull the direction of the transform
  Vector2 force = TransformComponent_GetForward(transform);

  //modify the values based on teh current magnitude of the right trigger
  force.x *= 5000;
  force.y *= 5000;

  //set the force of the transform
  RigidbodyComponent_AddForce(player->rigidbodyComponent, force, FORCE_TYPE_FORCE);
}

static void Temp_Keyboard_Force_PropulsionBoost(Player *player)
{
    //pull the transform
    TransformComponent *transform = Entity_GetTransform(player->playerEntity);

    //pull the direction of the transform
    Vector2 force = TransformComponent_GetForward(transform);

    //modify the values based on teh current magnitude of the right trigger
    force.x *= 10000;
    force.y *= 10000;

    //set the force of the transform
    RigidbodyComponent_AddForce(player->rigidbodyComponent, force, FORCE_TYPE_FORCE);
}


