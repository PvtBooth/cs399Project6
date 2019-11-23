/******************************************************************************/
/*!
	\file   PhysicsManager.h
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "RigidbodyComponent.h"
#include "Physics_layers.h"
#include <stdbool.h>

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

//!Forward declaration to avoid circular referencing
typedef struct ColliderComponent ColliderComponent;

//!Forward declaration to avoid circular referencing
typedef struct RigidbodyComponent RigidbodyComponent;

typedef struct RaycastData
{
  Entity *entity;
  TransformComponent *transform;
  ColliderComponent *collider;
  RigidbodyComponent *rigidbody;
  Vector2 origin;
  Vector2 direciton;
  Vector2 point;
  Vector2 normal;
  Vector2 reflection;
  float distance;
} RaycastData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void PhysicsManager_Init();

void PhysicsManager_NewGridFromCount(float width, float height, int xCount, int yCount);

void PhysicsManager_NewGridFromSize(float width, float height, float nodeWidth, int nodeHeight);

bool PhysicsManager_GetUnusedRigidbody(Component** component);

void PhysicsManager_RegisterRigidbody(RigidbodyComponent *rigidbody);

void PhysicsManager_RemoveRigidbody(RigidbodyComponent *rigidbody);

void PhysicsManager_Update(float dt);

bool PhysicsManager_Raycast(RaycastData *raycastData, Vector2 origin, Vector2 direction, float distance);

bool PhysicsManager_RaycastWithMask(RaycastData *raycastData, Vector2 origin, Vector2 direction, float distance, LayerMask layerMask);

const ColliderComponent** PhysicsManager_BoxCast(Vector2 origin, Vector2 halfSize, int *collidersHit);

const ColliderComponent** PhysicsManager_CircleCast(Vector2 origin, float radius, int *collidersHit);

float PhysicsManager_GetArenaRadius();

void PhysicsManager_Exit();
