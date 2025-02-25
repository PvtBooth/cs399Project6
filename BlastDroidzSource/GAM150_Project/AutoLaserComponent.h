﻿/******************************************************************************/
/*!
	\file   AutoLaserComponent.h
	\author Evan Kau
	\date   3/2/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct AutoLaserData AutoLaserData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AutoLaserComponent_Create(int shotsFired, float rate, float range, float damage, float force, float speed, float spread, EVENT_TYPE triggerEvent);
