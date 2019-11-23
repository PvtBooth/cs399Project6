/******************************************************************************/
/*!
\file   IntroState.c
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
#include "RenderComponent.h"
#include "Camera.h"
#include "MainMenuState.h"
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
//----------------------------------------------------------------------------*/

static GameState *introStateInstance;

static TextureAsset *textureTest;
static Sprite *spriteTest;
static Entity *entityTest;


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

GameState* IntroState_GetState()
{
    if (introStateInstance)
    {
        return introStateInstance;
    }

    introStateInstance = malloc(sizeof(GameState));
    introStateInstance->load = Load;
    introStateInstance->init = Init;
    introStateInstance->update = Update;
    introStateInstance->exit = Exit;
    introStateInstance->unload = Unload;

    GameState_RegisterExit(OnGameExit);

    return introStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
    if (introStateInstance)
    {
        free(introStateInstance);
    }
}

/*!
\brief
*/
static void Load(void)
{
}

/*!
\brief
*/
static void Init(void)
{
	RECT windowSize = Graphics_GetWindowResolution();
  Camera_Init();
  Vector2 size;
  size.x = (float)(windowSize.right - windowSize.left);
  size.y = (float)(windowSize.bottom - windowSize.top);
  //textureTest = Texture_Load("Assets/prealphastates/Intro.png");
  textureTest = AssetTable_TryLoadTextureAsset("IntroTex", "Assets/prealphastates/Intro.png");
  assert(textureTest);
	spriteTest = Sprite_Create(textureTest);
	entityTest = Entity_Create("Background");
	Entity_AttachComponent(entityTest, TransformComponent_Create());
  TransformComponent_AddScale(entityTest->transformComponent, size);
	Entity_AttachComponent(entityTest, RenderComponent_CreateSprite(spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_BACKGROUND));
}

/*!
\brief
\param	dt Change in time (in seconds) since last game loop.
*/
static void Update(float dt)
{
    Camera_Update(dt);

    //After 3 seconds chande to main menu
    if (AEInputCheckTriggered(VK_RETURN))
    {
        GameState_SetNextState(MainMenuState_GetState());
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
