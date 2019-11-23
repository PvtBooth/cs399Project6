/******************************************************************************/
/*!
	\file   BarComponent.h
	\author Henry Brobeck
	\date   3/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include "Vector2.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/
typedef enum HUDBAR_TYPE
{
  HUDBAR_TYPE_LEFT,
  HUDBAR_TYPE_RIGHT,
  HUDBAR_TYPE_CENTER
}HUDBAR_TYPE;
/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct HUDBar HUDBar;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *BarComponent_Create(Vector2 offset);

HUDBar *BarComponent_CreateBar(Component *component, float max, Vector2 scale, float padding, float r, float g, float b, float a);

void BarComponent_DestroyBar(Component *parent, HUDBar *bar);

void BarComponent_AddBar(Component *component, HUDBar *bar);

void BarComponent_RemoveBar(Component *component, HUDBar *bar);

void BarComponent_SetCurrent(HUDBar *bar, float current);

void BarComponent_SetMax(HUDBar *bar, float max);

void BarComponent_SetScale(HUDBar *bar, Vector2 scale);

void BarComponent_SetRGBA(HUDBar *bar, float r, float g, float b, float a);

float BarComponent_GetCurrent(HUDBar *bar);

float BarComponent_GetMax(HUDBar *bar);

Vector2 BarComponent_GetScale(HUDBar *bar);

void BarComponent_SetBarType(HUDBar *bar, HUDBAR_TYPE type);

HUDBAR_TYPE BarComponent_GetBarType(HUDBar *bar);

void BarComponent_SetWorld(Component *component, bool isWorld);

void BarComponent_SetVisible(HUDBar *bar, bool show);





/*----------------------------------------------------------------------------*/


