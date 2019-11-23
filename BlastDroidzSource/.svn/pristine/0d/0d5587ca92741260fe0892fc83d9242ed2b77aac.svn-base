/******************************************************************************/
/*!
	\file   AISwarm.c
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AISwarm.h"
#include "AISwarmAgent.h"
#include <stdlib.h>
#include <AEEngine.h>
#include "Graphics.h"
#include "Camera.h"
#include "AIManager.h"
#include "Player.h"
#include "Physics_BroadPhase.h"
#include "AIGraph.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define AI_SWARM_INIT_COLLIDERS_LIST_SIZE 32

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct AISwarm
{
  AISwarmAgent *agents;
  int agentCount;

  AIMarker *target;
  Vector2 center;
  float radius;
  Component *transformComponent;
  TransformComponent *transform;

  float targetAttraction;

  float centerAttraction;

  float agentRepulsion;
  float agentRepulsionFalloff;

  float objectRepulsion;
  float objectRepulsionFalloff;
  float objectPadding;

  //float matchVelocities;
  //float matchVelocitiesFalloff;

  bool isTracked;

  AISWARM_STATE state;
  int targetPlayer;
  float timeSincePlayerSeen;

  AISwarm *next;
  AISwarm *prev;

  AISwarm *markerNext;
  AISwarm *markerPrev;
} AISwarm;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static ColliderComponent **colliderList;
static int colliderListCount;
static int colliderListMax;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void HandleSwarmBehavior(AISwarm *swarm, float dt);

static void HandleObjectRepulsion(AISwarm *swarm, AISwarmAgent *agent, float dt);

static void HandleAgentRepulsion(AISwarm *swarm, AISwarmAgent *agent, float dt);

static void AddColliderToList(ColliderComponent *collider);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void AISwarm_Init(void)
{
  if (colliderList)
    free(colliderList);
  colliderList = (ColliderComponent**)calloc(AI_SWARM_INIT_COLLIDERS_LIST_SIZE, sizeof(ColliderComponent*));
  colliderListCount = 0;
  colliderListMax = AI_SWARM_INIT_COLLIDERS_LIST_SIZE;
}

/*!
 * \brief
 *   Creates a new AISwarm.
 * \param targetAttraction
 *   The attraction to the swarm target. 
 * \return
 *   A new AISwarm. Is not a component!
 */
AISwarm* AISwarm_Create(float targetAttraction, float centerAttraction, float agentRepulsion, float agentRepulsionFalloff,
                        float objectRepulsion, float objectRepulsionFalloff, float objectPadding /*, float matchVelocities, float matchVelocitiesFalloff*/)
{
  AISwarm *swarm = (AISwarm*)calloc(1, sizeof(AISwarm));
  swarm->targetAttraction = targetAttraction;
  swarm->centerAttraction = centerAttraction;
  swarm->agentRepulsion = agentRepulsion;
  swarm->agentRepulsionFalloff = agentRepulsionFalloff;
  swarm->objectRepulsion = objectRepulsion;
  swarm->objectRepulsionFalloff = objectRepulsionFalloff;
  swarm->objectPadding = objectPadding;
  //swarm->matchVelocities = matchVelocities;
  //swarm->matchVelocitiesFalloff = matchVelocitiesFalloff;
  swarm->state = AISWARM_STATE_IDLE;
  swarm->transformComponent = TransformComponent_Create();
  swarm->transform = (TransformComponent*)swarm->transformComponent->data;
  AIManager_AddSwarm(swarm);
  return swarm;
}

/*!
 * \brief
 *   Updates the values of a swarm, and sets the values of each agent within.
 * \param swarm
 *   The swarm to update.
 * \param dt
 *   The elapsed time.
 */
