/******************************************************************************/
/*!
	\file   ColeTestState2TheReStatening.c
	\author Henry Brobeck
	\date   2/6/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ColeTestState2TheReStatening.h"
#include "MainMenuState.h"
#include "ThankYouState.h"
#include "GameState.h"
#include <stdlib.h>
#include "RenderComponent.h"
#include "Entity.h"

#include "Input.h"
#include "Player.h"
#include "Controls.h"
#include "Camera.h"

#include "SimpleAIComponent.h"
#include "HealthComponent.h"
#include "RandGen.h"
#include "Game.h"
#include "Physics_BroadPhase.h"
#include "AISwarm.h"
#include "Enemy_SimpleComponent.h"
#include "AIManager.h"
#include "Time.h"
#include "ParticleSystem.h"
#include "AIForwardShoot.h"
#include "BarComponent.h"


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
static GameState *instance;

static Texture *textureTest;
static Sprite *spriteTest;
static Entity *entityTest;

static Vector2 playerCollider[3] = { { 25, 0 }, { -25, -25 }, { -25, 25}, };

static Vector2 asteroidCollider[4] = { {-40, -40}, {40, -40},{40, 40},{-40,40} };

static bool showPhysicsDebug = false;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void Load(void);
static void Init(void);
static void Update(float dt);
static void Exit(void);
static void Unload(void);
static void OnGameExit(void);

static void SpawnEnemies(void);
static void SpawnAsteroids(void);
static void SpawnBox(float x, float y, float width, float height);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

GameState* ColeTestState2_GetState()
{
  if (instance)
  {
    return instance;
  }

  instance = malloc(sizeof(GameState));
  instance->load = Load;
  instance->init = Init;
  instance->update = Update;
  instance->exit = Exit;
  instance->unload = Unload;

  GameState_RegisterExit(OnGameExit);

  return instance;
}

static void Load(void)
{
  
}

static void Init(void)
{
  PhysicsManager_NewGridFromCount(50000.0f, 50000.0f, 100, 100);
  Camera_Init();
  Camera_SetTracking(false);
  Camera_SetTargetPosition(0.0f, 0.0f);
  Camera_SetTargetSize(50.0f);
  //Player_Init();
  Controls_Init();
  SpawnEnemies();
  SpawnAsteroids();
  //Camera paralax test
  //Camera_UntrackEntity(Player_GetPlayer(1)->playerEntity);
  //Camera_UntrackEntity(Player_GetPlayer(2)->playerEntity);
  //Camera_UntrackEntity(Player_GetPlayer(3)->playerEntity);

  textureTest = Texture_Load("Assets/30secondsinphotoshop.png");
  assert(textureTest);
  spriteTest = Sprite_Create(textureTest);
  entityTest = Entity_Create("Background");
  Component *transformComponent = TransformComponent_Create();
  TransformComponent *tComp = transformComponent->data;
  Vector2 scale;
  scale.x = 6000.0f;
  scale.y = 6000.0f;
  TransformComponent_SetScale(tComp, scale);


  Entity_AttachComponent(entityTest, transformComponent);
  Component *paralaxRenderComponent = RenderComponent_CreateSprite(spriteTest, RENDER_BEHAVIOR_PARALAX, 0);
  Vector2 paralax = { 0.0001f, -0.0001f };
  RenderComponent_SetParalax(paralaxRenderComponent, paralax);
  Entity_AttachComponent(entityTest, paralaxRenderComponent);

  // Particle test
  Vector2 scaleparticle = { 25.0f, 25.0f };
  Vector2 velocityparticle = { 0.0f, 0.0f };
  Vector2 accelerationparticle = { -0.2f, 0.0f };
  Component *particleComponent = ParticleSystem_Create(1000, 100, 0.2f, 0.6f, 0.8f, 1.0f, 1, scaleparticle, velocityparticle,
                                                       accelerationparticle, 0.25f, 1.0f);
  ParticleSystem_SetLifetimeMax(particleComponent->data, 3);
  ParticleSystem_SetScaleFinal(particleComponent->data, 1.0f, 1.0f);
  ParticleSystem_SetRGBAFinal(particleComponent->data, 1.0f, 0.0f, 0.0f, 0);
  ParticleSystem_SetDispersal(particleComponent->data, PARTICLE_DISPERSAL_CONE);
  ParticleSystem_SetConeWidth(particleComponent->data, 0.5f);
  ParticleSystem_SetOffset(particleComponent->data, -75.0f, 0.0f);

  Entity_AttachComponent(Player_GetPlayer(0)->playerEntity, particleComponent);
  //spawn level bounds
  //SpawnBox(-2900, 0, 400, 6500);
  //SpawnBox(2900, 0, 400, 6500);
  //SpawnBox(0, -2900, 6500, 400);
  //SpawnBox(0, 2900, 6500, 400);
  
}

static void Update(float dt)
{
  const ColliderComponent **colliders;
  int hit;
  int i;
  //Entity_Update(dt);
  Input_Update(dt);
  Camera_Update(dt);
  PhysicsManager_Update(dt);

  //Vector2 mouse = Camera_MouseToWorld();

  //Camera_DrawLine(0, 0, 1, 1, 1, 1, mouse.x, mouse.y, 1, 1, 1, 1, true);

  //Cast to get the colliders within the box
  colliders = PhysicsManager_BoxCast(((Vector2){1000, 0}), ((Vector2) { 300, 300 }), &hit);
  //For each hit,
  for (i = 0; i < hit; i++)
  {
    //Get the transform
    //TransformComponent *trans = ColliderComponent_GetTransform(colliders[i]);
    //Set the position to (0,0)
    //TransformComponent_SetPosition(trans, Vector2_Zero());
  }
  
  if(showPhysicsDebug)
  {
    //Physics_BroadPhase_Debug();
    AIManager_Debug();
  }

  if(AEInputCheckCurr(VK_CONTROL))
    Physics_BroadPhase_Debug();

  if(AEInputCheckTriggered(VK_RETURN))
  {
      if(AEInputCheckTriggered('P'))
         showPhysicsDebug = !showPhysicsDebug;
  }

  if (AEInputCheckTriggered('Q'))
  {
    Camera_SetTracking(true);
  }

  if (AEInputCheckCurr(VK_ESCAPE))
  {
      GameState_SetNextState(ThankYouState_GetState());
  }

  /*if (AEInputCheckCurr('Q'))
  {
      GameState_SetNextState(MainMenuState_GetState());
  }*/
  if(AEInputCheckTriggered('V'))
  {
    SpawnEnemies();
  }

  if (!AIManager_GetNearestSwarm(Vector2_Zero()))
  {
    GameState_SetNextState(ThankYouState_GetState());
  }

}

