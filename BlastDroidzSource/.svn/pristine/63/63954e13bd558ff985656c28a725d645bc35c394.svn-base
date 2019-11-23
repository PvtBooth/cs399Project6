/******************************************************************************/
/*!
	\file   AISwarmAgent.c
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AISwarmAgent.h"
#include "AISwarm.h"
#include <stdlib.h>
#include "TransformComponent.h"
#include "Entity.h"
#include "RigidbodyComponent.h"
#include "Camera.h"
#include "AIManager.h"
#include "Player.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct AISwarmAgent
{
  AISwarm *swarm;

  float authority;

  float sightDistance;

  Vector2 toTarget;
  Vector2 toCenter;
  Vector2 agentRepulsion;
  Vector2 objectRepulsion;
  Vector2 matchVelocity;
  float distanceToTarget;

  TransformComponent *transform;
  RigidbodyComponent *rigidbody;
  Component *component;

  AISwarmAgent *next;
  AISwarmAgent *prev;
} AISwarmAgent;

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
static void EventHandle();

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *   Creates a component with an AISwarmAgent attached as data.
 * \param authority
 *   The authority of this agent, which is used to determine certain values of the boid vectors.
 * \return
 *   The new component with an AISwarmAgent attached as data.
 */
Component* AISwarmAgent_Create(float authority, float sightDistance)
{
  Component *component = Component_Create();

  AISwarmAgent *agent = (AISwarmAgent*)calloc(1, sizeof(AISwarmAgent));
  agent->swarm = NULL;
  agent->authority = authority;
  agent->sightDistance = sightDistance;
  agent->component = component;

  component->data = agent;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->destroy = &Destroy;
  component->type = COMPONENT_TYPE_AISWARMAGENT;

  return component;
}

/*!
* \brief
*   Sets the toTarget vector on the agent.
* \param agent
*   The agent to set the toTarget vector of.
* \param toTargetVector
*   The vector to use.
*/
void AISwarmAgent_SetToTargetVector(AISwarmAgent *agent, Vector2 toTargetVector)
{
  agent->toTarget = toTargetVector;
}

/*!
 * \brief
 *   Sets the toCenter vector on the agent.
 * \param agent
 *   The agent to set the toCenter vector of.
 * \param toCenterVector
 *   The vector to use.
 */
void AISwarmAgent_SetToCenterVector(AISwarmAgent *agent, Vector2 toCenterVector)
{
  agent->toCenter = toCenterVector;
}

void AISwarmAgent_AddToAgentRepulsionVector(AISwarmAgent *agent, Vector2 repulsionVector)
{
  agent->agentRepulsion = Vector2_Add(agent->agentRepulsion, repulsionVector);
}

void AISwarmAgent_AddToObjectRepulsionVector(AISwarmAgent *agent, Vector2 repulsionVector)
{
  agent->objectRepulsion = Vector2_Add(agent->objectRepulsion, repulsionVector);
}

/*!
* \brief
*   Sets the distance to target value on the agent.
* \param agent
*   The agent to set distance to target value of.
* \param distance
*   The distance to the target.
*/
void AISwarmAgent_SetDistanceToTarget(AISwarmAgent *agent, float distance)
{
  agent->distanceToTarget = distance;
}

Vector2 AISwarmAgent_GetToTargetVector(AISwarmAgent *agent)
{
  return agent->toTarget;
}

Vector2 AISwarmAgent_GetToCenterVector(AISwarmAgent *agent)
{
  return agent->toCenter;
}

Vector2 AISwarmAgent_GetAgentRepulsionVector(AISwarmAgent *agent)
{
  return agent->agentRepulsion;
}

Vector2 AISwarmAgent_GetObjectRepulsionVector(AISwarmAgent *agent)
{
  return agent->objectRepulsion;
}

float AISwarmAgent_GetDistanceToTarget(AISwarmAgent *agent)
{
  return agent->distanceToTarget;
}

