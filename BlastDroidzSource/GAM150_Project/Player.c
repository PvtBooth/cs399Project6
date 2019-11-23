/******************************************************************************/
/*!
\file   Player.c
\author Ryan Booth, Cole Astaire(minor)
\date   2/4/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes the functions that handle players
*/
/******************************************************************************/

#include <stdlib.h>
#include "Player.h"
#include "Camera.h"
#include "LaserComponent.h"
#include "Log.h"
#include "Vector2.h"

#include "AutoLaserComponent.h"
#include "BombAbilityComponent.h"
#include "DashAbilityComponent.h"
#include "AIGraph.h"

#include "PlayerMovementComponent.h"

#include "BarComponent.h"
#include "ParticleSystem.h"
#include "PlayerHealthComponent.h"
#include "MissileBarrageComponent.h"
#include "GravMissileAbilityComponent.h"

#include "RenderScoreComponent.h"
#include "Colors.h"
#include "GravTorpedoComponent.h"
#include "CurtainMissileComponent.h"

#define PLAYER_SPEED_DEFAULT 18000

#define SET_RGBA_BABYBLUE *r = 0; *g = 0.75f; *b = 1; *a = 1;
#define SET_RGBA_SLIMEGREEN *r = 0; *g = 1; *b = 0; *a = 1;
#define SET_RGBA_TURQUOISE *r = 0; *g = 1; *b = .75f; *a = 1;
#define SET_RGBA_BRIGHTPURPLE *r = 0.65f; *g = 0; *b = 1; *a = 1;

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static Player player[4];

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static MeshGroupAsset* GetPlayerMesh(int id);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
/*!
\brief Initializes the player array
*/
void Player_Init(StackState *state)
{
  UNREFERENCED_PARAMETER(state);
  int i;
  //Sets all the players pointers to NULL
  for (i = 0; i < 4; i++)
  {
    player[i].playerEntity = NULL;
  }
}
/*!
\brief Adds an ability to a player
*/
void Player_AddAbility(Player *p, Component *ability)
{
  AbilityData *data = ability->data;
  data->nextAbility = p->abilities;
  p->abilities = ability;
}
/*!
\brief Disables the player's use of abilities
*/
void Player_DeactivateAbilities(Player *p)
{
  p->abilityActive = false;
}

/*!
\brief
Gets the player's position (Zero based, because that's the correct way to do things, RYAN)
*/
Vector2 Player_GetPosition(int id)
{
  if(!player[id].transformComponent)
  {
    return Vector2_Zero();
  }
  
  return TransformComponent_GetPosition(player[id].transformComponent);
}

