/******************************************************************************/
/*!
	\file   RenderComponent.c
	\author Henry Brobeck
	\date   1/21/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Camera.h"
#include "Text.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/
#define DEFAULT_RENDER_LAYER RENDER_LAYER_NEUTRAL
/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct RenderData RenderData;
struct RenderData
{
    //! Will render this primitive
  Primitive *primitive; //Pork bacon pork pork bacon
  MeshGroup *group;
  Sprite *sprite;
  Vector2 paralax;
  RENDER_BEHAVIOR behavior;
  int renderLayer;

  bool registered;

  TransformComponent *transform;

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

static void Update(Component *self, float dt);
static void Destroy(Component *self);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static Component* Copy(Component *self);
static void Render(Component *self);

static void RenderSprite(Component *self);
static void RenderSpriteParalax(Component *self);
static void RenderMeshGroup(Component *self);
static void RenderSpriteSheet(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief Creates a new RenderComponent using a primitive
 * \param primitive The primitive to render with the component
 * \return A new Component
 * \deprecated Primitives work but aren't good, use MeshGroups instead
 */
Component* RenderComponent_CreatePrimitive(Primitive* primitive)
{
  Component *component = Component_Create();
  RenderData *data = calloc(1,sizeof(RenderData));
  
  data->primitive = primitive;
  data->group = NULL;
  data->transform = NULL;

  component->update = Update;
  component->destroy = Destroy;
  component->onAttach = OnAttach;
  component->onOtherAttach = OnOtherAttach;
  component->copy = &Copy;
  component->data = data;

  

  return component;
}

Component* RenderComponent_CreateMeshGroup(MeshGroup* group, RENDER_LAYER renderLayer)
{
  Component *component = Component_Create();
  RenderData *data = calloc(1,sizeof(RenderData));

  data -> group = group;
  data->primitive = NULL;
  data->transform = NULL;
  data->sprite = NULL;
  data->behavior = RENDER_BEHAVIOR_GROUP;
  data->renderLayer = renderLayer;

  component->update = Update;
  component->destroy = Destroy;
  component->onAttach = OnAttach;
  component->onOtherAttach = OnOtherAttach;
  component->copy = &Copy;

  component->data = data;
  

  return component;
}

/**
 * \brief Creates a new sprite renderer with given render behavior
 * \param sprite The sprite to use for the render
 * \param behavior The behavior type, recommended either RENDER_BEHAVIOR_SPRITE or RENDER_BEHAVIOR_PARALAX
 * \return the new component
 */
Component* RenderComponent_CreateSprite(Sprite* sprite, RENDER_BEHAVIOR behavior, RENDER_LAYER renderLayer)
{
  Component *component = Component_Create();
  RenderData *data = calloc(1, sizeof(RenderData));
  data->sprite = sprite;
  data->primitive = NULL;
  data->transform = NULL;
  data->group = NULL;
  data->paralax = Vector2_Zero();
  data->behavior = behavior;
  data->renderLayer = renderLayer;

  component->update = Update;
  component->destroy = Destroy;
  component->onAttach = OnAttach;
  component->onOtherAttach = OnOtherAttach;
  component->copy = &Copy;

  component->data = data;

  

  return component;
}

Component* RenderComponent_Copy(RenderData *renderData)
{
  Component *component = Component_Create();
  RenderData *data = (RenderData*)calloc(1, sizeof(RenderData));
  data->sprite = renderData->sprite;
  data->primitive = renderData->primitive;
  data->group = renderData->group;
  data->paralax = renderData->paralax;
  data->behavior = renderData->behavior;
  data->renderLayer = renderData->renderLayer;

  component->update = &Update;
  component->destroy = &Destroy;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;

  component->data = data;

  

  return component;
}

Component* RenderComponent_SetParalax(Component* component, Vector2 paralax)
{
  RenderData *data = component->data;
  data->paralax = paralax;
  return component;
}

