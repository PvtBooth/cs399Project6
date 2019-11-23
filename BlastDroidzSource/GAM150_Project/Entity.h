/******************************************************************************/
/*!
	\file   Entity.h
	\author HeneryBrobeck
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include "TransformComponent.h"

//FORWARD DELCARATION
typedef struct EntityManager EntityManager;
typedef struct GraphicsManager GraphicsManager;


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum ENTITY_FLAG
{
  EFLAG_DESTROY,
  EFLAG_FREEZE,
  EFLAG_MAX
} ENTITY_FLAG;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct Entity Entity;
struct Entity
{
  //!Head of component linked list
  Component *headComponent;

  TransformComponent *transformComponent;

  Component  *renderComponent;
  
  //!Name of the entity, in a null terminated character array
  char *name;

  //!Next entity in linked list
  Entity *next;
  //!Previous entity in linked list
  Entity *prev;

  int flags[EFLAG_MAX];

  unsigned int id;

  EntityManager *manager;

};
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Entity_Init(void);

void Entity_Update(float dt);

void Entity_Exit(void);

Entity *Entity_Create(char *name);



void Entity_AttachComponent(Entity *entity, Component *component);

void Entity_DetatchComponent(Component *component, int destroy);

Entity* Entity_Copy(Entity *entity);

Component *Entity_FindComponentByType(Entity *entity, COMPONENT_TYPE type);

void Entity_SetFlag(Entity *entity, ENTITY_FLAG flag, int value);

bool Entity_GetFlag(Entity *entity, ENTITY_FLAG flag);

TransformComponent *Entity_GetTransform(Entity *entity);

void Entity_GlobalEvent(int type, void *data);

void Entity_LocalEvent(Entity *entity, int type, void *data);

unsigned int Entity_GetID(Entity *entity);

void Entity_DestroyAllSafe();

void Entity_DestroyAllImmediate();

void Entity_DestroyOrphan(Entity *entity);

Entity *Entity_CreateOrphan(char *name);

EntityManager *Entity_GetEntityManager(Entity *entity);

GraphicsManager *Entity_GetGraphicsManager(Entity *entity);


/*----------------------------------------------------------------------------*/

