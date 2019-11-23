/******************************************************************************/
/*!
	\file   Sprite.c
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Sprite.h"
#include "Matrix3x3.h"
#include "Graphics.h"
#include "AssetTable.h"

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
static Sprite *spriteHead;
static void AddSpriteToList(Sprite *sprite);
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
Sprite* Sprite_Create(TextureAsset* texture)
{
  Sprite *sprite = calloc(1, sizeof(Sprite));
  assert(sprite);
  sprite->texture = texture;
  AddSpriteToList(sprite);
  return sprite;
}



void Sprite_DestroyAll()
{
  Sprite *iterator = spriteHead;
  while (iterator)
  {
    Sprite *next = iterator->next;
    free(iterator);
    iterator = next;
  }
  spriteHead = NULL;
}

void Sprite_Destroy(Sprite* sprite)
{
  if(!sprite)
  {
    return;
  }

  if (sprite == spriteHead)
  {
    spriteHead = spriteHead->next;
    free(sprite);
    return;
  }
  Sprite *last = spriteHead;
  Sprite *iterator = spriteHead->next;
  while (iterator)
  {
    if(iterator == sprite)
    {
      last->next = sprite->next;
      free(sprite);
      return;
    }
    last = iterator;
    iterator = iterator->next;
  }
}

void Sprite_Render(Sprite* sprite, Matrix3x3 *transform)
{
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

  
  AEGfxSetTransform((float(*)[3])transform);
  AEGfxTextureSet(sprite->texture->texture, sprite->uOffset, sprite->vOffset);
  AEGfxSetTransparency(1.0);
  AEGfxMeshDraw(Graphics_GetUnitMesh(), AE_GFX_MDM_TRIANGLES);
}

/**
* \brief Renders a sprite using a vector for position and scale instead of transform matrix
* \param sprite The sprite to render
* \param pos The position
* \param scale The scale
*/
void Sprite_RenderPosScale(Sprite* sprite, Vector2 pos, Vector2 scale)
{
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  AEGfxSetFullTransformWithZOrder(pos.x, pos.y, 0, 0, scale.x, scale.y);
  AEGfxTextureSet(sprite->texture->texture, sprite->uOffset, sprite->vOffset);
  AEGfxSetTransparency(1.0);
  AEGfxMeshDraw(Graphics_GetUnitMesh(), AE_GFX_MDM_TRIANGLES);
}

void Sprite_SetSpriteSheet(Sprite* sprite, SpriteSheet* spriteSheet)
{
  if (sprite && spriteSheet)
    sprite->spriteSheet = spriteSheet;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void AddSpriteToList(Sprite *sprite)
{
  if (spriteHead)
  {
    sprite->next = spriteHead;
    spriteHead = sprite;
  }
  else
  {
    spriteHead = sprite;
  }
}