///*!
//\brief
//Creates a player
//*/
//void Player_Create(char* name, int player_number, u32 color, Vector2 position, StackState *state)
//{
//  Component *transformComponent;
//  Component *rigidbodyComponent;
//  Component *colliderComponent;
//  TransformComponent *transData;
//  Vector2 halfSize = { 50, 50 };
//  Vector2 zero = { 0, 0 };
//
//
//  player[player_number - 1]->playerEntity = EntityManager_CreateEntity(state->entityManager, name);
//
//  if (player_number == 1)
//  {
//	  player[player_number - 1]->method = BOTH;
//  }
//  else
//  {
//	  player[player_number - 1]->method = GAMEPAD;
//  }
//
//  //Transform
//  transformComponent = TransformComponent_Create();
//
//  player[player_number - 1]->transformComponent = (TransformComponent*)transformComponent->data;
//
//  TransformComponent_SetPosition(player[player_number - 1]->transformComponent, position);
//
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, transformComponent);
//
//  //Rigidbody
//  rigidbodyComponent = RigidbodyComponent_Create(10 * 1 * 1 * 1, 6000.0f, 0.25f, 0.75f, false, false);
//
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, rigidbodyComponent);
//
//  player[player_number - 1]->rigidbodyComponent = (RigidbodyComponent*)rigidbodyComponent->data;
//
//  //Collider
//  colliderComponent = ColliderComponent_CreatePolygon(playerCollider, 3, false);
//
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, colliderComponent);
//
//  player[player_number - 1]->colliderComponent = (ColliderComponent*)colliderComponent->data;
//
//  ColliderComponent_SetLayer(colliderComponent->data, PHYSICS_LAYER_PLAYER);
//
//  MeshGroup *meshGroup = MeshGroup_Create();
//
//  //MeshGroup_AddRect(meshGroup, 100, 100, 0xFFFFFFFF);
//  MeshGroup_AddTriangle(meshGroup, -40, 40, -40, -40, 40, 0.0f, color);
//  //Render
//  player[player_number - 1]->renderComponent = RenderComponent_CreateMeshGroup(meshGroup);
//
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, player[player_number - 1]->renderComponent);
//
//  Camera_TrackEntity(player[player_number-1]->playerEntity);
//
//  //MeshGroup *bullet = MeshGroup_Create();
//  //MeshGroup_AddRect(bullet, 20.0f, 20.0f, 0xFFFFFFFF);
//
//  //Component *shootComponent = ShootComponent_Create(bullet, PHYSICS_LAYER_PLAYER);
//  //player[player_number - 1]->shootComponent = shootComponent;
//  //Entity_AttachComponent(player[player_number - 1]->playerEntity, shootComponent);
//  //
//  //Vector2 offset = { 25, 0 };
//  //Component *laserComponent = LaserComponent_Create(offset, 1000.0f, 0.1f, 1);
//  //player[player_number - 1]->laserComponent = laserComponent;
//  //Entity_AttachComponent(player[player_number - 1]->playerEntity, laserComponent);
//
//  Component *autoLaser1 = AutoLaserComponent_Create(10, 3000, 1, 400, 4000, 0.05f, EVENT_ABILITY_0);
//  AbilityComponent_SetPlayer(autoLaser1, player[player_number - 1]);
//  Component *autoLaser2 = AutoLaserComponent_Create(200, 750, 1, 400, 3000, 0.9f, EVENT_ABILITY_1);
//  AbilityComponent_SetPlayer(autoLaser2, player[player_number - 1]);
//  Component *bombAbility = BombAbilityComponent_Create(0.25f, 1.5f, 1000, 50, EVENT_ABILITY_2);
//  AbilityComponent_SetPlayer(bombAbility, player[player_number - 1]);
//  Component *dash = DashAbilityComponent_Create(3, 0.25f, 100000, EVENT_ABILITY_3);
//  AbilityComponent_SetPlayer(dash, player[player_number - 1]);
//  //player[player_number - 1]->shootComponent = shootComponent;
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, autoLaser1);
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, autoLaser2);
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, bombAbility);
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, dash);
//
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, PlayerMovementComponent_Create(10000, 80, 20, player[player_number - 1]));
//
//  Component *inputGatherData = InputGatherComponent_Create();
//  Entity_AttachComponent(player[player_number - 1]->playerEntity, inputGatherData);
//  player[player_number - 1]->inputGatherComponent = inputGatherData->data;
//
//  //Logging
//  LogDataGeneric genericData;
//  genericData.sourceFile = "Player.c";
//  LogDataSpawn spawnData;
//  spawnData.position = (Vector2) { 0, 0 }; //Need to set this to wherever the player spawns
//  spawnData.entity = player[player_number - 1]->playerEntity;
//  spawnData.isPlayer = true;
//  Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);
//
//  AIGraph_RegisterMarker(player[player_number - 1]->playerEntity, AIMARKER_TYPE_PLAYER);
//  player[player_number - 1]->aiMarker = Entity_FindComponentByType(player[player_number - 1]->playerEntity, COMPONENT_TYPE_AIMARKER)->data;
//
//
//}
/*!
\brief
Gets the player's id (zero based)
*/
Player* Player_GetPlayer(int id)
{
  return &player[id];
}
/*!
\brief
Sets the player's input method (zero based)
*/
void Player_SetInputMethod(int id, INPUT_METHOD method)
{
	player[id].method = method;
}
/*!
\brief
Gets the player's input method
*/
INPUT_METHOD Player_GetInputMethod(Player* p)
{
	return p->method;
}

