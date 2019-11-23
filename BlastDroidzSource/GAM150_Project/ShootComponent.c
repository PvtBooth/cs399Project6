/******************************************************************************/
/*!
	\file   ShootComponent.c
	\author Henry Brobeck
	\date   1/23/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ShootComponent.h"
#include "Entity.h"
#include "RigidbodyComponent.h"
#include "RenderComponent.h"
#include "LifetimeComponent.h"
#include "Sound.h"
#include "Camera.h"
#include "CollisionDestroyComponent.h"
#include "CollisionDamageComponent.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct ShootData
{
  MeshGroup *bullet;
  TransformComponent *transform;
  float lifetime;
  float shootDelay;
  float currentDelay;
  PhysicsLayer layer;
  


}ShootData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt);
static void Destroy(Component * self);
static void OnAttach(Component * self);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief Creates a new ShootComponent, which will fire a bullet with a given primitive
 * \param bullet The bullet to shoot
 * \return A new ShootComponent
 */
Component* ShootComponent_Create(MeshGroup *bullet, PhysicsLayer bulletLayer)
{
  Component* newComponent = Component_Create();
  newComponent->update = Update;
  newComponent->destroy = Destroy;
  newComponent->onAttach = OnAttach;
  newComponent->eventHandle = EventHandle;

  ShootData *data = calloc(1, sizeof(ShootData));

  data->bullet = bullet;
  data->currentDelay = 0;
  data->shootDelay = 0.1f;
  data->layer = bulletLayer;

  newComponent->data = data;

  return newComponent;
}

void ShootComponent_Shoot(Component* self)
{
  // get data
  ShootData *data = (ShootData*)self->data;
  // if we have no transform yet, do nothing
  if (!(data->transform))
  {
    return;
  }
  if(data->currentDelay > 0)
  {
    return;
  }
  data->currentDelay = data->shootDelay;

  Sound_PlayEffect(SOUND_BASIC_SHOOT);

  //create new entity for bullet
  Entity *newShot = Entity_Create("bullet");
  //create a transform for the new bullet
  Component *shotTCom = TransformComponent_Create();
  //get the transformcomponent for later use
  TransformComponent *shotTrans = (TransformComponent*)shotTCom->data;
  Entity_AttachComponent(newShot, shotTCom);

  //creating the initial position for the bullet
  Vector2 position = TransformComponent_GetPosition(data->transform);
  float angle = TransformComponent_GetRotation(data->transform);
  Vector2 offset = { 51, 0 };
  offset = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(data->transform), offset);
  position = Vector2_Add(position, offset);
  //apply the angle/position for the bullet
  TransformComponent_SetPosition(shotTrans, position);
  TransformComponent_SetRotation(shotTrans, angle);

  //add physics to the bullet
  Component *shotRigidbodyCom = RigidbodyComponent_Create(0.1f, 100, 1.0f, 0.0f, 0, true);
  RigidbodyComponent *shotRigidbody = (RigidbodyComponent*)shotRigidbodyCom->data;
  Entity_AttachComponent(newShot, shotRigidbodyCom);
  //RigidbodyComponent_AddForce(shotRigidbody, &offset);
  RigidbodyComponent_AddVelocityFloats(shotRigidbody, offset.x * 50.0f, offset.y*50.0f);

  //add collision to the bullet
  Vector2 halfBullet;
  halfBullet.x = 10; //TODO make this dynamic
  halfBullet.y = 10;

  Vector2 zero = Vector2_Zero();
  Component *shotColliderCom = ColliderComponent_CreateBox(halfBullet, zero, false);
  ColliderComponent_SetLayer(shotColliderCom->data, data->layer);
  Entity_AttachComponent(newShot, shotColliderCom);


  //add the shape to the bullet
  Component *shotRenderCom = RenderComponent_CreateMeshGroup(data->bullet, RENDER_LAYER_PLAYER);
  Entity_AttachComponent(newShot, shotRenderCom);

  //ad lifetime to the bullet
  Entity_AttachComponent(newShot, LifetimeComponent_Create(1));
  Entity_AttachComponent(newShot, CollisionDestroyComponent_Create());

  Entity_AttachComponent(newShot, CollisionDamageComponent_Create(1));

  
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static void Update(Component* self, float dt)
{
  
  ShootData *data = (ShootData*)self->data;
  data->currentDelay -= dt;
  Vector2 position = TransformComponent_GetPosition(data->transform);
//  float angle = TransformComponent_GetRotation(data->transform);
  Vector2 offset = { 51, 0 };
  offset = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(data->transform), offset);
  //offset.x = cos(angle) * 51; //TODO make this a dynamic offset
  //offset.y = sin(angle) * 51;
  position = Vector2_Add(position, offset);
  //RaycastData raycastData;
  //int i;
  //PhysicsManager_RaycastWithMask(&raycastData, position, offset, 400, Physics_Layers_GetLayerMask(PHYSICS_LAYER_PLAYER));
  //Camera_DrawLine(raycastData.origin.x, raycastData.origin.y, 0, 1, 0, 1, raycastData.point.x, raycastData.point.y, 0, 1, 0, 1, true);
  //Camera_DrawLine(raycastData.point.x, raycastData.point.y, 0, 1, 1, 1, raycastData.point.x + raycastData.normal.x * 25, raycastData.point.y + raycastData.normal.y * 25, 0, 1, 1, 1, true);
  //Camera_DrawLine(raycastData.point.x, raycastData.point.y, 1, 0, 1, 1, raycastData.point.x + raycastData.reflection.x * 25, raycastData.point.y + raycastData.reflection.y * 25, 1, 0, 1, 1, true);

  
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

static void OnAttach(Component* self)
{
  Component *transform = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  if(transform)
  {
    ShootData *data = (ShootData*)self->data;
    data->transform = (TransformComponent*)transform->data;
  }
}


static void EventHandle(Component *self, int type, void *eventData, bool isLocal)
{
  UNREFERENCED_PARAMETER(eventData);
  UNREFERENCED_PARAMETER(isLocal);
  if(type == EVENT_ABILITY_1)
  {
    ShootComponent_Shoot(self);
  }
}