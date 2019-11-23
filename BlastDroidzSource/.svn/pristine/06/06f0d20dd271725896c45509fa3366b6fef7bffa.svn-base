/******************************************************************************/
/*!
	\file   BarComponent.c
	\author Henry Brobeck
	\date   3/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "BarComponent.h"
#include "Graphics.h"
#include "Entity.h"
#include "GraphicsManager.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

struct HUDBar
{
  float current, max;
  Vector2 scale;
  float padding;
  float r, g, b, a;
  HUDBAR_TYPE type;

  bool show;

  HUDBar *next;
};

typedef struct BarData
{
  HUDBar *head;
  Vector2 offset;

  bool notRegistered;
  bool isWorld;
}BarData;


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

static void OnAttach(Component *self);

static void Destroy(Component * self);

static void Render(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *BarComponent_Create(Vector2 offset)
{
  Component* component = Component_Create();

  component->update = Update;
  component->destroy = Destroy;
  component->onAttach = OnAttach;

  BarData *data = calloc(1, sizeof(BarData));
  data->head = NULL;
  data->offset = offset;
  data->notRegistered = true;
  data->isWorld = true;

  component->data = data;

  component->type = COMPONENT_TYPE_BARCOMPONENT;

  

  
  return component;
}

HUDBar* BarComponent_CreateBar(Component* component, float max, Vector2 scale, float padding, float r, float g, float b, float a)
{
  HUDBar *bar = calloc(1, sizeof(HUDBar));
  if(!bar)
  {
    return NULL;
  }

  bar->max = max;
  bar->current = max;
  bar->scale = scale;
  bar->padding = padding;
  bar->r = r;
  bar->g = g;
  bar->b = b;
  bar->a = a;

  bar->show = true;

  bar->type = HUDBAR_TYPE_LEFT;

  BarComponent_AddBar(component, bar);

  return bar;

}

void BarComponent_DestroyBar(Component* parent, HUDBar* bar)
{
  BarComponent_RemoveBar(parent, bar);
  free(bar);
}

void BarComponent_AddBar(Component* component, HUDBar* bar)
{
  BarData *data = component->data;
  
  bar->next = data->head;
  data->head = bar;
}

void BarComponent_RemoveBar(Component* component, HUDBar* bar)
{
  BarData *data = component->data;
  if(!bar || !data->head)
  {
    return;
  }
  if(bar == data->head)
  {
    data->head = data->head->next;
    return;
  }
  HUDBar * iterator = data->head->next;
  HUDBar * previous = data->head;
  while(iterator)
  {
    if(iterator == bar)
    {
      data->head->next = iterator->next;
      return;
    }
    previous = iterator;
    iterator = iterator->next;
  }
}

void BarComponent_SetCurrent(HUDBar* bar, float current)
{
  bar->current = current;
}

void BarComponent_SetMax(HUDBar* bar, float max)
{
  bar->max = max;
}

void BarComponent_SetScale(HUDBar* bar, Vector2 scale)
{
  bar->scale = scale;
}


void BarComponent_SetRGBA(HUDBar* bar, float r, float g, float b, float a)
{
  bar->r = r;
  bar->g = g;
  bar->b = b;
  bar->a = a;
}

float BarComponent_GetCurrent(HUDBar* bar)
{
  return bar->current;
}

float BarComponent_GetMax(HUDBar* bar)
{
  return bar->max;
}

Vector2 BarComponent_GetScale(HUDBar* bar)
{
  return bar->scale;
}

void BarComponent_SetBarType(HUDBar* bar, HUDBAR_TYPE type)
{
  bar->type = type;
}

HUDBAR_TYPE BarComponent_GetBarType(HUDBar* bar)
{
  return bar->type;
}

void BarComponent_SetWorld(Component* component, bool isWorld)
{
  if(!component || !component->parent)
  {
    return;
  }
  
  BarData *data = (BarData*)component->data;

  if(data->isWorld == isWorld)
  {
    return;
  }
  //find the render layer that the component is current in
  RENDER_LAYER renderLayer = RENDER_LAYER_WORLD_HUD;
  if(!data->isWorld)
  {
    renderLayer = RENDER_LAYER_TOP_LEFT;
  }
  //remove from the render layer
  if(!data->notRegistered)
  {
    GraphicsManager_RemoveComponentRender(Entity_GetGraphicsManager(component->parent), component, renderLayer);
  }
  

  //swap the render layer with the one we want to add it to
  if(renderLayer == RENDER_LAYER_WORLD_HUD)
  {
    renderLayer = RENDER_LAYER_TOP_LEFT;
  }
  else
  {
    renderLayer = RENDER_LAYER_WORLD_HUD;
  }
  //add it back to the graphics manager in the correct layer
  GraphicsManager_RegisterComponentRender(Entity_GetGraphicsManager(component->parent), Render, component, renderLayer);
  data->notRegistered = false;


  //set the new isWorld value
  data->isWorld = isWorld;
}

void BarComponent_SetVisible(HUDBar* bar, bool show)
{
  if(bar)
  {
    bar->show = show;
  }
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  BarData *data = self->data;
  if(data->notRegistered)
  {
    GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);
    if (graphicsManager)
    {
      RENDER_LAYER renderLayer = RENDER_LAYER_WORLD_HUD;
      if(!data->isWorld)
      {
        renderLayer = RENDER_LAYER_TOP_LEFT;
      }
      GraphicsManager_RegisterComponentRender(graphicsManager, Render, self, renderLayer);
      data->notRegistered = false;
    }
  }
}

static void OnAttach(Component *self)
{
    UNREFERENCED_PARAMETER(self);
}

static void Destroy(Component* self)
{
  BarData *data = self->data;
  HUDBar *bar = data->head;
  while(bar)
  {
    HUDBar* next = bar->next;
    BarComponent_DestroyBar(self, bar);
    bar = next;
  }
  RENDER_LAYER renderLayer = RENDER_LAYER_WORLD_HUD;
  if (!data->isWorld)
  {
    renderLayer = RENDER_LAYER_TOP_LEFT;
  }
  GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);

  GraphicsManager_RemoveComponentRender(graphicsManager, self, renderLayer);
  
  free(data);

  free(self);

}

static void Render(Component *self)
{
  BarData *data = self->data;

  Vector2 position = TransformComponent_GetPosition(self->parent->transformComponent);
  
  position = Vector2_Add(position, data->offset);
  
  HUDBar *bar = data->head;
  float paddingOffset = 0;
  while(bar)
  {
    if(!bar->show)
    {
      bar = bar->next;
      continue;
    }
    Vector2 barPos = position;
    Vector2 barScale = bar->scale;

    barPos.y += paddingOffset;
    barScale.x = (bar->current / bar->max) * bar->scale.x;
    switch(bar->type)
    {
    case HUDBAR_TYPE_LEFT:
      barPos.x += barScale.x / 2.0f;
      break;
    case HUDBAR_TYPE_RIGHT:
      barPos.x += bar->scale.x - barScale.x / 2.0f;
      
      break;
    case HUDBAR_TYPE_CENTER:
      barPos.x += bar->scale.x / 2.0f;
      break;
    default: ;
    }

    if(data->isWorld)
    {
      Graphics_DrawRect(barPos, barScale, 0, bar->r, bar->g, bar->b, bar->a);
    }
    else
    {
      Graphics_DrawRectCamera(barPos, barScale, 0, bar->r, bar->g, bar->b, bar->a);
    }

    paddingOffset += bar->padding + bar->scale.y;

    bar = bar->next;

  }
}