/*!
\brief
Tries freeing the player's data
*/
void Player_Exit(void)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    Entity_SetFlag(player[i].playerEntity, EFLAG_DESTROY, 1);
    Player_Reset();
  }
}
/*!
\brief Gets the color of the player
*/
void Player_GetColor(int id, TEAMCOLOR type, float* r, float* g, float* b, float* a)
{
  Color color = Colors_GetPlayerColor(id, type);
  *r = color.r;
  *g = color.g;
  *b = color.b;
  *a = color.a;
}
/*!
\brief Resets all members of all players pointers to NULL
*/
void Player_Reset()
{
  for(int i = 0; i < 4; i++)
  {
    player[i].playerEntity = NULL;
    player[i].colliderComponent = NULL;
    player[i].inputGatherComponent = NULL;
    player[i].renderComponent = NULL;
    player[i].rigidbodyComponent = NULL;
    player[i].transformComponent = NULL;
    player[i].abilities = NULL;
  }
}
/*!
\brief Resets all members of the players pointers to NULL
*/
void Player_ResetID(int i)
{

  player[i].playerEntity = NULL;
  player[i].colliderComponent = NULL;
  player[i].inputGatherComponent = NULL;
  player[i].renderComponent = NULL;
  player[i].rigidbodyComponent = NULL;
  player[i].transformComponent = NULL;
  player[i].method = 0;
}

