/******************************************************************************/
/*!
	\file   ParalaxBackgroundComponent.c
	\author Henry Brobeck
	\date   2/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ParalaxBackgroundComponent.h"
#include "Camera.h"
#include <stdlib.h>


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct ParalaxData ParalaxData;
struct ParalaxData
{
  Sprite *sprite;
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

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt);
static void Destroy(Component * self);


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief A paralaxbackground attaches itself to the camera and creates a back
 * \param sprite 
 * \return 
 */
Component* ParalaxBackgroundComponent_Create(Sprite *sprite)
{
  Component* newComponent = Component_Create();
  newComponent->update = Update;
  newComponent->destroy = Destroy;
  ParalaxData *data = calloc(1, sizeof(ParalaxData));
  data->sprite = sprite;
  newComponent->data = data;

  return newComponent;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
    UNREFERENCED_PARAMETER(self);

}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}