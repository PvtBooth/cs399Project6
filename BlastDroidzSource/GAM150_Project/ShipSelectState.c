/******************************************************************************/
/*!
\file   ShipSelectState.c
\author Ryan Booth
\date   2/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "ShipSelectState.h"
#include "Game.h"
#include "Graphics.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "MainMenuState.h"
#include "ColeTestState2TheReStatening.h"
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

static GameState *shipselectStateInstance;

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

GameState* ShipSelectState_GetState()
{
    if (shipselectStateInstance)
    {
        return shipselectStateInstance;
    }

    shipselectStateInstance = malloc(sizeof(GameState));
    shipselectStateInstance->load = Load;
    shipselectStateInstance->init = Init;
    shipselectStateInstance->update = Update;
    shipselectStateInstance->exit = Exit;
    shipselectStateInstance->unload = Unload;

    GameState_RegisterExit(OnGameExit);

    return shipselectStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
    if (shipselectStateInstance)
    {
        free(shipselectStateInstance);
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
  Vector2 size;
  size.x = (float)(windowSize.right - windowSize.left);
  size.y = (float)(windowSize.bottom - windowSize.top);
  Camera_Init();
  //textureTest = Texture_Load("Assets/prealphastates/tutorial.png");
  textureTest = AssetTable_TryLoadTextureAsset("Tutorial", "Assets/finalstates/tutorial.png");
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

    if (AEInputCheckTriggered(VK_RETURN))
    {
       // GameState_SetNextState(ColeTestState2_GetState());
    }
	else if (AEInputCheckTriggered(VK_ESCAPE))
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
