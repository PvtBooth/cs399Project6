/******************************************************************************/
/*!
	\file   AssetTable.h
	\author Evan Kau
	\date   3/18/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <AEEngine.h>
#include "MeshGroup.h"
#include "Entity.h"
#include <fmod.h>

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum ASSET_TYPE
{
  ASSET_TYPE_TEXTURE,
  ASSET_TYPE_SOUND,
  ASSET_TYPE_MESH,
  ASSET_TYPE_MESHGROUP,
  ASSET_TYPE_PREFAB
} ASSET_TYPE;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Asset
{
  const char *name;
  ASSET_TYPE type;
  void *data;
  struct Asset *next;
} Asset;

typedef struct MeshAsset
{
  Vector2 *mesh;
  int meshVertCount;
  Vector2 *colliderMesh;
  int colliderVertCount;
} MeshAsset;

typedef struct MeshGroupAsset
{
  MeshGroup *meshGroup;
  MeshAsset *sourceAsset;
} MeshGroupAsset;

typedef struct SoundAsset
{
  FMOD_SOUND *sound;
} SoundAsset;

typedef struct TextureAsset
{
  AEGfxTexture *texture;
} TextureAsset;

typedef struct PrefabAsset
{
  Entity *prefab;
} PrefabAsset;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void AssetTable_AddTextureAsset(const char *name, const char *file);

void AssetTable_AddMeshAsset(const char *name, Vector2 *mesh, int meshVertCount, Vector2 *collisionMesh, int collisionMeshVertCount);

void AssetTable_AddMeshGroupAsset(const char *name, MeshAsset *source, float r, float g, float b, float a);

void AssetTable_AddSoundAsset(const char *name, const char *file);

Asset* AssetTable_GetAsset(const char *name, ASSET_TYPE type);

TextureAsset* AssetTable_TryLoadTextureAsset(const char *name, const char *file);

void AssetTable_Exit(void);
