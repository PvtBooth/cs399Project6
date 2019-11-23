/******************************************************************************/
/*!
	\file   JumpgateComponent.c
	\author Evan Kau
	\date   3/11/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "JumpgateComponent.h"
#include "Component.h"
#include "Physics_Layers.h"
#include <stdlib.h>
#include "TransformComponent.h"
#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Assignment.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct JumpgateData
{
  float activationRadius;
  bool playersLeaving[4];
  bool playerFound;
  TransformComponent *transform;
  void(*NextLevel)();
} JumpgateData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static Component* Copy(Component *self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* JumpgateComponent_Create(float activationRadius, void(*NextLevel)())
{
  Component *component = Component_Create();
  JumpgateData *data = (JumpgateData*)calloc(1, sizeof(JumpgateData));

  data->activationRadius = activationRadius;
  data->NextLevel = NextLevel;

  component->data = data;
  component->type = COMPONENT_TYPE_JUMPGATE;
  component->update = &Update;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;

  return component;
}

Component* JumpgateComponent_Copy(JumpgateData *data)
{
  return JumpgateComponent_Create(data->activationRadius, data->NextLevel);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  JumpgateData *data = (JumpgateData*)self->data;
  TransformComponent *transform = (TransformComponent*)data->transform;
  int i;
  
  for (i = 0; i < 4; i++)
  {
    Player *player = Player_GetPlayer(i);
    if (player->playerEntity)
    {
      data->playerFound = true;
      Vector2 playerPos = Player_GetPosition(i);
      Vector2 jumpPos = TransformComponent_GetPosition(transform);
      float playerColliderRadius = ColliderComponent_GetBoundingCircleRadius(player->colliderComponent);
      float distSqrd = Vector2_DistanceSquared(playerPos, jumpPos);
      if (distSqrd > (playerColliderRadius + data->activationRadius) * (playerColliderRadius + data->activationRadius))
      {
        data->playersLeaving[i] = false;
      }
      else
      {
        InputEvent *leftBumper = InputGatherComponent_GetInput(player->inputGatherComponent, INPUT_TYPE_BUTTON_LB);

        bool trig = AEInputCheckTriggered('F');
        if (((leftBumper && leftBumper->inputEventType == INPUT_EVENT_TYPE_TRIGGERED) || (i == 0 && trig)) && Assignment_Complete())
        {
          data->playersLeaving[i] = !data->playersLeaving[i];
        }
        float r = 0;
        float g = 0;
        float b = 0;
        if(data->playersLeaving[i])
        {
          g = 1.0f;
        }
        else
        {
          r = 1.0f;
        }

        Vector2 midPoint = Vector2_Sub(playerPos, jumpPos);

        float angle = Vector2_ToAngle(midPoint);
        midPoint.x = jumpPos.x + midPoint.x / 2.0f;
        midPoint.y = jumpPos.y + midPoint.y / 2.0f;

        float length = Vector2_Distance(playerPos, jumpPos);
        
        Vector2 scale;
        scale.x = length;
        scale.y = 10;

        Graphics_DrawRect(midPoint,scale ,angle, r,g,b,1.0f);
      }
    }
  }

  bool exitLevel = true;
  int playerFound = 0;
  for(i = 0; i < 4; i++)
  {
    if(Player_GetPlayer(i)->playerEntity)
    {
      playerFound++;
      if (!data->playersLeaving[i])
      {
        exitLevel = false;
        break;
      }
    }
    
  }

  if ((exitLevel && data->playerFound && playerFound > 0) || AEInputCheckTriggered('U'))
  {
    data->NextLevel();
  }
}

static void OnAttach(Component *self)
{
  Component *transform = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  if (transform)
  {
    JumpgateData *data = (JumpgateData*)self->data;
    data->transform = (TransformComponent*)transform->data;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    JumpgateData *data = (JumpgateData*)self->data;
    data->transform = (TransformComponent*)component->data;
  }
}

static Component* Copy(Component *self)
{
  return JumpgateComponent_Copy((JumpgateData*)self->data);
}
