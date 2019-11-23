/******************************************************************************/
/*!
\file   OptionsState.c
\author Ryan Booth
\date   2/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "OptionsState.h"
#include "Game.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "MainMenuState.h"
#include <stdlib.h>
#include "Entity.h"
#include "GameStateStack.h"
#include "ShipSelectStackState.h"
#include "OptionsStackState.h"

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

static GameState *optionStateInstance;

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

GameState* OptionsState_GetState()
{
    if (optionStateInstance)
    {
        return optionStateInstance;
    }

    optionStateInstance = malloc(sizeof(GameState));
    optionStateInstance->load = Load;
    optionStateInstance->init = Init;
    optionStateInstance->update = Update;
    optionStateInstance->exit = Exit;
    optionStateInstance->unload = Unload;

    GameState_RegisterExit(OnGameExit);

    return optionStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
    if (optionStateInstance)
    {
        free(optionStateInstance);
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
  //textureTest = Texture_Load("Assets/prealphastates/options.png");
  textureTest = AssetTable_TryLoadTextureAsset("Options", "Assets/finalstates/options.png");
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

    if (AEInputCheckTriggered(VK_ESCAPE))
    {
        GameState_SetNextState(MainMenuState_GetState());
    }

    if (AEInputCheckTriggered('1'))
    {
        Window_Mode(FULLSCREEN);
    }

    if (AEInputCheckTriggered('2'))
    {
        Window_Mode(WINDOWED);
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

static void Window_Mode(WindowMode select)
{
    if (Graphics_GetWindowMode() != select)
    {
        Graphics_SetWindowMode(select);
        Graphics_ResetWindow();
        Graphics_SetWindowPosition(select);
    }
    
}