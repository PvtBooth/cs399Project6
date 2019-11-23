/******************************************************************************/
/*!
\file   MainMenuState.c
\author Ryan Booth
\date   2/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "MainMenuState.h"
#include "Game.h"
#include "Graphics.h"
#include "ColeTestState2TheReStatening.h"
#include "RenderComponent.h"
#include "Camera.h"
#include <stdlib.h>
#include "Entity.h"
#include "ShipSelectState.h"
#include "OptionsState.h"
#include "Text.h"

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

static GameState *mainmenuStateInstance;

static TextureAsset *textureTest;
static Sprite *spriteTest;
static Entity *entityTest;

static TextureAsset *texture1;
static Sprite *sprite1;
static SpriteSheet *spriteSheet;
static Entity *entity1;
static Vector2 position1 = { -400, 100 };
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

GameState* MainMenuState_GetState()
{
    if (mainmenuStateInstance)
    {
        return mainmenuStateInstance;
    }

    mainmenuStateInstance = malloc(sizeof(GameState));
    mainmenuStateInstance->load = Load;
    mainmenuStateInstance->init = Init;
    mainmenuStateInstance->update = Update;
    mainmenuStateInstance->exit = Exit;
    mainmenuStateInstance->unload = Unload;

    GameState_RegisterExit(OnGameExit);

    return mainmenuStateInstance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void OnGameExit(void)
{
    if (mainmenuStateInstance)
    {
        free(mainmenuStateInstance);
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
  //textureTest = Texture_Load("Assets/prealphastates/mainmenu.png");
  textureTest = AssetTable_TryLoadTextureAsset("MainMenuState", "Assets/prealphastates/mainmenu.png");
  assert(textureTest);
	spriteTest = Sprite_Create(textureTest);
	entityTest = Entity_Create("Background");
	Entity_AttachComponent(entityTest, TransformComponent_Create());
  TransformComponent_AddScale(entityTest->transformComponent, size);
	Entity_AttachComponent(entityTest, RenderComponent_CreateSprite(spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_BACKGROUND));

  Vector2 scale1 = {65, 65};

  //texture1 = Texture_Load("Assets/Text/Font_fixed.png");
  texture1 = AssetTable_TryLoadTextureAsset("Font", "Assets/Text/Font_fixed.png");

  spriteSheet = Text_SpriteSheetCreate("COLE", 11, 3, texture1);

  sprite1 = Sprite_Create(spriteSheet->texture);

  Sprite_SetSpriteSheet(sprite1, spriteSheet);

  entity1 = Entity_Create("Text");

  Entity_AttachComponent(entity1, TransformComponent_Create());

  TransformComponent_AddScale(entity1->transformComponent, scale1);

  Entity_AttachComponent(entity1, RenderComponent_CreateSprite(sprite1, RENDER_BEHAVIOR_SPRITESHEET, RENDER_LAYER_NEUTRAL));

  TransformComponent_SetPosition(entity1->transformComponent, position1);


}

/*!
\brief
\param	dt Change in time (in seconds) since last game loop.
*/
static void Update(float dt)
{


    Camera_Update(dt);
    //Rudimentary controls keyboard controls
    if (AEInputCheckTriggered('P'))
    {
        GameState_SetNextState(ShipSelectState_GetState());
    }
    if (AEInputCheckTriggered('O'))
    {
        GameState_SetNextState(OptionsState_GetState());
    }
    else if (AEInputCheckTriggered(VK_ESCAPE))
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
    //Entity_SetFlag(entity1, EFLAG_DESTROY, 1);
}
