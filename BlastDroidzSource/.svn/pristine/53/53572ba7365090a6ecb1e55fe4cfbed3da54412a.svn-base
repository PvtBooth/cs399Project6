/******************************************************************************/
/*!
	\file   AbilityComponent.h
	\author Evan Kau
	\date   3/3/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include "Input.h"
#include "Player.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum ABILITY_STATE
{
  ABILITY_STATE_IDLE,
  ABILITY_STATE_PRIMED,
  ABILITY_STATE_ACTIVE
} ABILITY_STATE;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Player Player;

typedef struct AbilityData
{
  EVENT_TYPE triggerEvent;
  void (*abilityUpdate)(Component *self, float dt);
  void (*destroy)(Component *self);
  void (*triggered)(Component *self, EVENT_TYPE type);
  void *data;
  Player *player;
  ABILITY_STATE state;

  float cooldownPercentage;

  Component *nextAbility;
} AbilityData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AbilityComponent_Create(EVENT_TYPE triggerEvent);

void AbilityComponent_SetPlayer(Component *self, Player *player);
