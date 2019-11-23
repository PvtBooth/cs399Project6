/******************************************************************************/
/*!
\file   ShipSelectStackState.c
\author Ryan Booth
\date   3/9/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "ShipSelectStackState.h"
#include "GameStateStack.h"
#include "Graphics.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "LevelState.h"
#include "InputEvent.h"
#include "Player.h"
#include "HUDScore.h"
#include "AssetTable.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct ShipSelectStackState ShipSelectStackState;
struct ShipSelectStackState
{
  StackState stackState;

  TextureAsset* shipTextures[4];
  Sprite* shipSprites[4];
  Entity* shipEntities[4];
};
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static int current_ship = 0;
static int playerShips[4];
static TextureAsset* Texture1;
static TextureAsset* Texture2;
static TextureAsset* Texture3;
static TextureAsset* Texture4;
static TextureAsset* Texture5;

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
/*!
\brief Creates a ship select stack state
*/
StackState *ShipSelectStackState_Create()
{
  ShipSelectStackState *state = calloc(1, sizeof(ShipSelectStackState));
  state->stackState.init = Init;
  state->stackState.inputHandle = InputHandle;
  state->stackState.update = Update;
  state->stackState.exit = Exit;
  state->stackState.render = Render;
  state->stackState.renderBelow = false;
  state->stackState.updateBelow = false;
  state->stackState.name = "ShipSelect";

  return &state->stackState;
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
/*!
\brief Creates a ship select stack state
*/
static void Init(StackState *self)
{
  ShipSelectStackState *state = (ShipSelectStackState*)self;
  self->entityManager = EntityManager_Create(self);
  self->graphicsManager = GraphicsManager_Create(self);

  RECT windowSize = Graphics_GetWindowResolution();
  Vector2 size;
  size.x = (float)(windowSize.right - windowSize.left);
  size.y = (float)(windowSize.bottom - windowSize.top);
  size.x = (size.x / 4.0f);

  Camera_Init();

  Texture1 = AssetTable_TryLoadTextureAsset("Ship1Display", "Assets/finalstates/ship1.png");
  Texture2 = AssetTable_TryLoadTextureAsset("Ship2Display", "Assets/finalstates/ship2.png");
  Texture3 = AssetTable_TryLoadTextureAsset("ShipMissing", "Assets/finalstates/missing.png");
  Texture4 = AssetTable_TryLoadTextureAsset("Ship3Display", "Assets/finalstates/ship3.png");

  Texture5 = AssetTable_TryLoadTextureAsset("Ship4Display", "Assets/finalstates/ship4.png");

  Vector2 position1 = { 0,0 };

  position1.x = -(size.x + (size.x / 2.0f)); //Reset the position;

  for (int i = 0; i < 4; i++)
  {
    if (i)
      position1.x += size.x;
    state->shipTextures[i] = AssetTable_TryLoadTextureAsset("ShipSelect", "Assets/finalstates/shipselect_template.png");
    assert(state->shipTextures[i]);
    if (!state->shipSprites[i])
    {
      state->shipSprites[i] = Sprite_Create(state->shipTextures[i]); //LEAK
    }
    state->shipEntities[i] = EntityManager_CreateEntity(self->entityManager, "ShipSelectionMenu");
    Entity_AttachComponent(state->shipEntities[i], TransformComponent_Create());
    TransformComponent_AddScale(state->shipEntities[i]->transformComponent, size);
    Entity_AttachComponent(state->shipEntities[i], RenderComponent_CreateSprite(state->shipSprites[i], RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_PARTICLE));
    TransformComponent_SetPosition(state->shipEntities[i]->transformComponent, position1);
  }

  for (int i = 0; i < 4; i++)
  {
    playerShips[i] = 0;
  }
}
/*!
\brief Updates any variables asscoiated with this state
*/
static void Update(StackState *self, float dt)
{
  ShipSelectStackState *state = (ShipSelectStackState*)self;

  //No controller connected for players 2-4
  for (int i = 1; i < 4; i++)
  {
    if (!InputEvent_ControllerIsConnected(i))
    {
      state->shipSprites[i]->texture = Texture3;
      playerShips[i] = -1;
    }
  }

  if (AEInputCheckTriggered(VK_RIGHT))
  {
    playerShips[0] += 1;
    if (playerShips[0] >= 4)
    {
      playerShips[0] = 0;
    }
  }
  if (AEInputCheckTriggered(VK_LEFT))
  {
    playerShips[0] -= 1;
    if (playerShips[0] <= -1)
    {
      playerShips[0] = 3;
    }
  }

  for (int i = 0; i < 4; i++)
  {
    if (playerShips[i] == 0)
      state->shipSprites[i]->texture = Texture1;
    else if (playerShips[i] == 1)
      state->shipSprites[i]->texture = Texture2;
    else if (playerShips[i] == 2)
      state->shipSprites[i]->texture = Texture4;
    else if (playerShips[i] == 3)
      state->shipSprites[i]->texture = Texture5;
  }

  Camera_Update(dt);

  if (AEInputCheckTriggered(VK_RETURN))
  {
    //GameStateStack_PushState(TestStackState_Create());
    HUDScore_ResetScore();
    GameStateStack_PushState(LevelState_Create());
    GameStateStack_PopState();
  }
  else if (AEInputCheckTriggered(VK_ESCAPE))
  {
    GameStateStack_PopState();
  }
}
/*!
\brief Renders any entity in this state
*/
static void Render(StackState *self)
{
  ShipSelectStackState *state = (ShipSelectStackState*)self;

  Color colors;

  for (int j = 0; j < 4; j++)
  {
    colors = Colors_GetPlayerColor(j, COLORT_SECONDARY);
    Graphics_DrawRectCamera(TransformComponent_GetPosition(state->shipEntities[j]->transformComponent), TransformComponent_GetScale(state->shipEntities[j]->transformComponent), 0.0f, colors.r, colors.g, colors.b, colors.a);
  }

  GraphicsManager_Render(self->graphicsManager);
}
/*!
\brief Free's all memory asscoiated with this state
*/
static void Exit(StackState *self)
{
  ShipSelectStackState* state = (ShipSelectStackState*)self;
  EntityManager_Destroy(self->entityManager);
  GraphicsManager_Destroy(self->graphicsManager);

  for(int i = 0; i < 4; i++)
  {
    if(state->shipSprites[i])
    {
      Sprite_Destroy(state->shipSprites[i]);
      state->shipSprites[i] = NULL;
    }
  }
  
}
/*!
\brief Returns the player ships selected 
*/
int  ShipSelectStackState_GetPlayerShip(int id)
{
  return playerShips[id];
}
/*!
\brief Handles the input for this stack state
*/
static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
  ShipSelectStackState* state = (ShipSelectStackState*)self;


  if (inputEvent->inputEventType != INPUT_EVENT_TYPE_TRIGGERED)
  {
    return false;
  }

  if (inputEvent->inputType == INPUT_TYPE_DPAD_RIGHT)
  {
    playerShips[inputEvent->playerID] += 1;
    if (playerShips[inputEvent->playerID] >= 4)
    {
      playerShips[inputEvent->playerID] = 0;
    }
    return true;
  }

  if (inputEvent->inputType == INPUT_TYPE_DPAD_LEFT)
  {
    playerShips[inputEvent->playerID] -= 1;
    if (playerShips[inputEvent->playerID] <= -1)
    {
      playerShips[inputEvent->playerID] = 3;
    }
    return true;
  }

  if (inputEvent->inputType == INPUT_TYPE_BUTTON_A)
  {
    if (playerShips[inputEvent->playerID] == -1)
    {
      state->shipSprites[inputEvent->playerID]->texture = Texture1;
      playerShips[inputEvent->playerID] = 0;
    }
    else
      return true;

    return true;
  }

  if (inputEvent->inputType == INPUT_TYPE_BUTTON_START)
  {
    HUDScore_ResetScore();
    GameStateStack_PushState(LevelState_Create());
    GameStateStack_PopState();

    return true;
  }

  if (inputEvent->inputType == INPUT_TYPE_BUTTON_B)
  {
    GameStateStack_PopState();
    return true;
  }

  return true;
}