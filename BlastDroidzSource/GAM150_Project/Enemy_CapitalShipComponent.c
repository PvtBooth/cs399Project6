/******************************************************************************/
/*!
	\file   Enemy_CapitalShipComponent.c
	\author Evan Kau
	\date   4/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "Enemy_CapitalShipComponent.h"
#include "Component.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "AISwarmAgent.h"
#include <stdlib.h>
#include "Assignment.h"
#include "Camera.h"
#include "AISwarm.h"
#include "AITurretComponent.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct Enemy_CapitalShipData
{
  AISwarmAgent *agent;
  TransformComponent *transform;
  RigidbodyComponent *rigidbody;
  float speed;
  float turnSpeed;
  AITurretData *shooters;
} Enemy_CapitalShipData;

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

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* Enemy_CapitalShipComponent_Create()
{
  Component *component = Component_Create();

  Enemy_CapitalShipData *data = (Enemy_CapitalShipData*)calloc(1, sizeof(Enemy_CapitalShipData));
  data->speed = 1000;
  data->turnSpeed = 5;

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

static void Update(Component *component, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  Enemy_CapitalShipData *data = component->data;

  Vector2 target = Vector2_Add(AISwarmAgent_GetToCenterVector(data->agent), AISwarmAgent_GetToTargetVector(data->agent));
  Vector2 agentRepulsion = Vector2_Scale(AISwarmAgent_GetAgentRepulsionVector(data->agent), 1);
  target = Vector2_Add(target, agentRepulsion);
  target = Vector2_Add(target, AISwarmAgent_GetObjectRepulsionVector(data->agent));
  RigidbodyComponent_PIDLoopToDirection(data->rigidbody, target, data->turnSpeed, 5);
  RigidbodyComponent_AddForce(data->rigidbody, Vector2_Scale(TransformComponent_GetForward(data->transform), data->speed * RigidbodyComponent_GetMass(data->rigidbody)), FORCE_TYPE_FORCE);

  //if (AISwarmAgent_GetState(data->agent) == AISWARM_STATE_ATTACK)
  //{
    //AITurret_SetEnabled();
  //}
}

static void OnAttach(Component *component)
{
  Enemy_CapitalShipData *data = component->data;
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

  //Track the entity
  //Camera_TrackEntity(component->parent);
}

static void OnOtherAttach(Component *component, Component *other)
{
  Enemy_CapitalShipData *data = component->data;
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
  Enemy_CapitalShipData *data = component->data;
  Camera_UntrackEntity(component->parent);
  Assignment_ReciveEvent(ASSIGNMENT_EVENT_SHIP_DIE);
  free(data);
  free(component);
}
