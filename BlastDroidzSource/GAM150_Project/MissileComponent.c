/******************************************************************************/
/*!
	\file   MissileComponent.c
	\author Henry Brobeck
	\date   3/20/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "MissileComponent.h"
#include <stdlib.h>
#include "Entity.h"
#include "RigidbodyComponent.h"
#include "HealthSubject.h"
#include "EntityManager.h"
#include "ParticleSystem.h"
#include "LifetimeComponent.h"


#define MISSILE_TIGHTNESS 20.0f
#define MISSILE_CORRECTION_FACTOR 0.1f

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
struct MissileData
{
  float speed;
  float turnRate;
  int damage;
  float radius;
  RigidbodyComponent *rigidbody;
  Entity *target;
  bool targetLost;
  bool retargetFailed;
  void(*payload)(Component *component, int damage, float radius, Vector2 pos);
  Component *payloadComponent;
};
typedef struct MissileData MissileData;
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
static void OnOtherAttach(Component * self, Component * component);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);
static void Detonate(Component *self, MissileData *data, Vector2 position);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* MissileComponent_Create(float speed, float turnRate, int damage, float radius, Entity *target)
{

  Component *component = Component_Create();
  component->update = Update;
  component->onAttach = OnAttach;
  component->onOtherAttach = OnOtherAttach;
  component->eventHandle = EventHandle;

  MissileData *data = calloc(1, sizeof(MissileData));
  data->speed = speed;
  data->turnRate = turnRate;
  data->damage = damage;
  data->radius = radius;
  data->target = target;
  data->payload = NULL;
  
  component->data = data;
  component->type = COMPONENT_TYPE_MISSILEBARRAGE;
  
  return component;

}

void MissileComponent_SetPayload(Component* component, void(* payload)(Component* self, int damage, float radius, Vector2 pos), Component *parent)
{
  MissileData *data = (MissileData*)component->data;
  data->payload = payload;
  data->payloadComponent = parent;

}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  if(!self->parent->transformComponent)
  {
    return;
  }
  MissileData *data = (MissileData*)self->data;
  Vector2 curPos = TransformComponent_GetPosition(self->parent->transformComponent);
  if(data->retargetFailed)
  {
    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
    if(data->payload)
    {
      data->payload(data->payloadComponent,data->damage, data->radius, curPos);
    }
    else
    {
      Detonate(self, data, curPos);
    }
    return;
  }

  if(!data->rigidbody || !data->target)
  {
    return;
  }


  

  if(data->targetLost)
  {
    int numFound = 0;
    const ColliderComponent **col = PhysicsManager_CircleCast(curPos, 500, &numFound);
    Entity *target = NULL;
    for(int i = 0; i < numFound; i++)
    {
      if(ColliderComponent_GetLayer(col[i]) == PHYSICS_LAYER_ENEMY)
      {
        Entity *potentialTarget = ColliderComponent_GetEntity(col[i]);;
        if (!Entity_GetFlag(potentialTarget, EFLAG_DESTROY))
        {
          target = potentialTarget;
          break;
        }
        
      }
    }
    if(!target)
    {
      data->targetLost = true;
      data->retargetFailed = true;
      return;
    }
    
    data->targetLost = false;
    data->retargetFailed = false;
    data->target = target;
  }
  
  Vector2 tarPos = TransformComponent_GetPosition(data->target->transformComponent);
  Vector2 tarVel = RigidbodyComponent_GetVelocity(Entity_FindComponentByType(data->target, COMPONENT_TYPE_RIGIDBODY)->data);
  
  tarPos.x += tarVel.x * MISSILE_CORRECTION_FACTOR;
  tarPos.y += tarVel.y * MISSILE_CORRECTION_FACTOR;

  Vector2 delta;
  delta.x = tarPos.x - curPos.x;
  delta.y = tarPos.y - curPos.y;
  

  Vector2 direction = Vector2_Normalize(delta);

  //RigidbodyComponent_AddForce(data->rigidbody, direction, FORCE_TYPE_FORCE);
  Vector2 forward = TransformComponent_GetForward(self->parent->transformComponent);
  forward.x *= data->speed;
  forward.y *= data->speed;

  Vector2 curVel = RigidbodyComponent_GetVelocity(data->rigidbody);

  Vector2 newVel;
  newVel.x = -curVel.x + forward.x;
  newVel.y = -curVel.y + forward.y;

  RigidbodyComponent_AddVelocity(data->rigidbody, newVel);

  RigidbodyComponent_PIDLoopToDirection(data->rigidbody, direction, data->turnRate, MISSILE_TIGHTNESS);

  RaycastData raycastData;
  bool hit = PhysicsManager_RaycastWithMask(&raycastData, curPos, direction, data->radius * 0.75f, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_ENEMY, PHYSICS_LAYER_DEFAULT));
  if (hit)
  {
    if (data->payload)
    {
      data->payload(data->payloadComponent, data->damage, data->radius, curPos);
    }
    else
    {
      Detonate(self, data, curPos);
    }
    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
  }
  
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

static void OnAttach(Component* self)
{
  MissileData *data = (MissileData*)self->data;
  Component *rigidbody = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_RIGIDBODY);
  if(rigidbody)
  {
    data->rigidbody = (RigidbodyComponent*)rigidbody->data;
  }
}

static void OnOtherAttach(Component* self, Component* component)
{
  MissileData *data = (MissileData*)self->data;
  if(data->rigidbody)
  {
    return;
  }
  if(component->type == COMPONENT_TYPE_RIGIDBODY)
  {
    data->rigidbody = (RigidbodyComponent*)component->data;
  }
}
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal)
{
    UNREFERENCED_PARAMETER(isLocal);
  if(type != EVENT_ENTITY_DESTROY)
  {
    return;
  }
  MissileData *data = (MissileData*)self->data;
  Entity *destroyed = (Entity*)eventData;
  if (data->target == destroyed)
  {
    data->targetLost = true;
    data->retargetFailed = false;
    //Detonate(self, data, TransformComponent_GetPosition(self->parent->transformComponent));
    //Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
  }
}
static void Detonate(Component *self, MissileData *data, Vector2 position)
{
  
  int numHit = 0;
  const ColliderComponent **col = PhysicsManager_CircleCast(position, data->radius, &numHit);
  if (col && numHit != 0)
  {
    for (int i = 0; i < numHit; i++)
    {
      const ColliderComponent *collider = col[i];
      if (ColliderComponent_GetLayer(collider) != PHYSICS_LAYER_ENEMY)
      {
        continue;
      }
      HealthSubject_DamageNotify(self->parent, ColliderComponent_GetEntity(collider), (float)data->damage);
    }
  }

  Entity *entity = EntityManager_CreateEntity(Entity_GetEntityManager(self->parent), "MissileDetonation");

  Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
  Component *transCom = TransformComponent_Create();
  TransformComponent_SetPosition((TransformComponent*)transCom->data, pos);

  Entity_AttachComponent(entity, transCom);

  Entity_AttachComponent(entity, LifetimeComponent_Create(1.0f));

  Vector2 velocity = { 5.f, 0.f };
  Vector2 scale = { 10.f, 10.f };
  Vector2 acceleration = { 0.0f, 0.0f };

  Component *particleComponent = ParticleSystem_Create(20, 100, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CIRCLE);
  ParticleSystem_SetRGBAFinal(particleComponent->data, 1.0f, 1.0f, 0.0f, 0.5f);
  ParticleSystem_SetisLooping(particleComponent->data, false);


  Entity_AttachComponent(entity, particleComponent);

}