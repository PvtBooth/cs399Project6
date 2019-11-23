/******************************************************************************/
/*!
	\file   SpriteComponent.c
	\author Henry Brobeck
	\date   2/6/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "SpriteComponent.h"
#include "Entity.h"
#include "Graphics.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct SpriteData SpriteData;
struct SpriteData
{
  Sprite *sprite;
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
static void Render(Component *self);

static SpriteData *GetSpriteData(Component *component);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* SpriteComponent_Create(Sprite *sprite)
{
  Component *component = Component_Create();
  component->update = Update;
  component->type = COMPONENT_TYPE_SPRITE;
  component->destroy = Destroy;
  SpriteData *data = calloc(1, sizeof(SpriteData));
  data->sprite = sprite;
  component->data = data;
  
  Graphics_RegisterComponentRender(Render, component, 1 );

  return component;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Update(Component *self, float dt)
{
  
  
}
static void Destroy(Component *self)
{
  Graphics_RemoveComponentRender(self, 1);
  free(self->data);
  free(self);
}
static SpriteData *GetSpriteData(Component *component)
{
  return (SpriteData*)component->data;
}
static void Render(Component *self)
{
  SpriteData *data = GetSpriteData(self);
  Matrix3x3 *transform = TransformComponent_GetTransformMatrix(Entity_GetTransform(self->parent));
  Sprite_Render(data->sprite, transform);
}