/******************************************************************************/
/*!
\file   ColeTestState.c
\author Evan Kau, Henry Brobeck, Cole Astaire, Ryan Booth
\date   1/23/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "ColeTestState.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "ColliderComponent.h"
#include "Physics_Resolver.h"
#include "Physics_BroadPhase.h"
#include "PhysicsManager.h"
#include "Sound.h"
#include "ShootComponent.h"
#include "InputTest.h"
#include "Camera.h"
#include "EnemyTest.h"
#include "EnemyTestComponent.h"

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
static GameState *cole_instance = NULL;

static Entity *testEntity;
static Component *transformComponent;
static Component *rigidbodyComponent;
static Component *colliderComponent;
static TransformComponent *transData;
static Entity *testEntity2;
static Component *transformComponent2;
static Component *rigidbodyComponent2;
static Component *colliderComponent2;
static Entity *testEntity3;
static Component *transformComponent3;
static Component *colliderComponent3;

static Entity *polygonalEntity;
static Component *transformComponentPolygonal;
static Component *rigidbodyComponentPolygonal;
static Component *colliderComponentPolygonal;
static Vector2 polygonalMesh[4] = { { 50, -50 },{ -50, -50 },{ -25, 60 },{ 25, 50 } };
static Component polygonalMeshRenderer;
static Primitive polygonalPrimitive;

static Entity *polygonalEntity2;
static Component *transformComponentPolygonal2;
static Component *rigidbodyComponentPolygonal2;
static Component *colliderComponentPolygonal2;
static Vector2 polygonalMesh2[4] = { { 50, -45 },{ -50, -65 },{ -25, 60 },{ 50, 50 } };
static Component polygonalMeshRenderer2;
static Primitive polygonalPrimitive2;

static Entity *polygonalEntityGate;
static Component *transformComponentPolygonalGate;
static Component *rigidbodyComponentPolygonalGate;
static Component *colliderComponentPolygonalGate;
static Vector2 polygonalMeshGate[4] = { { 50, -50 },{ -50, -20 },{ -25, 60 },{ 50, 50 } };
static Component polygonalMeshRendererGate;
static Primitive polygonalPrimitiveGate;

static Entity *enemyTestEntity;

static Component *shootComponent;

static int showDebug = 0;
static bool frameFrozen = false;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Load(void);
static void Init(void);
static void Update(float dt);
static void Exit(void);
static void Unload(void);

static void OnGameExit(void);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

GameState* ColeTestState_GetState()
{
  if (cole_instance)
  {
    return cole_instance;
  }

  cole_instance = malloc(sizeof(GameState));
  cole_instance->load = Load;
  cole_instance->init = Init;
  cole_instance->update = Update;
  cole_instance->exit = Exit;
  cole_instance->unload = Unload;

  GameState_RegisterExit(OnGameExit);

  return cole_instance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void OnGameExit(void)
{
  if (cole_instance)
  {
    free(cole_instance);
  }
}

static void Load(void)
{

}

/*!
\brief
*/
static void Init(void)
{
  Vector2 zero = { 0, 0 };
  Vector2 halfSize = { 50, 50 };

  Camera_Init();

  testEntity = Entity_Create("testEntity");
  testEntity2 = Entity_Create("testEntity2");
  testEntity3 = Entity_Create("testEntity3");

  transformComponent = TransformComponent_Create();
  transformComponent2 = TransformComponent_Create();
  transformComponent3 = TransformComponent_Create();

  transData = (TransformComponent*)transformComponent->data;

  Entity_AttachComponent(testEntity, transformComponent);
  Entity_AttachComponent(testEntity2, transformComponent2);
  Entity_AttachComponent(testEntity3, transformComponent3);

  rigidbodyComponent = RigidbodyComponent_Create(10 * 1 * 1 * 1, 300, 0.25f, 0.75f, false, false);
  rigidbodyComponent2 = RigidbodyComponent_Create(10, 300, 0.55f, 0.95f, false, false);

  Entity_AttachComponent(testEntity, rigidbodyComponent);
  Entity_AttachComponent(testEntity2, rigidbodyComponent2);

  colliderComponent = ColliderComponent_CreateBox(halfSize, zero, false);
  colliderComponent2 = ColliderComponent_CreateBox(halfSize, zero, false);
  colliderComponent3 = ColliderComponent_CreateBox(halfSize, zero, false);

  Entity_AttachComponent(testEntity, colliderComponent);
  ColliderComponent_SetLayer(colliderComponent->data, PHYSICS_LAYER_PLAYER);
  Entity_AttachComponent(testEntity2, colliderComponent2);
  ColliderComponent_SetLayer(colliderComponent2->data, PHYSICS_LAYER_PLAYER);
  Entity_AttachComponent(testEntity3, colliderComponent3);
  ColliderComponent_SetLayer(colliderComponent3->data, PHYSICS_LAYER_ENEMY);

  TransformComponent_SetPositionFloats((TransformComponent*)transformComponent->data, 100, 100);
  TransformComponent_SetPositionFloats((TransformComponent*)transformComponent2->data, -200, 0);
  TransformComponent_SetPositionFloats((TransformComponent*)transformComponent3->data, 0, -200);

  Entity_AttachComponent(testEntity, RenderComponent_CreatePrimitive(Primitive_CreateRect(100, 100, 0xFFFF0000)));
  Entity_AttachComponent(testEntity2, RenderComponent_CreatePrimitive(Primitive_CreateRect(100, 100, 0xFF00FF00)));
  Entity_AttachComponent(testEntity3, RenderComponent_CreatePrimitive(Primitive_CreateRect(100, 100, 0xFF0000FF)));


  polygonalEntity = Entity_Create("polygonalEntity");

  transformComponentPolygonal = TransformComponent_Create();
  Entity_AttachComponent(polygonalEntity, transformComponentPolygonal);

  rigidbodyComponentPolygonal = RigidbodyComponent_Create(10, 300, 0.95f, 0.95f, true, false);
  Entity_AttachComponent(polygonalEntity, rigidbodyComponentPolygonal);

  colliderComponentPolygonal = ColliderComponent_CreatePolygon(polygonalMesh, 4, false);
  Entity_AttachComponent(polygonalEntity, colliderComponentPolygonal);

  enemyTestEntity = EnemyTest_Create(500, -500, 0, 400, 20);

  AEGfxMeshStart();

  // This shape has 2 triangles
  AEGfxTriAdd(
    polygonalMesh[3].x, polygonalMesh[3].y, 0xFF8800FF, 0.0f, 1.0f,
    polygonalMesh[2].x, polygonalMesh[2].y, 0xFF8800FF, 1.0f, 1.0f,
    polygonalMesh[1].x, polygonalMesh[1].y, 0xFF8800FF, 0.0f, 0.0f);

  AEGfxTriAdd(
    polygonalMesh[0].x, polygonalMesh[0].y, 0xFF8800FF, 0.0f, 1.0f,
    polygonalMesh[3].x, polygonalMesh[3].y, 0xFF8800FF, 1.0f, 1.0f,
    polygonalMesh[1].x, polygonalMesh[1].y, 0xFF8800FF, 0.0f, 0.0f);

  polygonalPrimitive.mesh = AEGfxMeshEnd();

  Entity_AttachComponent(polygonalEntity, RenderComponent_CreatePrimitive(&polygonalPrimitive));

  TransformComponent_SetPositionFloats((TransformComponent*)transformComponentPolygonal->data, 0, 200);


  polygonalEntity2 = Entity_Create("polygonalEntity2");

  transformComponentPolygonal2 = TransformComponent_Create();
  Entity_AttachComponent(polygonalEntity2, transformComponentPolygonal2);

  rigidbodyComponentPolygonal2 = RigidbodyComponent_Create(10, 300, 0.95f, 0.95f, false, false);
  Entity_AttachComponent(polygonalEntity2, rigidbodyComponentPolygonal2);

  colliderComponentPolygonal2 = ColliderComponent_CreatePolygon(polygonalMesh2, 4, false);
  Entity_AttachComponent(polygonalEntity2, colliderComponentPolygonal2);

  AEGfxMeshStart();

  // This shape has 2 triangles
  AEGfxTriAdd(
    polygonalMesh2[3].x, polygonalMesh2[3].y, 0xFFFF0088, 0.0f, 1.0f,
    polygonalMesh2[2].x, polygonalMesh2[2].y, 0xFFFF0088, 1.0f, 1.0f,
    polygonalMesh2[1].x, polygonalMesh2[1].y, 0xFFFF0088, 0.0f, 0.0f);

  AEGfxTriAdd(
    polygonalMesh2[0].x, polygonalMesh2[0].y, 0xFFFF0088, 0.0f, 1.0f,
    polygonalMesh2[3].x, polygonalMesh2[3].y, 0xFFFF0088, 1.0f, 1.0f,
    polygonalMesh2[1].x, polygonalMesh2[1].y, 0xFFFF0088, 0.0f, 0.0f);

  polygonalPrimitive2.mesh = AEGfxMeshEnd();

  Entity_AttachComponent(polygonalEntity2, RenderComponent_CreatePrimitive(&polygonalPrimitive2));

  TransformComponent_SetPositionFloats((TransformComponent*)transformComponentPolygonal2->data, -200, 250);

  polygonalEntityGate = Entity_Create("polygonalEntityGate");

  transformComponentPolygonalGate = TransformComponent_Create();
  Entity_AttachComponent(polygonalEntityGate, transformComponentPolygonalGate);

  rigidbodyComponentPolygonalGate = RigidbodyComponent_Create(10, 300, 0.95f, 0.95f, false, false);
  Entity_AttachComponent(polygonalEntityGate, rigidbodyComponentPolygonalGate);

  colliderComponentPolygonalGate = ColliderComponent_CreatePolygon(polygonalMeshGate, 4, false);
  Entity_AttachComponent(polygonalEntityGate, colliderComponentPolygonalGate);

  ColliderComponent_SetLayer(colliderComponentPolygonalGate->data, PHYSICS_LAYER_PLAYER);

  AEGfxMeshStart();

  // This shape has 2 triangles
  AEGfxTriAdd(
      polygonalMeshGate[3].x, polygonalMeshGate[3].y, 0xFFFFFF88, 0.0f, 1.0f,
      polygonalMeshGate[2].x, polygonalMeshGate[2].y, 0xFFFFFF88, 1.0f, 1.0f,
      polygonalMeshGate[1].x, polygonalMeshGate[1].y, 0xFFFFFF88, 0.0f, 0.0f);

  AEGfxTriAdd(
      polygonalMeshGate[0].x, polygonalMeshGate[0].y, 0xFFFFFF88, 0.0f, 1.0f,
      polygonalMeshGate[3].x, polygonalMeshGate[3].y, 0xFFFFFF88, 1.0f, 1.0f,
      polygonalMeshGate[1].x, polygonalMeshGate[1].y, 0xFFFFFF88, 0.0f, 0.0f);

  polygonalPrimitiveGate.mesh = AEGfxMeshEnd();

  Entity_AttachComponent(polygonalEntityGate, RenderComponent_CreatePrimitive(&polygonalPrimitiveGate));

  TransformComponent_SetPositionFloats((TransformComponent*)transformComponentPolygonalGate->data, 200, -250);

  MeshGroup *bullet = MeshGroup_Create();
  MeshGroup_AddRect(bullet, 20, 20, 0xFFFFFFFF);
  shootComponent = ShootComponent_Create(bullet, PHYSICS_LAYER_DEFAULT);
  Entity_AttachComponent(testEntity, shootComponent);

  Camera_TrackEntity(testEntity);
  Camera_TrackEntity(testEntity2);
  Camera_TrackEntity(enemyTestEntity);
}

