/******************************************************************************/
/*!
	\file   AutoLaserComponent.c
	\author Evan Kau
	\date   3/2/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AutoLaserComponent.h"
#include "Component.h"
#include <stdlib.h>
#include "Entity.h"
#include "Event.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "RaycastProjectileComponent.h"
#include "RandGen.h"
#include "AbilityComponent.h"
#include "Sound.h"
#include "EntityManager.h"
#include "StandardMeshGroups.h"
#include "AssetTable.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct AutoLaserData
{
  int shotsFired;
  float rate;
  float delay;
  float range;
  float speed;
  float spread;
  Entity *projectile;
  TransformComponent *transform;
  AbilityData *abilityData;
  ShotLogData shotLogTemplate;
} AutoLaserData;

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
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static void Triggered(Component* self, EVENT_TYPE type);
static void Fire(AutoLaserData *data, float startingTime);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
* \brief
*   Creates a new AutoLaserComponent, which fires laser bolts at a given rate.
* \param speed
* The speed of the projectile.
* \param spread
*   The spread of the projectile in radians.
* \return A new AutoLaserComponent
*/
Component* AutoLaserComponent_Create(int shotsFired, float rate, float range, float damage, float force, float speed, float spread, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = (AbilityData*)component->data;
  AutoLaserData *data = (AutoLaserData*)calloc(1, sizeof(AutoLaserData));

  data->shotsFired = shotsFired;
  data->rate = rate;
  data->range = range;
  data->speed = speed;
  data->spread = spread;
  data->abilityData = abilityData;

  data->projectile = Entity_CreateOrphan("AutoLaserProjectile");

  Entity_AttachComponent(data->projectile, TransformComponent_Create());
  TransformComponent_SetScaleFloats(data->projectile->transformComponent, 100, 5);

  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Unit", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(data->projectile, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER));

  data->shotLogTemplate.initialized = false;
  Entity_AttachComponent(data->projectile, RaycastProjectileComponent_Create(speed, range, damage, force, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_DEFAULT, PHYSICS_LAYER_ENEMY)));

  abilityData->data = data;
  abilityData->destroy = &Destroy;
  abilityData->abilityUpdate = &Update;
  abilityData->triggered = &Triggered;
  //abilityData->state = ABILITY_STATE_IDLE;
  component->type = COMPONENT_TYPE_AUTOLASER;
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
  AutoLaserData *data = abilityData->data;
  if(!abilityData->player->inputGatherComponent)
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

  if (!data->shotLogTemplate.initialized)
  {
    data->shotLogTemplate.isPlayer = true;
    data->shotLogTemplate.weaponType = COMPONENT_TYPE_AUTOLASER;
    data->shotLogTemplate.sourceEntity = *self->parent;
    data->shotLogTemplate.initialized = true;
    RaycastProjectileData *projectile = Entity_FindComponentByType(data->projectile, COMPONENT_TYPE_RAYCASTPROJECTILE)->data;
    RaycastProjectileComponent_SetShotLog(projectile, &data->shotLogTemplate);
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
      Sound_PlayEffect(SOUND_BASIC_SHOOT);
    while (data->delay > 1 / data->rate)
    {
      data->delay -= 1 / data->rate;
      for (int i = 0; i < data->shotsFired; i++)
      {
        Fire(data, data->delay);
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
    AutoLaserData *data = abilityData->data;
    data->transform = (TransformComponent*)transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    AbilityData *abilityData = self->data;
    AutoLaserData *data = abilityData->data;
    data->transform = (TransformComponent*)component->data;
  }
}

static void Destroy(Component *self)
{
  AbilityData *abilityData = self->data;
  AutoLaserData *data = abilityData->data;
  Entity_DestroyOrphan(data->projectile);
  free(data);
}

static void Triggered(Component* self, EVENT_TYPE type)
{
  AbilityData *abilityData = self->data;
  //AutoLaserData *data = abilityData->data;

  if (type != abilityData->triggerEvent)
  {
    abilityData->state = ABILITY_STATE_IDLE;
  }
  else
  {
    abilityData->state = ABILITY_STATE_PRIMED;
  }
}

static void Fire(AutoLaserData *data, float startingTime)
{
  Entity *projectile = Entity_Copy(data->projectile);
  EntityManager_AddEntity(Entity_GetEntityManager(data->abilityData->player->playerEntity), projectile);
  Component *rpComponent = Entity_FindComponentByType(projectile, COMPONENT_TYPE_RAYCASTPROJECTILE);
  TransformComponent *transform = projectile->transformComponent;
  InputEvent *rightStick = InputGatherComponent_GetInput(data->abilityData->player->inputGatherComponent, INPUT_TYPE_STICK_R);

  TransformComponent_SetPosition(transform, TransformComponent_GetPosition(data->transform));
  TransformComponent_SetRotation(transform, rightStick->angle + data->spread * (RandGen_Float() - 0.5f) * 2);
  rpComponent->update(rpComponent, startingTime); //Update it so we can fire tones in a single frame with high speeds and have them appear to fired at their own times
}