/******************************************************************************/
/*!
	\file   TestStackState.c
	\author Henry Brobeck
	\date   3/6/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "TestStackState.h"
#include "StackState.h"
#include "RigidbodyComponent.h"
#include "RenderComponent.h"
#include "RandGen.h"
#include "HealthComponent.h"
#include "AISwarm.h"
#include "Enemy_SimpleComponent.h"
#include "AIForwardShoot.h"
#include "GameState.h"
#include "Camera.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "AIManager.h"
#include "Physics_BroadPhase.h"
#include "GameStateStack.h"
#include "EntityManager.h"
#include "PausedStackState.h"
#include "BarComponent.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct TestStackState TestStackState;
struct TestStackState
{
  StackState stackState;

  TextureAsset *textureTest;
  Sprite *spriteTest;
  Entity *entityTest;


  bool showPhysicsDebug;

  bool gameplayIsRunning;
};
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static Vector2 playerCollider[3] = { { 25, 0 },{ -25, -25 },{ -25, 25 }, };

static Vector2 asteroidCollider[4] = { { -40, -40 },{ 40, -40 },{ 40, 40 },{ -40,40 } };


/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void Init(StackState *self);
static void Update(StackState *self, float dt);
static void Render(StackState *self);
static void Exit(StackState *self);
static bool InputHandle(StackState *self, InputEvent *inputEvent);

static void SpawnEnemies(void);
static void SpawnAsteroids(void);
static void SpawnBox(float x, float y, float width, float height);


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

StackState *TestStackState_Create()
{
  TestStackState *state = calloc(1, sizeof(TestStackState));
  state->stackState.init = Init;
  state->stackState.update = Update;
  state->stackState.exit = Exit;
  state->stackState.inputHandle = InputHandle;
  state->stackState.render = Render;
  state->stackState.renderBelow = false;
  state->stackState.updateBelow = false;

  state->gameplayIsRunning = true;

  return &state->stackState;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Init(StackState *self)
{
  TestStackState *state = (TestStackState*)self;
  self->entityManager = EntityManager_Create(self);
  self->graphicsManager = GraphicsManager_Create(self);
  
  PhysicsManager_NewGridFromCount(50000, 50000, 100, 100);
  Camera_Init();
  Camera_SetTracking(false);
  Camera_SetTargetPosition(0, 0);
  Camera_SetTargetSize(50);
  Player_Init(self);
  Vector2 position = { 1000, 1000 };
  Player_Type1_Create(0, position);
  //SpawnEnemies();
//  /SpawnAsteroids();
  //Camera paralax test
  //Camera_UntrackEntity(Player_GetPlayer(1)->playerEntity);
  //Camera_UntrackEntity(Player_GetPlayer(2)->playerEntity);
  //Camera_UntrackEntity(Player_GetPlayer(3)->playerEntity);

  state->showPhysicsDebug = false;

  
  
  //state->textureTest = Texture_Load("Assets/30secondsinphotoshop.png");
  state->textureTest = AssetTable_TryLoadTextureAsset("Background", "Assets/30secondsinphotoshop.png");
  assert(state->textureTest);
  state->spriteTest = Sprite_Create(state->textureTest);
  state->entityTest = EntityManager_CreateEntity(self->entityManager, "Background");
  Component *transformComponent = TransformComponent_Create();
  TransformComponent *tComp = transformComponent->data;
  Vector2 scale;
  scale.x = 6000;
  scale.y = 6000;
  TransformComponent_SetScale(tComp, scale);


  Entity_AttachComponent(state->entityTest, transformComponent);
  Component *paralaxRenderComponent = RenderComponent_CreateSprite(state->spriteTest, RENDER_BEHAVIOR_PARALAX, 0);
  Vector2 paralax = { 0.0001f, -0.0001f };
  RenderComponent_SetParalax(paralaxRenderComponent, paralax);
  Entity_AttachComponent(state->entityTest, paralaxRenderComponent);

  // Particle test
  //Vector2 scaleparticle = { 10.0f, 10.0f };
  //Vector2 velocityparticle = { 1.0f, 0.0f };
  //Vector2 accelerationparticle = { 0.0f, 0.0f };
  //Component *particleComponent = ParticleSystem_Create(100, 100, 0.2f, 0.6f, 0.8f, 1.0f, 1, scaleparticle, velocityparticle,
    //accelerationparticle, 1.0f, 1.0f, RENDER_LAYER_PARTICLE);
  

  //Entity_AttachComponent(Player_GetPlayer(0)->playerEntity, particleComponent);


  for (int i = 0; i < 25; i++)
  {
    Entity *enemy = EntityManager_CreateEntity(self->entityManager, "Enemy1");
    Entity_AttachComponent(enemy, TransformComponent_Create());
    MeshGroup *meshGroup = MeshGroup_Create();

    MeshGroup_AddTriangle(meshGroup, -25, 25, -25, -25, 25, 0, 0xFFFF0000);
    
//    Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup));
    Component *colCom = ColliderComponent_CreatePolygon(playerCollider, 3, 0);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
    Entity_AttachComponent(enemy, colCom);

    Entity_AttachComponent(enemy, RigidbodyComponent_Create(5.0f, 2000.0f, 0.05f, 0.25f, false, false));

    //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

    TransformComponent_SetPositionFloats(enemy->transformComponent, (float)RandGen_Int(50, 150), (float)RandGen_Int(50, 150));

    Entity_AttachComponent(enemy, HealthComponent_Create(3, 0));

    Entity_AttachComponent(enemy, AISwarmAgent_Create(1, 750));

    Entity_AttachComponent(enemy, Enemy_SimpleComponent_Create());

    //Entity_AttachComponent(enemy, AIForwardShoot_Create(2, 400, 0.6f, 1500, 1, 250, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER)));
    Component *component = BarComponent_Create(Vector2_Zero());

    //Vector2 scale2 = { 100, 10 };
    //HUDBar *bar = BarComponent_CreateBar(component, 100, scale2, 10, 1.0, 0.5, 0.5, 1.0);
    //HUDBar *bar2 = BarComponent_CreateBar(component, 1000, scale2, 5, 0, 0, 1, 1);

    //BarComponent_SetBarType(bar2, HUDBAR_TYPE_RIGHT);

    //HUDBar *bar3 = BarComponent_CreateBar(component, 1, scale2, 0, 0, 0.9, 0, 1);

    //BarComponent_SetBarType(bar3, HUDBAR_TYPE_CENTER);


    Entity_AttachComponent(enemy, component);
  }

  for (int i = 0; i < 300; i++)
  {
    Entity *asteroid = EntityManager_CreateEntity(self->entityManager, "Asteroid");
    Entity_AttachComponent(asteroid, TransformComponent_Create());
    MeshGroup *meshGroup = MeshGroup_Create();
    Vector2 polygonalMesh[] = { { 0, 40 },{ 35, 35 },{ 25, -35 },{ -25, -35 },{ -35, 35 } };
    //{ {-40, -40}, {40, -40},{40, 40},{-40,40} };
    /*MeshGroup_AddTriangle(meshGroup, -40, -40, 40, -40, 40, 40, 0xFFFFFF00);
    MeshGroup_AddTriangle(meshGroup, -40, 40, -40, 40, 40, 40, 0xFFFFFF00);*/
    for (int j = 0; j < 5; j++)
    {
      MeshGroup_AddTriangle(meshGroup, polygonalMesh[j].x, polygonalMesh[j].y, 0, 0, polygonalMesh[(j + 1) % 5].x, polygonalMesh[(j + 1) % 5].y, 0xFF804000);
    }
    //MeshGroup_AddRect(meshGroup, 40, 40, 0xFF804000);
