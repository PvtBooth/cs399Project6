/******************************************************************************/
/*!
\file   TutorialStackState.c
\author Ryan Booth
\date   3/9/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#pragma warning(disable: 4130)

#include "TutorialStackState.h"
#include "GameStateStack.h"
#include "MainMenuStackState.h"
#include "ShipSelectStackState.h"
#include "Graphics.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct TutorialStackState TutorialStackState;
struct TutorialStackState
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
static bool InputHandle(StackState *self, InputEvent *inputEvent);
static void Render(StackState *self);
static void Exit(StackState *self);
static void Window_Mode(WindowMode select);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
StackState *TutorialStackState_Create()
{
  TutorialStackState *state = calloc(1, sizeof(TutorialStackState));
  state->stackState.init = Init;
  state->stackState.update = Update;
  state->stackState.inputHandle = InputHandle;
  state->stackState.exit = Exit;
  state->stackState.render = Render;
  state->stackState.renderBelow = false;
  state->stackState.updateBelow = false;
  state->stackState.name = "Tutorial";

  return &state->stackState;
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
  TutorialStackState *state = (TutorialStackState*)self;
  self->entityManager = EntityManager_Create(self);
  self->graphicsManager = GraphicsManager_Create(self);

  RECT windowSize = Graphics_GetWindowResolution();
  Vector2 size;
  size.x = (float)(windowSize.right - windowSize.left);
  size.y = (float)(windowSize.bottom - windowSize.top);

  //state->textureTest = Texture_Load("Assets/finalstates/tutorial.png");
  state->textureTest = AssetTable_TryLoadTextureAsset("Tutorial", "Assets/finalstates/tutorial.png");
  assert(state->textureTest);
  state->spriteTest = Sprite_Create(state->textureTest); //LEAK
  state->entityTest = EntityManager_CreateEntity(self->entityManager, "Background");
  Entity_AttachComponent(state->entityTest, TransformComponent_Create());
  TransformComponent_AddScale(state->entityTest->transformComponent, size);
  Entity_AttachComponent(state->entityTest, RenderComponent_CreateSprite(state->spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_BACKGROUND)); //LEAK

}

static void Update(StackState *self, float dt)
{
  UNREFERENCED_PARAMETER(self);
  Camera_Update(dt);
  if (AEInputCheckTriggered(VK_RETURN) && self->below->name == "MainMenu") //M = MainMenuState, in case you also want to do enter for something else
  {
    GameStateStack_PushState(ShipSelectStackState_Create());
    GameStateStack_PopState();
    return;
  }
  if (AEInputCheckTriggered(VK_RETURN) && self->below->name == "Paused")//P = PausedState
  {
      GameStateStack_PopState();
      return;
  }

  if(AEInputCheckTriggered(VK_ESCAPE))
      GameStateStack_PopState();
}

static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
}
static void Exit(StackState *self)
{
  EntityManager_DestroyAllImmediate(self->entityManager);
  GraphicsManager_Destroy(self->graphicsManager);
  EntityManager_Destroy(self->entityManager);
}

static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
    UNREFERENCED_PARAMETER(self);
  if (inputEvent->inputEventType != INPUT_EVENT_TYPE_TRIGGERED)
  {
    return false;
  }

  if (inputEvent->inputType == INPUT_TYPE_BUTTON_A || inputEvent->inputType == INPUT_TYPE_BUTTON_START)
  {
    if (self->below->name == "MainMenu")
    {
      GameStateStack_PushState(ShipSelectStackState_Create());
      GameStateStack_PopState();
      return true;
    }

    if (self->below->name == "Paused")//P = PausedState
    {
      GameStateStack_PopState();
      return true;
    }
  }

  if (inputEvent->inputType == INPUT_TYPE_BUTTON_B)
  {
    GameStateStack_PopState();
    return true;
  }

  return true;
}
