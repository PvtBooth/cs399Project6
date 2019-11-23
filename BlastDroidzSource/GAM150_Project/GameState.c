/******************************************************************************/
/*!
	\file   GameState.c
	\author Henry Brobeck
	\date   1/19/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "GameState.h"
#include <stddef.h>
#include <stdlib.h>
#include "Time.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

enum GS_FLAGS
{
  GS_RESTART,
  GS_QUIT,
  GS_MAX
};

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct ExitObs ExitObserver;
struct ExitObs
{
    //! function to call on exit
  void(*exit)(void);
    //! Next node in list
  ExitObserver *next;

};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
  
static ExitObserver *head = NULL; //Head node of exit linked list

static GameState *currentState; //Current game state to be updated
static GameState *nextState; //Will switch to this state at the end of the next update loop

static int flags[GS_MAX] = {0}; //Flags for game state control

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief Initialize the Game State module
 */
void GameState_Init(GameState *initial)
{

  currentState = initial;
  
  currentState->load();
  currentState->init();

}

/*!
 \brief Update the active game state
 \param	dt Change in time (in seconds) since last game loop.
 */
void GameState_Update(float dt)
{
  if(currentState)
  {
    currentState->update(dt);
  }
  if(flags[GS_RESTART])
  {
    Time_ResetStateTime();
    flags[GS_RESTART] = 0;
    currentState->exit();
    currentState->init();
  }
  if (nextState)
  {
    Time_ResetStateTime();
    currentState->exit();
    currentState->unload();
    currentState = nextState;
    currentState->load();
    currentState->init();
    nextState = NULL;
  }
  if(flags[GS_QUIT])
  {
    flags[GS_QUIT] = 0;
    currentState->exit();
    currentState->unload();
    currentState = NULL;
  }
  
}

/*!
 \brief Shut down the game state module
 */
void GameState_Exit(void)
{

  currentState->exit();
  currentState->unload();
  
  //Run shutdown on all observers
  ExitObserver *iterator = head;
  while(iterator)
  {
    iterator->exit();
    iterator = iterator->next;
  }


}

/**
 * \brief Register a new shutdown observer for callback on game shutdown
 * \param exitFunc a function taking void and returning void, to be called on gamestate module shutdown
 */
void GameState_RegisterExit(void(* exitFunc)(void))
{
  ExitObserver *newObserver = malloc(sizeof(ExitObserver));

  newObserver->exit = exitFunc;
  newObserver->next = NULL;

  if(head)
  {
    newObserver->next = head;
    head = newObserver;
    return;
  }
  
  head = newObserver;

}

/**
 * \brief this function will switch the active gamestate to a new state at end of update loop
 * \param next the state to switch to
 */
void GameState_SetNextState(GameState* next)
{
  nextState = next;
}

/**
 * \brief Flag the current GameState to restart at end of update loop
 */
void GameState_Restart()
{
  flags[GS_RESTART] = 1;
}

/**
 * \brief Flag the GameState system to stop
 */
void GameState_Quit()
{
  flags[GS_QUIT] = 1;
}

/**
 * \brief Check if there is an active current state
 * \return 1 if a state is active, 0 otherwise
 */
int GameState_IsActive()
{
  return (currentState != NULL);
}
GameState* GameState_GetCurrent()
{
  return currentState;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