void AISwarm_Update(AISwarm *swarm, float dt)
{
  swarm->center = Vector2_Zero();
  //swarm->target = Player_GetPosition(0);

  AISwarmAgent *iterator = swarm->agents;
  while (iterator) //Get center, clear boid values
  {
    AISwarmAgent_Clear(iterator);
    swarm->center = Vector2_Add(swarm->center, Vector2_Scale(AISwarmAgent_GetPosition(iterator), 1.0f / swarm->agentCount));
    TransformComponent_SetPosition(swarm->transform, swarm->center);

    iterator = AISwarmAgent_GetNext(iterator);
  }

  HandleSwarmBehavior(swarm, dt);

  //for (int i = 0; i < 4; i++)
  //{
  //  if (!Player_GetPlayer(i - 1))
  //    continue;
  //  if (Vector2_LengthSquared(Player_GetPosition(i)) < Vector2_LengthSquared(swarm->target))
  //    swarm->target = Player_GetPosition(i);
  //}

  iterator = swarm->agents;
  while (iterator)
  {
    Vector2 toCenter = Vector2_Scale(Vector2_Sub(swarm->center, AISwarmAgent_GetPosition(iterator)), swarm->centerAttraction);
    AISwarmAgent_SetToCenterVector(iterator, toCenter);

    swarm->radius = max(Vector2_DistanceSquared(AISwarmAgent_GetPosition(iterator), swarm->center), swarm->radius);
    TransformComponent_SetScaleFloats(swarm->transform, swarm->radius, swarm->radius);

    Vector2 toTarget = Vector2_Scale(Vector2_Normalize(Vector2_Sub(AIMarker_GetPosition(swarm->target), AISwarmAgent_GetPosition(iterator))), swarm->targetAttraction);
    AISwarmAgent_SetToTargetVector(iterator, toTarget);

    AISwarmAgent_SetDistanceToTarget(iterator, Vector2_Distance(AIMarker_GetPosition(swarm->target), AISwarmAgent_GetPosition(iterator)));

    HandleObjectRepulsion(swarm, iterator, dt);
    HandleAgentRepulsion(swarm, iterator, dt);

    iterator = AISwarmAgent_GetNext(iterator);
  }

  swarm->radius = sqrtf(swarm->radius);

  bool track = false;
  bool untrack = true;
  
  if (swarm->isTracked)
  {
    if (swarm->state == AISWARM_STATE_ATTACK)
    {
      untrack = false;
    }
    else
    {
      for (int i = 0; i < 4; i++)
      {
        if (Player_GetPlayer(i)->playerEntity)
        {
          if (Vector2_Distance(swarm->center, Player_GetPosition(i)) < 1750 + swarm->radius)
            untrack = false;
        }
      }
    }
  }
  else
  {
    if (swarm->state == AISWARM_STATE_ATTACK)
    {
      track = true;
    }
    else
    {
      for (int i = 0; i < 4; i++)
      {
        if (Player_GetPlayer(i)->playerEntity)
        {
          if (Vector2_Distance(swarm->center, Player_GetPosition(i)) < 1250 + swarm->radius)
          {
            track = true;
            break;
          }
        }
      }
    }
  }

  if (swarm->isTracked)
  {
    if (untrack)
    {
      swarm->isTracked = false;
      iterator = swarm->agents;
      while (iterator)
      {
        Camera_UntrackEntity(AISwarmAgent_GetEntity(iterator));
        iterator = AISwarmAgent_GetNext(iterator);
      }
    }
  }
  else
  {
    if (track)
    {
      swarm->isTracked = true;
      iterator = swarm->agents;
      while (iterator)
      {
        Camera_TrackEntity(AISwarmAgent_GetEntity(iterator));
        iterator = AISwarmAgent_GetNext(iterator);
      }
    }
  }
}

void AISwarm_AddAgent(AISwarm *swarm, AISwarmAgent *agent)
{
  AISwarmAgent_SetNext(agent, swarm->agents);
  if (swarm->agents)
  {
    AISwarmAgent_SetPrev(swarm->agents, agent);
  }
  swarm->agents = agent;
  swarm->agentCount++;

  swarm->center = Vector2_Zero();
  AISwarmAgent *iterator = swarm->agents;
  while (iterator)
  {
    swarm->center = Vector2_Add(swarm->center, Vector2_Scale(AISwarmAgent_GetPosition(iterator), 1.0f / swarm->agentCount));
    TransformComponent_SetPosition(swarm->transform, swarm->center);

    iterator = AISwarmAgent_GetNext(iterator);
  }

  iterator = swarm->agents;
  while (iterator)
  {
    swarm->radius = max(Vector2_DistanceSquared(AISwarmAgent_GetPosition(iterator), swarm->center), swarm->radius);
    TransformComponent_SetScaleFloats(swarm->transform, swarm->radius, swarm->radius);

    iterator = AISwarmAgent_GetNext(iterator);
  }

  swarm->radius = sqrtf(swarm->radius);
}