//    Entity_AttachComponent(asteroid, RenderComponent_CreateMeshGroup(meshGroup));
    //Vector2 halfSize = { 20,20 };
    Component *colCom = ColliderComponent_CreatePolygon(polygonalMesh, 5, false);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_DEFAULT);
    Entity_AttachComponent(asteroid, colCom);

    Component *rigidComponent = RigidbodyComponent_Create(10000.0f, 10000000.0f, 0.95f, 0.25f, false, false);
    Entity_AttachComponent(asteroid, rigidComponent);
    RigidbodyComponent *rigidbody = rigidComponent->data;
    RigidbodyComponent_AddVelocityFloats(rigidbody, (float)RandGen_Int(-50, 50), (float)RandGen_Int(-50, 50));

    float asteroidScale = RandGen_Float() * 50;
    TransformComponent_SetScaleFloats(asteroid->transformComponent, asteroidScale, asteroidScale);

    Vector2 pos = Vector2_FromAngle(RandGen_Float() * 2 * PI);
    float rand = RandGen_Float();
    pos = Vector2_Scale(pos, sqrtf(rand) * (PhysicsManager_GetArenaRadius() - ColliderComponent_GetBoundingCircleRadius(collider) - 50));
    TransformComponent_SetPosition(asteroid->transformComponent, pos);
  }

  
  //EntityManager_DestroyAllSafe(state->entityManager);
  //spawn level bounds
  //SpawnBox(-2900, 0, 400, 6500);
  //SpawnBox(2900, 0, 400, 6500);
  //SpawnBox(0, -2900, 6500, 400);
  //SpawnBox(0, 2900, 6500, 400);

  
}
static void Update(StackState *self, float dt)
{
  TestStackState *state = (TestStackState*)self;

  if (!state->gameplayIsRunning)
  {
      GameStateStack_PopState();
  }

  const ColliderComponent **colliders;
  int hit;
  int i;
  //Entity_Update(dt);
  Camera_Update(dt);
  PhysicsManager_Update(dt);
  EntityManager_Update(self->entityManager, dt);

  //Vector2 mouse = Camera_MouseToWorld();

  //Camera_DrawLine(0, 0, 1, 1, 1, 1, mouse.x, mouse.y, 1, 1, 1, 1, true);

  //Cast to get the colliders within the box
  colliders = PhysicsManager_BoxCast(((Vector2) { 1000, 0 }), ((Vector2) { 300, 300 }), &hit);
  //For each hit,
  for (i = 0; i < hit; i++)
  {
    //Get the transform
    //TransformComponent *trans = ColliderComponent_GetTransform(colliders[i]);
    //Set the position to (0,0)
    //TransformComponent_SetPosition(trans, Vector2_Zero());
  }

  if (state->showPhysicsDebug)
  {
    //Physics_BroadPhase_Debug();
    AIManager_Debug();
  }

  if (AEInputCheckCurr(VK_CONTROL))
    Physics_BroadPhase_Debug();
  if (AEInputCheckTriggered('P'))
    state->showPhysicsDebug = !state->showPhysicsDebug;

  if (AEInputCheckTriggered(VK_RETURN))
  {
    GameStateStack_PushState(PausedStackState_Create());
  }


  if (AEInputCheckTriggered('Q'))
  {
    Camera_SetTracking(true);
  }

  if (AEInputCheckTriggered('V'))
  {
    //SpawnEnemies();
  }

  if (!AIManager_GetNearestSwarm(Vector2_Zero()))
  {

  }
}
static void Render(StackState *self)
{
  GraphicsManager_Render(self->graphicsManager);
}
static void Exit(StackState *self)
{
    UNREFERENCED_PARAMETER(self);
}
static bool InputHandle(StackState *self, InputEvent *inputEvent)
{
    UNREFERENCED_PARAMETER(self);
  if(inputEvent->inputType == INPUT_TYPE_BUTTON_B)
  {
    //Memes
    inputEvent = inputEvent;
  }

  return false;
}

