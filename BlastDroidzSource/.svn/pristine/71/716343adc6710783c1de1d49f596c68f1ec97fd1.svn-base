/******************************************************************************/
/*!
	\file   EnemyTestComponent.c
	\author Evan Kau
	\date   2/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "EnemyTestComponent.h"
#include "Vector2.h"
#include "RigidbodyComponent.h"
#include <stdlib.h>
#include "Player.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct EnemyTestComponent
{
  Vector2 targetPosition;
  float speed;
  float rotationSpeed;
  RigidbodyComponent *rigidbody;
} EnemyTestComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *component, float dt);
static void Destroy(Component *component);
static void OnOtherAttach(Component *this, Component *other);
static void OnAttach(Component *this);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
Component* EnemyTestComponent_Create(Vector2 target, float speed, float rotationSpeed)
{
  Component *component = Component_Create();
  EnemyTestComponent *enemyTestComponent = (EnemyTestComponent*)calloc(1, sizeof(EnemyTestComponent));

  enemyTestComponent->targetPosition = target;
  enemyTestComponent->speed = speed;
  enemyTestComponent->rotationSpeed = rotationSpeed;

  component->data = enemyTestComponent;
  component->update = &Update;
  component->destroy = &Destroy;
  component->onOtherAttach = &OnOtherAttach;
  component->onAttach = &OnAttach;
  component->type = COMPONENT_TYPE_ENEMYTEST;

  return component;
}

void EnemyTestComponent_SetTargetPosition(EnemyTestComponent *enemyTestComponent, Vector2 target)
{
  enemyTestComponent->targetPosition = target;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component *component, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  //Hank stuff
  Player *player = Player_GetPlayer(0);
  TransformComponent *transCom = player->transformComponent;
  Vector2 playerPos = TransformComponent_GetPosition(transCom);
  EnemyTestComponent_SetTargetPosition(component->data, playerPos);

  //End hank stuff

  EnemyTestComponent *enemyTestComponent = component->data;
 // Vector2 pos = TransformComponent_GetPosition(Entity_GetTransform(component->parent));
  Vector2 force = Vector2_Normalize(TransformComponent_GetForward(Entity_GetTransform(component->parent)));//Vector2_Sub(&enemyTestComponent->targetPosition, &pos);
  force = Vector2_Scale(force, enemyTestComponent->speed * RigidbodyComponent_GetMass(enemyTestComponent->rigidbody));

  RigidbodyComponent_PIDLoopToPoint(enemyTestComponent->rigidbody, enemyTestComponent->targetPosition, enemyTestComponent->rotationSpeed, 5);

  RigidbodyComponent_AddForce(enemyTestComponent->rigidbody, force, FORCE_TYPE_FORCE);
}

static void Destroy(Component *component)
{
  free(component->data);
  free(component);
}

static void OnOtherAttach(Component *this, Component *other)
{
  if (other->type == COMPONENT_TYPE_RIGIDBODY)
  {
    EnemyTestComponent *enemyTestComponent = this->data;
    enemyTestComponent->rigidbody = other->data;
  }
}

static void OnAttach(Component *this)
{
  EnemyTestComponent *enemyTestComponent = this->data;
  Component *component = Entity_FindComponentByType(this->parent, COMPONENT_TYPE_RIGIDBODY);
  if (component)
  {
    enemyTestComponent->rigidbody = component->data;
  }
}