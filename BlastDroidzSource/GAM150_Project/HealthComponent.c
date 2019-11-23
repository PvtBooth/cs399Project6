/******************************************************************************/
/*!
	\file   HealthComponent.c
	\author Henry Brobeck
	\date   2/8/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "HealthComponent.h"
#include "HealthSubject.h"
#include <stdlib.h>
#include "Sound.h"
#include "Log.h"
#include "BarComponent.h"
#include "EntityManager.h"
#include "RenderScoreComponent.h"
#include "LifetimeComponent.h"
#include "ParticleSystem.h"
#include "HUDScore.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct HealthData HealthData;
struct HealthData
{
  float currrentHealth;
  float maxHealth;
  int score;

  HUDBar *healthBar;

};

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/


static void Destroy(Component * self);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief  Health components take damage from the EVENT_TYPE_DAMAGE
 * \param maxHP The maximum HP
 * \param score The score given from killing this enemy
 * \return The new health component
 */
Component* HealthComponent_Create(int maxHP, int score)
{
  Component* newComponent = Component_Create();

  newComponent->destroy = Destroy;
  newComponent->onOtherAttach = OnOtherAttach;
  newComponent->onAttach = OnAttach;
  newComponent->eventHandle = EventHandle;
  
  HealthData *data = calloc(1, sizeof(HealthData));
  data->currrentHealth = (float)maxHP;
  data->maxHealth = (float)maxHP;
  data->score = score;

  newComponent->data = data;

  return newComponent;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

static void OnAttach(Component *self)
{
  HealthData *data = self->data;
  if(!data->healthBar)
  {
    Component *bar = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_BARCOMPONENT);
    if(bar)
    {
      Vector2 scale = { 100, 10 };
      data->healthBar = BarComponent_CreateBar(bar, data->maxHealth, scale, 10, 1.0, 0, 0, 1);
      
      BarComponent_SetVisible(data->healthBar, false);
      BarComponent_SetCurrent(data->healthBar, data->currrentHealth);

    }
  }
}
static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type != COMPONENT_TYPE_BARCOMPONENT)
  {
    return;
  }

  HealthData *data = self->data;
  Vector2 scale = { 100, 10 };
  data->healthBar = BarComponent_CreateBar(component, data->maxHealth, scale, 10, 1.0, 0, 0, 1);
  BarComponent_SetVisible(data->healthBar, false);
  BarComponent_SetCurrent(data->healthBar, data->currrentHealth);
  


}

static void EventHandle(Component *self, int type, void *eventData, bool isLocal)
{
  if(!isLocal)
  {
    return;
  }
  if(type != EVENT_DAMAGE)
  {
    return;
  }

  DamageData *damageData = eventData;
  HealthData *healthData = self->data;
  
  healthData->currrentHealth -= damageData->damage;
  
  if(healthData->healthBar)
  {
    BarComponent_SetCurrent(healthData->healthBar, healthData->currrentHealth);
    BarComponent_SetVisible(healthData->healthBar, true);
  }
  

  if(healthData->currrentHealth <= 0 && !Entity_GetFlag(self->parent, EFLAG_DESTROY))
  {
    Vector2 position2 = TransformComponent_GetPosition(self->parent->transformComponent);

    LogDataDeath death;
    death.entity = self->parent;
    death.isPlayer = false;
    death.position = position2;

    Log_LogData(LOG_TYPE_DEATH, ((LogDataGeneric){"HealthComponent.c"}), &death);
    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
    Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
    EntityManager *manager = Entity_GetEntityManager(self->parent);
    if(manager)
    {
      Entity *scorePop = EntityManager_CreateEntity(manager, "ScorePopup");
      Component *transCom = TransformComponent_Create();
      TransformComponent_SetPosition((TransformComponent*)transCom->data, pos);
      Entity_AttachComponent(scorePop, transCom);
      Entity_AttachComponent(scorePop, RenderScoreComponent_Create(healthData->score));
      Entity_AttachComponent(scorePop, LifetimeComponent_Create(2.0f));
      Vector2 velocity = { 8.f, 0.f };
      Vector2 scale = { 10.f, 10.f };
      Vector2 acceleration = { 0.0f, 0.0f };

      Component *particleComponent = ParticleSystem_Create(100, 400, 1.0f, 0.0f, 0.0f, 1.0f, 1, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
      ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CIRCLE);
      ParticleSystem_SetRGBAFinal(particleComponent->data, 1.0f, 0.0f, 0.0f, 0.1f);
      ParticleSystem_SetisLooping(particleComponent->data, false);

      Entity_AttachComponent(scorePop, particleComponent);
    }
    HUDScore_AddScore(healthData->score);
    Sound_PlayEffect(SOUND_ENEMY_EXPLODE);
  }

}