/******************************************************************************/
/*!
	\file   Enemy_StationComponent.c
	\author Evan Kau
	\date   4/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Enemy_StationComponent.h"
#include <stdbool.h>
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include <stdlib.h>
#include "Assignment.h"
#include "Camera.h"
#include "Player.h"
#include "AIForwardShoot.h"
#include "AIManager.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct Enemy_StationData
{
  TransformComponent *transform;
  RigidbodyComponent *rigidbody;
  AIForwardShootData *shooter;
  float detectionRadius;
  float timeToUntrack;
} Enemy_StationData;

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
static void DetectPlayer(Enemy_StationData *data, int playerID);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* Enemy_StationComponent_Create(float detectionRadius)
{
  Component *component = Component_Create();
  Enemy_StationData *data = (Enemy_StationData*)calloc(1, sizeof(Enemy_StationData));

  data->detectionRadius = detectionRadius;

  component->data = data;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->destroy = &Destroy;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
  UNREFERENCED_PARAMETER(dt);
  Enemy_StationData *data = self->data;
  int targetPlayer = -1;

  float minDistance = data->detectionRadius;
  for (int i = 0; i < 4; i++)
  {
    if (Player_GetPlayer(i)->playerEntity)
    {
      float distance = Vector2_Distance(TransformComponent_GetPosition(data->transform), Player_GetPosition(i));
      if (distance < minDistance)
      {
        RaycastData raycast;
        if (PhysicsManager_RaycastWithMask(&raycast, TransformComponent_GetPosition(data->transform), Vector2_Sub(Player_GetPosition(i), TransformComponent_GetPosition(data->transform)),
            data->detectionRadius, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_NEUTRAL, PHYSICS_LAYER_PLAYER)))
        {
          minDistance = distance;
          targetPlayer = i;
        }
      }
    }
  }

  data->timeToUntrack -= dt;

  if (targetPlayer != -1)
  {
    AIForwardShoot_SetEnabled(data->shooter, true);
    DetectPlayer(data, targetPlayer);
    if (data->timeToUntrack < 0)
    {
      Camera_TrackEntity(self->parent);
    }
    data->timeToUntrack = 3;
  }
  else
  {
    AIForwardShoot_SetEnabled(data->shooter, false);
    if (data->timeToUntrack <= 0)
    {
      Camera_UntrackEntity(self->parent);
    }
  }
}

static void OnAttach(Component *component)
{
  Enemy_StationData *data = component->data;
  Component *transform = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_TRANSFORM);
  if (transform)
  {
    data->transform = (TransformComponent*)transform->data;
  }
  Component *rigidbody = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_RIGIDBODY);
  if (rigidbody)
  {
    data->rigidbody = (RigidbodyComponent*)rigidbody->data;
  }
  Component *shooter = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_AIFORWARDSHOOT);
  if (shooter)
  {
    data->shooter = (AIForwardShootData*)shooter->data;
  }
}

static void OnOtherAttach(Component *component, Component *other)
{
  Enemy_StationData *data = component->data;
  if (other->type == COMPONENT_TYPE_TRANSFORM)
  {
    data->transform = other->data;
  }
  else if (other->type == COMPONENT_TYPE_RIGIDBODY)
  {
    data->rigidbody = other->data;
  }
  else if (other->type == COMPONENT_TYPE_AIFORWARDSHOOT)
  {
    data->shooter = other->data;
  }
}

static void Destroy(Component *component)
{
  Enemy_StationData *data = component->data;
  Camera_UntrackEntity(component->parent);
  Assignment_ReciveEvent(ASSIGNMENT_EVENT_SHIP_DIE);
  free(data);
  free(component);
}

static void DetectPlayer(Enemy_StationData *data, int playerID)
{
  AISwarm *swarm = AIManager_GetNearestSwarm(TransformComponent_GetPosition(data->transform));
  
  if (swarm)
  {
    if (Vector2_Distance(AISwarm_GetCenter(swarm), TransformComponent_GetPosition(data->transform)) - AISwarm_GetRadius(swarm) < data->detectionRadius / 2)
    {
      AISwarm_DetectPlayer(swarm, playerID);
    }
  }
}