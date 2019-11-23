/******************************************************************************/
/*!
\file   InputTest.c
\author Cole Astaire | Henry Brobeck (minor)
\par    Course: GAM100
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "InputTest.h"
#include <stdlib.h>
#include <AEengine.h>
#include "Game.h"
#include <Xinput.h>
#include <Windows.h>
#include <winerror.h>
#include <stdio.h>
#include "ShootComponent.h"
#include "Sound.h"
#include "GameState.h"
#include <math.h>

#define INPUT_DEADZONE 7000
#define NORMALIZED_DEADZONE 0.50f

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

void Test_Keyboard_Controls_Update(Vector2 *force, Vector2 *scale, float rotation, float dt)
{
  if (AEInputCheckCurr(VK_ESCAPE))
  {
    Game_SetRunning(0);
  }
  if (AEInputCheckCurr('W'))
  {
    force->y += 5000.0;
  }
  if (AEInputCheckCurr('A'))
  {
    force->x -= 5000.0;
  }
  if (AEInputCheckCurr('S'))
  {
    force->y -= 5000.0;
  }
  if (AEInputCheckCurr('D'))
  {
    force->x += 5000.0;
  }
  if (AEInputCheckCurr('E'))
  {
    rotation -= 5.00f * dt;
  }
  if (AEInputCheckCurr('Q'))
  {
    rotation += 5.00f * dt;
  }
  if (AEInputCheckCurr('R'))
  {
    *scale = Vector2_AddFloats(*scale, 0.1f, 0.0f);
  }
  if (AEInputCheckCurr('F'))
  {
    *scale = Vector2_AddFloats(*scale, -0.1f, -0.0f);
  }
}

/**
* \brief Checks input from gamepad, for use in game update.
* \para Physics variables to be manipulated through input.
*/
void Test_Pad_Controls_Update(Component *shoot, Vector2 *force, Vector2 *scale, float *rotation, float dt)
{
  UNREFERENCED_PARAMETER(dt);

  DWORD dwResult;
  for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
  {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    // Simply get the state of the controller from XInput.
    dwResult = XInputGetState(i, &state);

    if (dwResult == ERROR_SUCCESS)
    {
      // Controller is connected 

      /*//Left thumbpad values
      float LX = state.Gamepad.sThumbLX; //x value between -7800 and 7800
      float LY = state.Gamepad.sThumbLY; //y value between -7800 and 7800
      float L_magnitude = sqrt(LX*LX + LY*LY); //distance from center
      float normalizedLX = fabs(LX) / L_magnitude; //LX distance from center from 0-1
      float normalizedLY = fabs(LY) / L_magnitude; //LY distance from center

      //Right thumbpad values
      float RX = state.Gamepad.sThumbRX; //x value between -7800 and 7800
      float RY = state.Gamepad.sThumbRY; //y value between -7800 and 7800
      float R_magnitude = sqrt(RX*RX + RY*RY); //distance from center
      float normalizedRX = fabs(RX) / R_magnitude; //RX distance from center from 0-1
      float normalizedRY = fabs(RY) / R_magnitude; //RY distance from center from 0-1

      int aCheckFlag = 1;
      float curAngle = 0.0;

      //print axis vectors
      //printf("Left thum y axis: %d\n", state.Gamepad.sThumbLY);
      //printf("Left thum x axis: %d\n", state.Gamepad.sThumbLX);

      //determine current thumbstick position in radians, set current rotation to that value each frame if past deadzone
      curAngle = atan2(RY,RX);
      if (normalizedRX > 0.50f || normalizedRY > 0.50f)
      {
        *rotation = curAngle;
      }*/

      int aCheckFlag = 1;

      float LX = state.Gamepad.sThumbLX;
      float LY = state.Gamepad.sThumbLY;

      float L_curAngle = 0.0;
      L_curAngle = atan2f(LY, LX);

      //determine how far the controller is pushed
      float L_magnitude = sqrtf(LX*LX + LY*LY);

      //determine the direction the controller is pushed
      //float normalizedLX = LX / L_magnitude;
      //float normalizedLY = LY / L_magnitude;

      float L_normalizedMagnitude = 0;

      //check if the controller is outside a circular dead zone
      if (L_magnitude > INPUT_DEADZONE)
      {
        //clip the magnitude at its expected maximum value
        if (L_magnitude > 32767) L_magnitude = 32767;

        //adjust magnitude relative to the end of the dead zone
        L_magnitude -= INPUT_DEADZONE;

        //optionally normalize the magnitude with respect to its expected range
        //giving a magnitude value of 0.0 to 1.0
        L_normalizedMagnitude = L_magnitude / (32767 - INPUT_DEADZONE);
      }
      else //if the controller is in the deadzone zero out the magnitude
      {
        L_magnitude = 0.0;
        L_normalizedMagnitude = 0.0;
      }

      //repeat for right thumb stick
      float RX = state.Gamepad.sThumbRX;
      float RY = state.Gamepad.sThumbRY;

      float R_curAngle = 0.0;
      R_curAngle = atan2f(RY, RX);

      //determine how far the controller is pushed
      float R_magnitude = sqrtf(RX*RX + RY*RY);

      //determine the direction the controller is pushed
      //float normalizedRX = RX / R_magnitude;
      //float normalizedRY = RY / R_magnitude;

      float R_normalizedMagnitude = 0;

      //check if the controller is outside a circular dead zone
      if (R_magnitude > INPUT_DEADZONE)
      {
        //clip the magnitude at its expected maximum value
        if (R_magnitude > 32767) R_magnitude = 32767;

        //adjust magnitude relative to the end of the dead zone
        R_magnitude -= INPUT_DEADZONE;

        //optionally normalize the magnitude with respect to its expected range
        //giving a magnitude value of 0.0 to 1.0
        R_normalizedMagnitude = R_magnitude / (32767 - INPUT_DEADZONE);
      }
      else //if the controller is in the deadzone zero out the magnitude
      {
        R_magnitude = 0.0;
        R_normalizedMagnitude = 0.0;
      }

      //rotation control
      if (R_normalizedMagnitude > NORMALIZED_DEADZONE)
      {
        *rotation = R_curAngle;
      }

      // force control, commented out to implement rotation set
      if(L_normalizedMagnitude > NORMALIZED_DEADZONE)
      {
        if (state.Gamepad.sThumbLY > 10000)
          force->y += 10000;
        else
          if (state.Gamepad.sThumbLY < -10000)
            force->y -= 10000;
        if (state.Gamepad.sThumbLX > 10000)
          force->x += 10000;
        else
          if (state.Gamepad.sThumbLX < -10000)
            force->x -= 10000;
      }


      //scale control
      if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0)
      {
        //printf_s("A");
        *scale = Vector2_AddFloats(*scale, 0.1f, 0.1f);
      }

      if (((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0))
      {
        if(aCheckFlag)
        {
          ShootComponent_Shoot(shoot);
          
          aCheckFlag = 0;
        }
        
      }
      else
      {
        aCheckFlag = 1;
      }

      if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0)
      {
        //printf_s("A");
        GameState_Restart();
      }
        

      if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0)
        *scale = Vector2_AddFloats(*scale, -0.1f, -0.1f);

      //*rotation = atan(state.Gamepad.sThumbLY / state.Gamepad.sThumbLX);
      

    }
    else
    {
      // Controller is not connected 
      //printf_s("Controller %d is not connected", i);
    }
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