void RenderComponent_UnRegister(Component* component)
{
  RenderData *data = component->data;
  if(data->registered)
  {
    GraphicsManager_RemoveComponentRender(Entity_GetGraphicsManager(component->parent), component, data->renderLayer);
    data->registered = false;
  }
  
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
void Update(Component *self, float dt)
{
  UNREFERENCED_PARAMETER(dt);
  RenderData *data = self->data;

  if(!data->registered)
  {
    GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);
    if(graphicsManager)
    {
      GraphicsManager_RegisterComponentRender(graphicsManager, Render, self, data->renderLayer);
      data->registered = true;
    }
  }

  
  
  
  
  
}
void Destroy(Component *self)
{

  RenderData *data = (RenderData*)self->data;
  GraphicsManager_RemoveComponentRender(Entity_GetGraphicsManager(self->parent), self, data->renderLayer);

  
  free(data);
  free(self);
}

static void OnAttach(Component *self)
{
  RenderData *data = (RenderData*)self->data;
  GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);
  if(graphicsManager && !data->registered)
  {
    GraphicsManager_RegisterComponentRender(graphicsManager, Render, self, data->renderLayer);
    data->registered = true;
  }
  
  if (data->transform == NULL)
  {
    Component *transComponent = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
    if (transComponent)
    {
      data->transform = transComponent->data;
    }
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  RenderData *data = (RenderData*)self->data;
  if (data->transform)
    return;

  if(component->type == COMPONENT_TYPE_TRANSFORM)
  {
    data->transform = (TransformComponent*)component->data;
  }
}

static Component* Copy(Component *self)
{
  return RenderComponent_Copy((RenderData*)self->data);
}

static void Render(Component *self)
{
  if(Entity_GetFlag(self->parent, EFLAG_FREEZE))
  {
    return;
  }
  RenderData *data = self->data;
  switch (data->behavior)
  {
  case RENDER_BEHAVIOR_PARALAX:
    RenderSpriteParalax(self);
    break;
  case RENDER_BEHAVIOR_GROUP:
    RenderMeshGroup(self);
    break;
  case RENDER_BEHAVIOR_SPRITE:
    RenderSprite(self);
    break;
  case RENDER_BEHAVIOR_SPRITESHEET:
    RenderSpriteSheet(self);
    break;
  default: return;
  }
}

static void RenderSprite(Component *self)
{
  RenderData *data = self->data;
  Matrix3x3 *transform = TransformComponent_GetTransformMatrix(Entity_GetTransform(self->parent));
  Sprite_Render(data->sprite, transform);
}

static void RenderSpriteSheet(Component *self)
{
  UNREFERENCED_PARAMETER(self);
//  RenderData *data = self->data;
//  Matrix3x3 *transform = TransformComponent_GetTransformMatrix(Entity_GetTransform(self->parent));
  //Graphics_RenderText(data->sprite, transform);
}
static void RenderSpriteParalax(Component *self)
{
  RenderData *data = self->data;
  Vector2 cameraPos = Camera_GetPosition();
  data->sprite->uOffset = cameraPos.x * data->paralax.x;
  data->sprite->vOffset = cameraPos.y * data->paralax.y;
//  float cameraSize = Camera_GetSize();
  //Vector2 pos = {0,0};
  //Vector2 scale = { 1,1 };

  /*scale.x = 25 / (cameraSize / 2);
  scale.y = 25 / (cameraSize / 2);*/

  TransformComponent *transform = self->parent->transformComponent;
  TransformComponent_SetPosition(transform, cameraPos);
  //TransformComponent_SetScale(transform, scale);
  //Sprite_RenderPosScale(data->sprite, pos , scale);
  Sprite_Render(data->sprite, TransformComponent_GetCameraTransformMatrix(transform));
}
static void RenderMeshGroup(Component *self)
{
  RenderData *data = self->data;
//  Vector2 position = TransformComponent_GetPosition(data->transform);
  MeshGroup_Render(data->group, TransformComponent_GetCameraTransformMatrix(data->transform));
}