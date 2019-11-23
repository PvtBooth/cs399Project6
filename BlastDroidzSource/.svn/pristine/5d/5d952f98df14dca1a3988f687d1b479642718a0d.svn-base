/******************************************************************************/
/*!
\file   RenderScoreComponent.c
\author Ryan Booth
\date   3/30/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "RenderScoreComponent.h"
#include "Graphics.h"
#include "Entity.h"
#include "GraphicsManager.h"
#include "Text.h"
#include "TransformComponent.h"
#include <stdlib.h>

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
struct RenderScore
{
    Vector2 offset;
    int score;
    bool alive;
    bool isRegistered;
};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static float rate = 100.0f;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt);

static void OnAttach(Component *self);

static void Destroy(Component * self);

static void Render(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *RenderScoreComponent_Create(int score)
{
    Component* component = Component_Create();

    component->update = Update;
    component->destroy = Destroy;
    component->onAttach = OnAttach;

    RenderScore *data = calloc(1, sizeof(RenderScore));
    data->isRegistered = false;
    data->alive = true;
    data->score = score;

    component->data = data;

    component->type = COMPONENT_TYPE_RENDERSCORE;

    return component;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
    RenderScore *data = self->data;
    data->offset = Vector2_AddFloats(data->offset, 0.0f, (dt * rate));
    if (!data->isRegistered)
    {
        GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);
        if (graphicsManager)
        {
            GraphicsManager_RegisterComponentRender(graphicsManager, Render, self, RENDER_LAYER_WORLD_HUD);
            data->isRegistered = true;
        }
    }
}

static void OnAttach(Component *self)
{
    UNREFERENCED_PARAMETER(self);
}

static void Destroy(Component* self)
{
    //RenderScore *data = self->data;

    GraphicsManager_RemoveComponentRender(Entity_GetGraphicsManager(self->parent), self, RENDER_LAYER_WORLD_HUD);

    free(self->data); //TEST

    free(self);
}

static void Render(Component *self)
{
    RenderScore *data = self->data;

    Vector2 position = Vector2_Add(TransformComponent_GetPosition(self->parent->transformComponent), data->offset);


    char text[5]; //temporarly 5 bytes (or 5 digits)


    _itoa_s(data->score, text, 5, 10);

    //TODO: Add render stoof here
    Text_Print(position, text, 15.0f, RENDER_LAYER_PLAYER);
}

