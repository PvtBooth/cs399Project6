/******************************************************************************/
/*!
	\file   EnemyTest.c
	\author Evan Kau
	\date   2/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "EnemyTest.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "EnemyTestComponent.h"
#include "RenderComponent.h"
#include "Log.h"

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

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
Entity* EnemyTest_Create(float x, float y, float rotation, float speed, float rotationSpeed)
{
  UNREFERENCED_PARAMETER(rotation);
  Vector2 halfSize = { 45, 30 };
  Vector2 zero = { 0, 0 };
  Entity *enemy = Entity_Create("enemytest");
  MeshGroup *meshGroup = MeshGroup_Create();
  MeshGroup_AddRect(meshGroup, halfSize.x * 2, halfSize.y * 2, 0XFFFF8800);

  Entity_AttachComponent(enemy, TransformComponent_Create());
  TransformComponent_SetPositionFloats(Entity_GetTransform(enemy), x, y);
  Entity_AttachComponent(enemy, ColliderComponent_CreateBox(halfSize, zero, false));
  ColliderComponent_SetLayer(Entity_FindComponentByType(enemy, COMPONENT_TYPE_COLLIDER)->data, PHYSICS_LAYER_ENEMY);
  Entity_AttachComponent(enemy, RigidbodyComponent_Create(10, 250, 0.45f, 0.85f, false, false));
  Entity_AttachComponent(enemy, EnemyTestComponent_Create(zero, speed, rotationSpeed));
  Entity_AttachComponent(enemy, RenderComponent_CreateMeshGroup(meshGroup, RENDER_LAYER_PLAYER));

  //Logging
  LogDataGeneric genericData;
  genericData.sourceFile = "EnemyTest.c";
  LogDataSpawn spawnData;
  spawnData.position.x = x;
  spawnData.position.y = y;
  spawnData.entity = enemy;
  spawnData.isPlayer = false;
  Log_LogData(LOG_TYPE_SPAWN, genericData, &spawnData);

  return enemy;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
