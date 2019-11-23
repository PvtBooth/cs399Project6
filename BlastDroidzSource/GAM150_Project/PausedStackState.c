/******************************************************************************/
/*!
	\file   PausedStackState.c
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PausedStackState.h"
#include "MainMenuStackState.h"
#include "LevelState.h"
#include "TestStackState.h"
#include "StackState.h"
#include <stdlib.h>
#include "AEEngine.h"
#include "GameStateStack.h"
#include "TutorialStackState.h"
#include "RenderLayer.h"
#include "Graphics.h"
#include "Camera.h"
#include "Texture.h"
#include "EntityManager.h"
#include "RenderComponent.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct PausedStackState PausedStackState;
struct PausedStackState
{
  StackState stackState;

  //TODO: add members for image
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
static bool InputHandle(StackState *self, InputEvent *inputEvent);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

StackState *PausedStackState_Create()
{
  PausedStackState *state = calloc(1, sizeof(PausedStackState));
  state->stackState.init = Init;
  state->stackState.update = Update;
  state->stackState.exit = Exit;
  state->stackState.inputHandle = InputHandle;
  state->stackState.render = Render;
  state->stackState.renderBelow = true;
  state->stackState.updateBelow = false;
  state->stackState.name = "Paused";

  return &state->stackState;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
    PausedStackState *state = (PausedStackState*)self;
    self->entityManager = EntityManager_Create(self);
    self->graphicsManager = GraphicsManager_Create(self);


    RECT windowSize = Graphics_GetWindowResolution();
    Vector2 size;
    size.x = (float)(windowSize.right - windowSize.left);
    size.y = (float)(windowSize.bottom - windowSize.top);
    //state->textureTest = Texture_Load("Assets/finalstates/Pause.png");
    state->textureTest = AssetTable_TryLoadTextureAsset("Pause", "Assets/finalstates/Pause.png");
    assert(state->textureTest);
    state->spriteTest = Sprite_Create(state->textureTest);
    state->entityTest = EntityManager_CreateEntity(self->entityManager, "Background");
    Entity_AttachComponent(state->entityTest, TransformComponent_Create());
    TransformComponent_AddScale(state->entityTest->transformComponent, size);
    Entity_AttachComponent(state->entityTest, RenderComponent_CreateSprite(state->spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_PLAYER));
}


static void Update(StackState *self, float dt)
{
    UNREFERENCED_PARAMETER(dt);

  if(AEInputCheckTriggered(VK_ESCAPE))
  {
    GameStateStack_PopState();
  }

  if (AEInputCheckTriggered(VK_RETURN))
  {
      LevelStackState_SetGameplayBool(self->below, false);
      GameStateStack_PopState();
  }

  if (AEInputCheckTriggered('O'))
  {
      GameStateStack_PushState(TutorialStackState_Create());
  }
}
static void Render(StackState *self)
{
    GraphicsManager_Render(self->graphicsManager);
}
static void Exit(StackState *self)
{
  EntityManager_Destroy(self->entityManager);
  GraphicsManager_Destroy(self->graphicsManager);
}
static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
    if (inputEvent->inputEventType != INPUT_EVENT_TYPE_TRIGGERED)
    {
        return false;
    }

    if (inputEvent->inputType == INPUT_TYPE_BUTTON_START || inputEvent->inputType == INPUT_TYPE_BUTTON_B)
    {
        GameStateStack_PopState();
        return true;
    }

    if (inputEvent->inputType == INPUT_TYPE_BUTTON_Y)
    {
        LevelStackState_SetGameplayBool(self->below, false);
        GameStateStack_PopState();
        return true;
    }

    if (inputEvent->inputType == INPUT_TYPE_BUTTON_X)
    {
      GameStateStack_PushState(TutorialStackState_Create());
    }
    

    return true;
}