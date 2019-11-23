/******************************************************************************/
/*!
	\file   PlayerMovementComponent.c
	\author Evan Kau
	\date   3/13/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PlayerMovementComponent.h"
#include <stdlib.h>

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct PlayerMovementData
{
  float speed;
  float turnSpeed;
  float tightness;
  float targetRotation;
  Player *player;
} PlayerMovementData;

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

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* PlayerMovementComponent_Create(float speed, float turnSpeed, float tightness, Player *player)
{
  Component *component = Component_Create();
  PlayerMovementData *data = (PlayerMovementData*)calloc(1, sizeof(PlayerMovementData));

  data->speed = speed;
  data->turnSpeed = turnSpeed;
  data->tightness = tightness;
  data->player = player;

  component->data = data;
  component->update = &Update;

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  PlayerMovementData *data = (PlayerMovementData*)self->data;
  InputEvent *leftStick = InputGatherComponent_GetInput(data->player->inputGatherComponent, INPUT_TYPE_STICK_L);
  
  if(leftStick)
  {
    Vector2 stickAngle = Vector2_FromAngle(leftStick->angle);
    Vector2 force = Vector2_FromAngle(TransformComponent_GetRotation(data->player->transformComponent));

    force = Vector2_Scale(force, Vector2_Dot(stickAngle, force) * data->speed * leftStick->magnitude);

    RigidbodyComponent_AddForce(data->player->rigidbodyComponent, force, FORCE_TYPE_FORCE);

    if (leftStick->magnitude > 0.05f)
      data->targetRotation = leftStick->angle;

    
  }
  
  
  RigidbodyComponent_PIDLoopToAngle(data->player->rigidbodyComponent, data->targetRotation, data->turnSpeed, data->tightness);
  
}
