/******************************************************************************/
/*!
\file   FailStackState.c
\author Ryan Booth
\date   3/9/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "FailStackState.h"
#include "GameStateStack.h"
#include "TestStackState.h"
#include "Graphics.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "AEEngine.h"
#include "AEInput.h"
#include "LevelState.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct FailStackState FailStackState;
struct FailStackState
{
  StackState stackState;

  TextureAsset *textureTest;
  Sprite *spriteTest;
  Entity *entityTest;

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
static void Window_Mode(WindowMode select);
static bool InputHandle(StackState *self, InputEvent *inputEvent);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
StackState *FailStackState_Create()
{
  FailStackState *state = calloc(1, sizeof(FailStackState));
  state->stackState.init = Init;
  state->stackState.update = Update;
  state->stackState.exit = Exit;
  state->stackState.inputHandle = InputHandle;
  state->stackState.render = Render;
  state->stackState.renderBelow = true;
  state->stackState.updateBelow = false;
  state->stackState.name = "Fail";

  return &state->stackState;
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
  FailStackState *state = (FailStackState*)self;
  self->entityManager = EntityManager_Create(self);
  self->graphicsManager = GraphicsManager_Create(self);


  RECT windowSize = Graphics_GetWindowResolution();
  Vector2 size;
  size.x = (float)(windowSize.right - windowSize.left);
  size.y = (float)(windowSize.bottom - windowSize.top);
  state->textureTest = AssetTable_TryLoadTextureAsset("Fail", "Assets/temp_fail.png");
  assert(state->textureTest);
  state->spriteTest = Sprite_Create(state->textureTest);
  state->entityTest = EntityManager_CreateEntity(self->entityManager, "Background");
  Entity_AttachComponent(state->entityTest, TransformComponent_Create());
  TransformComponent_AddScale(state->entityTest->transformComponent, size);
  Entity_AttachComponent(state->entityTest, RenderComponent_CreateSprite(state->spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_PLAYER));


}

static void Update(StackState *self, float dt)
{
  Camera_Update(dt);
  if (AEInputCheckCurr(VK_ESCAPE)) // or B press
  {
    //TestStackState_SetGameplayBool(self->below, false);
    LevelStackState_SetGameplayBool(self->below, false);
    GameStateStack_PopState();
  }
}

static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
}
static void Exit(StackState *self)
{
  EntityManager_DestroyAllImmediate(self->entityManager);
  EntityManager_Destroy(self->entityManager);
  GraphicsManager_Destroy(self->graphicsManager);
}

static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
    if (inputEvent->inputEventType != INPUT_EVENT_TYPE_TRIGGERED)
    {
        return false;
    }

    if (inputEvent->inputType == INPUT_TYPE_BUTTON_B)
    {
        LevelStackState_SetGameplayBool(self->below, false);
        GameStateStack_PopState();
    }

    return true;
}
