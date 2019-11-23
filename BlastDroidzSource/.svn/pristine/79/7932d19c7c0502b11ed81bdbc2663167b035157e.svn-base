/******************************************************************************/
/*!
\file   ThankYouState.c
\author Ryan Booth
\date   2/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "ThankYouState.h"
#include "Game.h"
#include "Graphics.h"
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
//----------------------------------------------------------------------------*/

static GameState *thankyouStateInstance;

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

GameState* ThankYouState_GetState()
{
    if (thankyouStateInstance)
    {
        return thankyouStateInstance;
    }

    thankyouStateInstance = malloc(sizeof(GameState));
    thankyouStateInstance->load = Load;
    thankyouStateInstance->init = Init;
    thankyouStateInstance->update = Update;
    thankyouStateInstance->exit = Exit;
    thankyouStateInstance->unload = Unload;

    GameState_RegisterExit(OnGameExit);

    return thankyouStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
    if (thankyouStateInstance)
    {
        free(thankyouStateInstance);
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
  //textureTest = Texture_Load("Assets/finalstates/credits.png");
  textureTest = AssetTable_TryLoadTextureAsset("ThankYouS", "Assets/finalstates/credits.png");
  assert(textureTest);
	spriteTest = Sprite_Create(textureTest);
	entityTest = Entity_Create("Background");
	Entity_AttachComponent(entityTest, TransformComponent_Create());
  TransformComponent_AddScale(entityTest->transformComponent, size);
	Entity_AttachComponent(entityTest, RenderComponent_CreateSprite(spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_PARTICLE));
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
        Game_SetRunning(0);
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
