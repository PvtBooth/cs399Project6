/******************************************************************************/
/*!
	\file   GenericTorpedoComponent.c
	\author Henry Brobeck
	\date   4/3/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "GenericTorpedoComponent.h"
#include <stdlib.h>
#include "Entity.h"
#include "RigidbodyComponent.h"
#include <math.h>

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct TorpedoData
{
  float curTime;
  float maxTime;
  float speed;
  void(*detonate)(Vector2 pos, Component* component);
  Component *detonator;
}TorpedoData;
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
static void Destroy(Component *self);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static Component* Copy(Component *self);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* GenericTorpedoComponent_Create(float speed, float time, void(*detonate)(Vector2 pos, Component* component), Component* detonator)
{
  Component *component = Component_Create();
  component->update = Update;
  component->destroy = Destroy;
  
  TorpedoData *data = calloc(1, sizeof(TorpedoData));
  data->curTime = 0;
  data->maxTime = time;
  data->speed = speed;
  data->detonate = detonate;
  data->detonator = detonator;
  component->data = data;

  return component;
}

void GenericTorpedoComponent_Detonate(Component* component)
{
  TorpedoData *data = (TorpedoData*)component->data;
  Vector2 pos = TransformComponent_GetPosition(component->parent->transformComponent);
  data->detonate(pos, data->detonator);
  Entity_SetFlag(component->parent, EFLAG_DESTROY, 1);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Update(Component *self, float dt)
{

  TorpedoData *data = (TorpedoData*)self->data;

  

  data->curTime += dt;
  if(data->curTime >= data->maxTime)
  {
    Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
    data->detonate(pos, data->detonator);
    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
  }
  Component *rigidComponent = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_RIGIDBODY);
  if(!rigidComponent)
  {
    return;
  }


//  RigidbodyComponent *rigidbody = (RigidbodyComponent*)rigidComponent->data;
//  
  float rot = TransformComponent_GetRotation(self->parent->transformComponent);
  float x = cosf(rot);
  float y = sinf(rot);

  Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
  pos.x += (x * data->speed * dt);
  pos.y += (y * data->speed* dt);

  TransformComponent_SetPosition(self->parent->transformComponent, pos);

//
//  RigidbodyComponent_AddForceFloats(rigidbody, x * data->speed, y*data->speed, FORCE_TYPE_IMPULSE);

}
static void Destroy(Component *self)
{
  free(self->data);
  free(self);
}