/******************************************************************************/
/*!
	\file   BombAbilityComponent.c
	\author Evan Kau
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "BombAbilityComponent.h"
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
#include "BombProjectileComponent.h"
#include "Camera.h"
#include "EntityManager.h"
#include "AssetTable.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct BombAbilityData
{
  float rate;
  float delay;
  float timer;
  float speed;
  //float timeActivated;
  Entity *projectile;
  //TransformComponent *transform; //Obtainable through abilityData->player->transformComponent
  AbilityData *abilityData;
  ShotLogData shotLogTemplate;
} BombAbilityData;

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
static void Fire(BombAbilityData *data, float startingTime);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* BombAbilityComponent_Create(float rate, float timer, float speed, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = (AbilityData*)component->data;
  BombAbilityData *data = (BombAbilityData*)calloc(1, sizeof(BombAbilityData));

  data->abilityData = abilityData;

  data->rate = rate;
  data->timer = timer;
  data->speed = speed;
  
  data->projectile = Entity_CreateOrphan("BombAbilityProjectile");
  Vector2 size = { 75, 75 };
  Entity_AttachComponent(data->projectile, TransformComponent_Create());
  TransformComponent_SetScale(data->projectile->transformComponent, size);

  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Unit", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(data->projectile, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER));

  Component *colliderComponent = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);
  ColliderComponent *collider = colliderComponent->data;
  ColliderComponent_SetLayer(collider, PHYSICS_LAYER_PLAYER); //TODO: Player projectile layer, option for it to be on the player layer. Pass in
  Entity_AttachComponent(data->projectile, colliderComponent);
  Entity_AttachComponent(data->projectile, BombProjectileComponent_Create(speed, timer, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_ENEMY, PHYSICS_LAYER_DEFAULT)));

  data->shotLogTemplate.initialized = false;

  abilityData->data = data;
  abilityData->abilityUpdate = &Update;
  abilityData->destroy = &Destroy;
  component->type = COMPONENT_TYPE_BOMBABILITY;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  AbilityData *abilityData = self->data;
  BombAbilityData *data = abilityData->data;
  TransformComponent *transform = abilityData->player->transformComponent;
  InputEvent *rightStick = InputGatherComponent_GetInput(abilityData->player->inputGatherComponent, INPUT_TYPE_STICK_R);

  data->delay = min(data->delay, 1 / data->rate); //Cap the delay while not firing to the inverse fire rate
  data->delay += dt;

  if (abilityData->state == ABILITY_STATE_IDLE)
  {
    abilityData->cooldownPercentage = min(data->delay / data->rate, 1);
    return;
  }

  if (!data->shotLogTemplate.initialized)
  {
    data->shotLogTemplate.isPlayer = true;
    data->shotLogTemplate.weaponType = COMPONENT_TYPE_BOMBPROJECTILE;
    data->shotLogTemplate.sourceEntity = *self->parent;
    data->shotLogTemplate.initialized = true;
    BombProjectileData *projectile = Entity_FindComponentByType(data->projectile, COMPONENT_TYPE_BOMBPROJECTILE)->data;
    BombProjectileComponent_SetShotLog(projectile, &data->shotLogTemplate);
  }

  if (abilityData->state == ABILITY_STATE_PRIMED)
  {
    if (data->delay < 1 / data->rate)
    {
      abilityData->state = ABILITY_STATE_IDLE;
      Player_DeactivateAbilities(abilityData->player);
      return;
    }
    Vector2 pos = TransformComponent_GetPosition(transform);
    Vector2 end = Vector2_Add(pos, Vector2_Scale(Vector2_FromAngle(rightStick->angle), 700));
    if (rightStick->magnitude > 0.2f && rightStick->magnitude > 0.99f)
    {
      while (data->delay >= 1 / data->rate)
      {
        data->delay -= 1 / data->rate;
        Fire(data, data->delay);
        abilityData->state = ABILITY_STATE_IDLE;
        Player_DeactivateAbilities(abilityData->player);
      }
    }
    else
    {
      Camera_DrawLine(pos.x, pos.y, 0.5f, 1, 1, 1, end.x, end.y, 0.5f, 1, 1, 0.0f, true);
    }
  }
  abilityData->cooldownPercentage = min(data->delay / data->rate, 1);
}

static void Destroy(Component *self)
{
  AbilityData *abilityData = self->data;
  BombAbilityData *data = abilityData->data;
  Entity_DestroyOrphan(data->projectile);
  free(data);
}

static void Fire(BombAbilityData *data, float startingTime)
{
  Entity *projectile = Entity_Copy(data->projectile);
  EntityManager_AddEntity(Entity_GetEntityManager(data->abilityData->player->playerEntity), projectile);
  Component *rpComponent = Entity_FindComponentByType(projectile, COMPONENT_TYPE_BOMBPROJECTILE);
  TransformComponent *transform = projectile->transformComponent;
  InputEvent *rightStick = InputGatherComponent_GetInput(data->abilityData->player->inputGatherComponent, INPUT_TYPE_STICK_R);

  TransformComponent_SetPosition(transform, TransformComponent_GetPosition(data->abilityData->player->transformComponent));
  TransformComponent_SetRotation(transform, rightStick->angle);
  rpComponent->update(rpComponent, startingTime); //Update it so we can fire tones in a single frame with high speeds and have them appear to fired at their own times
}