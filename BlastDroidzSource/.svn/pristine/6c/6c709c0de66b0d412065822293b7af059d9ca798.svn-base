/******************************************************************************/
/*!
	\file   EntityManager.h
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "StackState.h"
#include "Event.h"
#include "Entity.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct EntityManager EntityManager;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
EntityManager *EntityManager_Create(StackState *parent);

Entity *EntityManager_CreateEntity(EntityManager *entityManager, char *name);

void EntityManager_Update(EntityManager *entityManager, float dt);

void EntityManager_AddEntity(EntityManager *entityManager, Entity *entity);

void EntityManager_RemoveEntity(EntityManager *entityManager, Entity *entity);

void EntityManager_GlobalEvent(EntityManager *entityManager, EVENT_TYPE type, void* data);

void EntityManager_DestroyAllSafe(EntityManager *entityManager);

void EntityManager_DestroyAllImmediate(EntityManager *entityManager);

StackState *EntityManager_GetParentState(EntityManager* entityManager);

void EntityManager_Destroy(EntityManager* entityManager);



/*----------------------------------------------------------------------------*/


