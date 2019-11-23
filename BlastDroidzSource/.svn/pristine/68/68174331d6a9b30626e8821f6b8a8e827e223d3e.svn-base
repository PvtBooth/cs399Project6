/******************************************************************************/
/*!
	\file   PhysicsTestState.c
	\author Evan Kau, Henry Brobeck, Cole Astaire
	\date   1/23/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PhysicsTestState.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "ColliderComponent.h"
#include "Physics_Resolver.h"
#include "Physics_BroadPhase.h"
#include "PhysicsManager.h"
#include "Sound.h"
#include "ShootComponent.h"
#include "Camera.h"

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
static GameState *instance = NULL;

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



GameState* PhysicsTestState_GetState()
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

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void OnGameExit(void)
{
  if(instance)
  {
    free(instance);
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
  Camera_Init();
  Camera_SetTracking(false);
  Camera_SetTargetPosition(0, 0);
  Camera_SetTargetSize(1);
  Vector2 zero = { 0, 0 };
  Vector2 halfSize = { 50, 50 };
  PhysicsManager_NewGridFromCount(50000, 50000, 100, 100);

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

  rigidbodyComponent = RigidbodyComponent_Create(1000, 100 * 100 * 100 / 12, 0.95f, 0.95f, false, false);
  rigidbodyComponent2 = RigidbodyComponent_Create(1000, 100 * 100 * 100 / 12, 0.95f, 0.95f, false, false);

  Entity_AttachComponent(testEntity, rigidbodyComponent);
  Entity_AttachComponent(testEntity2, rigidbodyComponent2);

  colliderComponent = ColliderComponent_CreateBox(halfSize, zero, false);
  colliderComponent2 = ColliderComponent_CreateBox(halfSize, zero, false);
  colliderComponent3 = ColliderComponent_CreateBox(halfSize, zero, false);

  Entity_AttachComponent(testEntity, colliderComponent);
  Entity_AttachComponent(testEntity2, colliderComponent2);
  Entity_AttachComponent(testEntity3, colliderComponent3);

  TransformComponent_SetPositionFloats((TransformComponent*)transformComponent->data, 100, 100);
  TransformComponent_SetPositionFloats((TransformComponent*)transformComponent2->data, -200, 0);
  TransformComponent_SetPositionFloats((TransformComponent*)transformComponent3->data, 0, -200);

  MeshGroup *testEntMeshGroup = MeshGroup_Create();
  MeshGroup_AddRect(testEntMeshGroup, 100, 100, 0xFFFF0000);

  //Entity_AttachComponent(testEntity, RenderComponent_CreateMeshGroup(testEntMeshGroup));
  Entity_AttachComponent(testEntity2, RenderComponent_CreatePrimitive(Primitive_CreateRect(100, 100, 0xFF00FF00)));
  Entity_AttachComponent(testEntity3, RenderComponent_CreatePrimitive(Primitive_CreateRect(100, 100, 0xFF0000FF)));


  polygonalEntity = Entity_Create("polygonalEntity");

  transformComponentPolygonal = TransformComponent_Create();
  Entity_AttachComponent(polygonalEntity, transformComponentPolygonal);

  rigidbodyComponentPolygonal = RigidbodyComponent_Create(10, 300, 0.95f, 0.95f, false, false);
  Entity_AttachComponent(polygonalEntity, rigidbodyComponentPolygonal);

  colliderComponentPolygonal = ColliderComponent_CreatePolygon(polygonalMesh, 4, false);
  Entity_AttachComponent(polygonalEntity, colliderComponentPolygonal);

  MeshGroup *mg = MeshGroup_Create();
  MeshGroup_AddTriangle(mg, polygonalMesh[3].x, polygonalMesh[3].y, polygonalMesh[2].x, polygonalMesh[2].y, polygonalMesh[1].x, polygonalMesh[1].y, 0xFF8800FF);
  MeshGroup_AddTriangle(mg, polygonalMesh[0].x, polygonalMesh[0].y, polygonalMesh[3].x, polygonalMesh[3].y, polygonalMesh[1].x, polygonalMesh[1].y, 0xFF8800FF);

//  Entity_AttachComponent(polygonalEntity, RenderComponent_CreateMeshGroup(mg));
    
  TransformComponent_SetPositionFloats((TransformComponent*)transformComponentPolygonal->data, 0, 200);


  polygonalEntity2 = Entity_Create("polygonalEntity2");

  transformComponentPolygonal2 = TransformComponent_Create();
  Entity_AttachComponent(polygonalEntity2, transformComponentPolygonal2);

  rigidbodyComponentPolygonal2 = RigidbodyComponent_Create(10, 300, 0.95f, 0.95f, false, false);
  Entity_AttachComponent(polygonalEntity2, rigidbodyComponentPolygonal2);

  colliderComponentPolygonal2 = ColliderComponent_CreatePolygon(polygonalMesh2, 4, false);
  Entity_AttachComponent(polygonalEntity2, colliderComponentPolygonal2);



//  MeshGroup *mg2 = MeshGroup_Create();
  MeshGroup_AddTriangle(mg, polygonalMesh2[3].x, polygonalMesh2[3].y, polygonalMesh2[2].x, polygonalMesh2[2].y, polygonalMesh2[1].x, polygonalMesh2[1].y, 0xFF8800FF);
  MeshGroup_AddTriangle(mg, polygonalMesh2[0].x, polygonalMesh2[0].y, polygonalMesh2[3].x, polygonalMesh2[3].y, polygonalMesh2[1].x, polygonalMesh2[1].y, 0xFF8800FF);

//  Entity_AttachComponent(polygonalEntity2, RenderComponent_CreateMeshGroup(mg2));

  TransformComponent_SetPositionFloats((TransformComponent*)transformComponentPolygonal2->data, -200, 200);
}

/*!
\brief
\param	dt Change in time (in seconds) since last game loop.
*/
static void Update(float dt)
{
  RaycastData raycastData = {0};
  Vector2 rayStart = {0, 0};
  Vector2 rayDirection = {1, 1};
  s32 mouseX, mouseY;
  Vector2 mousePos;
  Vector2 force = Vector2_Zero();
  Vector2 tempPosition2;

  AEInputGetCursorPosition(&mouseX, &mouseY);
  AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &mousePos.x, &mousePos.y);
  mousePos = Camera_MouseToWorld();
  mousePos = TransformComponent_GetPosition(transformComponent->data);
  rayDirection = Vector2_Sub(mousePos, rayStart);

  if (dt > 0.1f)
    dt = 0.1f;

  if (AEInputCheckCurr('X'))
    dt /= 5;

  //tempPosition = TransformComponent_GetPosition((TransformComponent*)transformComponent->data);
  tempPosition2 = TransformComponent_GetPosition((TransformComponent*)transformComponent2->data);


  float rotation = TransformComponent_GetRotation(transData);
  Vector2 scale = TransformComponent_GetScale(transData);


  if (AEInputCheckCurr(VK_ESCAPE))
  {
    Game_SetRunning(0);
  }
  if (AEInputCheckCurr('W'))
  {
    force.y += 100000.0f;
  }
  if (AEInputCheckCurr('A'))
  {
    force.x -= 100000.0f;
  }
  if (AEInputCheckCurr('S'))
  {
    force.y -= 100000.0f;
  }
  if (AEInputCheckCurr('D'))
  {
    force.x += 100000.0f;
  }
  if (AEInputCheckCurr('E'))
  {
    rotation -= 0.25f * dt;
  }
  if (AEInputCheckCurr('Q'))
  {
    rotation += 0.25f * dt;
  }
  if (AEInputCheckCurr('R'))
  {
    scale = Vector2_AddFloats(scale, 0.1f, 0.1f);
  }
  if (AEInputCheckCurr('F'))
  {
    scale = Vector2_AddFloats(scale, -0.1f, -0.1f);
  }
  if (AEInputCheckCurr('V'))
  {
    GameState_Restart();
  }

  /*           Sound Test Keys               */
  /**/if (AEInputCheckCurr(VK_SPACE))      /**/
    /**/ {                                    /**/
    /**/ if (AEInputCheckTriggered(VK_SPACE)) /**/
      /**/   Sound_Play(0);                /**/
                                               /**/
  }                                    /**/
  /**/if (AEInputCheckCurr('H'))           /**/
    /**/ {                                    /**/
    /**/ if (AEInputCheckTriggered('H'))     /**/
      /**/   Sound_Play(1);                  /**/
                                               /**/
  }                                    /**/
  /**/if (AEInputCheckCurr('P'))           /**/
    /**/ {                                    /**/
    /**/ if (AEInputCheckTriggered('P'))     /**/
      /**/   Sound_Play(2);              /**/
                                               /**/
  }                                    /**/
  /**/if (AEInputCheckCurr('T'))           /**/
    /**/ {                                    /**/
    /**/  if (AEInputCheckTriggered('T'))    /**/
      /**/   Sound_Play(3);            /**/
                                               /**/
  }                                    /**/
                                       /*           Sound Test Keys               */

  if (AEInputCheckCurr(VK_UP))
  {
    tempPosition2.y += 1000.0f * dt;
  }
  if (AEInputCheckCurr(VK_LEFT))
  {
    tempPosition2.x -= 1000.0f * dt;
  }
  if (AEInputCheckCurr(VK_DOWN))
  {
    tempPosition2.y -= 1000.0f * dt;
  }
  if (AEInputCheckCurr(VK_RIGHT))
  {
    tempPosition2.x += 1000.0f * dt;
  }

  //RigidbodyComponent_AddForceFloats(rigidbodyComponent->data, -50, 0);

  RigidbodyComponent_AddForce(rigidbodyComponent->data, force, FORCE_TYPE_FORCE);

  TransformComponent_SetPosition((TransformComponent*)transformComponent2->data, tempPosition2);

  TransformComponent_SetRotation(transData, rotation);
  TransformComponent_SetScale(transData, scale);

  PhysicsManager_Update(dt);

  Physics_BroadPhase_Debug();
  Physics_Resolver_Debug();

  //if (PhysicsManager_Raycast(&raycastData, &rayStart, &rayDirection, 600))
  {
    PhysicsManager_Raycast(&raycastData, rayStart, rayDirection, Vector2_Length(rayDirection));
    AEGfxSetPosition(0, 0);
    AEGfxLine(raycastData.origin.x, raycastData.origin.y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, raycastData.point.x, raycastData.point.y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    AEGfxLine(raycastData.point.x, raycastData.point.y, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, raycastData.point.x + raycastData.normal.x * 25.0f, raycastData.point.y + raycastData.normal.y * 25.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
  }
}

/*!
\brief
*/
static void Exit(void)
{
  Entity_SetFlag(testEntity, EFLAG_DESTROY, 1);
  //Entity_Destroy(testEntity2);
  //Entity_Destroy(testEntity3);
  Entity_SetFlag(polygonalEntity, EFLAG_DESTROY, 1);
  
}
static void Unload(void)
{
  
}