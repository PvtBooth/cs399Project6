/******************************************************************************/
/*!
	\file   DashAbilityComponent.c
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "DashAbilityComponent.h"
#include "AbilityComponent.h"
#include "EntityManager.h"
#include "LifetimeComponent.h"
#include "ParticleSystem.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct DashAbilityData
{
  float cooldown;
  float heat;
  float duration;
  float curDuration;
  float force;
  AbilityData *abilityData;
} DashAbilityData;

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
static void SpawnBoostParticles(Component *self, int playerID, Vector2 position, Vector2 forward);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* DashAbilityComponent_Create(float cooldown, float duration, float force, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = component->data;
  DashAbilityData *data = (DashAbilityData*)calloc(1, sizeof(DashAbilityData));

  data->abilityData = abilityData;
  data->cooldown = cooldown;
  data->duration = duration;
  data->force = force;

  abilityData->data = data;
  abilityData->destroy = &Destroy;
  abilityData->abilityUpdate = &Update;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  AbilityData *abilityData = self->data;
  DashAbilityData *data = abilityData->data;
  TransformComponent *transform = abilityData->player->transformComponent;
  RigidbodyComponent *rigidbody = abilityData->player->rigidbodyComponent;

  data->heat -= dt;
  if (data->heat < 0)
  {
    data->heat = 0;
  }

  if (data->curDuration > 0)
  {
    RigidbodyComponent_AddForce(rigidbody, Vector2_Scale(TransformComponent_GetForward(transform), data->force), FORCE_TYPE_FORCE);
    data->curDuration -= dt;
    if (data->curDuration < 0)
      data->curDuration = 0;
  }

  if (abilityData->state == ABILITY_STATE_IDLE)
  {
    abilityData->cooldownPercentage = min(1 - data->heat / data->cooldown, 1);
    return;
  }

  if (abilityData->state == ABILITY_STATE_PRIMED)
  {
    abilityData->state = ABILITY_STATE_IDLE;
    Player_DeactivateAbilities(abilityData->player); //??
    if (data->heat == 0)
    {
      data->curDuration += data->duration;
      data->heat += data->cooldown;

      SpawnBoostParticles(self, abilityData->player->id, Vector2_Add(TransformComponent_GetPosition(transform), Vector2_Scale(TransformComponent_GetForward(transform), -50)), TransformComponent_GetForward(transform));
    }
  }
  abilityData->cooldownPercentage = min(1 - data->heat / data->cooldown, 1);
}

static void Destroy(Component *self)
{
  AbilityData *abilityData = self->data;
  DashAbilityData *data = abilityData->data;
  free(data);
}

static void SpawnBoostParticles(Component *self, int playerID, Vector2 position, Vector2 forward)
{
  //First system
  Entity *particles1 = EntityManager_CreateEntity(self->parent->manager, "BoostParticles1");
  Component *transCom1 = TransformComponent_Create();
  TransformComponent_SetPosition((TransformComponent*)transCom1->data, position);
  TransformComponent_SetRotation((TransformComponent*)transCom1->data, Vector2_ToAngle(forward) - PI * 3 / 4);
  Entity_AttachComponent(particles1, transCom1);
  Entity_AttachComponent(particles1, LifetimeComponent_Create(1.0f));
  Vector2 velocity = { 4.f, 0.f };
  Vector2 scale = { 20.f, 10.f };
  Vector2 acceleration = { -0.02f, 0.0f };

  Color color = Colors_GetPlayerColor(playerID, COLORT_TINT);
  Component *particleComponent1 = ParticleSystem_Create(45, 400, color.r, color.g, color.b, color.a, 0.45f, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetDispersal(particleComponent1->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent1->data, PI / 6);
  ParticleSystem_SetRGBAFinal(particleComponent1->data, 1.0f, 1.0f, 1.0f, 0.0f);
  ParticleSystem_SetisLooping(particleComponent1->data, false);

  Entity_AttachComponent(particles1, particleComponent1);


  //Second system
  Entity *particles2 = EntityManager_CreateEntity(self->parent->manager, "BoostParticles2");
  Component *transCom2 = TransformComponent_Create();
  TransformComponent_SetPosition((TransformComponent*)transCom2->data, position);
  TransformComponent_SetRotation((TransformComponent*)transCom2->data, Vector2_ToAngle(forward) + PI * 3 / 4);
  Entity_AttachComponent(particles2, transCom2);
  Entity_AttachComponent(particles2, LifetimeComponent_Create(1.0f));

  Component *particleComponent2 = ParticleSystem_Create(45, 400, color.r, color.g, color.b, color.a, 0.45f, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetDispersal(particleComponent2->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent2->data, PI / 6);
  ParticleSystem_SetRGBAFinal(particleComponent2->data, 1.0f, 1.0f, 1.0f, 0.0f);
  ParticleSystem_SetisLooping(particleComponent2->data, false);

  Entity_AttachComponent(particles2, particleComponent2);
}