void AISwarm_RemoveAgent(AISwarm *swarm, AISwarmAgent *agent)
{
  if (AISwarmAgent_GetPrev(agent))
  {
    AISwarmAgent_SetNext(AISwarmAgent_GetPrev(agent), AISwarmAgent_GetNext(agent));
  }
  else
  {
    swarm->agents = AISwarmAgent_GetNext(agent);
  }
  if (AISwarmAgent_GetNext(agent))
  {
    AISwarmAgent_SetPrev(AISwarmAgent_GetNext(agent), AISwarmAgent_GetPrev(agent));
  }
  swarm->agentCount--;
  if (swarm->agentCount == 0)
  {
    AIManager_RemoveSwarm(swarm);
    if (swarm->target)
    {
      AIMarkerComponent_RemoveSwarm(swarm->target, swarm);
    }
    AISwarm_Free(swarm);
  }
}

void AISwarm_DetectPlayer(AISwarm *swarm, int playerSeen)
{
  if (swarm->state == AISWARM_STATE_IDLE || swarm->state == AISWARM_STATE_MOVE)
  {
    swarm->state = AISWARM_STATE_ATTACK;
    //swarm->target = Player_GetPlayer(playerSeen)->aiMarker;
    AIMarkerComponent_AddSwarm(Player_GetPlayer(playerSeen)->aiMarker, swarm);
    swarm->targetPlayer = playerSeen;
  }
  swarm->timeSincePlayerSeen = 0;
}

void AISwarm_MarkerLost(AISwarm *swarm)
{
  AIMarker *marker = AIGraph_GetNearestMarker(swarm->center, true);
  if (marker == NULL)
    return;
  //swarm->target = marker;
  AIMarkerComponent_AddSwarm(marker, swarm);
}

Vector2 AISwarm_GetCenter(AISwarm *swarm)
{
  return swarm->center;
}

float AISwarm_GetRadius(AISwarm *swarm)
{
  return swarm->radius;
}

void AISwarm_SetNext(AISwarm *swarm, AISwarm *next)
{
  swarm->next = next;
}

void AISwarm_SetPrev(AISwarm *swarm, AISwarm *prev)
{
  swarm->prev = prev;
}

AISwarm* AISwarm_GetNext(AISwarm *swarm)
{
  return swarm->next;
}

AISwarm* AISwarm_GetPrev(AISwarm *swarm)
{
  return swarm->prev;
}

void AISwarm_SetMarkerNext(AISwarm *swarm, AISwarm *markerNext)
{
  swarm->markerNext = markerNext;
}

void AISwarm_SetMarkerPrev(AISwarm *swarm, AISwarm *markerPrev)
{
  swarm->markerPrev = markerPrev;
}

AISwarm* AISwarm_GetMarkerNext(AISwarm *swarm)
{
  return swarm->markerNext;
}

AISwarm* AISwarm_GetMarkerPrev(AISwarm *swarm)
{
  return swarm->markerPrev;
}

void AISwarm_SetTargetMarker(AISwarm *swarm, AIMarker *marker)
{
  swarm->target = marker;
}

AIMarker* AISwarm_GetTargetMarker(AISwarm *swarm)
{
  return swarm->target;
}

AISWARM_STATE AISwarm_GetState(AISwarm *swarm)
{
  return swarm->state;
}

void AISwarm_Free(AISwarm *swarm)
{
  swarm->transformComponent->destroy(swarm->transformComponent);
  free(swarm);
}

void AISwarm_Exit()
{
  free(colliderList);
}

