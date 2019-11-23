/******************************************************************************/
/*!
\file   HUDBarComponent.c
\author Cole Astaire, Henry Brobeck
\date   2/13/2017
\par    Course: GAM150
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "HUDBarComponent.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct HUDBarComponent
{
  Vector2 offset;
  float padding;
  HUDBar *head;

} HUDBarComponent;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt);
static void Destroy(Component *self);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static void EventHandle(Component *self, int type, void *eventData, bool isLocal);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
* \brief Creates a new HUD Bar component, with necessary callbacks
*/
Component *HUDBarComponent_Create(Vector2 offset, float width, float height, float padding)
{
  //create new blank commponent
  Component *newComponent = Component_Create();

  //set component callbacks
  newComponent->update = Update;
  newComponent->destroy = Destroy;
  newComponent->onAttach = OnAttach;
  newComponent->onOtherAttach = OnOtherAttach;

  //allocate new HUDBar and set it to 0
  HUDBarComponent *HUDBar = calloc(1, sizeof(HUDBarComponent));

  //safety check
  if(HUDBar == NULL)
  {
    return NULL;
  }

  //set new hud bar to componenet's data
  newComponent->data = HUDBar;

  //assign appropriate values
  HUDBar->offset = offset;
  HUDBar->padding = padding;
  HUDBar->head = NULL;
  
  //return
  return NULL;
}

/**
* \brief Gets the current width of the HUD Bar
*/
float HUDBarComponent_GetWidth(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return 0;

  //get width
  float width = hudBar->width;

  return width;
}

/**
* \brief Gets the current height of the HUD Bar
*/
float HUDBarComponent_GetHeight(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return 0;

  //get height
  float height = hudBar->width;

  return height;
}

/**
* \brief Gets the current minimum of the HUD Bar
*/
float HUDBarComponent_GetMin(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return 0;

  //get min
  float min = hudBar->width;

  return min;
}

/**
* \brief Gets the current maximum of the HUD Bar
*/
float HUDBarComponent_GetMax(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return 0;

  //get max
  float max = hudBar->width;

  return max;
}

/**
* \brief Gets the current value of the HUD Bar
*/
float HUDBarComponent_GetValue(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return 0;

  //get value
  float value = hudBar->width;

  return value;
}

/**
* \brief Sets the width of the HUD Bar
*/
void HUDBarComponent_SetWidth(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return;
}

/**
* \brief Sets the width of the HUD Bar
*/
void HUDBarComponent_SetHeight(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return;
}

/**
* \brief Sets the minimum of the HUD Bar
*/
void HUDBarComponent_SetMin(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return;
}

/**
* \brief Sets the maximum of the HUD Bar
*/
void HUDBarComponent_SetMax(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return;
}

/**
* \brief Sets the value of the HUD Bar
*/
void HUDBarComponent_SetValue(HUDBar* hudBar)
{
  //safety check
  if (hudBar == NULL)
    return;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{

}
static void Destroy(Component *self)
{

}
static void OnAttach(Component *self)
{

}
static void OnOtherAttach(Component *self, Component *component)
{

}

static void EventHandle(Component *self, int type, void *eventData, bool isLocal)
{

}