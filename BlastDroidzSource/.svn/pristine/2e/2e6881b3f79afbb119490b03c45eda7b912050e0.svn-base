/******************************************************************************/
/*!
\file   DigiPenLogoState.c
\author Ryan Booth
\date   2/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "IntroState.h"
#include "Game.h"
#include "Graphics.h"
#include "MainMenuState.h"
#include "RenderComponent.h"
#include "Camera.h"
#include <stdlib.h>
#include "Entity.h"

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
//-----------------------------------------------------------------------------*/

static GameState *digipenlogoStateInstance;

static TextureAsset *textureTest;
static Sprite *spriteTest;
static Entity *entityTest;

static float time;


/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void OnGameExit(void);

static void Load(void);
static void Init(void);
static void Update(float dt);
static void Exit(void);
static void Unload(void);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

GameState* DigiPenLogoState_GetState()
{
  if (digipenlogoStateInstance)
  {
    return digipenlogoStateInstance;
  }

  digipenlogoStateInstance = malloc(sizeof(GameState));
  digipenlogoStateInstance->load = Load;
  digipenlogoStateInstance->init = Init;
  digipenlogoStateInstance->update = Update;
  digipenlogoStateInstance->exit = Exit;
  digipenlogoStateInstance->unload = Unload;

  GameState_RegisterExit(OnGameExit);

  return digipenlogoStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
  if (digipenlogoStateInstance)
  {
    free(digipenlogoStateInstance);
  }
}

/*!
\brief
*/
static void Load(void)
{
  time = 0.0;
}

/*!
\brief
*/
static void Init(void)
{
  RECT windowSize = Graphics_GetWindowResolution();
  Vector2 size;
  size.x = (float)(windowSize.right - windowSize.left);
  size.y = (float)(windowSize.bottom - windowSize.top);
  Camera_Init();
  textureTest = AssetTable_TryLoadTextureAsset("Logo", "Assets/DigiPenLogo.png");
  assert(textureTest);
  spriteTest = Sprite_Create(textureTest);
  entityTest = Entity_Create("Background");
  Entity_AttachComponent(entityTest, TransformComponent_Create());
  TransformComponent_AddScale(entityTest->transformComponent, size);
  Entity_AttachComponent(entityTest, RenderComponent_CreateSprite(spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_BACKGROUND));


}

/*!
Camera_Update(dt);
\brief
\param	dt Change in time (in seconds) since last game loop.
*/
static void Update(float dt)
{

  time += dt;
  //After 3 seconds chande to main menu
  if (time >= 3.0)
  {
    GameState_SetNextState(IntroState_GetState());
  }
}

/*!
\brief
*/
static void Exit(void)
{

}

/*!
\brief
*/
static void Unload(void)
{
  Entity_SetFlag(entityTest, EFLAG_DESTROY, 1);
}
