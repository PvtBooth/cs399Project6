/******************************************************************************/
/*!
	\file   CollisionSubject.h
	\author Henry Brobeck
	\date   1/30/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Entity.h"
#include "CollisionSubject.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct CollisionData
{
  Entity *first;
  Entity *second;

  Vector2 position;
  Vector2 normal;
  float penetration;
}CollisionData;



/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/



void CollisionSubject_Notify(Entity *first, Entity *second, Vector2 position, Vector2 normal, float penetration);

CollisionData *CollisionSubject_GetData(void *collisionData);

/*----------------------------------------------------------------------------*/


