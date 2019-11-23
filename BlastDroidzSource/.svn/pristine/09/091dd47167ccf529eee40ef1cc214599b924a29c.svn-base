/******************************************************************************/
/*!
	\file   PlayerHealthComponent.c
	\author Henry Brobeck
	\date   3/14/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PlayerHealthComponent.h"
#include <stdlib.h>
#include "BarComponent.h"
#include "HealthSubject.h"
#include "Log.h"
#include "Sound.h"
#include "Player.h"
#include "Camera.h"
#include "InputEvent.h"
#include "LifetimeComponent.h"
#include "EntityManager.h"
#include "ParticleSystem.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct PlayerHealthData PlayerHealthData;
struct PlayerHealthData
{
  int currrentHealth;
  int maxHealth;
  int id;

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

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt);
static void Destroy(Component * self);
static void OnAttach(Component * self);
static void OnOtherAttach(Component * self, Component * component);
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* PlayerHealthComponent_Create(int id, int maxHP)
{
  Component* newComponent = Component_Create();

  newComponent->destroy = Destroy;
  newComponent->onOtherAttach = OnOtherAttach;
  newComponent->onAttach = OnAttach;
  newComponent->eventHandle = EventHandle;

  PlayerHealthData *data = calloc(1, sizeof(PlayerHealthData));
  data->currrentHealth = maxHP;
  data->maxHealth = maxHP;
  data->id = id;

  newComponent->data = data;

  newComponent->type = COMPONENT_TYPE_PLAYERHEALTH;


  return newComponent;
}

float PlayerHealthComponent_GetHP(Component* component)
{
  if(!component)
  {
    return 0.0f;
  }

  PlayerHealthData *data = (PlayerHealthData*)component->data;
  return (float)data->currrentHealth;
}

float PlayerHealthComponent_GetMaxHP(Component* component)
{
  if (!component)
  {
    return 0.0f;
  }

  PlayerHealthData *data = (PlayerHealthData*)component->data;
  return (float)data->maxHealth;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Destroy(Component* self)
{
  Camera_UntrackEntity(self->parent);
  free(self->data);
  free(self);
}

static void OnAttach(Component *self)
{
  PlayerHealthData *data = self->data;
  if (!data->healthBar)
  {
    Component *bar = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_BARCOMPONENT);
    if (bar)
    {
      Vector2 scale = { 100.0f, 10.0f };
      data->healthBar = BarComponent_CreateBar(bar, (float)data->maxHealth, scale, 10.0f, 1.0f, 0.0f, 0.0f, 1.0f);

      BarComponent_SetCurrent(data->healthBar, (float)data->currrentHealth);

    }
  }
}
static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type != COMPONENT_TYPE_BARCOMPONENT)
  {
    return;
  }

  PlayerHealthData *data = self->data;

  Vector2 scale = { 100.0f, 10.0f };
  data->healthBar = BarComponent_CreateBar(component, (float)data->maxHealth, scale, 10.0f, 1.0f, 0.0f, 0.0f, 1.0f);

  BarComponent_SetCurrent(data->healthBar, (float)data->currrentHealth);


}

static void EventHandle(Component *self, int type, void *eventData, bool isLocal)
{
  if (!isLocal)
  {
    return;
  }
  if (type != EVENT_DAMAGE)
  {
    return;
  }

  DamageData *damageData = eventData;
  PlayerHealthData *healthData = self->data;

  healthData->currrentHealth -= (int)damageData->damage;

  if (healthData->healthBar)
  {
    BarComponent_SetCurrent(healthData->healthBar, (float)healthData->currrentHealth);
  }

  if (healthData->currrentHealth <= 0)
  {
    Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);

    LogDataDeath death;
    death.entity = self->parent;
    death.isPlayer = false;
    death.position = pos;

    Log_LogData(LOG_TYPE_DEATH, ((LogDataGeneric){"PlayerHealthComponent.c"}), &death);

    EntityManager *manager = Entity_GetEntityManager(self->parent);
    Entity *deathPartics = EntityManager_CreateEntity(manager, "ScorePopup");
    Component *transCom = TransformComponent_Create();
    TransformComponent_SetPosition((TransformComponent*)transCom->data, pos);
    Entity_AttachComponent(deathPartics, transCom);
    Entity_AttachComponent(deathPartics, LifetimeComponent_Create(2.0f));
    Vector2 velocity = { 8.f, 0.f };
    Vector2 scale = { 10.f, 10.f };
    Vector2 acceleration = { 0.0f, 0.0f };

    Color color = Colors_GetPlayerColor(healthData->id, COLORT_TINT);
    Component *particleComponent = ParticleSystem_Create(200, 800, color.r, color.g, color.b, color.a, 1, scale, velocity, acceleration, 1.0f, 0.0f, RENDER_LAYER_PARTICLE);
    ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CIRCLE);
    ParticleSystem_SetRGBAFinal(particleComponent->data, 0.0f, 0.0f, 0.0f, 0.0f);
    ParticleSystem_SetisLooping(particleComponent->data, false);

    Entity_AttachComponent(deathPartics, particleComponent);
    

    Entity_SetFlag(self->parent, EFLAG_DESTROY, 1);
    Camera_UntrackEntity(self->parent);
    Player_ResetID(healthData->id);
    
    Sound_PlayEffect(SOUND_PLAYER_EXPLODE);
  }

  InputEvent_ControllerVibrate(healthData->id, (damageData->damage / 100.0f), (damageData->damage / 100.0f) + 0.15f);
}