 /******************************************************************************/
/*!
\file   Text.c
\author Ryan Booth
\date   2/28/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes functions that handle printing text to the screen.
*/
/******************************************************************************/
#include <stdlib.h>
#include "Text.h"
#include "Entity.h" 
#include "Camera.h"
#include "RenderComponent.h"
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/
static AEGfxVertexList *unitMesh = NULL;
static Entity *textEntity;
static Sprite* textSprite;
static TextureAsset* texture1;
static SpriteSheet* spriteSheet;
/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static AEGfxVertexList *RectMeshUV(float width, float height, float u, float v, u32 color);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
\brief Intializes Text printing variables
*/
void Text_Init()
{
  Vector2 scale = { 15.0f, 15.0f }; //The default scalse of characters on the screen

  //Load the sprite sheet into a texture
  //texture1 = Texture_Load("Assets/Text/digital_font_bold.png");
  texture1 = AssetTable_TryLoadTextureAsset("Text", "Assets/Text/digital_font_bold.png");
  //Create a sprite sheet with the texture
  spriteSheet = Text_SpriteSheetCreate("TextSheet", 6, 7, texture1);
  //Create a sprite with the sprite sheet
  textSprite = Sprite_Create(spriteSheet->texture);
  //Set the Sprite's sprite sheet
  Sprite_SetSpriteSheet(textSprite, spriteSheet);
  //Create an orphan entity
  textEntity = Entity_CreateOrphan("Text");
  //Attatch a transform component to the entity
  Entity_AttachComponent(textEntity, TransformComponent_Create());
  //Set the default scale for the sprite sheet entity
  TransformComponent_SetScale(Entity_GetTransform(textEntity), scale);
  //Attach a render compoennt with the created sprite to the text entity
  Entity_AttachComponent(textEntity, RenderComponent_CreateSprite(textSprite, RENDER_BEHAVIOR_SPRITESHEET, RENDER_LAYER_PLAYER));
  //Initialize the unit mesh
  unitMesh = RectMeshUV(1, 1, 1 / 6.0f, 1 / 7.0f, 0xFFFFFFFF);
}
/*!
\brief Free's all data allocated with text printing
*/
void Text_Exit()
{
  Text_SpriteSheetFree(&spriteSheet);
  //Free's the memory associated with the text entity
  Entity_DestroyOrphan(textEntity);
  //Free's the unit mesh used for scaling
  AEGfxMeshFree(unitMesh);
}
/*!
\brief Creates a sprite sheet with a given texture and offsets
*/
SpriteSheet* Text_SpriteSheetCreate(char *name, int numCols, int numRows, TextureAsset *pTexture)
{
  //Allocate a new sprite sheet
  SpriteSheet* newSpriteSheet = calloc(1, sizeof(SpriteSheet));
  //Set the name
  newSpriteSheet->name = name;
  //Set the number of columns and rows in the sprite sheet
  newSpriteSheet->numCols = numCols;
  newSpriteSheet->numRows = numRows;
  //Set the texture to use
  newSpriteSheet->texture = pTexture;

  return newSpriteSheet;
}
/*!
\brief Free the data and set the pointer to NULL
*/
void Text_SpriteSheetFree(SpriteSheet** reference)
{
    //Free the memory allocated for the sprite sheet
    if (reference)
    {
        free(*reference);
        *reference = NULL;
    }

}
/*!
\brief Returns the texture being used by the sprite sheet
*/
TextureAsset* Text_SpriteSheetGetTexture(SpriteSheet* reference)
{
    //Safety check the pointer
    if (reference->texture)
        return reference->texture;
    else
        return NULL;
}
/*!
\brief Returns the maximum number of frames in the sprite sheet
*/
unsigned int Text_SpriteSheetGetFrameCount(SpriteSheet* reference)
{
    //Safety check the pointer
    if (reference)
    {
        return (reference->numRows * reference->numRows);
    }
    else
        return 0;
}
/*!
\brief Gets the uSize and vSize of the sprite sheet
*/
void Text_SpriteSheetGetUV(SpriteSheet* reference, unsigned int frameIndex, f32 * u, f32 * v)
{
    //Calcuate the uSize and vSize
    float uSize = (1.0f / reference->numCols);
    float vSize = (1.0f / reference->numRows);

    //Derefrence and set the variables passed in
    *u = uSize * (frameIndex % reference->numCols);

    *v = vSize * (frameIndex / reference->numCols);
}
/*!
\brief Renders the sprite if it has a sprite sheet
*/
void Graphics_RenderText(Sprite* sprite, Matrix3x3 *transform)
{
  //Safety check the pointer
  if (sprite->spriteSheet)
  {
    //Set the Render mode
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    //Set the transform
    AEGfxSetTransform((float (*)[3])transform);
    //Store the UV offsets from the sprite sheet into the sprite
    Text_SpriteSheetGetUV(sprite->spriteSheet, (unsigned int)sprite->frameIndex, &sprite->uOffset, &sprite->vOffset);
    //Set the texture from the sprite sheet
    AEGfxTextureSet(sprite->spriteSheet->texture->texture, sprite->uOffset, sprite->vOffset);
    //Transparency to full
    AEGfxSetTransparency(1.0f);
    //Draw it as a unit mesh
    AEGfxMeshDraw(unitMesh, AE_GFX_MDM_TRIANGLES);
    //Set the blend color to white
    AEGfxSetBlendColor(1.0f, 1.0f, 1.0f, 0.0f);
    //Set the blend mode to BLEND
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  }
}
/*!
\brief Prints the text to the screen
*/
void Text_Print(Vector2 position, char *text, float scale, RENDER_LAYER layer)
{
  int i;                        //Loop variable
  int length = strlen(text);    //The length of the text to print

  Vector2 position1 = position; //The position to print the text at
  Vector2 spriteScale;          //Scale for offset / size of the text

  spriteScale.x = scale;
  spriteScale.y = scale;

  //Set the scale of the text Entity
  TransformComponent_SetScale(Entity_GetTransform(textEntity), spriteScale);

  //For score printing in LevelState
  if (layer == RENDER_LAYER_PLAYER)
  {
    //convert position from camera to worldspace
    Matrix3x3 camera = *Camera_GetMatrix();
    position1 = Matrix3x3_TransformPoint(&camera, position1);
  }
  //Loop through the character array and print each character
  for (i = 0; i < length; i++)
  {
      //If there is more than one character, offset each one
      if (i > 0)
          position1.x += (scale * 1.0f); //offset

        //Safety check the pointer
      if (text)
      {
          //Covnerting from ASCII to sprite sheet frame
          int current_value = text[i] - 65;

          //Skip when there is a space so it prints nothign
          if (current_value == -33)
          {
              continue;
          }
          //Special value for a' / ' slash
          if (current_value == -18)
          {
              current_value = 40;
          }
          //Calculate number frame
          if (current_value < 0)
          {
              current_value += 17; //ASCII chart for 0
              current_value += 26; //After Z in sprite sheet
          }
          //Set the frame index to the sprite
          textSprite->frameIndex = (float)current_value;
          //Set the position to start at
          TransformComponent_SetPosition(textEntity->transformComponent, position1);
          //Render the text with the newly calculated values
          Graphics_RenderText(textSprite, TransformComponent_GetTransformMatrix(Entity_GetTransform(textEntity)));
      }
  }
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

/*!
\brief Creates a unit mesh
*/
static AEGfxVertexList *RectMeshUV(float width, float height, float u, float v, u32 color)
{
    AEGfxMeshStart();

    AEGfxTriAdd(
        -width / 2, -height / 2, color, 0.0f, v,
        width / 2, -height / 2, color, u, v,
        -width / 2, height / 2, color, 0.0f, 0.0f);

    AEGfxTriAdd(
        width / 2, -height / 2, color, u, v,
        width / 2, height / 2, color, u, 0.0f,
        -width / 2, height / 2, color, 0.0f, 0.0f);

    AEGfxVertexList *mesh = AEGfxMeshEnd();
    assert(mesh);
    return mesh;
}