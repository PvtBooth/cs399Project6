/******************************************************************************/
/*!
	\file   CurtainMissileComponent.h
	\author Evan Kau
	\date   4/13/2017
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

Component* CurtainMissileComponent_Create(int missiles, float damage, float force, float rate, float floatDistance, float floatDelay,
                                          float floatRate, float floatSpread, float speed, float range, EVENT_TYPE triggerEvent);
