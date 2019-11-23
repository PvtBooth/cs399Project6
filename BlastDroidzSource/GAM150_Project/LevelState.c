/******************************************************************************/
/*!
\file   LevelState.c
\author Evan Kau
\date   3/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "LevelState.h"
#include "MainMenuStackState.h"
#include "GameState.h"
#include <stdlib.h>
#include "RenderComponent.h"
#include "Entity.h"

#include "Input.h"
#include "Player.h"

#include "Camera.h"
#include "LevelGenNormal.h"

#include "EntityManager.h"
#include "GameStateStack.h"

#include "FailStackState.h"
#include "PausedStackState.h"
#include "Text.h"
#include "PlayerHUD.h"

#include "HUDScore.h"
#include "Assignment.h"
#include "AIManager.h"
#include "LevelMarker.h"
#include "ScoreLog.h"
#include "MainMenuState.h"
#include "Physics_BroadPhase.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct LevelState LevelState;
struct LevelState
{
  StackState stackState;

  Texture *textureTest;
  Sprite *spriteTest;
  Entity *entityTest;

  int playerShipID[4];

  //void(*NextLevel)(StackState *self);

  bool overviewStage;

  bool showPhysicsDebug;

  bool gameplayIsRunning;
};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static Vector2 playerCollider[3] = { { 25, 0 },{ -25, -25 },{ -25, 25 }, };

static Vector2 asteroidCollider[4] = { { -40, -40 },{ 40, -40 },{ 40, 40 },{ -40,40 } };

static bool showPhysicsDebug = false;

//text and sprite statics
static Texture *texture1;
static Sprite *sprite1;
static SpriteSheet *spriteSheet;
static Entity *entity1;
static Vector2 position1 = { 0.0f, 0.0f };
static Vector2 scale1 = { 15.0f, 15.0f };

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

StackState *LevelState_Create()
{
  LevelState *state = (LevelState*)calloc(1, sizeof(LevelState));
  state->stackState.init = &Init;
  state->stackState.update = &Update;
  state->stackState.exit = &Exit;
  state->stackState.inputHandle = &InputHandle;
  state->stackState.render = &Render;
  state->stackState.renderBelow = false;
  state->stackState.updateBelow = false;

  state->overviewStage = true;

  state->gameplayIsRunning = true;

  return &state->stackState;
}

void LevelState_NextLevel()
{
  GameStateStack_PushState(LevelState_Create());
  GameStateStack_PopState();
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Init(StackState *self)
{
  self->entityManager = EntityManager_Create(self);
  self->graphicsManager = GraphicsManager_Create(self);

  Camera_Init();
  Camera_SetTracking(false);
  Camera_SetTargetPosition(0, 0);
  Camera_SetTargetSize(20000.0f / (Graphics_GetWindowResolution().bottom - Graphics_GetWindowResolution().top));
  Player_Init(self);

  LevelGenNormal_Generate(10000, self);

  //text and sprite initialization
  //Text_Init();
  HUDScore_Init();

  LevelMarker_Init();
}

static void Update(StackState *self, float dt)
{
  LevelState* state = (LevelState*)self;

  Camera_Update(dt);
  PhysicsManager_Update(dt);
  EntityManager_Update(self->entityManager, dt);
    
  if (!state->gameplayIsRunning)
  {
    GameStateStack_PopState();
    return;
  }

/*  if (AEInputCheckCurr(VK_CONTROL))
    Physics_BroadPhase_Debug();

  if (AEInputCheckCurr(VK_SHIFT))
  {
    showPhysicsDebug = !showPhysicsDebug;
  }*/

  //TODO: add pad support to this condition for pressing the left bumper, input_type_LB or womthing ike that
  if (state->overviewStage && AEInputCheckTriggered('F'))
  {
    state->overviewStage = false;
    LevelGenNormal_SpawnPlayers(self);
    PlayerHUD_Init(self);
    Camera_SetTracking(true);
  } 

  //TODO: add pad support to this condition for pressing the start button, input_type_start or something like that
  if (AEInputCheckTriggered(VK_ESCAPE))
  {
    GameStateStack_PushState(PausedStackState_Create());
  }

  //  HealthSubject_DamageNotify(Player_GetPlayer(0)->playerEntity, Player_GetPlayer(0)->playerEntity, 1);

  int aliveCount = 0;
  for (int i = 0; i < 4; i++)
  {
    Player *player = Player_GetPlayer(i);
    if (player->playerEntity)
    {
      if (!Entity_GetFlag(player->playerEntity, EFLAG_FREEZE))
      {
        aliveCount++;
      }
    }
  }
  if (!aliveCount && !state->overviewStage)
  {
    //GameStateStack_PopState();
    GameStateStack_PushState(FailStackState_Create());
    LevelGenNormal_ResetLevel(); //Reset score
    //NextLevel();
  }
  //if (!AIManager_GetNearestSwarm(Vector2_Zero()))
  //{
  //  GameState_SetNextState(ThankYouState_GetState());
  //}

  
}

static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
  AIManager_Render();
  Assignment_Render();
  HUDScore_Update();
  //LevelMarker_Render(self->graphicsManager);
  LevelMarker_Render();

}

static void Exit(StackState *self)
{
  Assignment_Exit();

  Camera_UntrackAll();
  EntityManager_DestroyAllImmediate(self->entityManager);
  Player_Reset();
  
  GraphicsManager_Destroy(self->graphicsManager);
  EntityManager_Destroy(self->entityManager);

  LevelMarker_Clear();
  ScoreLog_Update();

  MainMenuState_UpdateScore();

  Physics_BroadPhase_FreeGrid();

  AEGfxSetBackgroundColor(0, 0, 0);
}

static bool InputHandle(StackState *self, InputEvent *inputEvent)
{


  Player *player = Player_GetPlayer(inputEvent->playerID);
  if (player)
  {
    Entity_LocalEvent(player->playerEntity, EVENT_INPUT, inputEvent);
  }
  if(inputEvent->inputEventType != INPUT_EVENT_TYPE_TRIGGERED)
  {
    return true;
  }
  LevelState* state = (LevelState*)self;
  if (inputEvent->inputType == INPUT_TYPE_BUTTON_START)
  {
    GameStateStack_PushState(PausedStackState_Create());
    return true;
  }
  if (state->overviewStage && inputEvent->inputType == INPUT_TYPE_BUTTON_LB)
  {
    state->overviewStage = false;
    LevelGenNormal_SpawnPlayers(self);
    Camera_SetTracking(true);
    PlayerHUD_Init(self);
    return true;
  }

  return true;
}

void LevelStackState_SetGameplayBool(StackState* below, bool active)
{
  LevelState* state = (LevelState*)below;

  LevelGenNormal_ResetLevel();

  state->gameplayIsRunning = active;
}