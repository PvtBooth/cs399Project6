/******************************************************************************/
/*!
	\file   GraphicsManager.c
	\author Henry Brobeck
	\date   3/11/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "GraphicsManager.h"
#include <stdlib.h>
#include <AEEngine.h>


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct RenderObserver RenderObserver;

struct RenderObserver
{
  void(*render)(Component *self);
  Component *component;
  RenderObserver *next;
};
struct GraphicsManager
{
  RenderObserver *renderHeads[RENDER_LAYER_STANDARD_MAX];
  RenderObserver *renderHeadTL;
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
static void AddRenderObserverToLayer(RenderObserver **head, RenderObserver *observer);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
GraphicsManager* GraphicsManager_Create(StackState *parent)
{
  GraphicsManager *graphicsManager = calloc(1, sizeof(GraphicsManager));
  if(!graphicsManager)
  {
    return NULL;
  }
  graphicsManager->parentState = parent;
  
  return graphicsManager;
}

void GraphicsManager_Render(GraphicsManager* graphicsManager)
{
  if(!graphicsManager)
  {
    return;
  }
  
  for (int i = 0; i < RENDER_LAYER_STANDARD_MAX; i++)
  {
    RenderObserver *iterator = graphicsManager->renderHeads[i];
    while (iterator)
    {
      iterator->render(iterator->component);
      iterator = iterator->next;
    }
  }
  AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  RenderObserver *iterator = graphicsManager->renderHeadTL;
  while (iterator)
  {
    iterator->render(iterator->component);
    iterator = iterator->next;
  }
  AEGfxSetWorldOriginMode(AE_WOM_NORMAL);
  

  

}

void GraphicsManager_Destroy(GraphicsManager* graphicsManager)
{
  if(!graphicsManager)
  {
    return;
  }
  for(int i = 0; i < RENDER_LAYER_STANDARD_MAX; i++)
  {
    RenderObserver *node = graphicsManager->renderHeads[i];
    while(node)
    {
      RenderObserver *next = node->next;
      free(node);
      node = next;
    }
  }
  free(graphicsManager);
}

void GraphicsManager_RegisterComponentRender(GraphicsManager* graphicsManager, void(* render)(Component* self), Component* component, RENDER_LAYER layer)
{
  if (layer < 0)
  {
    layer = 0;
  }
  RenderObserver *renderObserver = calloc(1, sizeof(RenderObserver));
  renderObserver->component = component;
  renderObserver->render = render;
  if(layer < RENDER_LAYER_STANDARD_MAX)
  {
    AddRenderObserverToLayer(&graphicsManager->renderHeads[layer], renderObserver);
  }
  else if(layer == RENDER_LAYER_TOP_LEFT)
  {
    AddRenderObserverToLayer(&graphicsManager->renderHeadTL, renderObserver);
  }
}

void GraphicsManager_RemoveComponentRender(GraphicsManager* graphicsManager, Component* self, int layer)
{
  if(!graphicsManager || !self)
  {
    return;
  }

  RenderObserver **head = NULL;
  if(layer < RENDER_LAYER_STANDARD_MAX)
  {
    head = &graphicsManager->renderHeads[layer];
  }
  else if(layer == RENDER_LAYER_TOP_LEFT)
  {
    head = &graphicsManager->renderHeadTL;
  }
  if(!head)
  {
    return;
  }

  if((*head)->component == self)
  {
    RenderObserver *destroy = *head;
    *head = destroy->next;
    free(destroy);
    return;
  }

  RenderObserver *iterator = (*head)->next;
  RenderObserver *last = *head;
  while (iterator)
  {

    if (iterator->component == self)
    {

      last->next = iterator->next;

      free(iterator);
      return;
    }
    last = iterator;
    iterator = iterator->next;
  }
  last = last;
}

static void AddRenderObserverToLayer(RenderObserver **head, RenderObserver *observer)
{
  if(!head || !observer)
  {
    return;
  }
  observer->next = *head;
  *head = observer;
}
