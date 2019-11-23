/******************************************************************************/
/*!
	\file   AIForwardShoot.c
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AIForwardShoot.h"
#include <stdbool.h>
#include "Physics_Layers.h"
#include "ButtonComponent.h"
#include <stdlib.h>
#include "Physics_BroadPhase.h"
#include "Camera.h"
#include "Graphics.h"
#include "RenderComponent.h"
#include "RaycastProjectileComponent.h"
#include "Player.h"
#include "EntityManager.h"
#include "StandardMeshGroups.h"
#include "AssetTable.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct AIForwardShootData
{
  Entity *projectile;
  RaycastData raycastData;
  float range;
  float rate;
  float delay;
  float fireAngle;
  LayerMask targetMask;
  TransformComponent *transform;
  ShotLogData shotLogTemplate;
  bool enabled;
  bool debugDraw;
} AIForwardShootData;

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

/*!
 * \brief
 *   Creates a new component with an AIForwardShootData attached.
 * \param range
 *   The maximum fire range.
 * \param fireAngle
 *   The maximum angle to fire at [radians].
 * \param targetMask
 *   The mask to target. Will not fire at targets not on the mask.
 * \return
 *   
 */
Component* AIForwardShoot_Create(float rate, float range, float fireAngle, float projectileSpeed, float damage, float force, LayerMask targetMask, Vector2 projectileSize)
{
  Component *component = Component_Create();
  AIForwardShootData *data = (AIForwardShootData*)calloc(1, sizeof(AIForwardShootData));
  data->rate = rate;
  data->range = range;
  data->fireAngle = fireAngle;
  data->targetMask = targetMask;
  data->debugDraw = false;
  data->enabled = true;

  component->data = data;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->destroy = &Destroy;
  component->type = COMPONENT_TYPE_AIFORWARDSHOOT;

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

void AIForwardShoot_SetEnabled(AIForwardShootData *shooter, bool enabled)
{
  shooter->enabled = enabled;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component* self, float dt)
{
  
  //TransformComponent *transform = component->parent->transformComponent;
  AIForwardShootData *data = (AIForwardShootData*)self->data;
  RaycastData raycast = {0};
  int i;

  data->delay = min(data->delay, 1 / data->rate);
  data->delay += dt;

  if (!data->shotLogTemplate.initialized)
  {
    data->shotLogTemplate.isPlayer = false;
    data->shotLogTemplate.weaponType = COMPONENT_TYPE_AIFORWARDSHOOT;
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
    float angle = Vector2_GetSignedAngle(TransformComponent_GetForward(data->transform), Vector2_Sub(Player_GetPosition(i), TransformComponent_GetPosition(data->transform)));
    angle = min(max(angle, -data->fireAngle), data->fireAngle);
    Vector2 aimVector = Vector2_FromAngle(TransformComponent_GetRotation(data->transform) + angle);
    if (PhysicsManager_RaycastWithMask(&raycast, TransformComponent_GetPosition(data->transform), aimVector, data->range, data->targetMask))
    {
      while (data->delay > 1 / data->rate)
      {
        data->delay -= 1 / data->rate;
        Entity *shot = Entity_Copy(data->projectile);
        EntityManager_AddEntity(Entity_GetEntityManager(self->parent), shot);
        TransformComponent *transform = shot->transformComponent;
        TransformComponent_SetPosition(transform, TransformComponent_GetPosition(data->transform));
        TransformComponent_SetRotation(transform, TransformComponent_GetRotation(data->transform) + angle);

        //Camera_DrawLine(raycast.origin.x, raycast.origin.y, 1, 0.25f, 0, 1, raycast.point.x, raycast.point.y, 1, 0.25f, 0, 1, true);
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
    AIForwardShootData *data = self->data;
    data->transform = transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *other)
{
  if (other->type == COMPONENT_TYPE_TRANSFORM)
  {
    AIForwardShootData *data = self->data;
    data->transform = other->data;
  }
}

static void Destroy(Component *self)
{
  AIForwardShootData *data = self->data;
  Entity_DestroyOrphan(data->projectile);
  free(data);
  free(self);
}
