/******************************************************************************/
/*!
\file   ButtonComponent.c
\author Ryan Booth
\date   2/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "ButtonComponent.h"
#include "Sound.h"
#include <stdlib.h>

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct ButtonData ButtonData;
struct ButtonData
{
    bool isSelected;
};
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void Update(Component* self, float dt);
static void Destroy(Component* self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* ButtonComponent_Create()
{
    Component* newComponent = Component_Create();

    newComponent->update = Update;
    newComponent->destroy = Destroy;
    

    ButtonData *data = calloc(1, sizeof(ButtonData));
    data->isSelected = false;

    newComponent->data = data;

    return newComponent;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component* self, float dt)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(dt);
}

static void Destroy(Component* self)
{
    free(self->data);
    free(self);
}
