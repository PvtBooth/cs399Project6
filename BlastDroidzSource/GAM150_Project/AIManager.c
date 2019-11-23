/******************************************************************************/
/*!
	\file   AIManager.c
	\author Evan Kau
	\date   2/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AIManager.h"
#include "AISwarm.h"
#include <stddef.h>
#include <stdlib.h>
#include "Camera.h"
#include "Graphics.h"


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

static AISwarm *swarmList = NULL;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void AIManager_Init(void)
{
  if (swarmList)
  {
    AIManager_Clean();
  }
  swarmList = NULL;
  AISwarm_Init();
}

void AIManager_Update(float dt)
{
  AISwarm *iterator = swarmList;
  while (iterator)
  {
    AISwarm_Update(iterator, dt);
    iterator = AISwarm_GetNext(iterator);
  }
}

void AIManager_AddSwarm(AISwarm *swarm)
{
  AISwarm_SetNext(swarm, swarmList);
  if (swarmList)
  {
    AISwarm_SetPrev(swarmList, swarm);
  }
  swarmList = swarm;
}

void AIManager_RemoveSwarm(AISwarm *swarm)
{
  if (AISwarm_GetPrev(swarm))
  {
    AISwarm_SetNext(AISwarm_GetPrev(swarm), AISwarm_GetNext(swarm));
  }
  else
  {
    swarmList = AISwarm_GetNext(swarm);
  }
  if (AISwarm_GetNext(swarm))
  {
    AISwarm_SetPrev(AISwarm_GetNext(swarm), AISwarm_GetPrev(swarm));
  }
}

AISwarm* AIManager_GetNearestSwarm(Vector2 position)
{
  if (!swarmList)
    return NULL;
  AISwarm *iterator = swarmList;
  float minDist = Vector2_Distance(position, AISwarm_GetCenter(iterator)) - min(AISwarm_GetRadius(iterator), 1000);
  AISwarm *closest = iterator;
  iterator = AISwarm_GetNext(iterator);
  while (iterator)
  {
    float dist = Vector2_Distance(position, AISwarm_GetCenter(iterator)) - min(AISwarm_GetRadius(iterator), 1000);

    if (dist < minDist)
    {
      minDist = dist;
      closest = iterator;
    }

    iterator = AISwarm_GetNext(iterator);
  }
  return closest;
}

/*!
 \brief
 */
void AIManager_Exit(void)
{
  AIManager_Clean();
  AISwarm_Exit();
}

void AIManager_Clean(void)
{
  AISwarm *iterator = swarmList;
  while (iterator)
  {
    AISwarm *temp = iterator;
    iterator = AISwarm_GetNext(iterator);
    AISwarm_Free(temp);
  }
}

void AIManager_Debug(void)
{
  AISwarm *iterator = swarmList;
  while (iterator)
  {
    AISwarm_Debug(iterator);
    iterator = AISwarm_GetNext(iterator);
  }
}

void AIManager_Render()
{
  //float cameraRatio = Camera_GetSize();
  Vector2 cameraPos = Camera_GetPosition();
  RECT window = Graphics_GetWindowResolution();
  float windowX = (float)(window.right - window.left);
  float windowY = (float)(window.bottom - window.top);
  Vector2 topLeft;
  topLeft.x = cameraPos.x - windowX / 2;
  topLeft.y = cameraPos.y + windowY / 2;
  Vector2 bottomRight;
  bottomRight.x = cameraPos.x + windowX / 2;
  bottomRight.y = cameraPos.y - windowY / 2;

  AISwarm *iterator = swarmList;
  while(iterator)
  {
    Vector2 swarmPos = AISwarm_GetCenter(iterator);
    
    if(topLeft.x > swarmPos.x && bottomRight.x < swarmPos.x )
    {
      if (topLeft.y > swarmPos.y && bottomRight.y < swarmPos.y)
      {
        //Graphics_DrawRect()
      }
    }
    
    iterator = AISwarm_GetNext(iterator);
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
