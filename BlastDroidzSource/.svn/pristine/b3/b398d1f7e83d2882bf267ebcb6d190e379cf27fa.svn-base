/******************************************************************************/
/*!
	\file   DelayedMissileComponent.c
	\author Evan Kau
	\date   4/13/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "DelayedMissileComponent.h"
#include "Entity.h"
#include "ParticleSystem.h"
#include "LifetimeComponent.h"
#include "EntityManager.h"
#include "ColliderComponent.h"
#include "HealthSubject.h"
#include "Unreferenced_Parameter.h"
#include <stdlib.h>
#include "Colors.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

typedef enum DELAYED_MISSILE_STAGE
{
  DELAYED_MISSILE_STAGE_FLOATING,
  DELAYED_MISSILE_STAGE_ATTACKING
} DELAYED_MISSILE_STAGE;

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct DelayedMissileData
{
  float speed;
  float range;
  float distanceTraveled;
  float damage;
  float force;
  float radius;
  Vector2 floatPosition;
  float floatDelay;
  float floatRate;
  float floatTime;
  float direction;
  DELAYED_MISSILE_STAGE stage;
  Color color;
  LayerMask targetLayer;
} DelayedMissileData;

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
static void Detonate(Component *self, ColliderComponent *hitCollider);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* DelayedMissileComponent_Create(float speed, float range, float damage, float force, float radius, Vector2 floatPosition, float floatDelay, float floatRate,
                                          float direction, Color color, LayerMask targetLayer)
{
  Component *component = Component_Create();
  component->update = Update;

  DelayedMissileData *data = (DelayedMissileData*)calloc(1, sizeof(DelayedMissileData));
  data->speed = speed;
  data->range = range;
  data->damage = damage;
  data->force = force;
  data->radius = radius;
  data->floatPosition = floatPosition;
  data->floatDelay = floatDelay;
  data->floatRate = floatRate;
  data->direction = direction;
  data->stage = DELAYED_MISSILE_STAGE_FLOATING;
  data->color = color;
  data->targetLayer = targetLayer;

  component->data = data;
  component->type = COMPONENT_TYPE_MISSILEBARRAGE;

  return component;

}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component* self, float dt)
{
  DelayedMissileData *data = (DelayedMissileData*)self->data;
  TransformComponent *transform = self->parent->transformComponent;
  Vector2 curPos = TransformComponent_GetPosition(self->parent->transformComponent);
  TransformComponent_SetRotation(transform, data->direction);

  if (data->stage == DELAYED_MISSILE_STAGE_FLOATING)
  {
    data->floatTime += dt;

    TransformComponent_SetPosition(transform, Vector2_Lerp(curPos, data->floatPosition, data->floatRate * dt, true));

    if (data->floatTime > data->floatDelay)
    {
      data->stage = DELAYED_MISSILE_STAGE_ATTACKING;
    }
  }
  else
  {
    data->distanceTraveled += data->speed * dt;
    if (data->distanceTraveled >= data->range)
    {
      Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
    }
    RaycastData raycastData;
    if (PhysicsManager_RaycastWithMask(&raycastData, curPos, Vector2_FromAngle(data->direction), data->speed * dt, data->targetLayer))
    {
      Detonate(self, raycastData.collider);
    }
    TransformComponent_AddPosition(transform, Vector2_Scale(Vector2_FromAngle(data->direction), data->speed * dt));
  }
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

static void Detonate(Component *self, ColliderComponent *hitCollider)
{
  DelayedMissileData *data = (DelayedMissileData*)self->data;
  Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
 
  HealthSubject_DamageNotify(self->parent, ColliderComponent_GetEntity(hitCollider), data->damage);
  if (ColliderComponent_GetRigidbody(hitCollider))
  {
    RigidbodyComponent_ApplyForceAtPoint(ColliderComponent_GetRigidbody(hitCollider),
      Vector2_Scale(TransformComponent_GetForward(self->parent->transformComponent), data->force), pos, FORCE_TYPE_IMPULSE);
  }
  

  Entity *particleExplosion = EntityManager_CreateEntity(Entity_GetEntityManager(self->parent), "DelayedMissileDetonation");

  Component *transCom = TransformComponent_Create();
  TransformComponent_SetPosition((TransformComponent*)transCom->data, pos);
  TransformComponent_SetRotation((TransformComponent*)transCom->data, data->direction);

  Entity_AttachComponent(particleExplosion, transCom);

  Entity_AttachComponent(particleExplosion, LifetimeComponent_Create(1.0f));

  Vector2 velocity = { 5.f, 0.f };
  Vector2 scale = { 20.f, 20.f };
  Vector2 acceleration = { -0.5f, 0.0f };

  Component *particleComponent = ParticleSystem_Create(20, 800, data->color.r, data->color.g, data->color.b, data->color.a, 0.7f, scale, velocity, acceleration,
                                                       1.0f, 0.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 0.2f);
  ParticleSystem_SetRGBAFinal(particleComponent->data, data->color.r, data->color.g, data->color.b, 0);
  ParticleSystem_SetisLooping(particleComponent->data, false);
  ParticleSystem_SetLifetimeMin(particleComponent->data, 0.35f);

  Entity_AttachComponent(particleExplosion, particleComponent);

  Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
}