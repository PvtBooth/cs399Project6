/******************************************************************************/
/*!
	\file   LaserComponent.c
	\author Henry Brobeck
	\date   2/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "LaserComponent.h"
#include "Vector2.h"
#include "Graphics.h"
#include "Entity.h"
#include "ColliderComponent.h"
#include "HealthSubject.h"
#include "Camera.h"



/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/
#define RENDER_LAYER 3
/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct LaserData LaserData;
struct LaserData
{
  Vector2 offset;
  float range;
  float cooldown;
  float delay;
  bool isShooting;
  int damage;
  RaycastData raycast;
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



static void Update(Component* self, float dt);
static void Destroy(Component * self);
static void OnAttach(Component * self);
static void OnOtherAttach(Component * self, Component * component);
static void Render(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief Create a new Laser Component, it shoots lasers
 * \param offset offset from the center of the entity to the firing point
 * \param range Range of the laser shot
 * \param cooldown Time in seconds between shots
 * \return the new component
 */
Component* LaserComponent_Create(Vector2 offset, float range, float cooldown, int damage)
{
  Component* newComponent = Component_Create();
  newComponent->update = Update;
  newComponent->destroy = Destroy;
  LaserData *data = calloc(1, sizeof(LaserData));
  data->cooldown = cooldown;
  data->range = range;
  data->offset = offset;
  data->isShooting = false;
  data->damage = damage;
  newComponent->data = data;

  
  //GraphicsManager_RegisterComponentRender(Render, newComponent, RENDER_LAYER);

  return newComponent;
}

void LaserComponent_Fire(Component* component)
{
  LaserData *data = component->data;
  if (data->delay > 0 || data->isShooting)
  {
    return;
  }
  data->isShooting = true;
  TransformComponent *transform = Entity_GetTransform(component->parent);
  Vector2 pos = TransformComponent_GetPosition(transform);
  pos = Vector2_Add(pos, data->offset);

  LayerMask layerMask = Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_ENEMY);
  
  if(PhysicsManager_RaycastWithMask(&data->raycast, pos, TransformComponent_GetForward(transform), data->range, layerMask))
  {
    ColliderComponent *collider = data->raycast.collider;
    Entity *target = ColliderComponent_GetEntity(collider);
    HealthSubject_DamageNotify(component->parent, target, (float)data->damage);
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/


static void Update(Component* self, float dt)
{
  LaserData *data = self->data;
  data->delay -= dt;
  if(data->delay < 0)
  {
    if(data->isShooting)
    {
      data->isShooting = false;
      data->delay = data->cooldown;
    }
  }
}

static void Destroy(Component* self)
{
  //GraphicsManager_RemoveComponentRender(self, RENDER_LAYER);
  free(self->data);
  free(self);
}

static void OnAttach(Component* self)
{
    UNREFERENCED_PARAMETER(self);
}

static void OnOtherAttach(Component* self, Component* component)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(component);
}
static void Render(Component *self)
{
  LaserData *data = self->data;
  if(!data->isShooting)
  {
    return;
  }
  TransformComponent *transform = Entity_GetTransform(self->parent);
  float length = data->raycast.distance;
  Vector2 pos = TransformComponent_GetPosition(transform);
  pos = Vector2_Add(pos, data->offset);
  Vector2 forward = TransformComponent_GetForward(transform);
  Vector2 endPos;
  endPos.x = pos.x + forward.x * length;
  endPos.y = pos.y + forward.y * length;

  Vector2 centerPos = Vector2_Average(endPos, pos);
  Vector2 size;
  size.x = length;
  size.y = 5;
  Graphics_DrawRect(centerPos, size, TransformComponent_GetRotation(transform), 1, 0, 0, 1);
}