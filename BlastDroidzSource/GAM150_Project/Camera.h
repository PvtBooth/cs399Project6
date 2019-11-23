/******************************************************************************/
/*!
	\file   Camera.h
	\author Evan Kau
	\date   1/28/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "Matrix3x3.h"
#include "Entity.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Camera Camera;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Camera_Init(void);

void Camera_Update(float dt);

void Camera_TrackEntity(Entity *entity);

void Camera_UntrackEntity(Entity *entity);

void Camera_UntrackAll();

void Camera_SetTracking(bool isTracking);

void Camera_SetPosition(float x, float y);

void Camera_AddPosition(float x, float y);

void Camera_SetTargetPosition(float x, float y);

void Camera_AddTargetPosition(float x, float y);

void Camera_SetSize(float size);

void Camera_AddSize(float size);

void Camera_SetTargetSize(float size);

void Camera_AddTargetSize(float size);

float Camera_GetSize(void);

float Camera_GetTargetSize(void);

Vector2 Camera_GetPosition(void);

Vector2 Camera_GetTargetPosition(void);

float Camera_GetRenderScaleFactor(void);

Matrix3x3* Camera_GetMatrix(void);

Vector2 Camera_MouseToWorld(void);

void Camera_DrawLine(float x1, float y1, float r1, float g1, float b1, float a1, float x2, float y2, float r2, float g2, float b2, float a2, bool worldSpace);

void Camera_DrawLineBox(Vector2 center, float halfWidth, float halfHeight, float r, float g, float b, float a);

void Camera_Exit();