/*!
\brief Creates the Bomb ship type
*/
Entity * Player_Type1_Create(int id, Vector2 position)
{
  Component *transformComponent;
  Component *rigidbodyComponent;
  Component *colliderComponent;

  player[id].id = id;
  player[id].playerEntity = Entity_CreateOrphan("PlayerType1");

  player[id].method = GAMEPAD;


  //Transform
  transformComponent = TransformComponent_Create();

  player[id].transformComponent = (TransformComponent*)transformComponent->data;

  TransformComponent_SetPosition(player[id].transformComponent, position);

  Entity_AttachComponent(player[id].playerEntity, transformComponent);

  //Rigidbody
  rigidbodyComponent = RigidbodyComponent_Create(10 * 1 * 1 * 1, 6000.0f, 0.25f, 0.75f, false, false);

  Entity_AttachComponent(player[id].playerEntity, rigidbodyComponent);

  player[id].rigidbodyComponent = (RigidbodyComponent*)rigidbodyComponent->data;

  float r = 0;
  float g = 0;
  float b = 0;
  float a = 0;
  Player_GetColor(id, COLORT_PRIMARY, &r, &g, &b, &a);
  
  MeshGroupAsset *meshGroup = GetPlayerMesh(id);

  player[id].renderComponent = RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER);

  Entity_AttachComponent(player[id].playerEntity, player[id].renderComponent);

  //Collider
  colliderComponent = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);

  Entity_AttachComponent(player[id].playerEntity, colliderComponent);

  player[id].colliderComponent = (ColliderComponent*)colliderComponent->data;

  ColliderComponent_SetLayer(colliderComponent->data, PHYSICS_LAYER_PLAYER);

  //abilities
  Component *autoLaser = AutoLaserComponent_Create(1, 8, 2000, 4, 1200, 6000, 0.04f, EVENT_ABILITY_0);
  AbilityComponent_SetPlayer(autoLaser, &player[id]);
  Component *bombAbility = BombAbilityComponent_Create(0.8f, 1.5f, 750, EVENT_ABILITY_2);
  AbilityComponent_SetPlayer(bombAbility, &player[id]);

  //AbilityComponent_SetPlayer(gravMissile, &player[id]);

  //player[player_number - 1]->shootComponent = shootComponent;
  Entity_AttachComponent(player[id].playerEntity, autoLaser);
  Entity_AttachComponent(player[id].playerEntity, bombAbility);

  //MeshGroup_AddRect(meshGroup, 100, 100, 0xFFFFFFFF);

  //Render
  Camera_TrackEntity(player[id].playerEntity);

  Entity_AttachComponent(player[id].playerEntity, PlayerMovementComponent_Create(PLAYER_SPEED_DEFAULT, 80, 20, &player[id]));

  //Data
  Component *inputGatherData = InputGatherComponent_Create();
  Entity_AttachComponent(player[id].playerEntity, inputGatherData);
  player[id].inputGatherComponent = inputGatherData->data;

  //Health
  Entity_AttachComponent(player[id].playerEntity, PlayerHealthComponent_Create(id, 100));

  //Entity_AttachComponent(player[id]->playerEntity, BarComponent_Create(Vector2_Zero()));

  //AI
  AIGraph_RegisterMarker(player[id].playerEntity, AIMARKER_TYPE_PLAYER);
  player[id].aiMarker = Entity_FindComponentByType(player[id].playerEntity, COMPONENT_TYPE_AIMARKER)->data;

  // Particles
  Player_GetColor(id, COLORT_TINT, &r, &g, &b, &a);
  Vector2 scaleparticle = { 25.0f, 25.0f };
  Vector2 velocityparticle = { 0.0f, 0.0f };
  Vector2 accelerationparticle = { -0.2f, 0.0f };
  Component *particleComponent = ParticleSystem_Create(1000, 100, r, g, b, a, 0.9f, scaleparticle, velocityparticle,
    accelerationparticle, 0.25f, 1.0f, RENDER_LAYER_BACKGROUNDPARTICLE);
  ParticleSystem_SetLifetimeMax(particleComponent->data, 1.5f);
  ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
  Player_GetColor(id, COLORT_SHADE, &r, &g, &b, &a);
  ParticleSystem_SetRGBAFinal(particleComponent->data, r, g, b, 0);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 0.5f);
  ParticleSystem_SetOffset(particleComponent->data, -50.0f, 0.0f);
  
  Entity_AttachComponent(player[id].playerEntity, particleComponent);

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(player[id].playerEntity, BarComponent_Create(barOffset));

  return player[id].playerEntity;
}
/*!
\brief Creates the shotgun ship type
*/
Entity * Player_Type2_Create(int id, Vector2 position)
{
  Component *transformComponent;
  Component *rigidbodyComponent;
  Component *colliderComponent;
  MeshGroupAsset *meshGroup = GetPlayerMesh(id);

  player[id].id = id;
  player[id].playerEntity = Entity_CreateOrphan("PlayerType2");

  player[id].method = GAMEPAD;

  Entity_AttachComponent(player[id].playerEntity, PlayerHealthComponent_Create(id, 100));
  //Transform
  transformComponent = TransformComponent_Create();

  player[id].transformComponent = (TransformComponent*)transformComponent->data;

  TransformComponent_SetPosition(player[id].transformComponent, position);

  Entity_AttachComponent(player[id].playerEntity, transformComponent);

  //Rigidbody
  rigidbodyComponent = RigidbodyComponent_Create(10 * 1 * 1 * 1, 6000.0f, 0.25f, 0.75f, false, false);

  Entity_AttachComponent(player[id].playerEntity, rigidbodyComponent);

  player[id].rigidbodyComponent = (RigidbodyComponent*)rigidbodyComponent->data;

  //Collider
  colliderComponent = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);

  Entity_AttachComponent(player[id].playerEntity, colliderComponent);

  player[id].colliderComponent = (ColliderComponent*)colliderComponent->data;

  ColliderComponent_SetLayer(colliderComponent->data, PHYSICS_LAYER_PLAYER);

  Component *autoLaser = AutoLaserComponent_Create(13, 2.25f, 675, 2.5f, 100, 2000, 0.55f, EVENT_ABILITY_0);
  AbilityComponent_SetPlayer(autoLaser, &player[id]);
  Component *dash = DashAbilityComponent_Create(2, 0.25f, 40000, EVENT_ABILITY_2);
  AbilityComponent_SetPlayer(dash, &player[id]);
  //player[player_number - 1]->shootComponent = shootComponent;
  Entity_AttachComponent(player[id].playerEntity, autoLaser);
  Entity_AttachComponent(player[id].playerEntity, dash);


  float r = 0;
  float g = 0;
  float b = 0;
  float a = 0;
  Player_GetColor(id, COLORT_PRIMARY, &r, &g, &b, &a);

  // Particles
  Player_GetColor(id, COLORT_TINT, &r, &g, &b, &a);
  Vector2 scaleparticle = { 25.0f, 25.0f };
  Vector2 velocityparticle = { 0.0f, 0.0f };
  Vector2 accelerationparticle = { -0.2f, 0.0f };
  Component *particleComponent = ParticleSystem_Create(1000, 100, r, g, b, a, 0.9f, scaleparticle, velocityparticle,
    accelerationparticle, 0.25f, 1.0f, RENDER_LAYER_BACKGROUNDPARTICLE);
  ParticleSystem_SetLifetimeMax(particleComponent->data, 1.5f);
  ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
  Player_GetColor(id, COLORT_SHADE, &r, &g, &b, &a);
  ParticleSystem_SetRGBAFinal(particleComponent->data, r, g, b, 0);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 0.5f);
  ParticleSystem_SetOffset(particleComponent->data, -50.0f, 0.0f);

  Entity_AttachComponent(player[id].playerEntity, particleComponent);

  //Render
  player[id].renderComponent = RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER);

  Entity_AttachComponent(player[id].playerEntity, player[id].renderComponent);

  Camera_TrackEntity(player[id].playerEntity);

  Entity_AttachComponent(player[id].playerEntity, PlayerMovementComponent_Create(PLAYER_SPEED_DEFAULT, 80, 20, &player[id]));

  Component *inputGatherData = InputGatherComponent_Create();
  Entity_AttachComponent(player[id].playerEntity, inputGatherData);
  player[id].inputGatherComponent = inputGatherData->data;

  AIGraph_RegisterMarker(player[id].playerEntity, AIMARKER_TYPE_PLAYER);
  player[id].aiMarker = Entity_FindComponentByType(player[id].playerEntity, COMPONENT_TYPE_AIMARKER)->data;

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(player[id].playerEntity, BarComponent_Create(barOffset));

  return player[id].playerEntity;
}
/*!
\brief Returns the number of players currently in the game
*/
int Player_PlayerCount()
{
    int count = 0;

    for (int i = 0; i < 4; i++)
    {
        if (player[i].playerEntity != NULL)
            count++;
    }

    return count;
}

