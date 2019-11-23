/******************************************************************************/
/*!
	\file   Sprite.h
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "Texture.h"
#include "Matrix3x3.h"
#include "AssetTable.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct Sprite Sprite;
typedef struct SpriteSheet
{
  char* name;

  int numRows;
  int numCols;

  TextureAsset* texture;

} SpriteSheet;
struct Sprite
{
  TextureAsset *texture;

  float uOffset, vOffset;

  float frameIndex;

  SpriteSheet *spriteSheet;

  Sprite *next;
};



/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
Sprite *Sprite_Create(TextureAsset *texture);

void Sprite_DestroyAll();

void Sprite_Destroy(Sprite* sprite);

void Sprite_Render(Sprite *sprite, Matrix3x3 *transform);

void Sprite_RenderPosScale(Sprite *sprite, Vector2 pos, Vector2 scale);

void Sprite_SetSpriteSheet(Sprite* sprite, SpriteSheet* spriteSheet);

/*----------------------------------------------------------------------------*/


