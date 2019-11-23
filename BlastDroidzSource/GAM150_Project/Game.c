/******************************************************************************/
/*!
	\file   Game.c
	\author Henry Brobeck | Cole Astaire
	\date   1/18/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Game.h"

#include "GameState.h"

#include "ColeTestState2TheReStatening.h"

#include "DigiPenLogoState.h"
#include "PhysicsTestState.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static int gameRunning = 0;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief Initialize the Game
 */
void Game_Init(void)
{
  gameRunning = 1;
  GameState_Init(DigiPenLogoState_GetState());
  //GameState_Init(ColeTestState2_GetState());
  //GameState_Init(PhysicsTestState_GetState());
}

/*!
 \brief Update Game
 \param	dt Change in time (in seconds) since last game loop.
 */
void Game_Update(float dt)
{
  GameState_Update(dt);
}

/*!
 \brief Shutdown Game
 */
void Game_Exit(void)
{
  GameState_Exit();
}

/*!
 * \brief Checks if the game has requested exit
 * \return 1 if game exit requested, 0 otherwise
 */
int Game_IsRunning(void)
{
  return gameRunning;
}

/**
 * \brief Allows the ability to request game exit
 * \param running send nonzero to request game exit 
 */
void Game_SetRunning(int running)
{
  gameRunning = running;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
