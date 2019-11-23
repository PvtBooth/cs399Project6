/******************************************************************************/
/*!
	\file   Physics_Resolver.h
	\author Evan Kau
	\date   1/19/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "ColliderComponent.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef struct Contact Contact;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Physics_Resolver_Init();

void Physics_Resolver_AddContact(ColliderComponent *first, ColliderComponent *second, Vector2 position, Vector2 normal, float penetration);

void Physics_Resolver_Update();

void Physics_Resolver_Debug();

void Physics_Resolver_Exit();
