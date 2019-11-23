/******************************************************************************/
/*!
	\file   Component.h
	\author HeneryBrobeck
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once

#include "Event.h"
#include <stdbool.h>

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum COMPONENT_TYPE
{
  COMPONENT_TYPE_DEFAULT,
  COMPONENT_TYPE_PRIMITIVE,
  COMPONENT_TYPE_RIGIDBODY,
  COMPONENT_TYPE_TRANSFORM,
  COMPONENT_TYPE_COLLIDER,
  COMPONENT_TYPE_ENEMYTEST,
  COMPONENT_TYPE_SPRITE,
  COMPONENT_TYPE_SIMPLEAI,
  COMPONENT_TYPE_RENDER,
  COMPONENT_TYPE_AISWARMAGENT,
  COMPONENT_TYPE_RAYCASTPROJECTILE,
  COMPONENT_TYPE_BOMBPROJECTILE,
  COMPONENT_TYPE_AUTOLASER,
  COMPONENT_TYPE_BARCOMPONENT,
  COMPONENT_TYPE_JUMPGATE,
  COMPONENT_TYPE_BOMBABILITY,
  COMPONENT_TYPE_AIFORWARDSHOOT,
  COMPONENT_TYPE_AIMARKER,
  COMPONENT_TYPE_MISSILEBARRAGE,
  COMPONENT_TYPE_PLAYERHEALTH,
  COMPONENT_TYPE_RENDERSCORE,
  COMPONENT_TYPE_SUICIDEENEMY,
  COMPONENT_TYPE_AITURRET,
  COMPONENT_TYPE_CURTAINMISSILE
}COMPONENT_TYPE;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Entity Entity;

typedef struct Component Component;
struct Component
{
  //!Called on every update of parent entity
  void(*update)(Component *self, float dt);
  
  //!Called when Component is destroyed
  void(*destroy)(Component *self);

  //!Called when Component is attached to an Entity
  void(*onAttach)(Component *self);

  //!Called when Component is destroyed
  void(*onOtherAttach)(Component *self, Component *component);

  //!Called when a copy of the Component is requested
  Component*(*copy)(Component *self);

  void(*eventHandle)(Component *self, EVENT_TYPE eventType, void *eventData, bool isLocal);

  //!The Component's parent Entity
  Entity *parent;

  //!The next Component in parent's linked list
  Component *next;

  //!The previous Component in the parent's linked list
  Component *prev;

  //!Generic data struct pointer to allow for polymorphism
  void *data;

  //!Component Type for finding inter-component communication
  COMPONENT_TYPE type;


};


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component *Component_Create(void);

void Component_Destroy(Component *component);

/*----------------------------------------------------------------------------*/

