/******************************************************************************/
/*!
	\file   StackState.h
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <stdbool.h>
#include "InputEvent.h"


//FORWARD DECLARATIONS
typedef struct EntityManager EntityManager;
typedef struct GraphicsManager GraphicsManager;


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct StackState StackState;
struct StackState
{
  const char *name;

  //load and unload will probably have no purpose so are currently not a part of the struct, can change if needed
  //void(*load)(StackState *self);
  void(*init)(StackState *self); //!< called when the state is pushed onto the stack
  void(*update)(StackState *self, float dt); //!< called every frame as long as the states above it (if any) allow updates below
  void(*render)(StackState *self); //!< called every frame as long as the states above it allow renders below
  void(*exit)(StackState *self); //!< called when a state is popped off the stack
                                 //void(*unload)(StackState *self);
  bool(*inputHandle)(StackState *self, InputEvent *inputEvent); //!< Called when an input event is passed to this state, return true to consume the event

  bool updateBelow; //!< should the states below this one on the stack be updated?
  bool renderBelow; //!< should the states below this one on the stack be rendered?

  EntityManager *entityManager;
  GraphicsManager *graphicsManager;

  StackState *below; //!< Previous state in the stack
  StackState *above; //!< Next state in the stack



};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/


