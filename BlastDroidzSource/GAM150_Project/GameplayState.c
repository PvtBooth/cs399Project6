/******************************************************************************/
/*!
	\file   GameplayState.c
	\author Henry Brobeck
	\date   1/19/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "GameplayState.h"
#include <stddef.h>
#include <stdlib.h>
#include "Graphics.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "Sound.h"
#include "Game.h"
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
//!Singleton of gameplaystate
static GameState *instance = NULL;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Load(void);
static void Init(void);
static void Update(float dt);
static void Exit(void);
static void Unload(void);

static void OnGameExit(void);

static Entity *testEntity;
static Component *transformComponent;
static TransformComponent *transData;

static Entity *testEntity2;
static Component *transformComponent2;

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
/**
 * \brief Get an instance of the Gameplay GameState
 * \return an instance of Gameplay GameState
 */
GameState* GameplayState_GetState()
{
  if(instance)
  {
    return instance;
  }

  instance = malloc(sizeof(GameState));
  instance->load = Load;
  instance->init = Init;
  instance->update = Update;
  instance->exit = Exit;
  instance->unload = Unload;

  GameState_RegisterExit(OnGameExit);

  return instance;
}

static void Load(void)
{
  
}

static void Init(void)
{
  
}
static void Update(float dt)
{
  UNREFERENCED_PARAMETER(dt);
  GameState_SetNextState(PhysicsTestState_GetState());
}
static void Exit(void)
{
  
}
static void Unload(void)
{
  
}

static void OnGameExit(void)
{
  if(instance)
  {
    free(instance);
  }
}