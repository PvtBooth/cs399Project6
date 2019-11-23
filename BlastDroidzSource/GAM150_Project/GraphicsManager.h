/******************************************************************************/
/*!
	\file   GraphicsManager.h
	\author Henry Brobeck
	\date   3/11/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once

#include "Component.h"
#include "RenderLayer.h"
#include "StackState.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct GraphicsManager GraphicsManager;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

GraphicsManager *GraphicsManager_Create(StackState *parent);

void GraphicsManager_Render(GraphicsManager* graphicsManager);

void GraphicsManager_Destroy(GraphicsManager* graphicsManager);

void GraphicsManager_RegisterComponentRender(GraphicsManager * graphicsManager, void(*render)(Component *self), Component *component, RENDER_LAYER layer);

void GraphicsManager_RemoveComponentRender(GraphicsManager * graphicsManager, Component *self, int layer);

/*----------------------------------------------------------------------------*/


