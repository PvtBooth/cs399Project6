/******************************************************************************/
/*!
	\file   EntityManager.c
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "EntityManager.h"
#include <stdlib.h>
#include "StackState.h"
#include <AEInput.h>
#include "EntitySubject.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
struct EntityManager
{
  Entity *head;
  unsigned int nextID;
  StackState *parentState;
};
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void UpdateEntity(Entity *entity, float dt);
static void PassEventToComponents(Entity *entity, int type, void *data, bool isLocal);
static void Entity_Destroy(EntityManager *entityManager,Entity *entity);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

EntityManager* EntityManager_Create(StackState *parent)
{
  EntityManager *entityManager = calloc(1, sizeof(EntityManager));
  if(!entityManager)
  {
    return NULL;
  }
  entityManager->parentState = parent;

 

  return entityManager;
}

Entity* EntityManager_CreateEntity(EntityManager *entityManager, char* name)
{
  Entity *entity = Entity_CreateOrphan(name);
  EntityManager_AddEntity(entityManager, entity);

  return entity;
}

void EntityManager_Update(EntityManager* entityManager, float dt)
{
  
  Entity *iterator = entityManager->head;
  while (iterator)
  {
    Entity *next = iterator->next;
    if(!Entity_GetFlag(iterator,EFLAG_FREEZE))
    {
      UpdateEntity(iterator, dt);
    }
    
    iterator = next;
  }

  //TODO make this better
  //check for deletions
  iterator = entityManager->head;
  while (iterator)
  {
    Entity *next = iterator->next;
    if (iterator->flags[EFLAG_DESTROY])
    {
      EntitySubject_NotifyDestroy(iterator, entityManager);
      Entity_Destroy(entityManager, iterator);
    }
    iterator = next;
  }
}

void EntityManager_AddEntity(EntityManager* entityManager, Entity* entity)
{
  //add to Head of linked list
  entity->next = entityManager->head;
  if (entityManager->head)
  {
    entityManager->head->prev = entity;
  }
  entityManager->head = entity;

  entity->manager = entityManager;
}

void EntityManager_RemoveEntity(EntityManager* entityManager, Entity* entity)
{
  if (entity == entityManager->head)
  {
    //entity is head, check if only entity in list
    if (entity->next)
    {
      //not only entity, delink the next entity
      entity->next->prev = NULL;
    }
    //Head will either be NULL (empty list) or the next entity in the list
    entityManager->head = entity->next;
  }
  else
  {
    //entity is not the head, that means there must be something before it
    if (entity->next)
    {
      //theres something after it, make the entity skip over
      entity->next->prev = entity->prev;
    }

    //since theres always something before it, we can simply link these two together. entity->next might be NULL, doesn't matter
    entity->prev->next = entity->next;

  }
}

void EntityManager_GlobalEvent(EntityManager* entityManager, EVENT_TYPE type, void* data)
{
  Entity *iterator = entityManager->head;
  while (iterator)
  {
    Entity *next = iterator->next;
    PassEventToComponents(iterator, type, data, false);
    iterator = next;
  }
}

/**
* \brief Destroys all entitys after next update loop. This is safe, but not suited for exiting
*/
void EntityManager_DestroyAllSafe(EntityManager *entityManager)
{
  Entity *iterator = entityManager->head;
  while (iterator)
  {
    Entity_SetFlag(iterator, EFLAG_DESTROY, 1);
    iterator = iterator->next;
  }
}

/**
* \brief WARNING! UNSAFE! Destroys all enemies immediately, without waiting for update loop. If this is called during an update the game WILL CRASH
*/
void EntityManager_DestroyAllImmediate(EntityManager *entityManager)
{
  Entity *iterator = entityManager->head;
  while (iterator)
  {
    Entity *next = iterator->next;
    EntitySubject_NotifyDestroy(iterator, entityManager);
    Entity_Destroy(entityManager, iterator);
    iterator = next;
  }
  entityManager->head = NULL;
}

StackState* EntityManager_GetParentState(EntityManager* entityManager)
{
  if(!entityManager)
  {
    return NULL;
  }
  return entityManager->parentState;
}

void EntityManager_Destroy(EntityManager* entityManager)
{
  EntityManager_DestroyAllImmediate(entityManager);
  free(entityManager);

}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

/*!
* \brief Detatch a component to an entity
* \param entity The entity to detatch from
* \param dt     Elapsed time since last update in seconds
*/
static void UpdateEntity(Entity *entity, float dt)
{
  //update each attached component
  Component *iterator = entity->headComponent;
  while (iterator)
  {
    //in case the component wants to delete itself
    Component *next = iterator->next;
    iterator->update(iterator, dt);

    iterator = next;

  }
}
/**
* \brief For each component in an entity, pass an event
* \param entity Entity to pass event to
* \param type type of event
* \param data event specific data
*/
static void PassEventToComponents(Entity *entity, int type, void *data, bool isLocal)
{
  //update each attached component
  Component *iterator = entity->headComponent;
  while (iterator)
  {
    //in case the component wants to delete itself
    Component *next = iterator->next;
    iterator->eventHandle(iterator, type, data, isLocal);
    iterator = next;

  }
}

/*!
* \brief Destroy an Entity, this also destroys all attached components
* \param entity The Entity to be destroyed
*/
static void Entity_Destroy(EntityManager *entityManager, Entity* entity)
{
  if (entity == entityManager->head)
  {
    //entity is head, check if only entity in list
    if (entity->next)
    {
      //not only entity, delink the next entity
      entity->next->prev = NULL;
    }
    //Head will either be NULL (empty list) or the next entity in the list
    entityManager->head = entity->next;
  }
  else
  {
    //entity is not the head, that means there must be something before it
    if (entity->next)
    {
      //theres something after it, make the entity skip over
      entity->next->prev = entity->prev;
    }

    //since theres always something before it, we can simply link these two together. entity->next might be NULL, doesn't matter
    entity->prev->next = entity->next;

  }

  //Destroy all components
  Component *iterator = entity->headComponent;
  while (iterator)
  {
    Component *old = iterator;
    iterator = iterator->next;
    Component_Destroy(old);
  }

  //finish by freeing entity memory
  free(entity);

}