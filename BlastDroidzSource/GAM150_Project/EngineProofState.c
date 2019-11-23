/******************************************************************************/
/*!
	\file   EngineProofState.c
	\author Evan Kau
	\date   2/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "EngineProofState.h"
#include "GameState.h"
#include <stdlib.h>
#include "Entity.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define ENGINE_PROOF_STATE_NUM_ENEMIES 12

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static GameState *engineStateInstance = NULL;

static Entity **enemies = NULL;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void OnGameExit(void);

static void Load(void);
static void Init(void);
static void Update(float dt);
static void Exit(void);
static void Unload(void);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

GameState* EngineProofState_GetState()
{
  if (engineStateInstance)
  {
    return engineStateInstance;
  }

  engineStateInstance = malloc(sizeof(GameState));
  engineStateInstance->load = Load;
  engineStateInstance->init = Init;
  engineStateInstance->update = Update;
  engineStateInstance->exit = Exit;
  engineStateInstance->unload = Unload;

  GameState_RegisterExit(OnGameExit);

  return engineStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
  if (engineStateInstance)
  {
    free(engineStateInstance);
  }
}

/*!
\brief
*/
static void Load(void)
{
}

/*!
\brief
*/
static void Init(void)
{
  enemies = (Entity**)calloc(ENGINE_PROOF_STATE_NUM_ENEMIES, sizeof(Entity*));
}

/*!
\brief
\param	dt Change in time (in seconds) since last game loop.
*/
static void Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

/*!
\brief
*/
static void Exit(void)
{
}

/*!
\brief
*/
static void Unload(void)
{
}