void AISwarm_Debug(AISwarm *swarm)
{
  AISwarmAgent *iterator = swarm->agents;

  while (iterator)
  {
    AISwarmAgent_Debug(iterator);
    //Camera_DrawLineBox(AISwarmAgent_GetPosition(iterator), swarm->agentRepulsionFalloff, swarm->agentRepulsionFalloff, 1, 0, 0, 1);

    iterator = AISwarmAgent_GetNext(iterator);
  }

  if (swarm->isTracked)
    Camera_DrawLineBox(swarm->center, swarm->radius, swarm->radius, 1, 0, 0, 0.5f);
  else
    Camera_DrawLineBox(swarm->center, swarm->radius, swarm->radius, 1, 0, 1, 0.5f);

  Primitive *p = Primitive_CreateRect(15, 15, 0XFFFFFFFF);
  
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);

  Vector2 drawPos = Camera_GetPosition();
  drawPos = Vector2_Scale(Vector2_Sub(swarm->center, drawPos), Camera_GetRenderScaleFactor());
  AEGfxSetPosition(drawPos.x, drawPos.y);

  AEGfxSetTintColor(0.45f, 0.15f, 1, 1);

  AEGfxMeshDraw(p->mesh, AE_GFX_MDM_TRIANGLES);

  AEGfxSetTintColor(1, 1, 1, 1);

  Primitive_Destroy(p);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void HandleSwarmBehavior(AISwarm *swarm, float dt)
{
  if (swarm->state == AISWARM_STATE_IDLE)
  {
    if (swarm->target == NULL)
    {
      //swarm->target = AIGraph_GetNearestMarker(swarm->center, true);
      AIMarkerComponent_AddSwarm(AIGraph_GetNearestMarker(swarm->center, true), swarm);
    }
  }
  else if (swarm->state == AISWARM_STATE_MOVE)
  {
    swarm->state = AISWARM_STATE_IDLE; //Unused state
  }
  else if (swarm->state == AISWARM_STATE_ATTACK)
  {
    //swarm->target = Player_GetPosition(swarm->targetPlayer);
    swarm->timeSincePlayerSeen += dt;
    if (swarm->timeSincePlayerSeen > 1)
    {
      swarm->state = AISWARM_STATE_IDLE;
      //swarm->target = AIGraph_GetNearestMarker(swarm->center, true);
      AIMarkerComponent_AddSwarm(AIGraph_GetNearestMarker(swarm->center, true), swarm);
    }
  }
}

static void HandleObjectRepulsion(AISwarm *swarm, AISwarmAgent *agent, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  int i, j;
  int nodeCount;
  GridNode **nodes = Physics_BroadPhase_GetNodeInRadius(AISwarmAgent_GetPosition(agent), swarm->objectRepulsionFalloff, &nodeCount);

  if (!nodes) //If not in grid
    return;

  colliderListCount = 0; //Reset the list of colliders

  for (i = 0; i < nodeCount; i++)
  {
    for (j = 0; j < Physics_BroadPhase_GetGridNodeRigidbodyCount(nodes[i]); j++)
    {
      ColliderComponent *collider = RigidbodyComponent_GetCollider(Physics_BroadPhase_GetGridNodeRigidbodies(nodes[i])[j]);
      AddColliderToList(collider);
    }

    for (j = 0; j < Physics_BroadPhase_GetGridNodeColliderCount(nodes[i]); j++)
    {
      ColliderComponent *collider = Physics_BroadPhase_GetGridNodeColliders(nodes[i])[j];
      AddColliderToList(collider);
    }
  }

  for (i = 0; i < colliderListCount; i++)
  {
    //float dist = Vector2_Distance(AISwarmAgent_GetPosition(iterator), ColliderComponent_GetBoundingCircleWorldPos(colliderList[i])); //Debug values
    //float rad = ColliderComponent_GetBoundingCircleRadius(colliderList[i]);
    float distFromEdge = Vector2_Distance(AISwarmAgent_GetPosition(agent), ColliderComponent_GetBoundingCircleWorldPos(colliderList[i]))
      - (ColliderComponent_GetBoundingCircleRadius(colliderList[i]) + swarm->objectPadding);
    distFromEdge = max(distFromEdge, 0);
    float repulsion = swarm->objectRepulsion * (1 - min((distFromEdge / swarm->objectRepulsionFalloff), 1));
    Vector2 circleToAgent = Vector2_Normalize(Vector2_Sub(AISwarmAgent_GetPosition(agent), ColliderComponent_GetBoundingCircleWorldPos(colliderList[i])));
    Vector2 repulsionDir = Vector2_GetOrthogonal(circleToAgent); //TODO: Raycast, get tangent of surface
    Vector2 directionVector = AISwarmAgent_GetVelocity(agent);
    if (Vector2_Dot(repulsionDir, directionVector) < 0) //TODO: try summing the targets, try different variations on this
    {
      repulsionDir = Vector2_Scale(repulsionDir, -1);
    }
    directionVector = Vector2_Normalize(directionVector);
    repulsionDir = Vector2_Scale(repulsionDir, -Vector2_Dot(circleToAgent, directionVector) / 2 + 0.5f);
    repulsionDir = Vector2_Scale(repulsionDir, repulsion);
    AISwarmAgent_AddToObjectRepulsionVector(agent, repulsionDir);
  }
}

