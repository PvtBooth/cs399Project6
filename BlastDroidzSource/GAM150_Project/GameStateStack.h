/******************************************************************************/
/*!
	\file   GameStateStack.h
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <stdbool.h>
#include "StackState.h"


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

void GameStateStack_Init(void);

void GameStateStack_Update(float dt);

void GameStateStack_Exit(void);

void GameStateStack_PushState(StackState *stackState);

void GameStateStack_PassInputEvent(InputEvent *event);

StackState *GameStateStack_PopState(void);

bool GameStateStack_StackEmpty(void);

/*----------------------------------------------------------------------------*/


