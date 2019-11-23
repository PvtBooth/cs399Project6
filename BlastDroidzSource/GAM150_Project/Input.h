/******************************************************************************/
/*!
	\file   Input.h
	\author Cole Astaire
	\date   1/30/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief  contains all function delcarations for input.c
 */
/******************************************************************************/

#pragma once
#include <stdbool.h>


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Input_Init(void);

void Input_Update(float dt);

void Input_Exit(void);

void Input_Set_Lthumb_Angle(void(*callback)(int id, float angle));

void Input_Set_Lthumb_Magnitude(void(*callback)(int id, float normalizedMagnitude));

void Input_Set_Rthumb_Angle(void(*callback)(int id, float angle));

void Input_Set_Rthumb_Magnitude(void(*callback)(int id, float normalizedMagnitude));

void Input_Set_BAXY(void(*callback)(int id, int button));

void Input_Set_Rtrigger_Magnitude(void(*callback)(int id, float normalizedMagnitude));

void Input_Set_Ltrigger_Magnitude(void(*callback)(int id, float normalizedMagnitude));

void Input_Set_RBumper(void(*callback)(int id, bool state));

void Input_SetOnConnectCallBack(void(*OnControllerConnect)(int id));

void Input_SetOnDisconnectCallback(void(*OnControllerDisconnect)(int id));

/*----------------------------------------------------------------------------*/


