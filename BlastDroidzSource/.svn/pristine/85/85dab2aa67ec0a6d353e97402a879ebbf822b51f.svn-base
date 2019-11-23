/******************************************************************************/
/*!
	\file   Entity.c
	\author Henry Brobeck
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Entity.h"
#include "StackState.h"
#include <stdlib.h>
#include "EntityManager.h"


/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static Entity *head;
static unsigned int nextID;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void UpdateEntity(Entity *entity, float dt);
static void PassEventToComponents(Entity *entity, int type, void *data, bool isLocal);
static void Entity_Destroy(Entity *entity);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief Start the Entity/Component module
 */
void Entity_Init(void)
{
  head = NULL;
  nextID = 0;
}

/*!
 \brief Update each active entity
 \param	dt Change in time (in seconds) since last game loop.
 */
void Entity_Update(float dt)
{

  Entity *iterator = head;
  while (iterator)
  {
    Entity *next = iterator->next;
    UpdateEntity(iterator, dt);
    iterator = next;
  }

  //TODO make this better
  //check for deletions
  iterator = head;
  while (iterator)
  {
    Entity *next = iterator->next;
    if(iterator->flags[EFLAG_DESTROY])
    {
      Entity_Destroy(iterator);
    }
    iterator = next;
  }

}

/*!
 \brief Shut down the Entity/Component module
 */
void Entity_Exit(void)
{
  Entity_DestroyAllImmediate();
}

/*!
 * \brief Create a new Entity
 * \param name Name of an enemy
 */
Entity* Entity_Create(char* name)
{
  Entity *newEntity = Entity_CreateOrphan(name);

  //add to Head of linked list
  newEntity->next = head;
  if(head)
  {
    head->prev = newEntity;
  }
  head = newEntity;

  return newEntity;
}

Entity *Entity_CreateOrphan(char *name)
{
  //Create new entity
  Entity *newEntity = calloc(1, sizeof(Entity));

  //initialize default values
  newEntity->headComponent = NULL;

  newEntity->name = name;

  newEntity->id = nextID++;

  return newEntity;
}

EntityManager* Entity_GetEntityManager(Entity* entity)
{
  if(entity)
  {
    return entity->manager;
  }
  return NULL;
}

GraphicsManager* Entity_GetGraphicsManager(Entity* entity)
{
  if(!entity || !entity->manager)
  {
    return NULL;
  }
  StackState *state = EntityManager_GetParentState(entity->manager);
  return state->graphicsManager;

}


/*!
* \brief Attach a component to an entity
* \param entity The entity to add to it
* \param component The component to add
*/
void Entity_AttachComponent(Entity* entity, Component* component)
{
  //Give all components of entity onOtherAttach callback
  Component *iterator = entity->headComponent;
  while (iterator)
  {
    iterator->onOtherAttach(iterator, component);
    iterator = iterator->next;
  }

  //add component to list by attaching at the head
  component->next = entity->headComponent;

  if (entity->headComponent)
  {
    entity->headComponent->prev = component;

  }
  entity->headComponent = component;
  component->parent = entity;

  //give new component attach callback
  component->onAttach(component);

  //check if a component is a garunteed type
  switch(component->type)
  {
  case COMPONENT_TYPE_TRANSFORM:
    entity->transformComponent = (TransformComponent*)component->data;
    break;
  case COMPONENT_TYPE_RENDER:
    entity->renderComponent = component;
  default:
    break;
  }
  
 
}
/*!
* \brief Detatch a component to an entity
* \param component The component to detatch
* \param destroy 1 to destroy the component, 0 to simply remove from the entity
*/
void Entity_DetatchComponent(Component* component, int destroy)
{
  
  Entity *entity = component->parent;
  //sanity check
  if(!entity)
  {
    return;
  }

  if (component == entity->headComponent)
  {
    //entity is head, check if only entity in list
    if (component->next)
    {
      //not only entity, delink the next entity
      component->next->prev = NULL;
    }
    //head will either be NULL (empty list) or the next entity in the list
    entity->headComponent = component->next;
  }
  else
  {
    //entity is not the Head, that means there must be something before it
    if (component->next)
    {
      //theres something after it, make the entity skip over
      component->next->prev = component->prev;
    }

    //since theres always something before it, we can simply link these two together. component->next might be NULL, doesn't matter
    component->prev->next = component->next;
  }

  if (destroy)
  {
    Component_Destroy(component);
  }
}

