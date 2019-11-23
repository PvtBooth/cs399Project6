/******************************************************************************/
/*!
\file   MainMenuStackState.c
\author Ryan Booth
\date   3/9/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "MainMenuStackState.h"
#include "GameStateStack.h"
#include "OptionsStackState.h"
#include "ShipSelectStackState.h"
#include "TutorialStackState.h"
#include "Graphics.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "Input.h"
#include "ThankYouStackState.h"
#include "Player.h"
#include "HUDScore.h"
#include "ScoreLog.h"
#include "Text.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct MainMenuStackState MainMenuStackState;
struct MainMenuStackState
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
static char scoreArray[100];
static RECT windowSize;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void Init(StackState *self);
static bool InputHandle(StackState *self, InputEvent *inputEvent);
static void Update(StackState *self, float dt);
static void Render(StackState *self);
static void Exit(StackState *self);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
StackState *MainMenuStackState_Create()
{
    MainMenuStackState *state = calloc(1, sizeof(MainMenuStackState));
    state->stackState.init = Init;
    state->stackState.update = Update;
    state->stackState.inputHandle = InputHandle;
    state->stackState.exit = Exit;
    state->stackState.render = Render;
    state->stackState.renderBelow = false;
    state->stackState.updateBelow = false;
    state->stackState.name = "MainMenu";

    return &state->stackState;
}

void MainMenuState_UpdateScore()
{
  char * score = ScoreLog_Init();

  sprintf_s(scoreArray, 100, "HIGH SCORE %s", score);
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
    MainMenuStackState *state = (MainMenuStackState*)self;
    self->entityManager = EntityManager_Create(self);
    self->graphicsManager = GraphicsManager_Create(self);
    Camera_Init();
    windowSize = Graphics_GetWindowResolution();
    Vector2 size;
    size.x = (float)(windowSize.right - windowSize.left);
    size.y = (float)(windowSize.bottom - windowSize.top) / 1.1f;

    //state->textureTest = Texture_Load("Assets/finalstates/mainmenu.png");
    state->textureTest = AssetTable_TryLoadTextureAsset("MainMenu", "Assets/finalstates/mainmenu.png");
    assert(state->textureTest);
    state->spriteTest = Sprite_Create(state->textureTest);
    state->entityTest = EntityManager_CreateEntity(self->entityManager, "Background");
    Entity_AttachComponent(state->entityTest, TransformComponent_Create());
    TransformComponent_AddScale(state->entityTest->transformComponent, size);
    Entity_AttachComponent(state->entityTest, RenderComponent_CreateSprite(state->spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_BACKGROUND));
    //Text_Init();
    
    MainMenuState_UpdateScore();
     
    
    
}

static void Update(StackState *self, float dt)
{
    UNREFERENCED_PARAMETER(self);

    Camera_Update(dt);

    //Rudimentary controls keyboard controls
    if (AEInputCheckTriggered(VK_RETURN))
    {
        GameStateStack_PushState(TutorialStackState_Create());
    }
    if (AEInputCheckTriggered('O'))
    {
        GameStateStack_PushState(OptionsStackState_Create());
    }
    if (AEInputCheckTriggered('C'))
    {
      GameStateStack_PushState(ThankYouStackState_Create());
    }
    else if (AEInputCheckTriggered(VK_ESCAPE))
    {
      GameStateStack_PushState(ThankYouStackState_Create());
      GameStateStack_PopState();
    }
}

static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
    UNREFERENCED_PARAMETER(self);

  if (inputEvent->inputEventType != INPUT_EVENT_TYPE_TRIGGERED)
  {
    return false;
  }
  if (inputEvent->inputType == INPUT_TYPE_BUTTON_START || inputEvent->inputType == INPUT_TYPE_BUTTON_A)
  {
    GameStateStack_PushState(TutorialStackState_Create());
    HUDScore_ResetScore();
    return true;
  }
  if (inputEvent->inputType == INPUT_TYPE_BUTTON_B)
  {
    GameStateStack_PushState(ThankYouStackState_Create());
    GameStateStack_PopState();
    return true;
  }
  if (inputEvent->inputType == INPUT_TYPE_BUTTON_X)
  {
    GameStateStack_PushState(OptionsStackState_Create());
    return true;
  }

  if (inputEvent->inputType == INPUT_TYPE_BUTTON_Y) // for credits state!
  {
    GameStateStack_PushState(ThankYouStackState_Create());
    return true;
  }

  

  return true;

}
static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
  Vector2 highScorePos;
  highScorePos.x = -500;
  highScorePos.y = -300;
  Text_Print(highScorePos, scoreArray, 50.0f, RENDER_LAYER_WORLD_HUD);
}
static void Exit(StackState *self)
{
  Camera_UntrackAll();
  EntityManager_DestroyAllImmediate(self->entityManager);
  Player_Reset();
  //Text_Exit();

  GraphicsManager_Destroy(self->graphicsManager);
  EntityManager_Destroy(self->entityManager);
  
}