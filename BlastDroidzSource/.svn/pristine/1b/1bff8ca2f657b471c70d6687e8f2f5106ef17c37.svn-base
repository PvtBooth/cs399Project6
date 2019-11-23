/******************************************************************************/
/*!
	\file   GravTorpedoComponent.c
	\author Henry Brobeck
	\date   4/3/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "GravTorpedoComponent.h"
#include "AbilityComponent.h"
#include "EntityManager.h"
#include "GenericTorpedoComponent.h"
#include "BlackHoleComponent.h"
#include "LifetimeComponent.h"
#include "ParticleSystem.h"
#include "AssetTable.h"
#include "Sound.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct GravTorpedoData
{
  float delay;
  float cooldown;
  float time;
  float speed;
  AbilityData *abilityData;
  Entity *torpedoEntity;
  Component *torpedoComponent;
  bool leavingAbility;
}GravTorpedoData;
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
static void Fire(Component *self, GravTorpedoData *data, float ang);
static void Detonate(Vector2 pos, Component *self);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* GravTorpedoComponent_Create(float cooldown, float time, float speed, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = (AbilityData*)component->data;
  GravTorpedoData *data = calloc(1, sizeof(GravTorpedoData));
  
  data->abilityData = abilityData;
  abilityData->data = data;
  data->delay = 0;
  data->cooldown = cooldown;
  data->speed = speed;
  data->time = time;

  abilityData->abilityUpdate = Update;
  abilityData->destroy = &Destroy;
  
  return component;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Update(Component *self, float dt)
{
  AbilityData *abilityData = (AbilityData*)self->data;
  GravTorpedoData *data = (GravTorpedoData*)abilityData->data;

  data->delay -= dt;

  if (abilityData->state == ABILITY_STATE_IDLE)
  {
    abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
    return;
  }
  //TransformComponent *transform = abilityData->player->transformComponent;
  InputEvent *rightStick = InputGatherComponent_GetInput(abilityData->player->inputGatherComponent, INPUT_TYPE_STICK_R);
  if(!rightStick)
  {
    return;
  }

  if(data->delay >= 0 && data->torpedoEntity == NULL)
  {
    Player_DeactivateAbilities(abilityData->player);
    abilityData->state = ABILITY_STATE_IDLE;
    abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
    return;
  }
  if(abilityData->state == ABILITY_STATE_PRIMED)
  {
    if(data->torpedoEntity != NULL)
    {
      GenericTorpedoComponent_Detonate(data->torpedoComponent);
      Player_DeactivateAbilities(abilityData->player);
      abilityData->state = ABILITY_STATE_IDLE;
      data->torpedoEntity = NULL;
      return;
    }
    if(rightStick->magnitude > 0.95)
    {
      Fire(self, data, rightStick->angle);
      abilityData->state = ABILITY_STATE_IDLE;
      Player_DeactivateAbilities(abilityData->player);
      data->delay = data->cooldown;
      return;
    }
    abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
  }
}

static void Destroy(Component *self)
{
  AbilityData *abilityData = self->data;
  GravTorpedoData *data = abilityData->data;
  free(data);
}

static void Fire(Component *self, GravTorpedoData *data, float ang)
{
  EntityManager *entityManager = Entity_GetEntityManager(self->parent);
  if(!entityManager)
  {
    return;
  }
  Entity *entity = EntityManager_CreateEntity(entityManager, "Torpedo");
  
  Component *transComponent = TransformComponent_Create();
  TransformComponent *transform = (TransformComponent*)transComponent->data;
  Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
  
  TransformComponent_SetPosition(transform, pos);
  TransformComponent_SetRotation(transform, ang);
  Entity_AttachComponent(entity, transComponent);

  Component *torpedoComponent = GenericTorpedoComponent_Create(data->speed, data->time, Detonate, self);

  Entity_AttachComponent(entity, torpedoComponent);

  Entity_AttachComponent(entity, RigidbodyComponent_Create(100.0f, 10.0f, 0.95f, 0.95f, false, true));

  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Missile", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(entity, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER));

  Vector2 particleScale = { 7,7 };
  Vector2 particleVelocity = { -0.5f, 0 };
  AbilityData *abilityData = data->abilityData;

  float r, g, b, a;
  //int id = 0;
  Player_GetColor(abilityData->player->id, COLORT_TINT, &r, &g, &b, &a);

//  Component *particleSystem = ParticleSystem_Create(500, 1000, r, g, b, a, 0.25, particleScale, particleVelocity, ((Vector2) {0, 0}), 0.0f, 0.0f);
//  ParticleSystem *system = (ParticleSystem*)particleSystem->data;
//  ParticleSystem_SetDispersal(system, PARTICLE_DISPERSAL_CONE);
//  ParticleSystem_SetConeWidth(system, 0.5f);

  //trail effect
  Component *particleComponent = ParticleSystem_Create(1000, 100, r, g, b, a, 1, particleScale, particleVelocity,
    ((Vector2) {0, 0}), 0.25f, 1.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetLifetimeMax(particleComponent->data, 3);
  ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
  ParticleSystem_SetRGBAFinal(particleComponent->data, 1.0f, 0.0f, 0.0f, 0);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 1.f);
  ParticleSystem_SetOffset(particleComponent->data, -10.0f, 0.0f);
  Entity_AttachComponent(entity, particleComponent);


  data->torpedoEntity = entity;
  data->torpedoComponent = torpedoComponent;


}

static void Detonate(Vector2 pos, Component *self)
{
  AbilityData *abilityData = (AbilityData*)self->data;
  GravTorpedoData *data = (GravTorpedoData*)abilityData->data;

  Entity *payload = EntityManager_CreateEntity(Entity_GetEntityManager(self->parent), "BlackHole");

  if (!payload)
  {
    return;
  }

  Component* transComponent = TransformComponent_Create();
  TransformComponent *transform = (TransformComponent *)transComponent->data;
  TransformComponent_SetPosition(transform, pos);

  Entity_AttachComponent(payload, transComponent);

  Entity_AttachComponent(payload, BlackHoleComponent_Create(750, 200, false));
  Entity_AttachComponent(payload, LifetimeComponent_Create(1.1f));

  Vector2 velocity = { 46.f, 0.f };
  Vector2 scale = { 10.f, 10.f };
  Vector2 acceleration = { -2.1f, 0.0f };

  //Component *particleComponent = ParticleSystem_Create(100, 300, 0.305f, 0.18f, 0.51f, 1.0f, 1, scale, velocity, acceleration, 1.0f, 0.0f);
  Component *particleComponent = ParticleSystem_Create(200, 400, 1.f, 0.63f, 0.f, 0.9f, 1, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CIRCLE);
  ParticleSystem_SetRGBAFinal(particleComponent->data, 1.f, 0.63f, 0.f, 0.0f);
  ParticleSystem_SetisLooping(particleComponent->data, false);

  Entity_AttachComponent(payload, particleComponent);

  data->torpedoComponent = NULL;
  data->torpedoEntity = NULL;

  Sound_PlayEffect(SOUND_ABILITY_GRAV);
}