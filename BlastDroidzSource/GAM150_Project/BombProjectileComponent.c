/******************************************************************************/
/*!
	\file   BombProjectileComponent.c
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "BombProjectileComponent.h"
#include "Physics_Layers.h"
#include "Component.h"
#include <stdlib.h>
#include "TransformComponent.h"
#include "Entity.h"
#include "RigidbodyComponent.h"
#include "HealthSubject.h"
#include "ParticleSystem.h"
#include "LifetimeComponent.h"
#include "EntityManager.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct BombProjectileData
{
  float maxTime;
  float curTime;
  float speed;
  LayerMask targetLayers;
  TransformComponent *transform;
  ShotLogData shotLog;
} BombProjectileData;

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
static void EventHandle(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal);
static void Explode(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* BombProjectileComponent_Create(float speed, float timer, LayerMask targetLayer)
{
  Component *component = Component_Create();
  BombProjectileData *data = (BombProjectileData*)calloc(1, sizeof(BombProjectileData));

  data->maxTime = timer;
  data->speed = speed;
  data->targetLayers = targetLayer;

  component->data = data;
  component->type = COMPONENT_TYPE_BOMBPROJECTILE;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;
  component->eventHandle = &EventHandle;

  return component;
}

Component* BombProjectileComponent_Copy(BombProjectileData *data)
{
  return BombProjectileComponent_Create(data->speed, data->maxTime, data->targetLayers);
}

void BombProjectileComponent_SetShotLog(BombProjectileData *data, ShotLogData *logTemplate)
{
  data->shotLog = *logTemplate;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  BombProjectileData *data = (BombProjectileData*)self->data;
  TransformComponent *transform = (TransformComponent*)data->transform;

  //Entity_SetFlag(self->parent, EFLAG_DESTROY, true);
  //Camera_DrawLine(raycastData.origin.x, raycastData.origin.y, 0.2f, 1, 0.65f, 1, raycastData.point.x, raycastData.point.y, 0.2f, 1, 0.65f, 1, true);

  TransformComponent_AddPosition(transform, Vector2_Scale(TransformComponent_GetForward(transform), data->speed * dt));
  data->curTime += dt; //To make sure that it goes over [for below], we just use the max travel value

  if (data->curTime >= data->maxTime)
  {
    Explode(self);
  }
}

static void OnAttach(Component *self)
{
  Component *transform = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  if (transform)
  {
    BombProjectileData *data = (BombProjectileData*)self->data;
    data->transform = (TransformComponent*)transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    BombProjectileData *data = (BombProjectileData*)self->data;
    data->transform = (TransformComponent*)component->data;
  }
}

static Component* Copy(Component *self)
{
  return BombProjectileComponent_Copy((BombProjectileData*)self->data);
}

static void EventHandle(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal)
{
    UNREFERENCED_PARAMETER(isLocal);
    UNREFERENCED_PARAMETER(eventData);
  if (eventType == EVENT_COLLISION)
  {
    Explode(self);
  }
}

static void Explode(Component *self)
{
  BombProjectileData *data = (BombProjectileData*)self->data;
  TransformComponent *transform = (TransformComponent*)data->transform;
  int hitColliderCount;
  const ColliderComponent **hitColliders = PhysicsManager_CircleCast(TransformComponent_GetPosition(transform), 700.0f, &hitColliderCount);
  float damage = 6;
  int i;

  for (i = 0; i < hitColliderCount; i++)
  {
    if (Physics_Layers_CheckLayerMask(data->targetLayers, ColliderComponent_GetLayer(hitColliders[i])))
    {
      Vector2 toHit = Vector2_Sub(TransformComponent_GetPosition(ColliderComponent_GetTransform(hitColliders[i])), TransformComponent_GetPosition(transform));
      float distance = max(Vector2_Length(toHit), 150.f);
      float damageDealt = damage / (distance / 150.f);
      ShotLogData_LogDamageDealt(&data->shotLog, damageDealt, TransformComponent_GetPosition(ColliderComponent_GetTransform(hitColliders[i])), //TODO: Only log if there's actually a health component here
                                 ColliderComponent_GetEntity(hitColliders[i]));
      HealthSubject_DamageNotify(self->parent, ColliderComponent_GetEntity(hitColliders[i]), damageDealt);
      RigidbodyComponent *rigidbody = ColliderComponent_GetRigidbody(hitColliders[i]);
      if (rigidbody)
      {
        toHit = Vector2_Normalize(toHit);
        Vector2 impulse = Vector2_Scale(toHit, (float)(150.f * 10000.f / distance));
        RigidbodyComponent_AddForce(rigidbody, impulse, FORCE_TYPE_IMPULSE);
      }
    }
  }

  Entity *payload = EntityManager_CreateEntity(Entity_GetEntityManager(self->parent), "BombParticles");

  if (!payload)
  {
    return;
  }

  Component* transComponent = TransformComponent_Create();
  TransformComponent *transform2 = (TransformComponent *)transComponent->data;
  TransformComponent_SetPosition(transform2, TransformComponent_GetPosition(transform));

  Entity_AttachComponent(payload, transComponent);

  Entity_AttachComponent(payload, LifetimeComponent_Create(1.1f));

  Vector2 velocity = { 90.f, 0.f };
  Vector2 scale = { 20.f, 20.f };
  Vector2 acceleration = { -2.5f, 0.0f };

  //Component *particleComponent = ParticleSystem_Create(100, 300, 0.305f, 0.18f, 0.51f, 1.0f, 1, scale, velocity, acceleration, 1.0f, 0.0f);
  Component *particleComponent = ParticleSystem_Create(200, 2000, 1.f, 0.63f, 0.f, 0.9f, 0.25f, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CIRCLE);
  ParticleSystem_SetRGBAFinal(particleComponent->data, 1.f, 0.63f, 0.f, 0.0f);
  ParticleSystem_SetScaleFinal(particleComponent->data, 5, 5);
  ParticleSystem_SetisLooping(particleComponent->data, false);

  Entity_AttachComponent(payload, particleComponent);

  Entity_SetFlag(self->parent, EFLAG_DESTROY, true);
}