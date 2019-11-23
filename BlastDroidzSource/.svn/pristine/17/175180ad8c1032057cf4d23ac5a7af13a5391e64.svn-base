/******************************************************************************/
/*!
	\file   PausedState.c
	\author Henry Brobeck
	\date   3/6/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PausedState.h"
#include "StackState.h"
#include <stdlib.h>
#include "AEEngine.h"
#include "GameStateStack.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct PauseState PauseState;
struct PauseState
{
  StackState stackState;
};
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void Init(StackState *self);
static void Update(StackState *self, float dt);
static void Render(StackState *self);
static void Exit(StackState *self);
static bool InputHandle(StackState *self, InputEvent *inputEvent);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

StackState *PausedState_Create()
{
  PauseState *state = calloc(1, sizeof(PauseState));
  state->stackState.init = Init;
  state->stackState.update = Update;
  state->stackState.exit = Exit;
  state->stackState.inputHandle = InputHandle;
  state->stackState.render = Render;
  state->stackState.renderBelow = false;
  state->stackState.updateBelow = false;

  return &state->stackState;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
  
}
static void Update(StackState *self, float dt)
{
  if(AEInputCheckTriggered(VK_RETURN))
  {
    GameStateStack_PopState();
  }
}
static void Render(StackState *self)
{
  
}
static void Exit(StackState *self)
{
  
}
static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
  
}