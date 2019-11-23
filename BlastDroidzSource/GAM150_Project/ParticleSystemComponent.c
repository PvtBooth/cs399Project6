/******************************************************************************/
/*!
	\file   ParticleSystemComponent.c
	\author Henry Brobeck
	\date   2/15/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ParticleSystemComponent.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/



static void Update(Component* self, float dt);
static void Destroy(Component * self);
static void OnAttach(Component * self);
static void OnOtherAttach(Component * self, Component * component);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* ParticleSystemComponent_Create(void)
{
  Component* newComponent = Component_Create();
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static void Update(Component* self, float dt)
{
}

static void Destroy(Component* self)
{
}

static void OnAttach(Component* self)
{
}

static void OnOtherAttach(Component* self, Component* component)
{
}
