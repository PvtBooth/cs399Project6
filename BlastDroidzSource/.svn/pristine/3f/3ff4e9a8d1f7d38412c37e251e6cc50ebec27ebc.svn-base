/******************************************************************************/
/*!
	\file   TransformComponent.h
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "Matrix3x3.h"
#include "Component.h"
//#include "RigidbodyComponent.h"  //WARNING: If you comment this back in, you will get over 1000 errors without a forward declaration

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct TransformComponent TransformComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* TransformComponent_Create();

Component* TransformComponent_Copy(TransformComponent *transform);

//void TransformComponent_SetParent(TransformComponent *transform, TransformComponent *parent);

//TransformComponent* TransformComponent_GetParent(TransformComponent *transform);

//RigidbodyComponent* TransformComponent_GetRigidbody(TransformComponent *transform);

Vector2 TransformComponent_GetPosition(const TransformComponent *transform);

float TransformComponent_GetRotation(const TransformComponent *transform);

Vector2 TransformComponent_GetScale(const TransformComponent *transform);

void TransformComponent_SetPosition(TransformComponent *transform, Vector2 position);

void TransformComponent_SetPositionFloats(TransformComponent *transform, float xPos, float yPos);

void TransformComponent_SetRotation(TransformComponent *transform, float rotation);

void TransformComponent_SetScale(TransformComponent *transform, Vector2 scale);

void TransformComponent_SetScaleFloats(TransformComponent *transform, float xScale, float yScale);

void TransformComponent_AddPosition(TransformComponent *transform, Vector2 position);

void TransformComponent_AddPositionFloats(TransformComponent *transform, float xPos, float yPos);

void TransformComponent_AddRotation(TransformComponent *transform, float rotation);

void TransformComponent_AddScale(TransformComponent *transform, Vector2 scale);

void TransformComponent_AddScaleFloats(TransformComponent *transform, float xScale, float yScale);

void TransformComponent_LookAt(TransformComponent *transform, Vector2 worldPos);

Vector2 TransformComponent_GetForward(const TransformComponent *transform);

Matrix3x3* TransformComponent_GetTransformMatrix(TransformComponent *transform);

Matrix3x3* TransformComponent_GetCameraTransformMatrix(TransformComponent *transform);