Entity* Entity_Copy(Entity *entity)
{
  Entity *newEntity = Entity_CreateOrphan(entity->name);

  //Copy the components
  Component *iterator = entity->headComponent;

  //For each component,
  while (iterator)
  {
    //Attach a copy onto the entity
    Entity_AttachComponent(newEntity, iterator->copy(iterator));
    iterator = iterator->next;
  }

  return newEntity;
}

/**
 * \brief Find a component of a specific type in an entity
 * \param entity Entity to search in
 * \param type Component type to search for
 * \return pointer to found component, NULL if none found
 */
Component* Entity_FindComponentByType(Entity* entity, COMPONENT_TYPE type)
{
  Component *iterator = entity->headComponent;
  while(iterator)
  {
    if(iterator->type == type)
    {
      return iterator;
    }
    iterator = iterator->next;
  }

  return NULL;
}

void Entity_SetFlag(Entity* entity, ENTITY_FLAG flag, int value)
{
  entity->flags[flag] = value;
}

bool Entity_GetFlag(Entity* entity, ENTITY_FLAG flag)
{
  if(!entity)
  {
    return false;
  }
  return entity->flags[flag];
}

/**
 * \brief Get the transform of an Entity
 * \param entity the entity to get the transform of
 * \return the transform
 */
TransformComponent* Entity_GetTransform(Entity* entity)
{
  return entity->transformComponent;
}

void Entity_GlobalEvent(int type, void* data)
{
  Entity *iterator = head;
  while (iterator)
  {
    Entity *next = iterator->next;
    PassEventToComponents(iterator, type, data, false);
    iterator = next;
  }
}

void Entity_LocalEvent(Entity* entity, int type, void* data)
{
  PassEventToComponents(entity, type, data, true);
}

unsigned int Entity_GetID(Entity* entity)
{
  return entity->id;
}

/**
 * \brief Destroys all entitys after next update loop. This is safe, but not suited for exiting
 */
void Entity_DestroyAllSafe()
{
  Entity *iterator = head;
  while(iterator)
  {
    Entity_SetFlag(iterator, EFLAG_DESTROY, 1);
    iterator = iterator->next;
  }
}

/**
 * \brief WARNING! UNSAFE! Destroys all enemies immediately, without waiting for update loop. If this is called during an update the game WILL CRASH
 */
void Entity_DestroyAllImmediate()
{
  Entity *iterator = head;
  while(iterator)
  {
    Entity *next = iterator->next;
    Entity_Destroy(iterator);
    iterator = next;
  }
  head = NULL;
}

void Entity_DestroyOrphan(Entity* entity)
{
  if(!entity)
  {
    return;
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


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
/*!
* \brief Update an entity
* \param entity The entity to update
* \param dt     Elapsed time since last update in seconds
*/
static void UpdateEntity(Entity *entity, float dt)
{
  //update each attached component
  Component *iterator = entity->headComponent;
  while(iterator)
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
static void PassEventToComponents(Entity *entity, int type,void *data, bool isLocal)
{
  //update each attached component
  if(!entity)
  {
    return;
  }
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
static void Entity_Destroy(Entity* entity)
{
  if (entity == head)
  {
    //entity is head, check if only entity in list
    if (entity->next)
    {
      //not only entity, delink the next entity
      entity->next->prev = NULL;
    }
    //Head will either be NULL (empty list) or the next entity in the list
    head = entity->next;
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