static void SpawnEnemies(void)
{
  for (int i = 0; i < 25; i++)
  {
    Entity *enemy = Entity_Create("Enemy");
    Entity_AttachComponent(enemy, TransformComponent_Create());
    MeshGroup *meshGroup = MeshGroup_Create();

    MeshGroup_AddTriangle(meshGroup, -25, 25, -25, -25, 25, 0, 0xFFFF0000);

//    Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup));
    Component *colCom = ColliderComponent_CreatePolygon(playerCollider, 3, 0);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_ENEMY);
    Entity_AttachComponent(enemy, colCom);

    Entity_AttachComponent(enemy, RigidbodyComponent_Create(5.0f, 2000.0f, 0.05f, 0.25f, false, false));

    //Entity_AttachComponent(enemy, SimpleAIComponent_Create());

    TransformComponent_SetPositionFloats(enemy->transformComponent, (float)RandGen_Int(50, 150), (float)RandGen_Int(50, 150));

    Entity_AttachComponent(enemy, HealthComponent_Create(3, 0));

    Entity_AttachComponent(enemy, AISwarmAgent_Create(1, 750));

    Entity_AttachComponent(enemy, Enemy_SimpleComponent_Create());

//    Entity_AttachComponent(enemy, AIForwardShoot_Create(2, 400, 0.6f, 1500, 1, 250, Physics_Layers_CreateLayerMask(1, PHYSICS_LAYER_PLAYER)));
  }
}

