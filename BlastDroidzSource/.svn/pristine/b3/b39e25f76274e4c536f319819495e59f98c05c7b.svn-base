/******************************************************************************/
/*!
	\file   LifetimeComponent.c
	\author Henry Brobeck
	\date   1/25/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "LifetimeComponent.h"
#include "Entity.h"
#include <stdlib.h>


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct LifetimeData
{
  float currentTime;
  float maxTime;
}LifetimeData;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt);
static void Destroy(Component * self);


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/



/**
 * \brief A lifetime component destroys an entity after a certain amount of time passes
 * \param lifetime The total life of the entity (in seconds)
 * \return A new component
 */
Component* LifetimeComponent_Create(float lifetime)
{
  Component* newComponent = Component_Create();
  newComponent->update = Update;

  LifetimeData *data = calloc(1, sizeof(LifetimeData));
  newComponent->data = data;

  data->currentTime = 0;
  data->maxTime = lifetime;
  
  return newComponent;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt)
{
  LifetimeData *data = (LifetimeData*)self->data;
  data->currentTime += dt;
  if (data->currentTime > data->maxTime)
  {
    //Flag for deletion
    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
  }
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}
