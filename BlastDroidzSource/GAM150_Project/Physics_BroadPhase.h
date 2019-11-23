/******************************************************************************/
/*!
	\file   Physics_BroadPhase.h
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "RigidbodyComponent.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct GridNode GridNode;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Physics_BroadPhase_CreateGrid(float levelWidth, float levelHeight, float nodeWidth, float nodeHeight);

void Physics_BroadPhase_RegisterRigidbody(RigidbodyComponent *rigidbody);

void Physics_BroadPhase_RemoveRigidbody(RigidbodyComponent *rigidbody);

void Physics_BroadPhase_RegisterCollider(ColliderComponent *collider);

void Physics_BroadPhase_RemoveCollider(ColliderComponent *rigidbody);

void Physics_BroadPhase_Update();

Vector2 Physics_BroadPhase_GetNearestPointWithinGrid(Vector2 point);

GridNode* Physics_BroadPhase_GetNodeAtPoint(Vector2 position);

Vector2 Physics_BroadPhase_GetNextNodePositionFromPoint(Vector2 position, Vector2 direction);

GridNode* Physics_BroadPhase_GetGridNodeFromNode(GridNode *node, int xOffset, int yOffset);

GridNode** Physics_BroadPhase_GetNodeInRadius(Vector2 position, float radius, int *nodeCount);

RigidbodyComponent** Physics_BroadPhase_GetGridNodeRigidbodies(GridNode *node);

int Physics_BroadPhase_GetGridNodeRigidbodyCount(GridNode *node);

Vector2 Physics_BroadPhase_GetGridNodeWorldPosition(GridNode *node);

ColliderComponent** Physics_BroadPhase_GetGridNodeColliders(GridNode *node);

int Physics_BroadPhase_GetGridNodeColliderCount(GridNode *node);

void Physics_BroadPhase_SanitizeGrid();

void Physics_BroadPhase_FreeGrid();

void Physics_BroadPhase_Debug();