static void Exit(void)
{
  //Entity_DestroyAllImmediate();
    //Player_Exit();
}

static void Unload(void)
{
  //Texture_DestroyAll();
  //Sprite_DestroyAll();
}

static void OnGameExit(void)
{
  if (instance)
  {
    free(instance);
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void SpawnEnemies(void)
{
  for(int i = 0; i < 25; i++)
  {
    Entity *enemy = Entity_Create("Enemy");
    Entity_AttachComponent(enemy, TransformComponent_Create());
    MeshGroup *meshGroup = MeshGroup_Create();
    
    MeshGroup_AddTriangle(meshGroup, -25.0f, 25.0f, -25.0f, -25.0f, 25.0f, 0.0f, 0xFFFF0000);
    
    Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup));
    Component *colCom = ColliderComponent_CreatePolygon(playerCollider, 3, 0);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
    Entity_AttachComponent(enemy, colCom);
    
    Entity_AttachComponent(enemy, RigidbodyComponent_Create(5.0f, 2000.0f, 0.05f, 0.25f, false, false));
    
    //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

    //TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50, 150), RandGen_Int(50, 150));
    TransformComponent_SetPositionFloats(enemy->transformComponent, RandGen_Int(50.0f, 50.0f), RandGen_Int(50.0f, 50.0f));

    Entity_AttachComponent(enemy, HealthComponent_Create(3));

    Entity_AttachComponent(enemy, AISwarmAgent_Create(1.0f, 1000.0f));

    Entity_AttachComponent(enemy, Enemy_SimpleComponent_Create());

    Entity_AttachComponent(enemy, AIForwardShoot_Create(2.0f, 400.0f, 0.6f, 1500.0f, 1, 250.0f, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER)));


    Entity_AttachComponent(enemy, BarComponent_Create(Vector2_Zero()));
  }
}

