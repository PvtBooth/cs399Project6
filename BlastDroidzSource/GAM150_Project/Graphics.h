/******************************************************************************/
/*!
	\file   Graphics.h
	\author Henry Brobeck
	\date   1/18/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <AEEngine.h>
#include "Vector2.h"
#include "TransformComponent.h"
#include "MeshGroup.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum WindowMode
{
    FULLSCREEN,
    WINDOWED
}WindowMode;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Graphics_Init(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show);

void Graphics_Update(float dt);

void Graphics_SetWindowMode(WindowMode select);

void Graphics_ResetWindow();

void Graphics_SetWindowPosition(WindowMode select);

WindowMode Graphics_GetWindowMode();

void Graphics_Exit(void);

RECT Graphics_GetDesktopResolution();

void Graphics_SetWindowResolution();

RECT Graphics_GetWindowResolution();

void Graphics_DrawRectCamera(Vector2 pos, Vector2 size, float roation, float r, float g, float b, float a);

void Graphics_DrawRect(Vector2 pos, Vector2 size, float rotation, float r, float g, float b, float a);

AEGfxVertexList *Graphics_GetUnitMesh();

HINSTANCE getHInstance();

/*----------------------------------------------------------------------------*/
