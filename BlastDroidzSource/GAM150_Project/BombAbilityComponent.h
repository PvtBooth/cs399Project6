﻿/******************************************************************************/
/*!
	\file   BombAbilityComponent.h
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Event.h"
#include "Component.h"

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

Component* BombAbilityComponent_Create(float rate, float timer, float speed, EVENT_TYPE triggerEvent);
