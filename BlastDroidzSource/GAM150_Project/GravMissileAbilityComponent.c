/******************************************************************************/
/*!
\file   GravMissileAbilityComponent.c
\author Henry Brobeck
\date   3/27/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "GravMissileAbilityComponent.h"
#include "Vector2.h"
#include "AbilityComponent.h"
#include "ParticleSystem.h"
#include "MissileComponent.h"
#include "StandardMeshGroups.h"
#include "EntityManager.h"
#include "BlackHoleComponent.h"
#include "LifetimeComponent.h"
#include "AssetTable.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct GravMissileData
{
  int damage;
  float range;
  float radius;
  float force;
  float speed;
  float turnRate;
  float delay;
  float cooldown;
  AbilityData *abilityData;
}GravMissileData;
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
static void Destroy(Component *self);
static void Fire(Component *self, Entity *target);
static void Payload(Component *self, int damage, float radius, Vector2 pos);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* GravMissileAbilityComponent_Create(int damage, float range, float radius, float force, float speed, float turnRate, float cooldown, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = (AbilityData*)component->data;
  GravMissileData *data = calloc(1, sizeof(GravMissileData));

  data->damage = damage;

  data->speed = speed;
  data->turnRate = turnRate;
  data->range = range;
  data->radius = radius;
  data->force = force;
  data->cooldown = cooldown;

  data->abilityData = abilityData;



  abilityData->data = data;
  abilityData->destroy = &Destroy;
  abilityData->abilityUpdate = &Update;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/


static void Update(Component* self, float dt)
{
  AbilityData *abilityData = self->data;
  GravMissileData *data = abilityData->data;

  data->delay -= dt;

  if (data->delay > 0)
  {
    abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
    Player_DeactivateAbilities(abilityData->player);
    abilityData->state = ABILITY_STATE_IDLE;
    return;
  }

  if (abilityData->state == ABILITY_STATE_IDLE)
  {
    abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
    return;
  }
  if (abilityData->state == ABILITY_STATE_PRIMED)
  {
    InputEvent *rightStick = InputGatherComponent_GetInput(abilityData->player->inputGatherComponent, INPUT_TYPE_STICK_R);
    if(!rightStick)
    {
      return;
    }
    if (rightStick->magnitude > 0.9f)
    {
      TransformComponent *transform = self->parent->transformComponent;
      Vector2 curPos = TransformComponent_GetPosition(transform);
      int numHit = 0;
      const ColliderComponent **col = PhysicsManager_CircleCast(curPos, data->range, &numHit);
      if (numHit && col)
      {

        float minDistance = 0;
        Entity *target = NULL;
        float stickAngle = rightStick->angle;

        for (int i = 1; i < numHit; i++)
        {
          const ColliderComponent *collider = col[i];
          if (ColliderComponent_GetLayer(collider) != PHYSICS_LAYER_ENEMY)
          {
            continue;
          }
          Entity *potentialTarget = ColliderComponent_GetEntity(collider);
          Vector2 tarPos = TransformComponent_GetPosition(potentialTarget->transformComponent);
          float angToTarget = Vector2_GetAngle(curPos, tarPos);
          float difference = (float)fabs(angToTarget - stickAngle);

          if (difference < 0.1)
          {
            float distance = Vector2_Distance(curPos, tarPos);
            if (target == NULL || minDistance > distance)
            {
              minDistance = distance;
              target = potentialTarget;
            }

          }
        }
        if (target)
        {
          //Fire!
          Fire(self, target);
          data->delay = data->cooldown;
          abilityData->state = ABILITY_STATE_IDLE;
          return;
        }

      }
    }
  }
  abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
}

static void Destroy(Component *self)
{
  AbilityData *abilityData = self->data;
  GravMissileData *data = abilityData->data;
  free(data);
}

static void Fire(Component *self, Entity *target)
{
  AbilityData *abilityData = self->data;
  GravMissileData *data = abilityData->data;

  TransformComponent *transform = self->parent->transformComponent;
  Vector2 curPos = TransformComponent_GetPosition(transform);

  EntityManager *entityManager = Entity_GetEntityManager(self->parent);

  Entity *missile = EntityManager_CreateEntity(entityManager, "missile");
  Component *transComponent = TransformComponent_Create();
  TransformComponent *missileTransform = (TransformComponent*)transComponent->data;
  TransformComponent_SetPosition(missileTransform, curPos);

  Entity_AttachComponent(missile, transComponent);
  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Missile", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(missile, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER));

  Component *missileComponent = MissileComponent_Create(data->speed, data->turnRate, data->damage, 50, target);

  MissileComponent_SetPayload(missileComponent, Payload, self);

  Entity_AttachComponent(missile, missileComponent);

  Vector2 particleScale = { 5,5 };
  Vector2 particleVelocity = { -2, 0 };


  Component *particleSystem = ParticleSystem_Create(100, 200, 1.0f, 0.0f, 0.0f, 1.0f, 0.25, particleScale, particleVelocity, ((Vector2) {0, 0}), 0.0f, 0.0f, RENDER_LAYER_PARTICLE);

  Entity_AttachComponent(missile, particleSystem);

  Component *rigidComponent = RigidbodyComponent_Create(1.0f, 10.0f, 0.05f, 0.1f, false, false);
  //RigidbodyComponent *rigidbody = (RigidbodyComponent*)rigidComponent->data;
  Entity_AttachComponent(missile, rigidComponent);

}

static void Payload(Component *self, int damage, float radius, Vector2 pos)
{
    UNREFERENCED_PARAMETER(damage);
    UNREFERENCED_PARAMETER(radius);
  AbilityData *abilityData = self->data;
  GravMissileData *data = abilityData->data;

  Entity *payload = EntityManager_CreateEntity(Entity_GetEntityManager(self->parent), "BlackHole");

  if (!payload)
  {
    return;
  }

  Component* transComponent = TransformComponent_Create();
  TransformComponent *transform = (TransformComponent *)transComponent->data;
  TransformComponent_SetPosition(transform, pos);

  Entity_AttachComponent(payload, transComponent);

  Entity_AttachComponent(payload, BlackHoleComponent_Create(data->radius, data->force, false));
  Entity_AttachComponent(payload, LifetimeComponent_Create(3.0f));

  

}