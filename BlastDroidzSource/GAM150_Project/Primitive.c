/******************************************************************************/
/*!
	\file   Primitive.c
	\author Henry Brobeck
	\date   3/4/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Primitive.h"
#include "TransformComponent.h"

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
*\brief Create a new Rect primtive
*\param width Width of the rectangle
*\param height Height of the rectangle
*\return A new rectangle Primitive
*/
Primitive* Primitive_CreateRect(float width, float height, u32 color)
{

  Primitive *newRect = malloc(sizeof(struct Primitive));

  AEGfxMeshStart();

  // This shape has 2 triangles
  AEGfxTriAdd(
    -width / 2, -height / 2, color, 0.0f, 1.0f,
    width / 2, -height / 2, color, 1.0f, 1.0f,
    -width / 2, height / 2, color, 0.0f, 0.0f);

  AEGfxTriAdd(
    width / 2, -height / 2, color, 1.0f, 1.0f,
    width / 2, height / 2, color, 1.0f, 0.0f,
    -width / 2, height / 2, color, 0.0f, 0.0f);


  newRect->mesh = AEGfxMeshEnd();

  return newRect;
}

/*!
*\brief Destroy a primitive
*\param primitive The Primitive to destroy
*/
void Primitive_Destroy(Primitive* primitive)
{
  AEGfxMeshFree(primitive->mesh);
  free(primitive);
}

/*!
* \brief Render a Primitive at given position
* \param primitive The primitive to render
* \param position The position to render the primitive in world space
*/
void Primitive_Render(Primitive* primitive, Vector2 position)
{
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  //AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  // Set poisition for object 1
  //AEGfxSetPosition(obj1X, obj1Y);
  AEGfxSetFullTransformWithZOrder(position.x, position.y, 0.0f, 0.0f, 1.0f, 1.0f);
  // No texture for object 1
  AEGfxTextureSet(NULL, 0, 0);
  // Drawing the mesh (list of triangles)
  AEGfxSetTransparency(1.0);

  AEGfxMeshDraw(primitive->mesh, AE_GFX_MDM_TRIANGLES);
}

void Primitive_RenderTransform(Primitive* primitive, TransformComponent* transform)
{

  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  //AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  // Set poisition for object 1
  //AEGfxSetPosition(obj1X, obj1Y);
  AEGfxSetTransform((float(*)[3])TransformComponent_GetCameraTransformMatrix(transform));
  // No texture for object 1
  AEGfxTextureSet(NULL, 0, 0);
  // Drawing the mesh (list of triangles)
  AEGfxSetTransparency(1.0);

  AEGfxMeshDraw(primitive->mesh, AE_GFX_MDM_TRIANGLES);
}



/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
