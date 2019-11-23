/******************************************************************************/
/*!
	\file   SimpleAIComponent.c
	\author Henry Brobeck
	\date   2/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "SimpleAIComponent.h"
#include <stdlib.h>
#include "RigidbodyComponent.h"
#include "Player.h"
#include "Camera.h"
#include "RandGen.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct SimpleAI
{
  Component *shootComponent;
  RigidbodyComponent *rigidbody;
  bool isAttacking;
  float delay;

}SimpleAI;

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

Component* SimpleAIComponent_Create(void)
{
  Component* newComponent = Component_Create();
  newComponent->update = Update;
  newComponent->destroy = Destroy;
  newComponent->onAttach = OnAttach;
  newComponent->onOtherAttach = OnOtherAttach;
  newComponent->type = COMPONENT_TYPE_SIMPLEAI;

  SimpleAI *data = calloc(1, sizeof(SimpleAI));
  newComponent->data = data;
  data->isAttacking = 0;
  data->delay = 2;


  return newComponent;

}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static void Update(Component* self, float dt)
{
  SimpleAI *data = self->data;
  data->delay -= dt;
  if(data->delay <= 0)
  {
    data->isAttacking = !data->isAttacking;
    data->delay = (float)RandGen_Int(1,3);
  }
  if(!data->rigidbody)
  {
    return;
  }

  Vector2 targetPos = { 0,0 };

  if(data->isAttacking)
  {
    TransformComponent *playerTrans = Player_GetPlayer(0)->transformComponent;
    targetPos = TransformComponent_GetPosition(playerTrans);
  }
  else
  {
    targetPos.x = (float)RandGen_Int(-6000, 6000);
    targetPos.y = (float)RandGen_Int(-6000, 6000);
  }

  Vector2 force = Vector2_Normalize(TransformComponent_GetForward(Entity_GetTransform(self->parent)));
  force = Vector2_Scale(force, 1000 * RigidbodyComponent_GetMass(data->rigidbody));

  RigidbodyComponent_PIDLoopToPoint(data->rigidbody, targetPos, 10, 5);

  RigidbodyComponent_AddForce(data->rigidbody, force, FORCE_TYPE_FORCE);
}

static void Destroy(Component* self)
{
  Camera_UntrackEntity(self->parent);
  free(self->data);
  free(self);
}

static void OnAttach(Component* self)
{
  Camera_TrackEntity(self->parent);
  Component *rigidCom = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_RIGIDBODY);
  SimpleAI *data = self->data;
  if(rigidCom)
  {
    
    data->rigidbody = rigidCom->data;
  }
}

static void OnOtherAttach(Component* self, Component* component)
{
  SimpleAI *data = self->data;
  if(component->type == COMPONENT_TYPE_RIGIDBODY)
  {
    
    data->rigidbody = component->data;
  }
}
