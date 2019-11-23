/******************************************************************************/
/*!
	\file   LevelMarker.c
	\author Henry Brobeck
	\date   4/13/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "LevelMarker.h"
#include <stdlib.h>
#include "Graphics.h"
#include "Camera.h"


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
static LevelMarker *head = NULL;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
void LevelMarker_Init(void)
{
  
  
}

/*!
 \brief
 */
void LevelMarker_Exit(void)
{
  LevelMarker_Clear();
}

void LevelMarker_Render()
{
  RECT window = Graphics_GetWindowResolution();
  float width = (float)(window.right - window.left);
  float height = (float)(window.bottom - window.top);
  
  float cameraScale = Camera_GetSize();

  Vector2 cameraPos = Camera_GetPosition();

  float visibleWidth = width * cameraScale;
  float visibleHeight = height * cameraScale;  
  float circleRad = min(visibleWidth, visibleHeight) / 2.2f;
  LevelMarker *iter = head;

  while(iter)
  {
    Vector2 pos = TransformComponent_GetPosition(iter->transform);
    //Vector2 scale = TransformComponent_GetScale(iter->transform);
    
//    float maxScale = max(scale.x, scale.y);

    Vector2 cameraToPos;
    cameraToPos.x = pos.x - cameraPos.x;
    cameraToPos.y = pos.y - cameraPos.y;
    cameraToPos = Vector2_Normalize(cameraToPos);
    
    Vector2 topLeft;
    topLeft.x = cameraPos.x - visibleWidth / 2.0f;
    topLeft.y = cameraPos.y + visibleHeight / 2.0f;

    Vector2 bottomRight;
    bottomRight.x = cameraPos.x + visibleWidth / 2.0f;
    bottomRight.y = cameraPos.y - visibleHeight / 2.0f;

    //pos.x = pos.x - cameraToPos.x * maxScale;
    //pos.x = pos.y - cameraToPos.y * maxScale;

    if(topLeft.x < pos.x && bottomRight.x > pos.x && topLeft.y > pos.y && bottomRight.y < pos.y)
    {
      iter = iter->next;
      continue;
    }

    Vector2 renderPos;
    renderPos.x = cameraPos.x + cameraToPos.x * circleRad;
    renderPos.y = cameraPos.y + cameraToPos.y * circleRad;

    Vector2 boxScale;
    boxScale.x = 20.0f * cameraScale;
    boxScale.y = 5.0f * cameraScale;


    Graphics_DrawRect(renderPos, boxScale, Vector2_ToAngle(cameraToPos), iter->r, iter->g, iter->b, iter->a);
    iter = iter->next;
  }
   
}

LevelMarker* LevelMarker_Create(TransformComponent* transform, float r, float g, float b, float a)
{
  LevelMarker *marker = calloc(1, sizeof(LevelMarker));
  if (!marker)
  {
    return NULL;
  }
  marker->next = head;
  head = marker;
  marker->transform = transform;

  marker->r = r;
  marker->g = g;
  marker->b = b;
  marker->a = a;

  return marker;
} 
void LevelMarker_Remove(LevelMarker* marker)
{

  if(marker == head)
  {
    head = head->next;
    free(marker);
    return;
  }

  LevelMarker *iterator = head->next;
  LevelMarker *last = head;
  while(iterator)
  {
    LevelMarker *next = iterator->next;
    if (iterator == marker)
    {
      last->next = next;
      free(iterator);
      return;
    }
    iterator = next;
    last = last->next;
  }

}

void LevelMarker_Clear()
{

  LevelMarker *iterator = head;
  while (iterator)
  {

    LevelMarker *next = iterator->next;
    free(iterator);
    iterator = next;
  }
  head = NULL;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
