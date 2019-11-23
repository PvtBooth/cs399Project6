/******************************************************************************/
/*!
	\file   HUDBarComponent.h
	\author Cole Astaire, Henry Brobeck
	\date   2/13/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "CollisionDamageComponent.h"
#include "Vector2.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct HUDBar HUDBar;

struct HUDBar
{
  float width;
  float height;
  float min;
  float max;
  float value;
  HUDBar *next;
  HUDBar *previous;
};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *HUDBarComponent_Create(Vector2 offset, float width, float height, float padding);

float HUDBarComponent_GetWidth(HUDBar *hudBar);
float HUDBarComponent_GetHeight(HUDBar *hudBar);
float HUDBarComponent_GetMin(HUDBar *hudBar);
float HUDBarComponent_GetMax(HUDBar *hudBar);
float HUDBarComponent_GetValue(HUDBar *hudBar);

void HUDBarComponent_SetWidth(HUDBar *hudBar);
void HUDBarComponent_SetHeight(HUDBar *hudBar);
void HUDBarComponent_SetMin(HUDBar *hudBar);
void HUDBarComponent_SetMax(HUDBar *hudBar);
void HUDBarComponent_SetValue(HUDBar *hudBar);

/*----------------------------------------------------------------------------*/


