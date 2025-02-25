﻿/******************************************************************************/
/*!
	\file   MissileBarrageComponent.h
	\author Henry Brobeck
	\date   3/20/2017
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

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* MissileBarrageComponent_Create(int damage, int numFired, float speed, float turnRate, float cooldown, EVENT_TYPE triggerEvent);


/*----------------------------------------------------------------------------*/


