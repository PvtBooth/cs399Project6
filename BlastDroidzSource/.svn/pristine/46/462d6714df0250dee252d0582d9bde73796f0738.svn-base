/******************************************************************************/
/*!
	\file   Physics_Layers.c
	\author Evan Kau
	\date   2/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Physics_Layers.h"
#include <string.h>
#include <stdarg.h>

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

static LayerMask collisionMask[PHYSICS_LAYER_MAX] = {0};

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void SetLayerCollision(PhysicsLayer layer1, PhysicsLayer layer2);

static bool CheckLayerCollision(PhysicsLayer layer1, PhysicsLayer layer2);

static bool CheckLayerMaskCollision(LayerMask layerMask, PhysicsLayer layer);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *   Initializes the physics layers. This sets up which layers interact with
 *   which layers. Must be run once at the start of the game.
 */
void Physics_Layers_Init(void)
{
  int i;
  memset(collisionMask, 0, sizeof(unsigned int) * PHYSICS_LAYER_MAX);
  for (i = 0; i < PHYSICS_LAYER_MAX; i++)
  {
    SetLayerCollision(PHYSICS_LAYER_DEFAULT, i);
  }
  SetLayerCollision(PHYSICS_LAYER_PLAYER, PHYSICS_LAYER_ENEMY);
}

/*!
 * \brief
 *    Checks two PhysicsLayers against each other to determine if they collide with each other.
 * \param layer1
 *   The first PhysicsLayer to check with.
 * \param layer2
 *   The second PhysicsLayer to check with.
 * \return Returns a boolean to say if the layers have collided or not.
 */
bool Physics_Layers_Check(PhysicsLayer layer1, PhysicsLayer layer2)
{
  return CheckLayerCollision(layer1, layer2);
}

/*!
* \brief
*    Checks physics layer against a layer mask to see if it is one of the layers to collide against.
* \param layerMask
*   The given layer mask to check against.
* \param layer
*   The PhysicsLayer to check with.
* \return Returns a boolean to say if the layer is on the layer mask.
*/
bool Physics_Layers_CheckLayerMask(LayerMask layerMask, PhysicsLayer layer)
{
  return CheckLayerMaskCollision(layerMask, layer);
}

/*!
 * \brief
 *   Gets the layer mask of a particular physics layer.
 * \param layer
 *   The layer to get the mask of, i.e. the enemy layer.
 * \return
 *   The mask of the given layer.
 */
LayerMask Physics_Layers_GetLayerMask(PhysicsLayer layer)
{
  return collisionMask[layer];
}

/*!
 * \brief
 *   Creates a custom layer mask from a set of physics layers. These layers will form the layer mask.
 *   Note: does NOT combine the masks from the given layers, but creates a mask that is made up of those
 *   layers.
 * \param collisionCount
 *   How many layers are being passed in.
 * \return
 *   A layer mask that will collide with the given layers.
 */
LayerMask Physics_Layers_CreateLayerMask(int collisionCount, ...)
{
  LayerMask layerMask = 0;
  int i;
  va_list args;

  va_start(args, collisionCount);
  for (i = 0; i < collisionCount; i++)
  {
    PhysicsLayer nextLayer = va_arg(args, PhysicsLayer);
    layerMask = layerMask | (1 << nextLayer);
  }
  va_end(args);

  return layerMask;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void SetLayerCollision(PhysicsLayer layer1, PhysicsLayer layer2)
{
  collisionMask[layer1] = collisionMask[layer1] | (1 << layer2);
  collisionMask[layer2] = collisionMask[layer2] | (1 << layer1);
}

static bool CheckLayerCollision(PhysicsLayer layer1, PhysicsLayer layer2)
{
  if ((collisionMask[layer1] & (1 << layer2)) != 0U || (collisionMask[layer2] & (1 << layer1)) != 0U)
    return true;
  return false;
}

static bool CheckLayerMaskCollision(LayerMask layerMask, PhysicsLayer layer)
{
//  unsigned int result = (layerMask & (1 << layer));
  //if ((layerMask & (1 << layer)) != 0U)
  //  return true;
  //return false;

  return (layerMask & (1 << layer));
}