static void HandleAgentRepulsion(AISwarm *swarm, AISwarmAgent *agent, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  AISwarmAgent *iterator = AISwarmAgent_GetNext(agent);

  while (iterator)
  {
    float dist = Vector2_Distance(AISwarmAgent_GetPosition(agent), AISwarmAgent_GetPosition(iterator));
    Vector2 repulsion = Vector2_Scale(Vector2_Normalize(Vector2_Sub(AISwarmAgent_GetPosition(iterator), AISwarmAgent_GetPosition(agent))),
                                      swarm->agentRepulsion * (1 - min((dist / swarm->agentRepulsionFalloff), 1)) * -1);

    float agentDotObjectRepulsion = max(Vector2_Dot(Vector2_Normalize(AISwarmAgent_GetObjectRepulsionVector(agent)), Vector2_Normalize(repulsion)) * 1, 0);
    agentDotObjectRepulsion = agentDotObjectRepulsion + (1 - agentDotObjectRepulsion) * min(1 - Vector2_Length(AISwarmAgent_GetObjectRepulsionVector(agent)) / 500, 1);
    Vector2 trueRepulsion = Vector2_Scale(repulsion, agentDotObjectRepulsion);
    AISwarmAgent_AddToAgentRepulsionVector(agent, trueRepulsion);

    repulsion = Vector2_Scale(repulsion, -1);
    agentDotObjectRepulsion = max(Vector2_Dot(Vector2_Normalize(AISwarmAgent_GetObjectRepulsionVector(iterator)), Vector2_Normalize(repulsion)) * 1, 0);
    agentDotObjectRepulsion = agentDotObjectRepulsion + (1 - agentDotObjectRepulsion) * min(1 - Vector2_Length(AISwarmAgent_GetObjectRepulsionVector(iterator)) / 500, 1);
    trueRepulsion = Vector2_Scale(repulsion, agentDotObjectRepulsion);
    AISwarmAgent_AddToAgentRepulsionVector(iterator, trueRepulsion);

    iterator = AISwarmAgent_GetNext(iterator);
  }
}

static void AddColliderToList(ColliderComponent *collider)
{
  int i;
  if (ColliderComponent_GetLayer(collider) != PHYSICS_LAYER_DEFAULT && ColliderComponent_GetLayer(collider) != PHYSICS_LAYER_PLAYER) //TODO: Have this be variable from swarm to swarm.
  {
    return;
  }
  for (i = 0; i < colliderListCount; i++) //Is the collider already in the list?
  {
    if (colliderList[i] == collider)
    {
      return;
    }
  }
  //Add to list
  if (colliderListCount == colliderListMax) //Allocate space if needed
  {
    colliderList = (ColliderComponent**)realloc(colliderList, sizeof(ColliderComponent*) * colliderListMax * 2);
    colliderListMax *= 2;
  }
  colliderList[colliderListCount] = collider;
  colliderListCount++;
}