/******************************************************************************/
/*!
\file   LevelGenNormal.c
\author Evan Kau
\date   3/11/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "LevelGenNormal.h"
#include <stdlib.h>
#include <stdbool.h>
#include "Entity.h"
#include "MeshGroup.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "RandGen.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "AISwarmAgent.h"
#include "Enemy_SimpleComponent.h"
#include "AIForwardShoot.h"
#include "Log.h"
#include "AIMarkerComponent.h"
#include "AIGraph.h"
#include "JumpgateComponent.h"
#include "LevelState.h"
#include "ShipSelectStackState.h"
#include "Player.h"
#include "BarComponent.h"
#include "ParticleSystem.h"
#include "Enemy_BerserkerComponent.h"
#include "Enemy_SuicideComponent.h"
#include "AssetTable.h"
#include "KillAssignment.h"
#include "Enemy_StationComponent.h"
#include "Enemy_CapitalShipComponent.h"
#include "AITurretComponent.h"
#include "LevelMarker.h"

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

static int enemyCount = 0;
static int levelNumber = 0;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static bool SpawnPlanet(float radius, StackState *state);
static bool SpawnAsteroid(StackState *state, int minRadius, int maxRadius);
static bool SpawnAsteroidAtPoint(StackState *state, Vector2 point);
static bool SpawnJumpgate(StackState *state);
static bool SpawnBasicEnemyCluster(StackState* state, int enemyPoints);
static bool SpawnEnemies(StackState *state, int enemyPoints);
static bool SpawnEnemyPoint(StackState *state);
static bool SpawnEnemyStation(StackState *state);
static int SpawnEnemyShip(StackState *state, Vector2 position);
static int SpawnEnemyBerserkerShip(StackState *state, Vector2 position);
static int SpawnEnemyCapitalShip(StackState *state, Vector2 position);
static int SpawnEnemySuicideShip(StackState *state, Vector2 position);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void LevelGenNormal_Generate(float levelRadius, StackState *state)
{
  levelNumber++;
  enemyCount = 0;

  int i = 0;
  levelRadius = max(levelRadius, 1500);
  PhysicsManager_NewGridFromCount(levelRadius * 2, levelRadius * 2, 25, 25);

  //SpawnPlanet(levelRadius * (RandGen_Float() * 0.1f + 0.1f), state);

  int enemyPoints = 1000 + 4 * levelNumber * (levelNumber + 40);
  int players = 0;
  for (i = 0; i < 4; i++)
  {
    if (ShipSelectStackState_GetPlayerShip(i) != -1) //Not playing == -1
    {
      players++;
    }
  }
  enemyPoints *= (int)(0.5f + players / 2.f);
  SpawnBasicEnemyCluster(state, enemyPoints);
  
  //int rings = RandGen_Int(1, 3);
  //for (i = 0; i < rings; i++)
  //{
  //  int minRadius = RandGen_Int((int)(levelRadius * 0.3f), (int)(levelRadius * 0.9f));
  //  int maxRadius = RandGen_Int(minRadius, (int)(levelRadius * 0.9f));
  //  int j = 0;

  //  if (maxRadius - minRadius > 0.5f / rings * levelRadius)
  //  {
  //    minRadius = (int)(maxRadius - 0.5f / rings * levelRadius);
  //  }
  //  int asteroidCount = max((maxRadius - minRadius) * (maxRadius - minRadius) / 50000, 60);

  //  while (j < asteroidCount)
  //  {
  //    if (SpawnAsteroid(state, minRadius, maxRadius))
  //    {
  //      j++;
  //      attempts = 0;
  //    }
  //    attempts++;
  //    if (attempts > 50)
  //    {
  //      j++;
  //    }
  //  }
  //}

  //int clusters = RandGen_Int(0, 10 - (rings * 2));
  //for (i = 0; i < clusters; i++)
  //{
  //  Vector2 point = Vector2_Scale(Vector2_FromAngle(RandGen_Float() * 2 * PI), (float)RandGen_Int((int)(levelRadius * 0.5f), (int)(levelRadius * 0.8f)));
  //  float radius = (RandGen_Float() * 0.15f + 0.05f) * levelRadius;

  //  int asteroidCount = RandGen_Int(1, (int)max(radius * radius / 50000, 20));

  //  int j = 0;
  //  while (j < asteroidCount)
  //  {
  //    if (SpawnAsteroidAtPoint(state, Vector2_Add(point, Vector2_Scale(Vector2_FromAngle(RandGen_Float() * 2 * PI), sqrtf(RandGen_Float()) * radius))))
  //    {
  //      j++;
  //      attempts = 0;
  //    }
  //    attempts++;
  //    if (attempts > 50)
  //    {
  //      j++;
  //    }
  //  }
  //}

  //SpawnEnemies(state, enemyPoints);

  Assignment_Set(KillAssignment_Create((int)(enemyCount / (3 / max(players / 2.f, 1)))));

  AEGfxSetBackgroundColor(RandGen_Float() * 0.075f, RandGen_Float() * 0.075f, RandGen_Float() * 0.075f);
}

bool LevelGenNormal_SpawnPlayers(StackState *state)
{
  Vector2 normalPos = Vector2_Scale(Vector2_FromAngle(PI * (0.65f + RandGen_Float() * 0.5f)), PhysicsManager_GetArenaRadius() - 150);
  for (int i = 0; i < 4; i++)
  {
    int playerShip = ShipSelectStackState_GetPlayerShip(i);
    Entity *playerEntity = NULL;
  
    switch (playerShip)
    {
    case 0:
      playerEntity = Player_Type1_Create(i, Vector2_Zero());
      TransformComponent_SetPosition(Player_GetPlayer(i)->transformComponent, Vector2_AddFloats(normalPos, RandGen_Float() * 100 - 50, RandGen_Float() * 100 - 50));
      break;
    case 1:
      playerEntity = Player_Type2_Create(i, Vector2_Zero());
      TransformComponent_SetPosition(Player_GetPlayer(i)->transformComponent, Vector2_AddFloats(normalPos, RandGen_Float() * 100 - 50, RandGen_Float() * 100 - 50));
      break;
    case 2:
      playerEntity = Player_Type3_Create(i, Vector2_Zero());
      TransformComponent_SetPosition(Player_GetPlayer(i)->transformComponent, Vector2_AddFloats(normalPos, RandGen_Float() * 100 - 50, RandGen_Float() * 100 - 50));
      break;
    case 3:
      playerEntity = Player_Type4_Create(i, Vector2_Zero());
      TransformComponent_SetPosition(Player_GetPlayer(i)->transformComponent, Vector2_AddFloats(normalPos, RandGen_Float() * 100 - 50, RandGen_Float() * 100 - 50));
      break;
    default:
      //playerEntity = Player_Type4_Create(i, Vector2_Zero());
      //TransformComponent_SetPosition(Player_GetPlayer(i)->transformComponent, Vector2_AddFloats(normalPos, RandGen_Float() * 100 - 50, RandGen_Float() * 100 - 50));
      break;
    }
  
    if (playerEntity)
    {
      EntityManager_AddEntity(state->entityManager, playerEntity);
    }
  }

  return true;
}

void LevelGenNormal_ResetLevel()
{
  levelNumber = 0;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static bool SpawnPlanet(float radius, StackState *state)
{
  Asset *meshGroupAsset = AssetTable_GetAsset("MG_Planet", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;

  Component *collider = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);
  ColliderComponent_SetLayer(collider->data, PHYSICS_LAYER_DEFAULT);

  Component *transform = TransformComponent_Create();
  TransformComponent_SetScaleFloats(transform->data, radius, radius);

  Entity *planet = EntityManager_CreateEntity(state->entityManager, "Planet");

  Entity_AttachComponent(planet, transform);
  Entity_AttachComponent(planet, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_NEUTRAL));
  Entity_AttachComponent(planet, collider);
  Entity_AttachComponent(planet, RigidbodyComponent_Create(10000000, 10000000000, 0, 0, true, false));

  TransformComponent_SetPositionFloats(planet->transformComponent, 0, 0);

  return true;
}

static bool SpawnJumpgate(StackState *state)
{
  float scale = 250;
  Vector2 position = Vector2_FromAngle(RandGen_Float() * 2 * PI);
  float rand = RandGen_Float();
  position = Vector2_Scale(position, sqrtf(rand) * (PhysicsManager_GetArenaRadius() / 5) + PhysicsManager_GetArenaRadius() / 5);

  int collidersInRadius = 0;
  PhysicsManager_CircleCast(position, scale + 100, &collidersInRadius);
  if (!collidersInRadius)
  {
    Entity *jumpgate = EntityManager_CreateEntity(state->entityManager, "Jumpgate");
    Entity_AttachComponent(jumpgate, TransformComponent_Create());

    Asset *meshGroupAsset = AssetTable_GetAsset("MG_Jumpgate", ASSET_TYPE_MESHGROUP);
    MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;

    Entity_AttachComponent(jumpgate, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_NEUTRAL));
    Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_DEFAULT);
    Entity_AttachComponent(jumpgate, colCom);

    // Particle test
    Vector2 scaleparticle = { 25.0f, 25.0f };
    Vector2 velocityparticle = { 0.0f, 0.0f };
    Vector2 accelerationparticle = { -0.2f, 0.0f };
    Component *particleComponent = ParticleSystem_Create(1000, 100, 0.65f, 0.85f, 0.85f, 1.f, 1, scaleparticle, velocityparticle,
      accelerationparticle, 0.25f, 1.0f, RENDER_LAYER_PARTICLE);
    ParticleSystem_SetLifetimeMax(particleComponent->data, 5);
    ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
    ParticleSystem_SetRGBAFinal(particleComponent->data, 0.65f, 0.85f, 0.85f, 0.f);
    ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CIRCLE);
    //ParticleSystem_SetConeWidth(particleComponent->data, 0.5f);
    ParticleSystem_SetOffset(particleComponent->data, 0.0f, 0.0f);
    
    Entity_AttachComponent(jumpgate, particleComponent);

    float rootMass = scale / 2;
    float mass = rootMass * rootMass;
    Component *rigidComponent = RigidbodyComponent_Create(mass, mass * mass * mass, 0.925f, 0.25f, false, false);
    Entity_AttachComponent(jumpgate, rigidComponent);
    //RigidbodyComponent *rigidbody = rigidComponent->data;

    Entity_AttachComponent(jumpgate, JumpgateComponent_Create(500, &LevelState_NextLevel));

    TransformComponent_SetScaleFloats(jumpgate->transformComponent, scale, scale);

    TransformComponent_SetPosition(jumpgate->transformComponent, position);


    LevelMarker_Create(jumpgate->transformComponent, 1.0f, 1.0f, 1.0f, 1.0f);


    return true;
  }
  return false;
}

static bool SpawnAsteroid(StackState *state, int minRadius, int maxRadius)
{
  Vector2 position = Vector2_FromAngle(RandGen_Float() * 2 * PI);
  position = Vector2_Scale(position, sqrtf(RandGen_Float()) * (maxRadius - minRadius) + minRadius);
  
  return SpawnAsteroidAtPoint(state, position);
}

static bool SpawnAsteroidAtPoint(StackState *state, Vector2 point)
{
  float scale = RandGen_Float() * 270 + 30;

  int collidersInRadius = 0;
  PhysicsManager_CircleCast(point, scale, &collidersInRadius);
  if (!collidersInRadius)
  {
    Entity *asteroid = EntityManager_CreateEntity(state->entityManager, "Asteroid");
    Entity_AttachComponent(asteroid, TransformComponent_Create());

    Asset *meshGroupAsset = AssetTable_GetAsset("MG_Asteroid", ASSET_TYPE_MESHGROUP);
    MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
    Entity_AttachComponent(asteroid, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_NEUTRAL));

    Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, 0);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_DEFAULT);
    Entity_AttachComponent(asteroid, colCom);

    float rootMass = scale / 12;
    float mass = rootMass * rootMass;
    Component *rigidComponent = RigidbodyComponent_Create(mass, mass * mass * mass, 0.925f, 0.25f, false, false);
    Entity_AttachComponent(asteroid, rigidComponent);
    RigidbodyComponent *rigidbody = rigidComponent->data;
    RigidbodyComponent_AddVelocityFloats(rigidbody, (float)RandGen_Int(-50, 50), (float)RandGen_Int(-50, 50));

    TransformComponent_SetScaleFloats(asteroid->transformComponent, scale, scale);
    TransformComponent_SetPosition(asteroid->transformComponent, point);
    TransformComponent_SetRotation(asteroid->transformComponent, RandGen_Float() * 2 * PI);

    return true;
  }
  return false;
}

static bool SpawnBasicEnemyCluster(StackState* state, int enemyPoints)
{
	int i = 0;
	int pointCount = 1;
	int swarmMinPoints = (int)(enemyPoints / pointCount * 0.85f);
	int swarmMaxPoints = (int)(enemyPoints / pointCount * 1.15f);

	while (i < pointCount)
	{
		if (SpawnEnemyPoint(state))
		{
			i++;
		}
	}

	AIMarker* marker = AIGraph_GetMarkerHead();
	for (i = 0; i < pointCount; i++)
	{
		int swarmPoints = RandGen_Int(swarmMinPoints, swarmMaxPoints);
		Entity* markerEntity = AIMarker_GetEntity(marker);
		Vector2 position = TransformComponent_GetPosition(markerEntity->transformComponent);
		while (swarmPoints > 0)
		{
			Vector2 modPosition = Vector2_AddFloats(position, RandGen_Float() * 400 - 200, RandGen_Float() * 400 - 200);
			swarmPoints -= SpawnEnemyShip(state, modPosition);
			enemyCount++;
		}
		if (AIMarker_GetNext(marker))
			marker = AIMarker_GetNext(marker);
	}

	return true;
}

static bool SpawnEnemies(StackState *state, int enemyPoints)
{
  int i = 0;
  int pointCount = RandGen_Int(5, 20);
  int swarmMinPoints = (int)(enemyPoints / pointCount * 0.85f);
  int swarmMaxPoints = (int)(enemyPoints / pointCount * 1.15f);

  while (i < pointCount)
  {
    int type = RandGen_Int(0, 1);
    if (type == 0)
    {
      if (SpawnEnemyPoint(state))
      {
        i++;
      }
    }
    else
    {
      if (SpawnEnemyStation(state))
      {
        i++;
      }
    }
  }

  AIMarker *marker = AIGraph_GetMarkerHead();
  for (i = 0; i < pointCount; i++)
  {
    int swarmPoints = RandGen_Int(swarmMinPoints, swarmMaxPoints);
    Entity *markerEntity = AIMarker_GetEntity(marker);
    Vector2 position = TransformComponent_GetPosition(markerEntity->transformComponent);
    int type = RandGen_Int(0, 10);
    if (type <= 4)
    {
      while (swarmPoints > 0)
      {
        bool capital = !(RandGen_Int(0, 10));
        Vector2 modPosition = Vector2_AddFloats(position, RandGen_Float() * 400 - 200, RandGen_Float() * 400 - 200);
        if (capital && swarmPoints > 200)
        {
          swarmPoints -= SpawnEnemyCapitalShip(state, modPosition);
        }
        else
        {
          swarmPoints -= SpawnEnemyShip(state, modPosition);
        }
        enemyCount++;
      }
    }
    else if (type <= 8)
    {
      while (swarmPoints > 0)
      {
        bool capital = !(RandGen_Int(0, 10));
        Vector2 modPosition = Vector2_AddFloats(position, RandGen_Float() * 400 - 200, RandGen_Float() * 400 - 200);
        if (capital && swarmPoints > 200)
        {
          swarmPoints -= SpawnEnemyCapitalShip(state, modPosition);
        }
        else
        {
          swarmPoints -= SpawnEnemyBerserkerShip(state, modPosition);
        }
        enemyCount++;
      }
    }
    else
    {
      while (swarmPoints > 0)
      {
        bool capital = !(RandGen_Int(0, 10));
        Vector2 modPosition = Vector2_AddFloats(position, RandGen_Float() * 400 - 200, RandGen_Float() * 400 - 200);
        if (capital && swarmPoints > 200)
        {
          swarmPoints -= SpawnEnemyCapitalShip(state, modPosition);
        }
        else
        {
          swarmPoints -= SpawnEnemySuicideShip(state, modPosition);
        }
        enemyCount++;
      }
    }
    if (AIMarker_GetNext(marker))
      marker = AIMarker_GetNext(marker);
  }

  return true;
}

static bool SpawnEnemyPoint(StackState *state)
{
  Vector2 position = Vector2_FromAngle(RandGen_Float() * 2 * PI);
  float rand = RandGen_Float();
  position = Vector2_Scale(position, sqrtf(rand) * (PhysicsManager_GetArenaRadius() * (0.35f + 0.3f)));

  int collidersInRadius = 0;
  PhysicsManager_CircleCast(position, 500, &collidersInRadius);
  if (!collidersInRadius)
  {
    Entity *point = EntityManager_CreateEntity(state->entityManager, "EnemyPoint");
    Entity_AttachComponent(point, TransformComponent_Create());
    TransformComponent_SetPosition(point->transformComponent, position);
    AIGraph_RegisterMarker(point, AIMARKER_TYPE_POINT);
    return true;
  }
  return false;
}

static bool SpawnEnemyStation(StackState *state)
{
  Vector2 position = Vector2_FromAngle(RandGen_Float() * 2 * PI);
  float rand = RandGen_Float();
  position = Vector2_Scale(position, sqrtf(rand) * (PhysicsManager_GetArenaRadius() * (0.35f + 0.3f)));

  int collidersInRadius = 0;
  PhysicsManager_CircleCast(position, 500, &collidersInRadius);
  if (!collidersInRadius)
  {
    Entity *station = EntityManager_CreateEntity(state->entityManager, "EnemyStation");
    Entity_AttachComponent(station, TransformComponent_Create());

    Asset *meshGroupAsset = AssetTable_GetAsset("MG_EnemyStation", ASSET_TYPE_MESHGROUP);
    MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
    Entity_AttachComponent(station, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_ENEMY));

    Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, false);
    ColliderComponent *collider = (ColliderComponent*)colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
    Entity_AttachComponent(station, colCom);

    Entity_AttachComponent(station, RigidbodyComponent_Create(500.0f, 300000.0f, 0.05f, 0.25f, false, false));

    //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

    //TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50, 150), RandGen_Int(50, 150));
    TransformComponent_SetPosition(station->transformComponent, position);
    TransformComponent_SetScaleFloats(station->transformComponent, 100, 100);

    Entity_AttachComponent(station, HealthComponent_Create(50, 125));
    Entity_AttachComponent(station, Enemy_StationComponent_Create(2000));
    Vector2 projectileSize = { 80, 7 };
    Entity_AttachComponent(station, AIForwardShoot_Create(6, 2000, 2 * PI, 1500, 4, 650, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER), projectileSize));

    Vector2 barOffset = { -50, -75 };
    Entity_AttachComponent(station, BarComponent_Create(barOffset));

    AIGraph_RegisterMarker(station, AIMARKER_TYPE_STATION);

    LogDataGeneric genericData = { "LevelGenNormal.c" };
    LogDataSpawn spawnData;
    spawnData.position = position;
    spawnData.isPlayer = false;
    spawnData.entity = station;
    //damageData.sourceEntityType = data->typeID;
    Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);

    return true;
  }
  
  return false;
}

static int SpawnEnemyShip(StackState *state, Vector2 position)
{
  Entity *enemy = EntityManager_CreateEntity(state->entityManager, "EnemySimple");
  Entity_AttachComponent(enemy, TransformComponent_Create());
  
  Asset *meshGroupAsset = AssetTable_GetAsset("MG_EnemyDefault", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_ENEMY));

  Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, 0);
  ColliderComponent *collider = (ColliderComponent*)colCom->data;
  ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
  Entity_AttachComponent(enemy, colCom);

  Entity_AttachComponent(enemy, RigidbodyComponent_Create(5.0f, 1500.0f, 0.05f, 0.25f, false, false));

  //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

  //TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50, 150), RandGen_Int(50, 150));
  TransformComponent_SetPosition(enemy->transformComponent, position);

  Entity_AttachComponent(enemy, HealthComponent_Create(7, 15));
  Entity_AttachComponent(enemy, AISwarmAgent_Create(1, 1000));
  Entity_AttachComponent(enemy, Enemy_SimpleComponent_Create());
  Vector2 projectileSize = { 30, 8 };
  Entity_AttachComponent(enemy, AIForwardShoot_Create(1.25f, 750, 0.6f, 1000, 1, 200, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER), projectileSize));

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(enemy, BarComponent_Create(barOffset));

  LogDataGeneric genericData = { "LevelGenNormal.c" };
  LogDataSpawn spawnData;
  spawnData.position = position;
  spawnData.isPlayer = false;
  spawnData.entity = enemy;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);

  return 10;
}


static int SpawnEnemyBerserkerShip(StackState *state, Vector2 position)
{
  //Vector2 mesh[3] = { { 25, 0 },{ -25, -25 },{ -25, 25 }, };

  Entity *enemy = EntityManager_CreateEntity(state->entityManager, "EnemyBerserker");
  Entity_AttachComponent(enemy, TransformComponent_Create());


  Asset *meshGroupAsset = AssetTable_GetAsset("MG_EnemyBerserker", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_ENEMY));

  Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, 0);
  ColliderComponent *collider = colCom->data;
  ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
  Entity_AttachComponent(enemy, colCom);

  Entity_AttachComponent(enemy, RigidbodyComponent_Create(5.0f, 1500.0f, 0.05f, 0.25f, false, false));

  //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

  //TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50, 150), RandGen_Int(50, 150));
  TransformComponent_SetPosition(enemy->transformComponent, position);

  Entity_AttachComponent(enemy, HealthComponent_Create(6, 20));
  Entity_AttachComponent(enemy, AISwarmAgent_Create(1, 1000));
  Entity_AttachComponent(enemy, Enemy_BerserkerComponent_Create());
  Vector2 projectileSize = { 15, 7 };
  Entity_AttachComponent(enemy, AIForwardShoot_Create(1.25f, 500, 0.5f, 1500, 1, 200, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER), projectileSize));

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(enemy, BarComponent_Create(barOffset));

  LogDataGeneric genericData = { "LevelGenNormal.c" };
  LogDataSpawn spawnData;
  spawnData.position = position;
  spawnData.isPlayer = false;
  spawnData.entity = enemy;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);

  return 15;
}


static int SpawnEnemyCapitalShip(StackState *state, Vector2 position)
{
 // Vector2 mesh[3] = { { 25, 0 },{ -25, -25 },{ -25, 25 }, };

  Entity *enemy = EntityManager_CreateEntity(state->entityManager, "EnemyCapitalShip");
  Entity_AttachComponent(enemy, TransformComponent_Create());


  Asset *meshGroupAsset = AssetTable_GetAsset("MG_EnemyCapitalShip", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_ENEMY));

  Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, 0);
  ColliderComponent *collider = colCom->data;
  ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
  Entity_AttachComponent(enemy, colCom);

  Entity_AttachComponent(enemy, RigidbodyComponent_Create(500.0f, 35000.f, 0.05f, 0.25f, false, false));

  //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

  //TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50, 150), RandGen_Int(50, 150));
  TransformComponent_SetPosition(enemy->transformComponent, position);

  Entity_AttachComponent(enemy, HealthComponent_Create(60, 225));
  Entity_AttachComponent(enemy, AISwarmAgent_Create(1, 1500));
  Entity_AttachComponent(enemy, Enemy_CapitalShipComponent_Create());
  Vector2 projectileSize = { 20, 8 };
  Entity_AttachComponent(enemy, AITurret_Create(((Vector2) { 55, 55 }), 4, 0.1f, 1750, 0.6f, 1400, 1, 200, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_PLAYER, PHYSICS_LAYER_NEUTRAL), projectileSize));
  Entity_AttachComponent(enemy, AITurret_Create(((Vector2) { 55, -55 }), 4, 0.1f, 1750, 0.6f, 1400, 1, 200, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_PLAYER, PHYSICS_LAYER_NEUTRAL), projectileSize));
  Entity_AttachComponent(enemy, AITurret_Create(((Vector2) { -45, 65 }), 4, 0.1f, 1750, 0.6f, 1400, 1, 200, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_PLAYER, PHYSICS_LAYER_NEUTRAL), projectileSize));
  Entity_AttachComponent(enemy, AITurret_Create(((Vector2) { -45, -65 }), 4, 0.1f, 1750, 0.6f, 1400, 1, 200, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_PLAYER, PHYSICS_LAYER_NEUTRAL), projectileSize));

  projectileSize.x = 80;
  projectileSize.y = 15;
  Entity_AttachComponent(enemy, AITurret_Create(((Vector2) { 0, 0 }), 1.5f, 0.0f, 2250, 0.05f, 2250, 5, 2000, Physics_Layers_CreateLayerMask(2, PHYSICS_LAYER_PLAYER, PHYSICS_LAYER_NEUTRAL), projectileSize));

  Vector2 barOffset = { -50, -100 };
  Entity_AttachComponent(enemy, BarComponent_Create(barOffset));

  LogDataGeneric genericData = { "LevelGenNormal.c" };
  LogDataSpawn spawnData;
  spawnData.position = position;
  spawnData.isPlayer = false;
  spawnData.entity = enemy;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);

  return 80;
}


static int SpawnEnemySuicideShip(StackState *state, Vector2 position)
{
  //Vector2 mesh[3] = { { 25, 0 },{ -25, -25 },{ -25, 25 }, };

  Entity *enemy = EntityManager_CreateEntity(state->entityManager, "EnemySuicide");
  Entity_AttachComponent(enemy, TransformComponent_Create());


  Asset *meshGroupAsset = AssetTable_GetAsset("MG_EnemySuicide", ASSET_TYPE_MESHGROUP);
  MeshGroupAsset *meshGroup = (MeshGroupAsset*)meshGroupAsset->data;
  Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup->meshGroup, RENDER_LAYER_ENEMY));

  Component *colCom = ColliderComponent_CreatePolygon(meshGroup->sourceAsset->colliderMesh, meshGroup->sourceAsset->colliderVertCount, 0);
  ColliderComponent *collider = colCom->data;
  ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
  Entity_AttachComponent(enemy, colCom);

  Entity_AttachComponent(enemy, RigidbodyComponent_Create(5.0f, 1500.0f, 0.05f, 0.25f, false, false));

  //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

  //TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50, 150), RandGen_Int(50, 150));
  TransformComponent_SetPosition(enemy->transformComponent, position);

  Entity_AttachComponent(enemy, HealthComponent_Create(12, 30));
  Entity_AttachComponent(enemy, AISwarmAgent_Create(1, 1000));
  Entity_AttachComponent(enemy, Enemy_SuicideComponent_Create());
  //Entity_AttachComponent(enemy, AIForwardShoot_Create(1.25f, 750, 0.6f, 1000, 1, 200, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER)));

  Vector2 barOffset = { -50, -75 };
  Entity_AttachComponent(enemy, BarComponent_Create(barOffset));

  LogDataGeneric genericData = { "LevelGenNormal.c" };
  LogDataSpawn spawnData;
  spawnData.position = position;
  spawnData.isPlayer = false;
  spawnData.entity = enemy;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);

  return 40;
}