/*!
 * \brief
 *   Clears the vectors of the given agent.
 * \param agent
 *   The given agent to clear.
 */
void AISwarmAgent_Clear(AISwarmAgent *agent)
{
  agent->toCenter.x = 0;
  agent->toCenter.y = 0;
  agent->toTarget.x = 0;
  agent->toTarget.y = 0;
  agent->agentRepulsion.x = 0;
  agent->agentRepulsion.y = 0;
  agent->objectRepulsion.x = 0;
  agent->objectRepulsion.y = 0;
  agent->matchVelocity.x = 0;
  agent->matchVelocity.y = 0;
}

/*!
 * \brief
 *   Returns the position of the given agent.
 * \param agent
 *   The agent to get the position of.
 * \return
 *   The world position of the agent.
 */
Vector2 AISwarmAgent_GetPosition(AISwarmAgent *agent)
{
  return TransformComponent_GetPosition(agent->transform);
}

/*!
 * \brief
 *   Returns the velocity of the given agent.
 * \param agent
 *   The agent to get the velocity of.
 * \return
 *   The velocity of the agent.
 */
Vector2 AISwarmAgent_GetVelocity(AISwarmAgent *agent)
{
  return RigidbodyComponent_GetVelocity(agent->rigidbody);
}

AISWARM_STATE AISwarmAgent_GetState(AISwarmAgent *agent)
{
  return AISwarm_GetState(agent->swarm);
}

/*!
 * \brief
 *   Adds the given agent to the given swarm.
 * \param agent
 *   The AISwarmAgent to add to the AISwarm.
 * \param swarm
 *   The AISwarm to add the agent to.
 */
void AISwarmAgent_AddToSwarm(AISwarmAgent *agent, AISwarm *swarm)
{
  AISwarm_AddAgent(swarm, agent);
  agent->swarm = swarm;
}

/*!
 * \brief
 *   Removes an agent from its swarm.
 * \param agent
 *   The AISwarmAgent remove from its AISwarm.
 */
void AISwarmAgent_RemoveFromSwarm(AISwarmAgent *agent)
{
  AISwarm_RemoveAgent(agent->swarm, agent);
  agent->swarm = NULL;
}

/*!
 * \brief
 *   Sets the next AISwarmAgent on an AISwarmAgent.
 * \param agent
 *   The AISwarmAgent to modify.
 * \param next
 *   The AISwarmAgent to use as the next for the other agent.
 */
void AISwarmAgent_SetNext(AISwarmAgent *agent, AISwarmAgent *next)
{
  agent->next = next;
}

/*!
 * \brief
 *   Sets the previous AISwarmAgent on an AISwarmAgent.
 * \param agent
 *   The AISwarmAgent to modify.
 * \param prev
 *   The AISwarmAgent to use as the previous for the other agent.
 */
void AISwarmAgent_SetPrev(AISwarmAgent *agent, AISwarmAgent *prev)
{
  agent->prev = prev;
}

/*!
 * \brief
 *   Gets the next AISwarmAgent from another.
 * \param agent
 *   The AISwarmAgent to get the next agent from.
 * \return
 *   The next AISwarmAgent.
 */
AISwarmAgent* AISwarmAgent_GetNext(AISwarmAgent *agent)
{
  return agent->next;
}

/*!
 * \brief
 *   Gets the previous AISwarmAgent from another.
 * \param agent
 *   The AISwarmAgent to get the previous agent from.
 * \return
 *   The previous AISwarmAgent.
 */
AISwarmAgent* AISwarmAgent_GetPrev(AISwarmAgent *agent)
{
  return agent->prev;
}

Entity* AISwarmAgent_GetEntity(AISwarmAgent *agent)
{
  return agent->component->parent;
}