/*!
\brief
\param	dt Change in time (in seconds) since last game loop.
*/
static void Update(float dt)
{
  Vector2 force = Vector2_Zero();
  Vector2 tempPosition2;

  if (dt > 0.05f)
    dt = 0.05f;

  if (AEInputCheckCurr('X'))
    dt /= 5;
   
  if (AEInputCheckCurr('U'))
    Camera_AddTargetSize(0.5f * dt);
  if (AEInputCheckCurr('O'))
    Camera_AddTargetSize(-0.5f * dt);
  if (AEInputCheckCurr('J'))
    Camera_AddTargetPosition(-200 * dt, 0);
  if (AEInputCheckCurr('L'))
    Camera_AddTargetPosition(200 * dt, 0);
  if (AEInputCheckCurr('I'))
    Camera_AddTargetPosition(0, 200 * dt);
  if (AEInputCheckCurr('K'))
    Camera_AddTargetPosition(0, -200 * dt);

  //tempPosition = TransformComponent_GetPosition((TransformComponent*)transformComponent->data);
  tempPosition2 = TransformComponent_GetPosition((TransformComponent*)transformComponent2->data);

  float rotation = TransformComponent_GetRotation(transData);
  Vector2 scale = TransformComponent_GetScale(transData);

  Test_Keyboard_Controls_Update(&force, &scale, rotation, dt);
  Test_Pad_Controls_Update(shootComponent, &force, &scale, &rotation, dt);
                
  if (AEInputCheckCurr(VK_SPACE))      
     {                                   
     if (AEInputCheckTriggered(VK_SPACE))
         {
           ShootComponent_Shoot(shootComponent);
         }
      
     }

  if (AEInputCheckCurr('V'))
  {
    GameState_Restart();
  }

  if (AEInputCheckTriggered(VK_RETURN))
  {
    showDebug = !showDebug;
  }

  if (AEInputCheckCurr(VK_UP))
  {
    tempPosition2.y += 100.0f * dt;
  }
  if (AEInputCheckCurr(VK_LEFT))
  {
    tempPosition2.x -= 100.0f * dt;
  }
  if (AEInputCheckCurr(VK_DOWN))
  {
    tempPosition2.y -= 100.0f * dt;
  }
  if (AEInputCheckCurr(VK_RIGHT))
  {
    tempPosition2.x += 100.0f * dt;
  }

  //RigidbodyComponent_AddForceFloats(rigidbodyComponent->data, -50, 0);

  RigidbodyComponent_AddForce(rigidbodyComponent->data, force, FORCE_TYPE_FORCE);

  TransformComponent_SetPosition((TransformComponent*)transformComponent2->data, tempPosition2);

  TransformComponent_SetRotation(transData, rotation);
  TransformComponent_SetScale(transData, scale);

  EnemyTestComponent_SetTargetPosition(Entity_FindComponentByType(enemyTestEntity, COMPONENT_TYPE_ENEMYTEST)->data, TransformComponent_GetPosition(transformComponent->data));

  PhysicsManager_Update(dt);

  if (showDebug)
  {
    Physics_Resolver_Debug();
    Physics_BroadPhase_Debug();
  }

  Camera_Update(dt);
}

/*!
\brief
*/
static void Exit(void)
{
  Camera_UntrackEntity(testEntity);
  Camera_UntrackEntity(testEntity2);
  //Camera_UntrackEntity(enemyTestEntity);
  Entity_SetFlag(testEntity, EFLAG_DESTROY, 1 );
  Entity_SetFlag(testEntity, EFLAG_DESTROY, 1);
  Entity_SetFlag(testEntity3, EFLAG_DESTROY, 1);
  Entity_SetFlag(polygonalEntity, EFLAG_DESTROY, 1);
  Physics_BroadPhase_SanitizeGrid();
}
static void Unload(void)
{

}