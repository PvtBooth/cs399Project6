/******************************************************************************/
/*!
	\file   StandardMeshGroups.c
	\author Henry Brobeck, Evan Kau
	\date   3/15/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "StandardMeshGroups.h"
#include "MeshGroup.h"
#include "AssetTable.h"
#include "Colors.h"

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
void StandardMeshGroups_Init(void)
{
  Vector2 enemyShipMesh[] = { { 30, 0 },{ 20,10 },{ 20, -10 },
                              { 20,10 },{ -20, 25 },{ -20, -25 },
                              { 20,10 },{ -20,-25 },{ 20,-10 } };
  Vector2 enemyShipColliderMesh[] = { { 30, 0 },{ 20, 10 },{ -20, -25 },{ -20, 25 },{ 20, -10 } };
  AssetTable_AddMeshAsset("M_ShipDefault", enemyShipMesh, sizeof(enemyShipMesh) / sizeof(enemyShipMesh[0]), enemyShipColliderMesh, sizeof(enemyShipColliderMesh) / sizeof(enemyShipColliderMesh[0]));
  Color enemyColor = Colors_GetEnemyColor(0, COLORT_PRIMARY);
  AssetTable_AddMeshGroupAsset("MG_EnemyDefault", AssetTable_GetAsset("M_ShipDefault", ASSET_TYPE_MESH)->data, enemyColor.r, enemyColor.g, enemyColor.b, enemyColor.a);

  Vector2 enemyBersekerMesh[] = { { -25, 17.5 },{ -25, -17.5 },{ 25, 0 } };
  //Vector2 enemyBersekerColliderMesh[] = { { 25, 0 },{ 20, 10 },{ -20, -25 },{ -20, 25 },{ 20, -10 } };
  AssetTable_AddMeshAsset("M_EnemyBerserker", enemyBersekerMesh, sizeof(enemyBersekerMesh) / sizeof(enemyBersekerMesh[0]), enemyBersekerMesh, sizeof(enemyBersekerMesh) / sizeof(enemyBersekerMesh[0]));
  AssetTable_AddMeshGroupAsset("MG_EnemyBerserker", AssetTable_GetAsset("M_EnemyBerserker", ASSET_TYPE_MESH)->data, enemyColor.r, enemyColor.g, enemyColor.b, enemyColor.a);

  Vector2 enemySuicideMesh[] = { { 20, 0 },{ 5,45 },{ 5, -45 },
                                 { 5,45 },{ -65, 25 },{ -65, -25 },
                                 { 5,45 },{ -65,-25 },{ 5,-45 } };
  Vector2 enemySuicideColliderMesh[] = { { 20, 0 },{ 5, 45 },{ -65, 25 },{ -65, -25 },{ 5, -45 } };
  AssetTable_AddMeshAsset("M_EnemySuicide", enemySuicideMesh, sizeof(enemySuicideMesh) / sizeof(enemySuicideMesh[0]),
    enemySuicideColliderMesh, sizeof(enemySuicideColliderMesh) / sizeof(enemySuicideColliderMesh[0]));
  AssetTable_AddMeshGroupAsset("MG_EnemySuicide", AssetTable_GetAsset("M_EnemySuicide", ASSET_TYPE_MESH)->data, enemyColor.r, enemyColor.g, enemyColor.b, enemyColor.a);

  Vector2 enemyCapitalShipMesh[] = { { 120, 0 },{ 55,55 },{ 55, -55 },
                                     { 55,55 },{ -45, 65 },{ -45, -65 },
                                     { 55,55 },{ -45,-65 },{ 55,-55 },
                                     { -45,65 }, { -80, 25 }, { -80,-25 },
                                     { -45,65 }, { -80,-25 }, { -45,-65 } };
  Vector2 enemyCapitalShipColliderMesh[] = { { 120, 0 },{ 55, 55 },{ -45, 65 },{-80,25},{-80,-25},{ -45, -65 },{ 55, -55 } };
  AssetTable_AddMeshAsset("M_EnemyCapitalShip", enemyCapitalShipMesh, sizeof(enemyCapitalShipMesh) / sizeof(enemyCapitalShipMesh[0]),
    enemyCapitalShipColliderMesh, sizeof(enemyCapitalShipColliderMesh) / sizeof(enemyCapitalShipColliderMesh[0]));
  AssetTable_AddMeshGroupAsset("MG_EnemyCapitalShip", AssetTable_GetAsset("M_EnemyCapitalShip", ASSET_TYPE_MESH)->data, enemyColor.r, enemyColor.g, enemyColor.b, enemyColor.a);

  Vector2 asteroidMesh[3 * 5] = { 0 };
  Vector2 asteroidColliderMesh[5] = { 0 };
  for (int i = 0; i < 5; i++)
  {
    asteroidMesh[i * 3] = Vector2_FromAngle((i / 5.f) * 2.f * PI);
    asteroidMesh[i * 3 + 1] = Vector2_Zero();
    asteroidMesh[i * 3 + 2] = Vector2_FromAngle(((i + 1) / 5.f) * 2.f * PI);
    asteroidColliderMesh[i] = Vector2_FromAngle((i / 5.f) * 2.f * PI);
  }
  AssetTable_AddMeshAsset("M_Asteroid", asteroidMesh, sizeof(asteroidMesh) / sizeof(asteroidMesh[0]), asteroidColliderMesh, sizeof(asteroidColliderMesh) / sizeof(asteroidColliderMesh[0]));
  Color asteroidColor = Colors_GetGeneralColor(COLOR_ASTEROID);
  AssetTable_AddMeshGroupAsset("MG_Asteroid", AssetTable_GetAsset("M_Asteroid", ASSET_TYPE_MESH)->data, asteroidColor.r, asteroidColor.g, asteroidColor.b, asteroidColor.a);

  Vector2 unitMesh[] = { { -0.5f, 0.5f },{ -0.5f, -0.5f },{ 0.5f, 0.5f },
                         { 0.5f, 0.5f },{ -0.5f, -0.5f },{ 0.5f, -0.5f } };
  Vector2 unitColliderMesh[] = { { -0.5f, 0.5f },{ -0.5f, -0.5f },{ 0.5f, -0.5f },{ 0.5f, 0.5f } };
  AssetTable_AddMeshAsset("M_Unit", unitMesh, sizeof(unitMesh) / sizeof(unitMesh[0]), unitColliderMesh, sizeof(unitColliderMesh) / sizeof(unitColliderMesh[0]));
  Color unitColor = { 1, 1, 1, 1 };
  AssetTable_AddMeshGroupAsset("MG_Unit", AssetTable_GetAsset("M_Unit", ASSET_TYPE_MESH)->data, unitColor.r, unitColor.g, unitColor.b, unitColor.a);

  Vector2 enemyStationMesh[] = { { 0.f, 1.f },{ -0.5f, 0.f },{ 0.5f, 0.f },
                                 { -0.5f, 0.f },{ 0.f, -1.f },{ 0.5f, 0.f } };
  Vector2 enemyStationColliderMesh[] = { { 0.f, 1.f },{ -0.5f, 0.f },{ 0.f, -1.f },{ 0.5f, 0.f } };
  Color darkEnemy = Colors_GetEnemyColor(0, COLORT_TINT);
  AssetTable_AddMeshAsset("M_EnemyStation", enemyStationMesh, sizeof(enemyStationMesh) / sizeof(enemyStationMesh[0]), enemyStationColliderMesh, sizeof(enemyStationColliderMesh) / sizeof(enemyStationColliderMesh[0]));
  AssetTable_AddMeshGroupAsset("MG_EnemyStation", AssetTable_GetAsset("M_EnemyStation", ASSET_TYPE_MESH)->data, darkEnemy.r, darkEnemy.g, darkEnemy.b, darkEnemy.a);


  Vector2 missileMesh[] = { { -10, 10 },{ -10, -10 },{ 20, 0 } };
  AssetTable_AddMeshAsset("M_Missile", missileMesh, sizeof(missileMesh) / sizeof(missileMesh[0]), missileMesh, sizeof(missileMesh) / sizeof(missileMesh[0]));
  Color missileColor = { 1.f, 1.f, 1.f, 1.f };
  AssetTable_AddMeshGroupAsset("MG_Missile", AssetTable_GetAsset("M_Missile", ASSET_TYPE_MESH)->data, missileColor.r, missileColor.g, missileColor.b, missileColor.a);
  //missile = MeshGroup_Create();
  //MeshGroup_AddTriangleRGBA(missile, -10, 10, -10, -10, 20, 0, 1.0f, 1.0f, 1.0f, 1.0f);


  Vector2 planetMesh[3 * 64] = { 0 };
  Vector2 planetColliderMesh[64] = { 0 };
  for (int i = 0; i < 64; i++)
  {
    planetMesh[i * 3] = Vector2_FromAngle((i / 64.f) * 2.f * PI);
    planetMesh[i * 3 + 1] = Vector2_Zero();
    planetMesh[i * 3 + 2] = Vector2_FromAngle(((i + 1) / 64.f) * 2.f * PI);
    planetColliderMesh[i] = Vector2_FromAngle((i / 64.f) * 2.f * PI);
  }
  AssetTable_AddMeshAsset("M_Planet", planetMesh, sizeof(planetMesh) / sizeof(planetMesh[0]), planetColliderMesh, sizeof(planetColliderMesh) / sizeof(planetColliderMesh[0]));
  Color planetColor = Colors_GetGeneralColor(COLOR_PLANET);
  AssetTable_AddMeshGroupAsset("MG_Planet", AssetTable_GetAsset("M_Planet", ASSET_TYPE_MESH)->data, planetColor.r, planetColor.g, planetColor.b, planetColor.a);


  Vector2 jumpgateMesh[] = { { 1, 0 }, {0, 0},{ 0.75f, -0.15f },
                             { 0.75f, -0.15f }, {0, 0}, { 0, -0.65f },
                             { 0, -0.65f }, {0, 0}, { -1.0f, -0.5f },
                             { -1.0f, -0.5f }, {0, 0}, { -1.0f, 0.5f },
                             { -1.0f, 0.5f }, {0, 0}, { 0, 0.65f },
                             { 0, 0.65f }, {0, 0}, { 0.75f, 0.15f },
                             { 0.75f, 0.15f }, { 0, 0 }, { 1, 0 }};
  Vector2 jumpgateColliderMesh[] = { { 1, 0 },{ 0.75f, -0.15f },{ 0, -0.65f },{ -1.0f, -0.5f },{ -1.0f, 0.5f },{ 0, 0.65f },{ 0.75f, 0.15f } };
  AssetTable_AddMeshAsset("M_Jumpgate", jumpgateMesh, sizeof(jumpgateMesh) / sizeof(jumpgateMesh[0]), jumpgateColliderMesh, sizeof(jumpgateColliderMesh) / sizeof(jumpgateColliderMesh[0]));
  Color jumpgateColor = { 0.65f, 0.85f, 0.85f, 1.f };
  AssetTable_AddMeshGroupAsset("MG_Jumpgate", AssetTable_GetAsset("M_Jumpgate", ASSET_TYPE_MESH)->data, jumpgateColor.r, jumpgateColor.g, jumpgateColor.b, jumpgateColor.a);


  Vector2 playerShipMesh[] = { {50, 0}, {-50, 50}, {-50, -50} };
  AssetTable_AddMeshAsset("M_PlayerShip", playerShipMesh, sizeof(playerShipMesh) / sizeof(playerShipMesh[0]), playerShipMesh, sizeof(playerShipMesh) / sizeof(playerShipMesh[0]));
  Color playerColor = Colors_GetPlayerColor(0, COLORT_PRIMARY);
  AssetTable_AddMeshGroupAsset("MG_PlayerShip0", AssetTable_GetAsset("M_PlayerShip", ASSET_TYPE_MESH)->data, playerColor.r, playerColor.g, playerColor.b, playerColor.a);
  playerColor = Colors_GetPlayerColor(1, COLORT_PRIMARY);
  AssetTable_AddMeshGroupAsset("MG_PlayerShip1", AssetTable_GetAsset("M_PlayerShip", ASSET_TYPE_MESH)->data, playerColor.r, playerColor.g, playerColor.b, playerColor.a);
  playerColor = Colors_GetPlayerColor(2, COLORT_PRIMARY);
  AssetTable_AddMeshGroupAsset("MG_PlayerShip2", AssetTable_GetAsset("M_PlayerShip", ASSET_TYPE_MESH)->data, playerColor.r, playerColor.g, playerColor.b, playerColor.a);
  playerColor = Colors_GetPlayerColor(3, COLORT_PRIMARY);
  AssetTable_AddMeshGroupAsset("MG_PlayerShip3", AssetTable_GetAsset("M_PlayerShip", ASSET_TYPE_MESH)->data, playerColor.r, playerColor.g, playerColor.b, playerColor.a);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