void AISwarmAgent_Debug(AISwarmAgent *agent)
{
  Vector2 position = TransformComponent_GetPosition(agent->transform);
  //Vector2 relToCenter = Vector2_Add(position, Vector2_Scale(agent->toCenter, 0.1f));
  //Vector2 relToTarget = Vector2_Add(position, Vector2_Scale(agent->toTarget, 0.1f));
  Vector2 relAgentRepulsion = Vector2_Add(position, Vector2_Scale(agent->agentRepulsion, 0.1f));
  //Vector2 relObjectRepulsion = Vector2_Add(position, Vector2_Scale(agent->objectRepulsion, 0.1f));
  //Camera_DrawLine(position.x, position.y, 0.8f, 0, 1, 1, relToCenter.x, relToCenter.y, 0.8f, 0, 1, 1, true);
  //Camera_DrawLine(position.x, position.y, 0.35f, 0, 1, 1, relToTarget.x, relToTarget.y, 0.35f, 0, 1, 1, true);
  Camera_DrawLine(position.x, position.y, 1, 1, 0.75f, 1, relAgentRepulsion.x, relAgentRepulsion.y, 1, 1, 0.75f, 1, true);
  //Camera_DrawLine(position.x, position.y, 1, 0, 1.0f, 1, relObjectRepulsion.x, relObjectRepulsion.y, 1, 0, 1, 1, true);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *component, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  AISwarmAgent *agent = component->data;
  float sightDistanceSquared = agent->sightDistance * agent->sightDistance;
  int i;

  if (agent->swarm == NULL)
  {
    AISwarm *swarm = AIManager_GetNearestSwarm(TransformComponent_GetPosition(agent->transform));
    if (!swarm || Vector2_Distance(AISwarm_GetCenter(swarm), TransformComponent_GetPosition(agent->transform)) > AISwarm_GetRadius(swarm) + 650) //TODO: refine fudge factor
    {
      swarm = AISwarm_Create(450, 0.15f, 500, 200, 2000, 500, 75);
    }
    AISwarmAgent_AddToSwarm(agent, swarm);
  }

  for (i = 0; i < 4; i++)
  {
    if (Player_GetPlayer(i))
    {
      if (sightDistanceSquared > Vector2_DistanceSquared(Player_GetPosition(i), TransformComponent_GetPosition(agent->transform)))
      {
        RaycastData data;
        if (PhysicsManager_RaycastWithMask(&data, TransformComponent_GetPosition(agent->transform), Vector2_Sub(Player_GetPosition(i), TransformComponent_GetPosition(agent->transform)), agent->sightDistance, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_DEFAULT, PHYSICS_LAYER_PLAYER)))
        {
          //Camera_DrawLine(data.origin.x, data.origin.y, 1, 0, 0, 1, data.point.x, data.point.y, 1, 0, 0, 1, true);
          if (data.entity == Player_GetPlayer(i)->playerEntity)
          {
            AISwarm_DetectPlayer(agent->swarm, i);
          }
        }
      }
    }
  }
}

static void OnAttach(Component *component)
{
  AISwarmAgent *agent = component->data;
  TransformComponent *transform = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_TRANSFORM)->data;
  if (transform)
  {
    agent->transform = transform;
  }
  RigidbodyComponent *rigidbody = Entity_FindComponentByType(component->parent, COMPONENT_TYPE_RIGIDBODY)->data;
  if (rigidbody)
  {
    agent->rigidbody = rigidbody;
  }
}

static void OnOtherAttach(Component *component, Component *other)
{
  AISwarmAgent *agent = component->data;
  if (other->type == COMPONENT_TYPE_TRANSFORM)
  {
    agent->transform = other->data;
  }
  else if (other->type == COMPONENT_TYPE_RIGIDBODY)
  {
    agent->rigidbody = other->data;
  }
}

static void Destroy(Component *component)
{
  AISwarmAgent *agent = component->data;
  if (agent->swarm)
  {
    AISwarm_RemoveAgent(agent->swarm, agent);
  }
  free(agent);
  free(component);
}