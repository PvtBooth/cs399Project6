/******************************************************************************/
/*!
	\file   Enemy_SuicideComponent.c
	\author Evan Kau
	\date   3/25/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "Enemy_SuicideComponent.h"
#include "Camera.h"
#include "AISwarmAgent.h"
#include "RigidbodyComponent.h"
#include <stdlib.h>
#include "CollisionSubject.h"
#include "ShotLogData.h"
#include "HealthSubject.h"
#include "Assignment.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct Enemy_SuicideData
{
  AISwarmAgent *agent;
  TransformComponent *transform;
  RigidbodyComponent *rigidbody;
  float speed;
  float turnSpeed;
  float explosionRadius;
  float damage;
  ShotLogData shotLog;
} Enemy_SuicideData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *component, float dt);
static void OnAttach(Component *component);
static void OnOtherAttach(Component *component, Component *other);
static void Destroy(Component *component);
static void EventHandle(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal);
static void Explode(Component *component);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* Enemy_SuicideComponent_Create()
{
  Component *component = Component_Create();

  Enemy_SuicideData *data = (Enemy_SuicideData*)calloc(1, sizeof(Enemy_SuicideData));
  data->speed = 3000;
  data->turnSpeed = 35;
  data->explosionRadius = 450;
  data->damage = 5;

  component->data = data;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->destroy = &Destroy;
  component->eventHandle = &EventHandle;
  component->type = COMPONENT_TYPE_SUICIDEENEMY;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *component, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  Enemy_SuicideData *data = component->data;

  Vector2 target = Vector2_Add(AISwarmAgent_GetToCenterVector(data->agent), Vector2_Scale(AISwarmAgent_GetToTargetVector(data->agent), 2));
  Vector2 agentRepulsion = AISwarmAgent_GetAgentRepulsionVector(data->agent);
  target = Vector2_Add(target, agentRepulsion);
  target = Vector2_Add(target, AISwarmAgent_GetObjectRepulsionVector(data->agent));

  RaycastData raycastData;
  if (PhysicsManager_RaycastWithMask(&raycastData, TransformComponent_GetPosition(data->transform), AISwarmAgent_GetToTargetVector(data->agent), 500, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_DEFAULT, PHYSICS_LAYER_PLAYER)))
  {
    if (ColliderComponent_GetLayer(raycastData.collider) == PHYSICS_LAYER_PLAYER)
    {
      target = AISwarmAgent_GetToTargetVector(data->agent);
    }
  }
  RigidbodyComponent_PIDLoopToDirection(data->rigidbody, target, data->turnSpeed, 20);
  RigidbodyComponent_AddForce(data->rigidbody, Vector2_Scale(TransformComponent_GetForward(data->transform), data->speed * RigidbodyComponent_GetMass(data->rigidbody)), FORCE_TYPE_FORCE);
}

static void OnAttach(Component *component)
{
  Enemy_SuicideData *data = component->data;
  AISwarmAgent *agent = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_AISWARMAGENT)->data;
  if (agent)
  {
    data->agent = agent;
  }
  TransformComponent *transform = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_TRANSFORM)->data;
  if (transform)
  {
    data->transform = transform;
  }
  RigidbodyComponent *rigidbody = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_RIGIDBODY)->data;
  if (rigidbody)
  {
    data->rigidbody = rigidbody;
  }
  data->shotLog.isPlayer = false;
  data->shotLog.weaponType = COMPONENT_TYPE_SUICIDEENEMY;
  data->shotLog.sourceEntity = *component->parent;
  data->shotLog.initialized = true;
}

static void OnOtherAttach(Component *component, Component *other)
{
  Enemy_SuicideData *data = component->data;
  if (other->type == COMPONENT_TYPE_AISWARMAGENT)
  {
    data->agent = other->data;
  }
  else if (other->type == COMPONENT_TYPE_TRANSFORM)
  {
    data->transform = other->data;
  }
  else if (other->type == COMPONENT_TYPE_RIGIDBODY)
  {
    data->rigidbody = other->data;
  }
}

static void Destroy(Component *component)
{
  Explode(component);
  Enemy_SuicideData *data = component->data;
  Camera_UntrackEntity(component->parent);
  Assignment_ReciveEvent(ASSIGNMENT_EVENT_SHIP_DIE);
  free(data);
  free(component);
}

static void EventHandle(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal)
{
  UNREFERENCED_PARAMETER(isLocal);
  if (eventType == EVENT_COLLISION)
  {
    if (Entity_GetFlag(self->parent, EFLAG_DESTROY))
      return;

    CollisionData *collision = (CollisionData*)eventData;
    if (ColliderComponent_GetLayer(Entity_FindComponentByType(collision->first, COMPONENT_TYPE_COLLIDER)->data) == PHYSICS_LAYER_PLAYER)
    {
      HealthSubject_DamageNotify(self->parent, self->parent, 2017);
      //Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
    }
    else if (ColliderComponent_GetLayer(Entity_FindComponentByType(collision->second, COMPONENT_TYPE_COLLIDER)->data) == PHYSICS_LAYER_PLAYER)
    {
      HealthSubject_DamageNotify(self->parent, self->parent, 2017);
      //Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
    }
  }
}

static void Explode(Component *component)
{
  Enemy_SuicideData *data = (Enemy_SuicideData*)component->data;
  int collidersHit;
  const ColliderComponent **colliderList = PhysicsManager_CircleCast(TransformComponent_GetPosition(data->transform), data->explosionRadius, &collidersHit);
  int i;

  for (i = 0; i < collidersHit; i++)
  {
    if (ColliderComponent_GetLayer(colliderList[i]) == PHYSICS_LAYER_PLAYER)
    {
      ShotLogData_LogDamageDealt(&data->shotLog, data->damage, TransformComponent_GetPosition(ColliderComponent_GetTransform(colliderList[i])), ColliderComponent_GetEntity(colliderList[i]));
      HealthSubject_DamageNotify(component->parent, ColliderComponent_GetEntity(colliderList[i]), data->damage);
      RigidbodyComponent *rigidbody = ColliderComponent_GetRigidbody(colliderList[i]);
      if (rigidbody)
      {
        Vector2 toRigidbody = Vector2_Sub(TransformComponent_GetPosition(RigidbodyComponent_GetTransform(rigidbody)), TransformComponent_GetPosition(data->transform));
        float distance = max(Vector2_Length(toRigidbody), 200);
        toRigidbody = Vector2_Normalize(toRigidbody);
        Vector2 impulse = Vector2_Scale(toRigidbody, 200 * 3500 / distance);
        RigidbodyComponent_AddForce(rigidbody, impulse, FORCE_TYPE_IMPULSE);
      }
    }
    else if (ColliderComponent_GetLayer(colliderList[i]) == PHYSICS_LAYER_NEUTRAL)
    {
      RigidbodyComponent *rigidbody = ColliderComponent_GetRigidbody(colliderList[i]);
      if (rigidbody)
      {
        Vector2 toRigidbody = Vector2_Sub(TransformComponent_GetPosition(RigidbodyComponent_GetTransform(rigidbody)), TransformComponent_GetPosition(data->transform));
        float distance = max(Vector2_Length(toRigidbody), 200);
        toRigidbody = Vector2_Normalize(toRigidbody);
        Vector2 impulse = Vector2_Scale(toRigidbody, 200 * 15000 / distance);
        RigidbodyComponent_AddForce(rigidbody, impulse, FORCE_TYPE_IMPULSE);
      }
    }
  }
}