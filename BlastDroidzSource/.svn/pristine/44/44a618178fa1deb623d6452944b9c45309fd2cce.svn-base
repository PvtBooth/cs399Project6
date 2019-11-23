/******************************************************************************/
/*!
	\file   MissileBarrageComponent.c
	\author Henry Brobeck
	\date   3/20/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "MissileBarrageComponent.h"
#include "AbilityComponent.h"
#include "EntityManager.h"
#include "StandardMeshGroups.h"
#include "MissileComponent.h"
#include "ParticleSystem.h"
#include "RandGen.h"
#include "BarComponent.h"
#include "AssetTable.h"
#include "Sound.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct MissileBarrageData
{
  int damage;
  int volleySize;
  float speed;
  float turnRate;
  float cooldown;
  float delay;

  Entity **targetList;
  bool *targetAttacked;

  HUDBar *bar;

  AbilityData *abilityData;

}MissileBarrageData;
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
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static void Triggered(Component* self, EVENT_TYPE type);
static void Fire(Vector2 pos, MissileBarrageData *data, EntityManager *entityManager);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* MissileBarrageComponent_Create(int damage, int numFired, float speed, float turnRate, float cooldown, EVENT_TYPE triggerEvent)
{
  Component *component = AbilityComponent_Create(triggerEvent);
  AbilityData *abilityData = (AbilityData*)component->data;
  MissileBarrageData *data = calloc(1, sizeof(MissileBarrageData));
  
  data->damage = damage;
  data->volleySize = numFired;
  data->speed = speed;
  data->turnRate = turnRate;
  data->cooldown = cooldown;
  data->delay = 0.0f;
  data->abilityData = abilityData;

  data->targetList = calloc(data->volleySize, sizeof(Entity*));
  data->targetAttacked = calloc(data->volleySize, sizeof(bool));

  

  abilityData->data = data;
  abilityData->abilityUpdate = Update;
  abilityData->destroy = &Destroy;
  abilityData->triggered = Triggered;

  component->onAttach = OnAttach;
  component->onOtherAttach = OnOtherAttach;

  return component;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(Component* self, float dt)
{
  AbilityData *abilityData = (AbilityData*)self->data;
  MissileBarrageData *data = (MissileBarrageData*)abilityData->data;
  
  data->delay -= dt;

  if(data->bar)
  {
    if(data->delay > 0)
    {
      BarComponent_SetCurrent(data->bar, data->delay);
      BarComponent_SetVisible(data->bar, true);
    }
    else
    {
      BarComponent_SetVisible(data->bar, false);
    }
  }
  
  if(abilityData->state == ABILITY_STATE_IDLE)
  {
    abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
    return;
  }

  if(abilityData->state == ABILITY_STATE_PRIMED)
  {
    abilityData->state = ABILITY_STATE_IDLE;
    Player_DeactivateAbilities(abilityData->player);
    if(data->delay <= 0)
    {
      data->delay = data->cooldown;
      Fire(TransformComponent_GetPosition(self->parent->transformComponent), data, Entity_GetEntityManager(self->parent));
    }
  }
  abilityData->cooldownPercentage = min(1 - data->delay / data->cooldown, 1);
}
static void OnAttach(Component *self)
{
  AbilityData *abilityData = (AbilityData*)self->data;
  MissileBarrageData *data = (MissileBarrageData*)abilityData->data;
  
  if(data->bar)
  {
    return;
  }

  Component *barComponent = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_BARCOMPONENT);
  if(!barComponent)
  {
    return;
  }

  HUDBar *bar = BarComponent_CreateBar(barComponent, data->cooldown, ((Vector2){100, 10}), 3, 1.0, 1.0, 1.0, 1.0);
  if(bar)
  {
    data->bar = bar;
  }

}
static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type != COMPONENT_TYPE_BARCOMPONENT)
  {
    return;
  }

  AbilityData *abilityData = (AbilityData*)self->data;
  MissileBarrageData *data = (MissileBarrageData*)abilityData->data;
  if(data->bar)
  {
    return;
  }
  HUDBar *bar = BarComponent_CreateBar(component, data->cooldown, ((Vector2){100, 10}), 3, 1.0, 1.0, 1.0, 1.0);
  if (bar)
  {
    data->bar = bar;
  }
}

static void Destroy(Component* self)
{
  AbilityData *abilityData = (AbilityData*)self->data;
  MissileBarrageData *data = (MissileBarrageData*)abilityData->data;

  free(data->targetAttacked);
  free(data->targetList);
  free(data);
}



static void Triggered(Component* self, EVENT_TYPE type)
{
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(self);
}

static void Fire(Vector2 pos, MissileBarrageData *data, EntityManager *entityManager)
{
  int numDetected = 0;
  const ColliderComponent **col = PhysicsManager_CircleCast(pos, 2000, &numDetected);
  
  

  if(!numDetected)
  {
    return;
  }

//  for(int i = 0; i < data->volleySize; i++)
//  {
//    data->targetAttacked[i] = false;
//    data->targetList[i] = NULL;
//  }
//  
////  memset(data->targetList, 0, data->volleySize * sizeof(Entity*));
////  memset(data->targetAttacked, false, data->volleySize * sizeof(bool));
//
//  int numEnemies = 0;
//  for(int i = 0; i < numDetected; i++)
//  {
//    if(ColliderComponent_GetLayer(col[i]) != PHYSICS_LAYER_ENEMY)
//    {
//      continue;
//    }
//    data->targetList[numEnemies++] = ColliderComponent_GetEntity(col[i]);
//  }
//
//  int numAttacks = min(numEnemies, data->volleySize);
//  
//  int numFired = 0;
//
//  while(numFired < numAttacks)
//  {
//    numFired++;
//    int attack = RandGen_Int(0, numEnemies);
//    while(data->targetAttacked[attack])
//    {
//      attack = RandGen_Int(0, numEnemies);
//    }
//    
//    data->targetAttacked[attack] = true;
//
//    Entity *missile = EntityManager_CreateEntity(entityManager, "missile");
//    Component *transComponent = TransformComponent_Create();
//    TransformComponent *transform = (TransformComponent*)transComponent->data;
//    TransformComponent_SetPosition(transform, pos);
//
//    Entity_AttachComponent(missile, transComponent);
//    Entity_AttachComponent(missile, RenderComponent_CreateMeshGroup(StandardMeshGroups_GetMissile()));
//    Entity_AttachComponent(missile, MissileComponent_Create(data->speed, data->turnRate, data->damage, 50, data->targetList[attack]));
//
//    Vector2 particleScale = { 5,5 };
//    Vector2 particleVelocity = { -2, 0 };
//
//
//    Component *particleSystem = ParticleSystem_Create(100, 200, 1.0f, 0.0f, 0.0f, 1.0f, 0.25, particleScale, particleVelocity, ((Vector2) {0, 0}), 0.0f, 0.0f);
//
//    Entity_AttachComponent(missile, particleSystem);
//
//    Component *rigidComponent = RigidbodyComponent_Create(1, 10, 0.05, 0.1, false, false);
//    RigidbodyComponent *rigidbody = (RigidbodyComponent*)rigidComponent->data;
//    Entity_AttachComponent(missile, rigidComponent);
//    
//    
//
//  }



  int numFired = 0;
  for(int i = 0; i < numDetected; i++)
  {
    if (ColliderComponent_GetLayer(col[i]) != PHYSICS_LAYER_ENEMY)
    {
      continue;
    }
    

    Entity *missile = EntityManager_CreateEntity(entityManager, "missile");
    Component *transComponent = TransformComponent_Create();
    TransformComponent *transform = (TransformComponent*)transComponent->data;
    TransformComponent_SetPosition(transform, pos);

    Entity_AttachComponent(missile, transComponent);
    Asset *meshGroupAsset = AssetTable_GetAsset("MG_Missile", ASSET_TYPE_MESHGROUP);
    MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
    Entity_AttachComponent(missile, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER));
    Entity_AttachComponent(missile, MissileComponent_Create(data->speed, data->turnRate, data->damage, 50, ColliderComponent_GetEntity(col[i])));

    AbilityData *abilityData = data->abilityData;
    

    Vector2 particleScale = { 5,5 };
    Vector2 particleVelocity = { -2, 0 };


    float r, g, b, a;
    //int id = 0;
    Player_GetColor(abilityData->player->id, COLORT_TINT, &r, &g, &b, &a);

    Component *particleSystem = ParticleSystem_Create(100, 200, r, g, b, a, 0.25, particleScale, particleVelocity, ((Vector2) {0,0}), 0.0f, 0.0f, RENDER_LAYER_PARTICLE);

    Entity_AttachComponent(missile, particleSystem);

    Component *rigidComponent = RigidbodyComponent_Create(1.0f, 10.0f, 0.05f, 0.1f, false, false);
    //RigidbodyComponent *rigidbody = (RigidbodyComponent*)rigidComponent->data;
    Entity_AttachComponent(missile, rigidComponent);
    

    numFired++;
    if(numFired > data->volleySize)
    {
      Sound_PlayEffect(SOUND_ABILITY_MISSLE);
      break;
    }

  }

}