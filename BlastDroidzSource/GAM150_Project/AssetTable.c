/******************************************************************************/
/*!
	\file   AssetTable.c
	\author Evan Kau
	\date   3/18/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "AssetTable.h"
#include <string.h>
#include <stdlib.h>
#include "Sound.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define ASSET_TABLE_SIZE 1024

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static Asset *assetTable[ASSET_TABLE_SIZE] = {0};

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static unsigned GetAssetHashValue(const char *name);

static void AddAssetToTable(Asset *asset);

static void FreeAsset(Asset *asset);

static void FreeMeshAsset(MeshAsset *asset);

static void FreeMeshGroupAsset(MeshGroupAsset *asset);

static void FreeSoundAsset(SoundAsset *asset);

static void FreeTextureAsset(TextureAsset *asset);

static void FreePrefabAsset(PrefabAsset *asset);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/


void AssetTable_AddTextureAsset(const char *name, const char *file)
{
  Asset *asset = (Asset*)calloc(1, sizeof(Asset));
  asset->name = name;
  asset->type = ASSET_TYPE_TEXTURE;

  TextureAsset *textureAsset = (TextureAsset*)calloc(1, sizeof(TextureAsset));
  textureAsset->texture = AEGfxTextureLoad(file);

  asset->data = textureAsset;

  AddAssetToTable(asset);
}

void AssetTable_AddMeshAsset(const char *name, Vector2 *mesh, int meshVertCount, Vector2 *collisionMesh, int collisionMeshVertCount)
{
  Asset *asset = (Asset*)calloc(1, sizeof(Asset));
  asset->name = name;
  asset->type = ASSET_TYPE_MESH;

  MeshAsset *meshAsset = (MeshAsset*)calloc(1, sizeof(MeshAsset));

  meshAsset->mesh = (Vector2*)calloc(meshVertCount, sizeof(Vector2));
  memcpy_s(meshAsset->mesh, sizeof(Vector2) * meshVertCount, mesh, sizeof(Vector2) * meshVertCount);
  meshAsset->meshVertCount = meshVertCount;

  meshAsset->colliderMesh = (Vector2*)calloc(collisionMeshVertCount, sizeof(Vector2));
  memcpy_s(meshAsset->colliderMesh, sizeof(Vector2) * collisionMeshVertCount, collisionMesh, sizeof(Vector2) * collisionMeshVertCount);
  meshAsset->colliderVertCount = collisionMeshVertCount;

  asset->data = meshAsset;

  AddAssetToTable(asset);
}


void AssetTable_AddMeshGroupAsset(const char *name, MeshAsset *source, float r, float g, float b, float a)
{
  Asset *asset = (Asset*)calloc(1, sizeof(Asset));
  asset->name = name;
  asset->type = ASSET_TYPE_MESHGROUP;

  MeshGroupAsset *meshGroupAsset = (MeshGroupAsset*)calloc(1, sizeof(MeshGroupAsset));

  meshGroupAsset->meshGroup = MeshGroup_Create();

  AEGfxVertexList *mesh;
  AEGfxMeshStart();
  for (int i = 0; i < source->meshVertCount; i += 3)
  {
    AEGfxTriAdd(source->mesh[i].x, source->mesh[i].y, 0xFFFFFFFF, 0, 0, source->mesh[i + 1].x, source->mesh[i + 1].y, 0xFFFFFFFF, 0, 0,
                              source->mesh[i + 2].x, source->mesh[i + 2].y, 0xFFFFFFFF, 0, 0);
  }
  mesh = AEGfxMeshEnd();
  MeshGroup_AddMeshRGBA(meshGroupAsset->meshGroup, mesh, r, g, b, a);
  meshGroupAsset->sourceAsset = source;

  asset->data = meshGroupAsset;

  AddAssetToTable(asset);
}

void AssetTable_AddSoundAsset(const char *name, const char *file)
{
  Asset *asset = (Asset*)calloc(1, sizeof(Asset));
  asset->name = name;
  asset->type = ASSET_TYPE_SOUND;

  SoundAsset *soundAsset = (SoundAsset*)calloc(1, sizeof(SoundAsset));
  Sound_CreateSoundAsset(file, &soundAsset->sound);

  asset->data = soundAsset;

  AddAssetToTable(asset);
}

/*!
 * \brief
 *   Gets an asset from the asset table.
 * \param name
 *   The name of the asset to get.
 * \param type
 *   The type of the asset to get.
 * \return
 *   The asset that matches in name and type. Returns NULL on fail. Will break on fail in debug mode.
 */
Asset* AssetTable_GetAsset(const char *name, ASSET_TYPE type)
{
  int index = GetAssetHashValue(name);
  Asset *asset = assetTable[index];

  while (asset)
  {
    if (strcmp(asset->name, name) != 0 || asset->type != type)
    {
      asset = asset->next;
    }
    else
    {
      break;
    }
  }

#if _DEBUG
  if (!asset)
  {
    asset = NULL; //If you got here, it means you messed up and didn't get the name or type correct. See me or Hank for help if you need it. --Evan
  }
#endif //_DEBUG

  return asset;
}

TextureAsset* AssetTable_TryLoadTextureAsset(const char *name, const char *file)
{
  Asset* asset = AssetTable_GetAsset(name, ASSET_TYPE_TEXTURE);
  if (!asset)
  {
    AssetTable_AddTextureAsset(name, file);
    asset = AssetTable_GetAsset(name, ASSET_TYPE_TEXTURE);
  }
  return (TextureAsset*)asset->data;
}

void AssetTable_Exit(void)
{
  int i;

  for (i = 0; i < ASSET_TABLE_SIZE; i++)
  {
    if (assetTable[i])
    {
      FreeAsset(assetTable[i]);
    }
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static unsigned GetAssetHashValue(const char *name)
{
  unsigned id = 1;
  int nameLength = strlen(name);
  int i;

  for (i = 0; i < nameLength; i++)
  {
    id *= name[i];
  }

  return id % ASSET_TABLE_SIZE;
}

static void AddAssetToTable(Asset *asset)
{
  int index = GetAssetHashValue(asset->name);

  asset->next = assetTable[index];
  assetTable[index] = asset;
}

static void FreeAsset(Asset *asset)
{
  if (asset->next)
  {
    FreeAsset(asset->next);
  }

  if (asset->type == ASSET_TYPE_MESH)
  {
    FreeMeshAsset(asset->data);
  }
  else if (asset->type == ASSET_TYPE_MESHGROUP)
  {
    FreeMeshGroupAsset(asset->data);
  }
  else if (asset->type == ASSET_TYPE_PREFAB)
  {
    FreePrefabAsset(asset->data);
  }
  else if (asset->type == ASSET_TYPE_TEXTURE)
  {
    FreeTextureAsset(asset->data);
  }
  else if (asset->type == ASSET_TYPE_SOUND)
  {
    FreeSoundAsset(asset->data);
  }
  free(asset->data);

  free(asset);
}

static void FreeMeshAsset(MeshAsset *asset)
{
  free(asset->mesh);
  free(asset->colliderMesh);
}

static void FreeMeshGroupAsset(MeshGroupAsset *asset)
{
  //Destroy mesh group?
    UNREFERENCED_PARAMETER(asset);
}

static void FreeSoundAsset(SoundAsset *asset)
{
  FMOD_Sound_Release(asset->sound);
}

static void FreeTextureAsset(TextureAsset *asset)
{
  AEGfxTextureUnload(asset->texture);
}

static void FreePrefabAsset(PrefabAsset *asset)
{
    UNREFERENCED_PARAMETER(asset);
}
