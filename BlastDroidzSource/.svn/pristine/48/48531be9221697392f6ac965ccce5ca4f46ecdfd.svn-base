/******************************************************************************/
/*!
\file   Text.h
\author Ryan Booth
\date   2/28/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes function declarations that handle printing text
        to the screen.
*/
/******************************************************************************/
#pragma once

#include "Graphics.h"
#include "Texture.h"
#include "Sprite.h"
#include "RenderLayer.h"


/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
SpriteSheet* Text_SpriteSheetCreate(char *name, int numCols, int numRows, TextureAsset* pTexture);

void Text_SpriteSheetFree(SpriteSheet** spriteSheet);

TextureAsset* Text_SpriteSheetGetTexture(SpriteSheet* spriteSheet);

unsigned int Text_SpriteSheetGetFrameCount(SpriteSheet* spriteSheet);

void Text_SpriteSheetGetUV(SpriteSheet* spriteSheet, unsigned int frameIndex, f32 * u, f32 * v);

void Graphics_RenderText(Sprite* sprite, Matrix3x3 *transform);

void Text_Print(Vector2 postition, char *text, float scale, RENDER_LAYER layer);

void Text_Init();

void Text_Exit();
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

