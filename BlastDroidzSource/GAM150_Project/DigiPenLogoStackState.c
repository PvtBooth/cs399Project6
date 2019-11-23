/******************************************************************************/
/*!
\file   DigiPenLogoStackState.c
\author Ryan Booth
\date   3/9/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "DigiPenLogoStackState.h"
#include "GameStateStack.h"
#include "IntroStackState.h"
#include "Graphics.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "RenderComponent.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct DigiPenLogoStackState DigiPenLogoStackState;
struct DigiPenLogoStackState
{
    StackState stackState;

    TextureAsset *textureTest;
    Sprite *spriteTest;
    Entity *entityTest;



    float time;

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
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
StackState *DigiPenLogoStackState_Create()
{
    DigiPenLogoStackState *state = calloc(1, sizeof(DigiPenLogoStackState));
    state->stackState.init = Init;
    state->stackState.update = Update;
    state->stackState.exit = Exit;
    state->stackState.render = Render;
    state->stackState.renderBelow = false;
    state->stackState.updateBelow = false;
    state->stackState.name = "DigiPenLogo";

    state->time = 0.0f;

    return &state->stackState;
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
    DigiPenLogoStackState *state = (DigiPenLogoStackState*)self;
    self->entityManager = EntityManager_Create(self);
    self->graphicsManager = GraphicsManager_Create(self);

    RECT windowSize = Graphics_GetWindowResolution();
    Vector2 size;
    size.x = (float)(windowSize.right - windowSize.left);
    size.y = (float)(windowSize.bottom - windowSize.top);
    Camera_Init();
    
    state->textureTest = AssetTable_TryLoadTextureAsset("ShipSelect", "Assets/finalstates/Splash.png");
    assert(state->textureTest);
    state->spriteTest = Sprite_Create(state->textureTest);
    state->entityTest = EntityManager_CreateEntity(self->entityManager, "Background");
    Entity_AttachComponent(state->entityTest, TransformComponent_Create());
    TransformComponent_AddScale(state->entityTest->transformComponent, size);
    Entity_AttachComponent(state->entityTest, RenderComponent_CreateSprite(state->spriteTest, RENDER_BEHAVIOR_SPRITE, RENDER_LAYER_BACKGROUND));

}

static void Update(StackState *self, float dt)
{
    DigiPenLogoStackState *state = (DigiPenLogoStackState*)self;

    state->time += dt;
    //After 3 seconds chande to main menu
    if (state->time >= 3.0)
    {
        GameStateStack_PushState(IntroStackState_Create());
        GameStateStack_PopState();
    }
}

static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
}
static void Exit(StackState *self)
{
    EntityManager_DestroyAllImmediate(self->entityManager);
    EntityManager_Destroy(self->entityManager);
    GraphicsManager_Destroy(self->graphicsManager);
}