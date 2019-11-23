/******************************************************************************/
/*!
	\file   PlayerHUDControllerComponent.c
	\author Henry Brobeck
	\date   3/30/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PlayerHUDControllerComponent.h"
#include "BarComponent.h"
#include <stdlib.h>
#include "Entity.h"
#include "Player.h"
#include "PlayerHealthComponent.h"
#include "HUDScore.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct PlayerHUDData
{
  int id;
  Player *player;
  Entity *playerEntity;
  Component *barComponent;
  Component *playerHealthComponent;
  HUDBar *healthBar;
  Component *abilityComponents[5];
  HUDBar *abilityBars[5];
  int numFound;
  bool playerDestroyed;
}PlayerHudData;
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
static void EventHandle(Component *self, EVENT_TYPE type, void *eventData, bool isLocal);


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* PlayerHUDControllerComponent_Create(int id)
{
  Component* component = Component_Create();

  component->update = Update;
  component->destroy = Destroy;
  component->onOtherAttach = OnOtherAttach;
  component->onAttach = OnAttach;
  component->eventHandle = EventHandle;

  PlayerHudData *data = calloc(1, sizeof(PlayerHudData));
  data->id = id;
  component->data = data;
  Player *player = Player_GetPlayer(data->id);
  data->player = player;
  data->playerEntity = player->playerEntity;
  

  return component;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static void Update(Component* self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  PlayerHudData *data = (PlayerHudData*)self->data;
  if(data->playerDestroyed)
  {
    return;
  }

  if(data->healthBar && data->playerHealthComponent)
  {
    BarComponent_SetCurrent(data->healthBar, PlayerHealthComponent_GetHP(data->playerHealthComponent));
    float max = PlayerHealthComponent_GetMaxHP(data->playerHealthComponent);
    float current = PlayerHealthComponent_GetHP(data->playerHealthComponent);
    HUDScore_RenderHealth(data->id, max, current);
  }
  for (int i = 0; i < data->numFound; i++)
  {
    AbilityData *abilityData = (AbilityData*)data->abilityComponents[i]->data;
    float value = abilityData->cooldownPercentage;
    if (value > 1)
    {
      value = 1.0f;
    }
    else if(value < 0)
    {
      value = 0.0f;
    }
    BarComponent_SetCurrent(data->abilityBars[i], value);
  }
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

static void OnAttach(Component* self)
{
  PlayerHudData *data = (PlayerHudData*)self->data;
  

  if(!data->playerHealthComponent)
  {    
    Component *playerHealth = Entity_FindComponentByType(data->playerEntity, COMPONENT_TYPE_PLAYERHEALTH);
    if(playerHealth)
    {
      data->playerHealthComponent = playerHealth;
    }
  }
  
  if(!data->barComponent)
  {
    Component *barComponent = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_BARCOMPONENT);
    if (barComponent)
    {
      data->barComponent = barComponent;
    }
    else
    {
      return;
    }
  }

  if(data->id == 0 || data->id == 1)
  {
    Component *abilityIterator = data->player->abilities;
    data->numFound = 0;
    while (abilityIterator)
    {
      AbilityData *abilityData = (AbilityData*)abilityIterator->data;
      data->abilityComponents[data->numFound] = abilityIterator;
      data->abilityBars[data->numFound] = BarComponent_CreateBar(data->barComponent, 1.0f, ((Vector2){100, 10}), 5, 1.0f, 1.0f, 1.0f, 1.0f);
      BarComponent_SetCurrent(data->abilityBars[data->numFound], 1.0f);
      abilityIterator = abilityData->nextAbility;
      data->numFound++;
    }
  }
  if(data->barComponent && data->playerHealthComponent)
  {
    float r, g, b, a = 0;
    Player_GetColor(data->id, COLORT_TINT, &r, &g, &b, &a);
    float max = PlayerHealthComponent_GetMaxHP(data->playerHealthComponent);
    data->healthBar = BarComponent_CreateBar(data->barComponent, max, ((Vector2){500, 50}), 5, r, g, b, a);
    
  }
  
  if (data->id == 2 || data->id == 3)
  {
    Component *abilityIterator = data->player->abilities;
    data->numFound = 0;
    while (abilityIterator)
    {
      AbilityData *abilityData = (AbilityData*)abilityIterator->data;
      data->abilityComponents[data->numFound] = abilityIterator;
      data->abilityBars[data->numFound] = BarComponent_CreateBar(data->barComponent, 1.0f, ((Vector2){100, 10}), 5, 1.0f, 1.0f, 1.0f, 1.0f);
      BarComponent_SetCurrent(data->abilityBars[data->numFound], 1.0f);
      abilityIterator = abilityData->nextAbility;
      data->numFound++;
    }
  }
  
  
}

static void OnOtherAttach(Component* self, Component* component)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(component);
}
static void EventHandle(Component *self, int type, void *eventData, bool isLocal)
{
  if(isLocal)
  {
    return;
  }
  if(type != EVENT_ENTITY_DESTROY)
  {
    return;
  }

  Entity *destroyed = (Entity*)eventData;
  PlayerHudData *data = (PlayerHudData*)self->data;
  if(destroyed == data->playerEntity)
  {
    data->playerDestroyed = true;
    //Entity_SetFlag(self->parent, EFLAG_DESTROY, true);
  }

}
