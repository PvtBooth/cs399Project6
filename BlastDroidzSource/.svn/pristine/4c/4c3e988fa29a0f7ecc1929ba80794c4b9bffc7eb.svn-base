/******************************************************************************/
/*!
	\file   Enemy_SimpleComponent.c
	\author Evan Kau
	\date   2/18/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "Enemy_SimpleComponent.h"
#include "AISwarm.h"
#include "TransformComponent.h"
#include <stdlib.h>
#include "RigidbodyComponent.h"
#include "Camera.h"
#include "Assignment.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct Enemy_SimpleData
{
  AISwarmAgent *agent;
  TransformComponent *transform;
  RigidbodyComponent *rigidbody;
  float speed;
  float turnSpeed;
} Enemy_SimpleData;

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

Component* Enemy_SimpleComponent_Create()
{
  Component *component = Component_Create();

  Enemy_SimpleData *data = (Enemy_SimpleData*)calloc(1, sizeof(Enemy_SimpleData));
  data->speed = 1500;
  data->turnSpeed = 250;

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
  Enemy_SimpleData *data = component->data;
  
  Vector2 target = Vector2_Add(AISwarmAgent_GetToCenterVector(data->agent), AISwarmAgent_GetToTargetVector(data->agent));
  Vector2 agentRepulsion = AISwarmAgent_GetAgentRepulsionVector(data->agent);
  //agentRepulsion.x /= 10;
  target = Vector2_Add(target, agentRepulsion);
  target = Vector2_Add(target, AISwarmAgent_GetObjectRepulsionVector(data->agent));
  RigidbodyComponent_PIDLoopToDirection(data->rigidbody, target, data->turnSpeed, 20);
  RigidbodyComponent_AddForce(data->rigidbody, Vector2_Scale(TransformComponent_GetForward(data->transform), data->speed * RigidbodyComponent_GetMass(data->rigidbody)), FORCE_TYPE_FORCE);
}

static void OnAttach(Component *component)
{
  Enemy_SimpleData *data = component->data;
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
  Enemy_SimpleData *data = component->data;
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
  Enemy_SimpleData *data = component->data;
  Camera_UntrackEntity(component->parent);
  Assignment_ReciveEvent(ASSIGNMENT_EVENT_SHIP_DIE);
  free(data);
  free(component);
}