/*!
\brief Creates the gravity ship type
*/
Entity * Player_Type3_Create(int id, Vector2 position)
{
  Component *transformComponent;
  Component *rigidbodyComponent;
  Component *colliderComponent;
  MeshGroupAsset *meshGroup = GetPlayerMesh(id);

  player[id].id = id;
  player[id].playerEntity = Entity_CreateOrphan("PlayerType3");

  player[id].method = GAMEPAD;

  //Transform
  transformComponent = TransformComponent_Create();

  player[id].transformComponent = (TransformComponent*)transformComponent->data;

  TransformComponent_SetPosition(player[id].transformComponent, position);

  Entity_AttachComponent(player[id].playerEntity, transformComponent);

  //Rigidbody
  rigidbodyComponent = RigidbodyComponent_Create(10 * 1 * 1 * 1, 6000.0f, 0.25f, 0.75f, false, false);

  Entity_AttachComponent(player[id].playerEntity, rigidbodyComponent);

  player[id].rigidbodyComponent = (RigidbodyComponent*)rigidbodyComponent->data;

  float r = 0;
  float g = 0;
  float b = 0;
  float a = 0;
  Player_GetColor(id, COLORT_PRIMARY, &r, &g, &b, &a);

  player[id].renderComponent = RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER);

  Entity_AttachComponent(player[id].playerEntity, player[id].renderComponent);

  //Collider
  colliderComponent = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);

  Entity_AttachComponent(player[id].playerEntity, colliderComponent);

  player[id].colliderComponent = (ColliderComponent*)colliderComponent->data;

  ColliderComponent_SetLayer(colliderComponent->data, PHYSICS_LAYER_PLAYER);

  //abilities
  Component *autoLaser = AutoLaserComponent_Create(2, 20, 1500, 0.9f, 200, 8000, 0.115f, EVENT_ABILITY_0);
  AbilityComponent_SetPlayer(autoLaser, &player[id]);

  Component *missileBarrage = MissileBarrageComponent_Create(2, 10, 1000, 150, 2.5f, EVENT_ABILITY_2);
  AbilityComponent_SetPlayer(missileBarrage, &player[id]);

  Entity_AttachComponent(player[id].playerEntity, autoLaser);
  Entity_AttachComponent(player[id].playerEntity, missileBarrage);


  //Render
  Camera_TrackEntity(player[id].playerEntity);

  Entity_AttachComponent(player[id].playerEntity, PlayerMovementComponent_Create(PLAYER_SPEED_DEFAULT, 80, 20, &player[id]));

  //Data
  Component *inputGatherData = InputGatherComponent_Create();
  Entity_AttachComponent(player[id].playerEntity, inputGatherData);
  player[id].inputGatherComponent = inputGatherData->data;

  //Health
  Entity_AttachComponent(player[id].playerEntity, PlayerHealthComponent_Create(id, 100));


  //AI
  AIGraph_RegisterMarker(player[id].playerEntity, AIMARKER_TYPE_PLAYER);
  player[id].aiMarker = Entity_FindComponentByType(player[id].playerEntity, COMPONENT_TYPE_AIMARKER)->data;

  // Particles
  Player_GetColor(id, COLORT_TINT, &r, &g, &b, &a);
  Vector2 scaleparticle = { 25.0f, 25.0f };
  Vector2 velocityparticle = { 0.0f, 0.0f };
  Vector2 accelerationparticle = { -0.2f, 0.0f };
  Component *particleComponent = ParticleSystem_Create(1000, 100, r, g, b, a, 0.9f, scaleparticle, velocityparticle,
    accelerationparticle, 0.25f, 1.0f, RENDER_LAYER_BACKGROUNDPARTICLE);
  ParticleSystem_SetLifetimeMax(particleComponent->data, 1.5f);
  ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
  Player_GetColor(id, COLORT_SHADE, &r, &g, &b, &a);
  ParticleSystem_SetRGBAFinal(particleComponent->data, r, g, b, 0);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 0.5f);
  ParticleSystem_SetOffset(particleComponent->data, -50.0f, 0.0f);

  Entity_AttachComponent(player[id].playerEntity, particleComponent);

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(player[id].playerEntity, BarComponent_Create(barOffset));

  return player[id].playerEntity;
}
/*!
\brief Creates the Missle ship type
*/
Entity * Player_Type4_Create(int id, Vector2 position)
{
  Component *transformComponent;
  Component *rigidbodyComponent;
  Component *colliderComponent;
  MeshGroupAsset *meshGroup = GetPlayerMesh(id);

  player[id].id = id;
  player[id].playerEntity = Entity_CreateOrphan("PlayerType4");

  player[id].method = GAMEPAD;


  //Transform
  transformComponent = TransformComponent_Create();

  player[id].transformComponent = (TransformComponent*)transformComponent->data;

  TransformComponent_SetPosition(player[id].transformComponent, position);

  Entity_AttachComponent(player[id].playerEntity, transformComponent);

  //Rigidbody
  rigidbodyComponent = RigidbodyComponent_Create(10 * 1 * 1 * 1, 6000.0f, 0.25f, 0.75f, false, false);

  Entity_AttachComponent(player[id].playerEntity, rigidbodyComponent);

  player[id].rigidbodyComponent = (RigidbodyComponent*)rigidbodyComponent->data;

  //Score Test
  //  renderScoreComponent = RenderScoreComponent_Create(1);
  //  Entity_AttachComponent(player[id].playerEntity, renderScoreComponent);


  //  Vector2 asteroidMesh[] = { { 0, 1 },{ 0.65f, 0.65f },{ 0.5f, -0.65f },{ -0.5f, -0.65f },{ -0.65f, 0.65f } };

  float r = 0;
  float g = 0;
  float b = 0;
  float a = 0;
  Player_GetColor(id, COLORT_PRIMARY, &r, &g, &b, &a);
  //for (int j = 0; j < 5; j++)
  //{
  //  //MeshGroup_AddTriangleRGBA(meshGroup, polygonalMesh[j].x, polygonalMesh[j].y, 0, 0, polygonalMesh[(j + 1) % 5].x, polygonalMesh[(j + 1) % 5].y, 1, 1, 1, 1);
  //  MeshGroup_AddTriangleRGBA(meshGroup, asteroidMesh[j].x, asteroidMesh[j].y, 0, 0, asteroidMesh[(j + 1) % 5].x, asteroidMesh[(j + 1) % 5].y, r, g, b, a);
  //}\

  player[id].renderComponent = RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_PLAYER);

  Entity_AttachComponent(player[id].playerEntity, player[id].renderComponent);

  //Collider
  colliderComponent = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);

  Entity_AttachComponent(player[id].playerEntity, colliderComponent);

  player[id].colliderComponent = (ColliderComponent*)colliderComponent->data;

  ColliderComponent_SetLayer(colliderComponent->data, PHYSICS_LAYER_PLAYER);


  //abilities
  Component *curtainMissile = CurtainMissileComponent_Create(5, 11, 8000, 1, 300, 0.65f, 5, PI / 1.5f, 2000, 3000, EVENT_ABILITY_0);
  AbilityComponent_SetPlayer(curtainMissile, &player[id]);

  //Component *gravMissile = GravMissileAbilityComponent_Create(10, 1000, 200, 100, 1000, 150, 1, EVENT_ABILITY_3);
  //AbilityComponent_SetPlayer(gravMissile, &player[id]);
  Component *gravTorpedo = GravTorpedoComponent_Create(1, 5.0f, 1000, EVENT_ABILITY_2);
  AbilityComponent_SetPlayer(gravTorpedo, &player[id]);
  //player[player_number - 1]->shootComponent = shootComponent;
  Entity_AttachComponent(player[id].playerEntity, curtainMissile);
  //Entity_AttachComponent(player[id].playerEntity, gravMissile);
  Entity_AttachComponent(player[id].playerEntity, gravTorpedo);



  //MeshGroup_AddRect(meshGroup, 100, 100, 0xFFFFFFFF);

  //Render
  Camera_TrackEntity(player[id].playerEntity);

  Entity_AttachComponent(player[id].playerEntity, PlayerMovementComponent_Create(PLAYER_SPEED_DEFAULT, 80, 20, &player[id]));

  //Data
  Component *inputGatherData = InputGatherComponent_Create();
  Entity_AttachComponent(player[id].playerEntity, inputGatherData);
  player[id].inputGatherComponent = inputGatherData->data;

  //Health
  Entity_AttachComponent(player[id].playerEntity, PlayerHealthComponent_Create(id, 100));

  //Entity_AttachComponent(player[id]->playerEntity, BarComponent_Create(Vector2_Zero()));

  //AI
  AIGraph_RegisterMarker(player[id].playerEntity, AIMARKER_TYPE_PLAYER);
  player[id].aiMarker = Entity_FindComponentByType(player[id].playerEntity, COMPONENT_TYPE_AIMARKER)->data;

  // Particles
  Player_GetColor(id, COLORT_TINT, &r, &g, &b, &a);
  Vector2 scaleparticle = { 25.0f, 25.0f };
  Vector2 velocityparticle = { 0.0f, 0.0f };
  Vector2 accelerationparticle = { -0.2f, 0.0f };
  Component *particleComponent = ParticleSystem_Create(1000, 100, r, g, b, a, 0.9f, scaleparticle, velocityparticle,
    accelerationparticle, 0.25f, 1.0f, RENDER_LAYER_BACKGROUNDPARTICLE);
  ParticleSystem_SetLifetimeMax(particleComponent->data, 1.5f);
  ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
  Player_GetColor(id, COLORT_SHADE, &r, &g, &b, &a);
  ParticleSystem_SetRGBAFinal(particleComponent->data, r, g, b, 0);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 0.5f);
  ParticleSystem_SetOffset(particleComponent->data, -50.0f, 0.0f);

  Entity_AttachComponent(player[id].playerEntity, particleComponent);

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(player[id].playerEntity, BarComponent_Create(barOffset));

  return player[id].playerEntity;
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static MeshGroupAsset* GetPlayerMesh(int id)
{
  Asset *asset;
  switch (id)
  {
  case 0:
    asset = AssetTable_GetAsset("MG_PlayerShip0", ASSET_TYPE_MESHGROUP);
    return asset->data;
  case 1:
    asset = AssetTable_GetAsset("MG_PlayerShip1", ASSET_TYPE_MESHGROUP);
    return asset->data;
  case 2:
    asset = AssetTable_GetAsset("MG_PlayerShip2", ASSET_TYPE_MESHGROUP);
    return asset->data;
  case 3:
    asset = AssetTable_GetAsset("MG_PlayerShip3", ASSET_TYPE_MESHGROUP);
    return asset->data;
  default:
    asset = AssetTable_GetAsset("MG_PlayerShip0", ASSET_TYPE_MESHGROUP);
    return asset->data;
  }
}