static void SpawnAsteroids(void)
{
  for(int i = 0; i < 300; i++)
  {
    Entity *asteroid = Entity_Create("Asteroid");
    Entity_AttachComponent(asteroid, TransformComponent_Create());
    MeshGroup *meshGroup = MeshGroup_Create();
    Vector2 polygonalMesh[] = { { 0, 40 },{ 35, 35 },{ 25, -35 },{ -25, -35 }, {-35, 35} };
    //{ {-40, -40}, {40, -40},{40, 40},{-40,40} };
    /*MeshGroup_AddTriangle(meshGroup, -40, -40, 40, -40, 40, 40, 0xFFFFFF00);
    MeshGroup_AddTriangle(meshGroup, -40, 40, -40, 40, 40, 40, 0xFFFFFF00);*/
    for (int j = 0; j < 5; j++)
    {
      MeshGroup_AddTriangle(meshGroup, polygonalMesh[j].x, polygonalMesh[j].y, 0, 0, polygonalMesh[(j + 1) % 5].x, polygonalMesh[(j + 1) % 5].y, 0xFF804000);
    }
    //MeshGroup_AddRect(meshGroup, 40, 40, 0xFF804000);
    Entity_AttachComponent(asteroid, RenderComponent_CreateMeshGroup(meshGroup));
    Vector2 halfSize = { 20,20 };
    Component *colCom = ColliderComponent_CreatePolygon(polygonalMesh, 5, false);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_DEFAULT);
    Entity_AttachComponent(asteroid, colCom);

    float scale = RandGen_Float() * 3;
    Component *rigidComponent = RigidbodyComponent_Create(300.0f * scale, 300000.0f * scale, 0.95f, 0.25f, false, false);
    Entity_AttachComponent(asteroid, rigidComponent);
    RigidbodyComponent *rigidbody = rigidComponent->data;
    RigidbodyComponent_AddVelocityFloats(rigidbody, RandGen_Int(-50, 50), RandGen_Int(-50, 50));

    TransformComponent_SetScaleFloats(asteroid->transformComponent, scale, scale);

    Vector2 pos = Vector2_FromAngle(RandGen_Float() * 2 * PI);
    float rand = RandGen_Float();
    pos = Vector2_Scale(pos, sqrtf(rand) * (PhysicsManager_GetArenaRadius() / 3 - ColliderComponent_GetBoundingCircleRadius(collider) - 50));
    TransformComponent_SetPosition(asteroid->transformComponent, pos);
  }
}

static void SpawnBox(float x, float y, float width, float height)
{
  Entity *box = Entity_Create("Box");
  
  MeshGroup *group = MeshGroup_Create();
  MeshGroup_AddRect(group, 1.0f, 1.0f, 0xFFFFFFFF);// 0x0FD3D3D3);

  Entity_AttachComponent(box, RenderComponent_CreateMeshGroup(group));
  
  Component *trans = TransformComponent_Create();
  TransformComponent_SetPositionFloats(trans->data,x, y);
  //TransformComponent_SetScaleFloats(trans->data, width, height);

  Entity_AttachComponent(box, trans);
  Vector2 colliderSize;
  colliderSize.x = width/2;
  colliderSize.y = height/2;
  Component *com = ColliderComponent_CreateBox(colliderSize, Vector2_Zero(), false);
  ColliderComponent *col = com->data;
  ColliderComponent_SetRestitution(col, 4.0f);
  Entity_AttachComponent(box, com);
  Entity_AttachComponent(box, RigidbodyComponent_Create(1.0f, 1.0f, 1.0f, 1.0f, true, true));
}