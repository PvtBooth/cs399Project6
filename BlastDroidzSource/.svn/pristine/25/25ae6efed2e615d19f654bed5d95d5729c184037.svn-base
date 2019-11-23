/******************************************************************************/
/*!
\file   Player.h
\author Ryan Booth
\date   2/4/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes the function declarations that handle players
*/
/******************************************************************************/

#pragma once
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ShootComponent.h"
#include "Entity.h"
#include "AbilityComponent.h"
#include "InputGatherComponent.h"
#include "AIMarkerComponent.h"
#include "Colors.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef enum INPUT_METHOD
{
	KEYBOARD,
	GAMEPAD,
	BOTH
}INPUT_METHOD;

typedef struct Player {

  Entity *playerEntity;
  TransformComponent *transformComponent;
  RigidbodyComponent *rigidbodyComponent;
  ColliderComponent *colliderComponent;
  InputGatherData *inputGatherComponent;
  AIMarker *aiMarker;
  Component *renderComponent;
  Component *abilities;
  bool abilityActive; //True if a non-default ability is primed/in use
  INPUT_METHOD method;
  int id;

}Player;



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

void Player_Init(StackState *state);

Vector2 Player_GetPosition(int id);

//Depricated
//void Player_Create(char* name, int player_number, u32 color, Vector2 position, StackState *state);

void Player_AddAbility(Player *player, Component *ability);

void Player_DeactivateAbilities(Player *player);

extern Player* Player_GetPlayer(int id);

extern void Player_SetInputMethod(int id, INPUT_METHOD method);

Entity * Player_Type1_Create(int id, Vector2 position);

Entity * Player_Type2_Create(int id, Vector2 position);

Entity * Player_Type3_Create(int id, Vector2 position);

Entity * Player_Type4_Create(int id, Vector2 position);

INPUT_METHOD Player_GetInputMethod(Player* player);

void Player_Exit(void);

void Player_GetColor(int id, TEAMCOLOR type, float *r, float *g, float *b, float *a);

void Player_Reset();

void Player_ResetID(int i);

//Depricated
//void Player_CreateHUD(int playerID);

int Player_PlayerCount();


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
