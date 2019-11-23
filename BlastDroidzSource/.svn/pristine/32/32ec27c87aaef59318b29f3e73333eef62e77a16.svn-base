/******************************************************************************/
/*!
	\file   Texture.c
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Texture.h"

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
static Texture *textureHead = NULL;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void AddTextureToList(Texture *texture);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
Texture* Texture_Load(char* filename)
{
  AEGfxTexture *data = AEGfxTextureLoad(filename);
  AE_ASSERT_MESG(data, "Failed to create texture1!!");
  assert(data);
  Texture *texture = calloc(1, sizeof(Texture));
  texture->data = data;

  AddTextureToList(texture);
  return texture;
}

void Texture_DestroyAll()
{
  Texture *iterator = textureHead;
  while (iterator)
  {
    Texture *next = iterator->next;
    AEGfxTextureUnload(iterator->data);
    free(iterator);
    iterator = next;
  }
  textureHead = NULL;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void AddTextureToList(Texture *texture)
{
  if (textureHead)
  {
    texture->next = textureHead;
    textureHead = texture;
  }
  else
  {
    textureHead = texture;
  }
}