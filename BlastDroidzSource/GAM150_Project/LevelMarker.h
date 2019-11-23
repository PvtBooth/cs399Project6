/******************************************************************************/
/*!
	\file   LevelMarker.h
	\author Henry Brobeck
	\date   4/13/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "TransformComponent.h"
#include "GraphicsManager.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/


typedef struct LevelMarker LevelMarker;

struct LevelMarker
{
  TransformComponent *transform;
  float r;
  float g;
  float b;
  float a;
  LevelMarker *next;
};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void LevelMarker_Init(void);


void LevelMarker_Exit(void);

void LevelMarker_Render();

LevelMarker *LevelMarker_Create(TransformComponent *transform, float r, float g, float b, float a);

void LevelMarker_Remove(LevelMarker *marker);

void LevelMarker_Clear();



/*----------------------------------------------------------------------------*/


