/******************************************************************************/
/*!
	\file   GameState.h
	\author Henry Brobeck
	\date   1/19/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct GameState
{
  void(*load)(void);
  void(*init)(void);
  void(*update)(float dt);
  void(*exit)(void);
  void(*unload)(void);

}GameState;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void GameState_Init(GameState *initial);

void GameState_Update(float dt);

void GameState_Exit(void);

void GameState_RegisterExit(void(*exitFunc)(void));

void GameState_SetNextState(GameState *next);

GameState *GameState_GetCurrent();

void GameState_Restart(void);

void GameState_Quit(void);

int GameState_IsActive(void);



/*----------------------------------------------------------------------------*/


