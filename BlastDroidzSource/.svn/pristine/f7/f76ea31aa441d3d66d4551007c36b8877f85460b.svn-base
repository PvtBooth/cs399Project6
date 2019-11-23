/******************************************************************************/
/*!
	\file   CurtainMissileComponent.c
	\author Evan Kau
	\date   4/13/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "CurtainMissileComponent.h"
#include "Entity.h"
#include "AbilityComponent.h"
#include "AssetTable.h"
#include "DelayedMissileComponent.h"
#include "Sound.h"
#include "EntityManager.h"
#include "RandGen.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct CurtainMissileData
{
  int missiles;
  float explosionRadius;
  float damage;
  float force;
  float rate;
  float delay;
  float floatDistance;
  float floatDelay;
  float floatSpread;
  float floatRate;
  float speed;
  float range;
  TransformComponent *transform;
  AbilityData *abilityData;
  //ShotLogData shotLogTemplate;   Not doing this anymore, no more data left to gather =/
} CurtainMissileData;

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
static void Destroy(Component *self);
static void Fire(CurtainMissileData *data, float fireAngle, float floatAngle);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
* \brief
*   Creates a new CurtainMissileComponent, which fires missiles in a line perpendicular to the fire direction, in the given direction.
* \return A new CurtainMissileComponent
*/
Component* CurtainMissileComponent_Create(int missiles, float damage, float force, float rate, float floatDistance, float floatDelay, float floatRate, float floatSpread, float speed, float range, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = (AbilityData*)component->data;
  CurtainMissileData *data = (CurtainMissileData*)calloc(1, sizeof(CurtainMissileData));

  data->missiles = missiles;
  data->damage = damage;
  data->force = force;
  data->rate = rate;
  data->floatDistance = floatDistance;
  data->floatDelay = floatDelay;
  data->floatSpread = floatSpread;
  data->floatRate = floatRate;
  data->speed = speed;
  data->range = range;
  data->abilityData = abilityData;

  abilityData->data = data;
  abilityData->abilityUpdate = &Update;
  abilityData->destroy = &Destroy;
  //abilityData->state = ABILITY_STATE_IDLE;
  component->type = COMPONENT_TYPE_CURTAINMISSILE;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  //component->update = &Update;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  AbilityData *abilityData = self->data;
  CurtainMissileData *data = abilityData->data;
  if (!abilityData->player->inputGatherComponent)
  {
    return;
  }
  InputEvent *rightStick = InputGatherComponent_GetInput(abilityData->player->inputGatherComponent, INPUT_TYPE_STICK_R);

  data->delay = min(data->delay, 1 / data->rate); //Cap the delay while not firing to the inverse fire rate
  data->delay += dt;

  if (abilityData->state == ABILITY_STATE_IDLE)
  {
    abilityData->cooldownPercentage = min(data->delay * data->rate, 1);
    return;
  }

  if (rightStick->magnitude > 0.25f)
  {
    abilityData->state = ABILITY_STATE_ACTIVE;
  }
  else
  {
    abilityData->state = ABILITY_STATE_PRIMED;
  }

  if (abilityData->state == ABILITY_STATE_ACTIVE)
  {
    if (data->delay > 1 / data->rate)
      Sound_PlayEffect(SOUND_ABILITY_MISSLE);
    while (data->delay > 1 / data->rate)
    {
      data->delay -= 1 / data->rate;
      float angle = (-data->floatSpread + data->floatSpread / data->missiles) / 2;
      for (int i = 0; i < data->missiles; i++)
      {
        Fire(data, rightStick->angle, rightStick->angle + PI + angle);
        angle += data->floatSpread / data->missiles;
      }
    }
  }

  abilityData->cooldownPercentage = min(data->delay * data->rate, 1);
}

static void OnAttach(Component *self)
{
  Component *transform = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  if (transform)
  {
    AbilityData *abilityData = self->data;
    CurtainMissileData *data = abilityData->data;
    data->transform = (TransformComponent*)transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    AbilityData *abilityData = self->data;
    CurtainMissileData *data = abilityData->data;
    data->transform = (TransformComponent*)component->data;
  }
}

static void Destroy(Component *self)
{
  AbilityData *abilityData = self->data;
  CurtainMissileData *data = abilityData->data;
  free(data);
}

static void Fire(CurtainMissileData *data, float fireAngle, float floatAngle)
{
  Vector2 firePosition = TransformComponent_GetPosition(data->transform);
  Vector2 floatPosition = Vector2_Add(firePosition, Vector2_Scale(Vector2_FromAngle(floatAngle), data->floatDistance));
  Entity *projectile = Entity_CreateOrphan("CurtainMissileProjectile");

  Entity_AttachComponent(projectile, TransformComponent_Create());
  TransformComponent_SetScaleFloats(projectile->transformComponent, 35, 15);

  TransformComponent_SetPosition(projectile->transformComponent, firePosition);

  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Unit", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(projectile, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER));

  Entity_AttachComponent(projectile, DelayedMissileComponent_Create(data->speed, data->range, data->damage, data->force, data->explosionRadius, floatPosition, data->floatDelay,
                                                                    data->floatRate, fireAngle, Colors_GetPlayerColor(data->abilityData->player->id, COLORT_TINT),
                                                                    Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_DEFAULT, PHYSICS_LAYER_ENEMY)));

  EntityManager_AddEntity(Entity_GetEntityManager(data->abilityData->player->playerEntity), projectile);
}