static void SpawnAsteroids(void)
{
  for (int i = 0; i < 300; i++)
  {
    Entity *asteroid = Entity_Create("Asteroid");
    Entity_AttachComponent(asteroid, TransformComponent_Create());
    MeshGroup *meshGroup = MeshGroup_Create();
    Vector2 polygonalMesh[] = { { 0, 40 },{ 35, 35 },{ 25, -35 },{ -25, -35 },{ -35, 35 } };
    //{ {-40, -40}, {40, -40},{40, 40},{-40,40} };
    /*MeshGroup_AddTriangle(meshGroup, -40, -40, 40, -40, 40, 40, 0xFFFFFF00);
    MeshGroup_AddTriangle(meshGroup, -40, 40, -40, 40, 40, 40, 0xFFFFFF00);*/
    for (int j = 0; j < 5; j++)
    {
      MeshGroup_AddTriangle(meshGroup, polygonalMesh[j].x, polygonalMesh[j].y, 0, 0, polygonalMesh[(j + 1) % 5].x, polygonalMesh[(j + 1) % 5].y, 0xFF804000);
    }
    //MeshGroup_AddRect(meshGroup, 40, 40, 0xFF804000);
//    Entity_AttachComponent(asteroid, RenderComponent_CreateMeshGroup(meshGroup));
    //Vector2 halfSize = { 20,20 };
    Component *colCom = ColliderComponent_CreatePolygon(polygonalMesh, 5, false);
    ColliderComponent *collider = colCom->data;
    ColliderComponent_SetLayer(collider, PHYSICS_LAYER_DEFAULT);
    Entity_AttachComponent(asteroid, colCom);

    Component *rigidComponent = RigidbodyComponent_Create(10000.0f, 10000000.0f, 0.95f, 0.25f, false, false);
    Entity_AttachComponent(asteroid, rigidComponent);
    RigidbodyComponent *rigidbody = rigidComponent->data;
    RigidbodyComponent_AddVelocityFloats(rigidbody, (float)RandGen_Int(-50, 50), (float)RandGen_Int(-50, 50));

    float scale = RandGen_Float() * 3;
    TransformComponent_SetScaleFloats(asteroid->transformComponent, scale, scale);

    Vector2 pos = Vector2_FromAngle(RandGen_Float() * 2 * PI);
    float rand = RandGen_Float();
    pos = Vector2_Scale(pos, sqrtf(rand) * (PhysicsManager_GetArenaRadius() - ColliderComponent_GetBoundingCircleRadius(collider) - 50));
    TransformComponent_SetPosition(asteroid->transformComponent, pos);
  }
}

static void SpawnBox(float x, float y, float width, float height)
{
  Entity *box = Entity_Create("Box");

  MeshGroup *group = MeshGroup_Create();
  MeshGroup_AddRect(group, 1, 1, 0xFFFFFFFF);// 0x0FD3D3D3);

//  Entity_AttachComponent(box, RenderComponent_CreateMeshGroup(group));

  Component *trans = TransformComponent_Create();
  TransformComponent_SetPositionFloats(trans->data, x, y);
  //TransformComponent_SetScaleFloats(trans->data, width, height);

  Entity_AttachComponent(box, trans);
  Vector2 colliderSize;
  colliderSize.x = width / 2;
  colliderSize.y = height / 2;
  Component *com = ColliderComponent_CreateBox(colliderSize, Vector2_Zero(), false);
  ColliderComponent *col = com->data;
  ColliderComponent_SetRestitution(col, 4);
  Entity_AttachComponent(box, com);
  Entity_AttachComponent(box, RigidbodyComponent_Create(1, 1, 1, 1, true, true));
}

void TestStackState_SetGameplayBool(StackState* below, bool active)
{
    TestStackState* state = (TestStackState*)below;


    state->gameplayIsRunning = active;
}
