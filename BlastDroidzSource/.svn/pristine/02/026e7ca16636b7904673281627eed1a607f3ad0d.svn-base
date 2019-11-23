/******************************************************************************/
/*!
	\file   GameStateStack.c
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "GameStateStack.h"
#include <stddef.h>
#include "Log.h"

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

static StackState *head; //!< Head of the state stack, the lowest stgate
static StackState *tail; //!< Tail of the state stack, the highest state
static StackState *addState; //!< State to be pushed onto the stack after the next update
static StackState *removeState; //!< State to be removed from the stack after the next update

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void AddState(StackState *state);
static void RemoveState(StackState *state);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief initializes the state stack
 */
void GameStateStack_Init(void)
{
  head = NULL;
  tail = NULL;
  addState = NULL;
  removeState = NULL;
}

/*!
 \brief Renders then updates the state stack
 \param	dt Change in time (in seconds) since last game loop.
 */
void GameStateStack_Update(float dt)
{

  
  if (removeState)
  {

    RemoveState(removeState);
    removeState->exit(removeState);
    free(removeState);
    removeState = NULL;
  }
  //push/pop states that need doing so
  if (addState)
  {
    AddState(addState);
    addState->init(addState);
    addState = NULL;
  }

  //only update if theres anything to update
  if(!head)
  {
    return;
  }

 
  //render up

  //find the lowest state that doesn't allow rendering below it
  //then render up from there
  //TODO should this be something that we only do if the state stack is "dirty"? Could states sometimes want to render below, sometimes not?
  StackState *iterator = tail;

  while(iterator)
  {
    if(!iterator->renderBelow)
    {
      break;
    }
    iterator = iterator->below;
  }
  
  //if iterator == NULL, we hit the bottom of the stack, start at head
  if(!iterator)
  {
    iterator = head;
  }

  //now render up from there
  while(iterator)
  {
    iterator->render(iterator);
    iterator = iterator->above;
  }

  //update down
  iterator = tail;
  while (iterator)
  {
    iterator->update(iterator, dt);
    if (!iterator->updateBelow)
    {
      break;
    }
    iterator = iterator->below;
  }

  

  
}

/*!
 \brief
 */
void GameStateStack_Exit(void)
{
  StackState *iterator = head;
  while(iterator)
  {
    iterator->exit(iterator);
  }
  head = NULL;
  tail = NULL;
}

void GameStateStack_PushState(StackState* stackState)
{
  addState = stackState;

  LogDataGeneric genericData = { "GameStateStack.c" };
  LogDataStatePush pushData;
  pushData.state = stackState;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_STATEPUSH, genericData, &pushData);
}

void GameStateStack_PassInputEvent(InputEvent* event)
{
  //input events are passed down the stack
  StackState *iterator = tail;
  while(iterator)
  {
    if(iterator->inputHandle)
    {
      if (iterator->inputHandle(iterator, event))
      {
        return;
      }
    }
    
    iterator = iterator->below;
  }
}

StackState* GameStateStack_PopState()
{
  LogDataGeneric genericData = { "GameStateStack.c" };
  LogDataStatePop popData;
  popData.state = tail;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_STATEPOP, genericData, &popData);

  removeState = tail;
  return tail;
}

bool GameStateStack_StackEmpty()
{
  return head == NULL;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void AddState(StackState *state)
{
  //if tail isn't null we can just stick it to the end
  if(tail)
  {
    tail->above = state;
    state->below = tail;
    tail = state;
  }
  //otherwise, the list must be empty, so the tail and head should be this state
  else
  {
    head = state;
    tail = state;
  }
}

static void RemoveState(StackState *state)
{
  if(state->below)
  {
    state->below->above = state->above;
    
  }
  else
  {
    head = state->above;
  }
  if(state->above)
  {
    state->above->below = state->below;
  }
  else
  {
    tail = state->below;
  }
  
}