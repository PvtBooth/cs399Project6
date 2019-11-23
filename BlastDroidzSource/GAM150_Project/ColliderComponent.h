/******************************************************************************/
/*!
	\file   ColliderComponent.h
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include "Vector2.h"
#include <stdbool.h>
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "PhysicsManager.h"
#include "Physics_Layers.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum COLLIDER_TYPE
{
  COLLIDER_TYPE_BOX,
  COLLIDER_TYPE_CIRCLE,
  COLLIDER_TYPE_CONVEX,
} COLLIDER_TYPE;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct BoundingCircle BoundingCircle;
typedef struct PhysicsMaterial PhysicsMaterial;

typedef struct BoxCollider
{
  Vector2 halfSize;
  Vector2 offset;
} BoxCollider;

typedef struct CircleCollider
{
  float radius;
  Vector2 offset;
} CircleCollider;

typedef struct PolygonColliderEdge
{
  Vector2 point1;
  Vector2 point2;
  Vector2 nearestToOrigin; //!The point closest to the center of the collider.
  Vector2 axis;
  float length;
} PolygonColliderEdge;

typedef struct PolygonalCollider
{
  Vector2 *mesh; //!The list of vertices that makes up the mesh. Assumes this draws a single line around the object.
  int vertexCount;
  PolygonColliderEdge *axisList; //!The list of axis that the collider has.
  int axisCount;
} PolygonalCollider;

typedef struct ColliderComponent ColliderComponent;

//!Forward declaration to avoid circular referencing errors
typedef struct RigidbodyComponent RigidbodyComponent;

//!Forward declaration to avoid circular referencing errors
typedef struct RaycastData RaycastData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* ColliderComponent_CreateBox(Vector2 halfSize, Vector2 offset, bool isTrigger);

Component* ColliderComponent_CreateCircle(float radius, Vector2 offset, bool isTrigger);

Component* ColliderComponent_CreatePolygon(const Vector2 *mesh, int vertexCount, bool isTrigger);

Component* ColliderComponent_Copy(ColliderComponent *collider);

void ColliderComponent_CreateProxyBox(ColliderComponent *proxyCollider, BoxCollider *proxyBox, TransformComponent *proxyTransform, Vector2 halfSize, Vector2 offset);

void ColliderComponent_CreateProxyCircle(ColliderComponent *proxyCollider, CircleCollider *proxyCircle, TransformComponent *proxyTransform, float radius, Vector2 offset);

float ColliderComponent_TransformToAxis(const ColliderComponent *collider, Vector2 axis, Vector2 toCenter);

void ColliderComponent_RaycastAgainstCollider(RaycastData *raycastData, ColliderComponent *collider);

void ColliderComponent_AddCollisionWith(ColliderComponent *this, const ColliderComponent *other);

bool ColliderComponent_CheckedCollisionWith(const ColliderComponent *this, const ColliderComponent *other);

unsigned int ColliderComponent_GetID(const ColliderComponent *collider);

COLLIDER_TYPE ColliderComponent_GetType(const ColliderComponent *collider);

const BoundingCircle* ColliderComponent_GetBoundingCircle(const ColliderComponent *collider);

TransformComponent* ColliderComponent_GetTransform(const ColliderComponent *collider);

RigidbodyComponent* ColliderComponent_GetRigidbody(const ColliderComponent *collider);

void* ColliderComponent_GetCollider(const ColliderComponent *collider);

Vector2 ColliderComponent_GetBoundingCircleWorldPos(const ColliderComponent *collider);

float ColliderComponent_GetBoundingCircleRadius(const ColliderComponent *collider);

Vector2* ColliderComponent_GetBoundingCircleMin(ColliderComponent *collider);

Vector2* ColliderComponent_GetBoundingCircleMax(ColliderComponent *collider);

PhysicsLayer ColliderComponent_GetLayer(const ColliderComponent *collider);

void ColliderComponent_SetLayer(ColliderComponent *collider, PhysicsLayer physicsLayer);

bool ColliderComponent_GetIsTrigger(const ColliderComponent *collider);

Entity* ColliderComponent_GetEntity(const ColliderComponent *collider);

void ColliderComponent_SetRigidbody(ColliderComponent *collider, RigidbodyComponent *rigidbody);

void ColliderComponent_SetIsTrigger(ColliderComponent *collider, bool isTrigger);

float ColliderComponent_GetRestitution(const ColliderComponent *collider);

void ColliderComponent_SetRestitution(ColliderComponent *collider, float restitution);

bool ColliderComponent_IsProxy(ColliderComponent *collider);