/******************************************************************************/
/*!
	\file   RaycastProjectileComponent.c
	\author Evan Kau
	\date   3/2/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "RaycastProjectileComponent.h"
#include "Component.h"
#include "TransformComponent.h"
#include "Physics_Layers.h"
#include <stdlib.h>
#include "Physics_BroadPhase.h"
#include "Camera.h"
#include "ShotLogData.h"
#include "HealthSubject.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct RaycastProjectileData
{
  TransformComponent *transform;
  float speed;
  float distanceCovered;
  float maxDistance;
  float damage;
  float force;
  LayerMask targetLayer;

  ShotLogData shotLog;
} RaycastProjectileData;

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
static void OnOtherAttach(Component *self, Component *component);
static Component* Copy(Component *self);
static void DealDamage(Component *self, RaycastData *raycastData);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* RaycastProjectileComponent_Create(float speed, float maxDistance, float damage, float force, LayerMask targetLayer)
{
  Component *component = Component_Create();
  RaycastProjectileData *data = (RaycastProjectileData*)calloc(1, sizeof(RaycastProjectileData));

  data->speed = speed;
  data->maxDistance = maxDistance;
  data->damage = damage;
  data->force = force;
  data->targetLayer = targetLayer;

  component->data = data;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;
  component->type = COMPONENT_TYPE_RAYCASTPROJECTILE;

  return component;
}

Component* RaycastProjectileComponent_Copy(RaycastProjectileData *data)
{
  Component *projectile = RaycastProjectileComponent_Create(data->speed, data->maxDistance, data->damage, data->force, data->targetLayer);
  RaycastProjectileComponent_SetShotLog(projectile->data, &data->shotLog);
  return projectile;
}

void RaycastProjectileComponent_SetShotLog(RaycastProjectileData *data, ShotLogData *logTemplate)
{
  data->shotLog = *logTemplate;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  RaycastProjectileData *data = (RaycastProjectileData*)self->data;
  TransformComponent *transform = (TransformComponent*)data->transform;
  RaycastData raycastData;
  
  float dist = min(data->maxDistance - data->distanceCovered, data->speed * dt);

  if (PhysicsManager_RaycastWithMask(&raycastData, TransformComponent_GetPosition(transform), TransformComponent_GetForward(transform), dist, data->targetLayer))
  {
    DealDamage(self, &raycastData);
  }
  //Camera_DrawLine(raycastData.origin.x, raycastData.origin.y, 1, 0.5f, 0, 1, raycastData.point.x, raycastData.point.y, 1, 0.5f, 0, 1, true);

  TransformComponent_SetPosition(transform, raycastData.point);
  data->distanceCovered += data->speed * dt; //To make sure that it goes over [for below], we just use the max travel value

  if (data->distanceCovered >= data->maxDistance)
  {
    Entity_SetFlag(self->parent, EFLAG_DESTROY, true);
  }
}

static void OnAttach(Component *self)
{
  Component *transform = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  if (transform)
  {
    RaycastProjectileData *data = (RaycastProjectileData*)self->data;
    data->transform = (TransformComponent*)transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    RaycastProjectileData *data = (RaycastProjectileData*)self->data;
    data->transform = (TransformComponent*)component->data;
  }
}

static Component* Copy(Component *self)
{
  return RaycastProjectileComponent_Copy((RaycastProjectileData*)self->data);
}

static void DealDamage(Component *self, RaycastData *raycastData)
{
  RaycastProjectileData *data = (RaycastProjectileData*)self->data;
  ShotLogData_LogDamageDealt(&data->shotLog, data->damage, raycastData->point, raycastData->entity);
  HealthSubject_DamageNotify(self->parent, raycastData->entity, data->damage);
  Vector2 impulse = Vector2_Scale(TransformComponent_GetForward(data->transform), data->force);
  if (raycastData->rigidbody)
    RigidbodyComponent_ApplyForceAtPoint(raycastData->rigidbody, impulse, raycastData->point, FORCE_TYPE_IMPULSE);
  Entity_SetFlag(self->parent, EFLAG_DESTROY, true);
}