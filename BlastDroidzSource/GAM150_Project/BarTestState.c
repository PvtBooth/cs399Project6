/******************************************************************************/
/*!
	\file   BarTestState.c
	\author Henry Brobeck
	\date   3/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "BarTestState.h"
#include "StackState.h"
#include <stdlib.h>
#include "EntityManager.h"
#include "BarComponent.h"
#include "Camera.h"
#include "InputEvent.h"
#include "GraphicsManager.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct BarTestState BarTestState;
struct BarTestState
{
  StackState stackState;

  HUDBar *bar;

  HUDBar *bar2;

  HUDBar *bar3;
  
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

StackState* BarTestState_Create()
{
  BarTestState *state = calloc(1, sizeof(BarTestState));
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
  BarTestState *state = (BarTestState*)self;

  Camera_Init();

  self->entityManager = EntityManager_Create(self);
  self->graphicsManager = GraphicsManager_Create(self);

  Entity *entity = EntityManager_CreateEntity(self->entityManager, "Test");

  Entity_AttachComponent(entity, TransformComponent_Create());

  Component *component = BarComponent_Create(Vector2_Zero());

  Vector2 scale = { 100, 10 };
  HUDBar *bar = BarComponent_CreateBar(component, 100, scale, 10, 1.0, 0.5, 0.5, 1.0);
  HUDBar *bar2 = BarComponent_CreateBar(component, 1000, scale, 5, 0, 0, 1, 1);

  BarComponent_SetBarType(bar2, HUDBAR_TYPE_RIGHT);

  HUDBar *bar3 = BarComponent_CreateBar(component, 1.0f, scale, 0.0f, 0.0f, 0.9f, 0.0f, 1.0f);

  BarComponent_SetBarType(bar3, HUDBAR_TYPE_CENTER);
  state->bar = bar;
  state->bar2 = bar2;
  state->bar3 = bar3;

  Entity_AttachComponent(entity, component);

  
  
}
static void Update(StackState *self, float dt)
{
  BarTestState *state = (BarTestState*)self;

  float current = BarComponent_GetCurrent(state->bar);
  current--;
  if(current < 0 )
  {
    current = BarComponent_GetMax(state->bar);
  }
  BarComponent_SetCurrent(state->bar, current);

  current = BarComponent_GetCurrent(state->bar2);
  current-= 5;
  if (current < 0)
  {
    current = BarComponent_GetMax(state->bar2);
  }
  BarComponent_SetCurrent(state->bar2, current);

  current = BarComponent_GetCurrent(state->bar3);
  current += 0.03f;
  if (current > BarComponent_GetMax(state->bar3))
  {
    current = 0.0f;
  }
  BarComponent_SetCurrent(state->bar3, current);

  EntityManager_Update(self->entityManager, dt);
}
static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
}
static void Exit(StackState *self)
{
  //BarTestState *state = (BarTestState*)self;
  EntityManager_DestroyAllImmediate(self->entityManager);
  EntityManager_Destroy(self->entityManager);
  GraphicsManager_Destroy(self->graphicsManager);
}
static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(inputEvent);
    return false;
}
