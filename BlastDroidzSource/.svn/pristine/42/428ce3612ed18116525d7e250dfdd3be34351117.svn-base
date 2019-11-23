/******************************************************************************/
/*!
	\file   RigidbodyComponent.h
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include <stdbool.h>
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Physics_Layers.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum FORCE_TYPE
{
  FORCE_TYPE_FORCE,
  FORCE_TYPE_IMPULSE,
} FORCE_TYPE;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct RigidbodyComponent RigidbodyComponent;

//!Forward declaration to avoid circular referencing errors
typedef struct ColliderComponent ColliderComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* RigidbodyComponent_Create(float mass, float inertia, float drag, float angularDrag, bool isKinematic, bool canLeave);

Component* RigidbodyComponent_Copy(RigidbodyComponent *rigidbody);

void RigidbodyComponent_Update(RigidbodyComponent *rigidbody, float dt);

void RigidbodyComponent_Translate(RigidbodyComponent *rigidbody, Vector2 deltaPosition);

void RigidbodyComponent_TranslateFloats(RigidbodyComponent *rigidbody, float deltaPositionX, float deltaPositionY);

void RigidbodyComponent_AddVelocity(RigidbodyComponent *rigidbody, Vector2 deltaVelocity);

void RigidbodyComponent_AddVelocityFloats(RigidbodyComponent *rigidbody, float deltaVelocityX, float deltaVelocityY);

void RigidbodyComponent_AddAngularVelocity(RigidbodyComponent *rigidbody, float deltaAngularVelocity);

void RigidbodyComponent_AddForce(RigidbodyComponent *rigidbody, Vector2 force, FORCE_TYPE type);

void RigidbodyComponent_AddForceFloats(RigidbodyComponent *rigidbody, float forceX, float forceY, FORCE_TYPE type);

void RigidbodyComponent_ApplyForceAtPoint(RigidbodyComponent *rigidbody, Vector2 force, Vector2 point, FORCE_TYPE type);

void RigidbodyComponent_ApplyForceAtPointFloats(RigidbodyComponent *rigidbody, float forceX, float forceY, float pointX, float pointY, FORCE_TYPE type);

void RigidbodyComponent_ApplyForceAtBodyPoint(RigidbodyComponent *rigidbody, Vector2 force, Vector2 point, FORCE_TYPE type);

void RigidbodyComponent_ApplyForceAtBodyPointFloats(RigidbodyComponent *rigidbody, float forceX, float forceY, float pointX, float pointY, FORCE_TYPE type);

void RigidbodyComponent_ApplyTorque(RigidbodyComponent *rigidbody, float torque);

void RigidbodyComponent_PIDLoopToPoint(RigidbodyComponent *rigidbody, Vector2 point, float rate, float tightness);

void RigidbodyComponent_PIDLoopToDirection(RigidbodyComponent *rigidbody, Vector2 direction, float rate, float tightness);

void RigidbodyComponent_PIDLoopToAngle(RigidbodyComponent *rigidbody, float targetAngle, float rate, float tightness);

void RigidbodyComponent_SetID(RigidbodyComponent *rigidbody, unsigned int id);

Component *RigidbodyComponent_GetComponent(const RigidbodyComponent *rigidbody);

void RigidbodyComponent_SetCollider(RigidbodyComponent *rigidbody, ColliderComponent *collider);

TransformComponent* RigidbodyComponent_GetTransform(const RigidbodyComponent *rigidbody);

ColliderComponent* RigidbodyComponent_GetCollider(const RigidbodyComponent *rigidbody);

PhysicsLayer RigidbodyComponent_GetLayer(const RigidbodyComponent *rigidbody);

void RigidbodyComponent_SetLayer(RigidbodyComponent *rigidbody, PhysicsLayer physicsLayer);

float RigidbodyComponent_GetMass(const RigidbodyComponent *rigidbody);

float RigidbodyComponent_GetInertia(const RigidbodyComponent *rigidbody);

Vector2 RigidbodyComponent_GetVelocity(const RigidbodyComponent *rigidbody);

float RigidbodyComponent_GetAngularVelocity(const RigidbodyComponent *rigidbody);

bool RigidbodyComponent_GetIsKinematic(const RigidbodyComponent *rigidbody);

unsigned int RigidbodyComponent_GetID(const RigidbodyComponent *rigidbody);

void RigidbodyComponent_SetNext(RigidbodyComponent *rigidbody, RigidbodyComponent *next);

void RigidbodyComponent_SetPrev(RigidbodyComponent *rigidbody, RigidbodyComponent *prev);

RigidbodyComponent* RigidbodyComponent_GetNext(const RigidbodyComponent *rigidbody);

RigidbodyComponent* RigidbodyComponent_GetPrev(const RigidbodyComponent *rigidbody);
