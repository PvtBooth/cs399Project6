/******************************************************************************/
/*!
	\file   Vector2.h
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <stdbool.h>

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Vector2
{
  float x;
  float y;
} Vector2;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Vector2 Vector2_Zero();

Vector2 Vector2_Add(Vector2 first, Vector2 second);

Vector2 Vector2_AddFloats(Vector2 vector, float x, float y);

Vector2 Vector2_Sub(Vector2 first, Vector2 second);

Vector2 Vector2_SubFloats(Vector2 vector, float x, float y);

Vector2 Vector2_Normalize(Vector2 vector);

float Vector2_Length(Vector2 vector);

float Vector2_LengthSquared(Vector2 vector);

Vector2 Vector2_Scale(Vector2 vector, float scale);

Vector2 Vector2_ComponentProduct(Vector2 first, Vector2 second);

float Vector2_Dot(Vector2 first, Vector2 second);

float Vector2_DotFloats(Vector2 vector, float x, float y);

Vector2 Vector2_CrossFloatVector(float z, Vector2 vector);

float Vector2_CrossVectorVector(Vector2 first, Vector2 second);

Vector2 Vector2_GetOrthogonal(Vector2 vector);

Vector2 Vector2_Average(Vector2 first, Vector2 second);

float Vector2_Distance(Vector2 first, Vector2 second);

float Vector2_DistanceSquared(Vector2 first, Vector2 second);

Vector2 Vector2_Lerp(Vector2 first, Vector2 second, float t, bool clamp);

float Vector2_GetAngle(Vector2 first, Vector2 second);

float Vector2_GetSignedAngle(Vector2 first, Vector2 second);

Vector2 Vector2_FromAngle(float angle);

float Vector2_ToAngle(Vector2 vector);