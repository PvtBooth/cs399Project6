/******************************************************************************/
/*!
	\file   AITurretComponent.c
	\author Evan Kau
	\date   4/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AITurretComponent.h"
#include "ShotLogData.h"
#include "PhysicsManager.h"
#include <stdlib.h>
#include "AssetTable.h"
#include "RenderComponent.h"
#include "RaycastProjectileComponent.h"
#include "Player.h"
#include "RandGen.h"
#include "EntityManager.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct AITurretData
{
  Entity *projectile;
  RaycastData raycastData;
  Vector2 positionOffset;
  float range;
  float rate;
  float delay;
  float fireSpread;
  float randomDelay;
  LayerMask checkMask;
  LayerMask targetMask;
  TransformComponent *transform;
  ShotLogData shotLogTemplate;
  bool enabled;
  bool debugDraw;
} AITurretData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component* component, float dt);

static void OnAttach(Component *self);

static void OnOtherAttach(Component *self, Component *other);

static void Destroy(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* AITurret_Create(Vector2 positionOffset, float rate, float randomDelay, float range, float fireSpread, float projectileSpeed,
                           float damage, float force, LayerMask checkMask, Vector2 projectileSize)
{
  Component *component = Component_Create();
  AITurretData *data = (AITurretData*)calloc(1, sizeof(AITurretData));
  data->positionOffset = positionOffset;
  data->rate = rate;
  data->range = range;
  data->fireSpread = fireSpread;
  data->randomDelay = randomDelay;
  data->checkMask = checkMask;
  //data->targetMask = targetMask;
  data->debugDraw = false;
  data->enabled = true;

  component->data = data;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->destroy = &Destroy;
  component->type = COMPONENT_TYPE_AITURRET;

  data->projectile = Entity_CreateOrphan("AIForwardShootProjecitle");

  Entity_AttachComponent(data->projectile, TransformComponent_Create());
  TransformComponent_SetScale(data->projectile->transformComponent, projectileSize);

  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Unit", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(data->projectile, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_ENEMY));
  Entity_AttachComponent(data->projectile, RaycastProjectileComponent_Create(projectileSpeed, range, damage, force, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_DEFAULT, PHYSICS_LAYER_PLAYER)));

  data->shotLogTemplate.initialized = false;

  return component;
}

void AITurret_SetEnabled(AITurretData *shooter, bool enabled)
{
  shooter->enabled = enabled;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component* self, float dt)
{
  AITurretData *data = (AITurretData*)self->data;
  RaycastData raycast = {0};
  int i;

  //data->delay = min(data->delay, 1 / data->rate);
  if (data->delay > 1 / data->rate)
  {
    data->delay -= 1 / data->rate;
    data->delay -= RandGen_Float() * data->randomDelay;
  }
  data->delay += dt;

  if (!data->shotLogTemplate.initialized)
  {
    data->shotLogTemplate.isPlayer = false;
    data->shotLogTemplate.weaponType = COMPONENT_TYPE_AITURRET;
    data->shotLogTemplate.sourceEntity = *self->parent;
    data->shotLogTemplate.initialized = true;
    RaycastProjectileData *projectile = Entity_FindComponentByType(data->projectile, COMPONENT_TYPE_RAYCASTPROJECTILE)->data;
    RaycastProjectileComponent_SetShotLog(projectile, &data->shotLogTemplate);
  }

  if (!data->enabled)
  {
    return;
  }

  for (i = 0; i < 4; i++)
  {
    if (!Player_GetPlayer(i) /*|| Vector2_DistanceSquared(TransformComponent_GetPosition(data->transform), Player_GetPosition(i)) > (data->range + 100) * (data->range + 100)*/)
      continue;
    Vector2 turretPosition = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(data->transform), data->positionOffset);
    Vector2 aimVector = Vector2_Sub(Player_GetPosition(i), turretPosition);
    if (PhysicsManager_RaycastWithMask(&raycast, turretPosition, aimVector, data->range, data->checkMask))
    {
      if (raycast.entity == Player_GetPlayer(i)->playerEntity)
      {
        while (data->delay > 1 / data->rate)
        {
          data->delay -= 1 / data->rate;
          data->delay -= RandGen_Float() * data->randomDelay;
          Entity *shot = Entity_Copy(data->projectile);
          EntityManager_AddEntity(Entity_GetEntityManager(self->parent), shot);
          TransformComponent *transform = shot->transformComponent;
          TransformComponent_SetPosition(transform, turretPosition);

          float angle = -Vector2_GetSignedAngle(aimVector, ((Vector2){1, 0}));
          TransformComponent_SetRotation(transform, angle + (RandGen_Float() * data->fireSpread - data->fireSpread / 2));

          //Camera_DrawLine(raycast.origin.x, raycast.origin.y, 1, 0.25f, 0, 1, raycast.point.x, raycast.point.y, 1, 0.25f, 0, 1, true);
        }
      }
    }
  }


  if (data->debugDraw)
  {
    Camera_DrawLine(raycast.origin.x, raycast.origin.y, 1, 0.25f, 0, 1, raycast.point.x, raycast.point.y, 1, 0.25f, 0, 1, true);
  }
}

static void OnAttach(Component *self)
{
  Component *transform = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  if (transform)
  {
    AITurretData *data = self->data;
    data->transform = transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *other)
{
  if (other->type == COMPONENT_TYPE_TRANSFORM)
  {
    AITurretData *data = self->data;
    data->transform = other->data;
  }
}

static void Destroy(Component *self)
{
  AITurretData *data = self->data;
  Entity_DestroyOrphan(data->projectile);
  free(data);
  free(self);
}
