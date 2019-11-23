/******************************************************************************/
/*!
	\file   TransformComponent.c
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "TransformComponent.h"
#include <stdbool.h>
#include <stdlib.h>
#include "Physics_BroadPhase.h"
#include "Camera.h"
#include <math.h>
#include <Windows.h>

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct TransformComponent
{
  //TransformComponent *parent;
  //TransformComponent *children;
  //TransformComponent *nextBrother;
  //TransformComponent *prevBrother;
  Vector2 position;
  Vector2 rotation;
  Vector2 scale;
  Matrix3x3 transformMatrix;
  Matrix3x3 cameraTransformMatrix;
  ColliderComponent *collider;
  bool transformMatrixDirty;
} TransformComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Destroy(Component *self);

static void OnAttach(Component *self);

static void OnOtherAttach(Component *self, Component *component);

static Component* Copy(Component *self);

static void RegisterColliderInBroadphase(TransformComponent *transform);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *   Creates a component with a transform component attached.
 * \return
 *   A pointer to a component with a transform component for data.
 */
Component* TransformComponent_Create()
{
  Component *component = Component_Create();
  TransformComponent *transform = (TransformComponent*)calloc(1, sizeof(TransformComponent));

  //TODO: Set the transform's parent to the entity's parent's tranform, or something equivalent later on in code

  transform->rotation.x = 1;
  transform->scale.x = 1;
  transform->scale.y = 1;
  transform->transformMatrixDirty = true;

  component->data = transform;
  component->destroy = &Destroy;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;
  component->type = COMPONENT_TYPE_TRANSFORM;

  return component;
}

Component* TransformComponent_Copy(TransformComponent *transform)
{
  Component *newTransform = TransformComponent_Create();
  TransformComponent *data = newTransform->data;
  data->position = transform->position;
  data->rotation = transform->rotation;
  data->scale = transform->scale;
  return newTransform;
}

//void TransformComponent_SetParent(TransformComponent *transform, TransformComponent *parent)
//{
//  transform->parent = parent;
//}

//TransformComponent* TransformComponent_GetParent(TransformComponent *transform)
//{
//  return transform->parent;
//}

//RigidbodyComponent* TransformComponent_GetRigidbody(TransformComponent *transform)
//{
//  return transform->rigidbody;
//}

/*!
 * \brief
 *   Get the position of a given transform component.
 * \param transform
 *   The transform component to get the position of.
 * \return
 *   The position of the given transform component.
 */
Vector2 TransformComponent_GetPosition(const TransformComponent *transform)
{
  if(!transform)
  {
    return Vector2_Zero();
  }
  //if (transform->parent)
  //  return Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(transform->parent), transform->position);
  return transform->position;
}

/*!
* \brief
*   Get the rotation of a given transform component.
* \param transform
*   The transform component to get the rotation of.
* \return
*   The rotation of the given transform component.
*/
float TransformComponent_GetRotation(const TransformComponent *transform)
{
  float rotation = atan2f(transform->rotation.y, transform->rotation.x);
  //if (transform->parent)
  //  rotation += TransformComponent_GetRotation(transform->parent);
  return rotation;
}

/*!
* \brief
*   Get the scale of a given transform component.
* \param transform
*   The transform component to get the scale of.
* \return
*   The scale of the given transform component.
*/
Vector2 TransformComponent_GetScale(const TransformComponent *transform)
{
  //if (transform->parent)
  //  return Vector2_ComponentProduct(TransformComponent_GetScale(transform->parent), transform->scale);
  return transform->scale;
}

/*!
 * \brief
 *   Sets the position of the given trasform component.
 * \param transform
 *   The transform component to set the position of.
 * \param position
 *   The position to use.
 */
void TransformComponent_SetPosition(TransformComponent *transform, Vector2 position)
{
  transform->position = position;
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Sets the position of the given trasform component without creating a Vector2.
 * \param transform
 *   The transform component to set the position of.
 * \param xPos
 *   The X position to use.
 * \param yPos
 *   The Y position to use.
 */
void TransformComponent_SetPositionFloats(TransformComponent *transform, float xPos, float yPos)
{
  transform->position.x = xPos;
  transform->position.y = yPos;
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Sets the rotaiton of the given trasform component.
 * \param transform
 *   The transform component to set the rotaiton of.
 * \param rotation
 *   The rotaiton to use.
 */
void TransformComponent_SetRotation(TransformComponent *transform, float rotation)
{
  transform->rotation.x = cosf(rotation);
  transform->rotation.y = sinf(rotation);
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Sets the scale of the given trasform component.
 * \param transform
 *   The transform component to set the scale of.
 * \param scale
 *   The scale to use.
 */
void TransformComponent_SetScale(TransformComponent *transform, Vector2 scale)
{
  transform->scale = scale;
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Sets the scale of the given trasform component without creating a Vector2.
 * \param transform
 *   The transform component to set the scale of.
 * \param xScale
 *   The X scale to use.
 * \param yScale
 *   The Y scale to use.
 */
void TransformComponent_SetScaleFloats(TransformComponent *transform, float xScale, float yScale)
{
  transform->scale.x = xScale;
  transform->scale.y = yScale;
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Adds a given amount to the position of the given trasform component.
 * \param transform
 *   The transform component to add to the position of.
 * \param position
 *   The offset to add.
 */
void TransformComponent_AddPosition(TransformComponent *transform, Vector2 position)
{
  transform->position = Vector2_Add(transform->position, position);
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

void TransformComponent_AddPositionFloats(TransformComponent *transform, float xPos, float yPos)
{
  transform->position = Vector2_AddFloats(transform->position, xPos, yPos);
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Adds a given amount to the rotaiton of the given trasform component.
 * \param transform
 *   The transform component to add to the rotaiton of.
 * \param rotation
 *   The amount to rotate by.
 */
void TransformComponent_AddRotation(TransformComponent *transform, float rotation)
{
  float totalRotation = atan2f(transform->rotation.y, transform->rotation.x) + rotation;
  transform->rotation.x = cosf(totalRotation);
  transform->rotation.y = sinf(totalRotation);
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Adds a given amount to the scale of the given trasform component.
 * \param transform
 *   The transform component to add to the scale of.
 * \param scale
 *   The scale to add.
 */
void TransformComponent_AddScale(TransformComponent *transform, Vector2 scale)
{
  transform->scale = Vector2_Add(transform->scale, scale);
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

void TransformComponent_AddScaleFloats(TransformComponent *transform, float xScale, float yScale)
{
  transform->scale = Vector2_AddFloats(transform->scale, xScale, yScale);
  RegisterColliderInBroadphase(transform);
  transform->transformMatrixDirty = true;
}

/*!
 * \brief
 *   Rotates a transform component such that it points towards a given point in space.
 * \param transform
 *   The transform component to rotate.
 * \param worldPos
 *   The position in world space to make it look at.
 */
void TransformComponent_LookAt(TransformComponent *transform, Vector2 worldPos)
{
  Vector2 towards = Vector2_Sub(worldPos, transform->position);
  transform->rotation = Vector2_Normalize(towards);
}

/*!
 * \brief
 *   Gets the forward vector of the given transform component.
 * \param transform
 *   The transform component to get the forward vector of.
 * \return
 *   The forward vector of the given transform component.
 */
Vector2 TransformComponent_GetForward(const TransformComponent *transform)
{
  return transform->rotation;
}

/*!
 * \brief
 *   Gets the transformation matrix of the given transform component.
 * \param transform
 *   The transform component to get the transformation matrix of.
 * \return
 *   The transformation matrix of the given transform component.
 */
Matrix3x3* TransformComponent_GetTransformMatrix(TransformComponent *transform)
{
  bool dirty = transform->transformMatrixDirty;
  //TransformComponent *parent = transform;
  //while (parent->parent && !dirty)
  //{
  //  dirty = parent->transformMatrixDirty; //TODO: Make sure that if the parent changed since the child was last updated, the child gets that the parent is dirty
  //  parent = parent->parent;
  //}
  if (dirty)
  {
    transform->transformMatrix = Matrix3x3_BuildTransform(transform->position, TransformComponent_GetRotation(transform), transform->scale); //TODO: Parenting
    //if (transform->parent)
    //{
    //  transform->transformMatrix = Matrix3x3_Multiply(TransformComponent_GetTransformMatrix(transform->parent), &transform->transformMatrix);
    //}
    transform->transformMatrixDirty = false;
  }
  return &transform->transformMatrix;
}

/*!
 * \brief
 *   Gets the screen space transformation matrix of the given transform component.
 * \param transform
 *   The transform component to get the screen space transformation matrix of.
 * \return
 *   The screen space transformation matrix of the given transform component.
 */
Matrix3x3* TransformComponent_GetCameraTransformMatrix(TransformComponent *transform)
{
  //Vector2 camPos = Camera_GetPosition();
  //Vector2 newPos = Vector2_Sub(transform->position, camPos);
  Matrix3x3 *mx = Camera_GetMatrix();
  //mx.data[0][0] = Camera_GetRenderScaleFactor();//cameraZoom;
  //mx.data[0][1] = 0;
  //mx.data[0][2] = 0;
  //mx.data[1][0] = 0;
  //mx.data[1][1] = Camera_GetRenderScaleFactor();//cameraZoom;
  //mx.data[1][2] = 0;
  //mx.data[2][0] = 0;
  //mx.data[2][1] = 0;
  //mx.data[2][2] = Camera_GetRenderScaleFactor();//cameraZoom;
  //newPos = Matrix3x3_TransformVector(&mx, &newPos);
  //if (transform->transformMatrixDirty)
  {
    //transform->cameraTransformMatrix = Matrix3x3_BuildTransform(transform->position, TransformComponent_GetRotation(transform), transform->scale);
    //if (transform->parent)
    //{
    //  transform->cameraTransformMatrix = Matrix3x3_Multiply(&transform->cameraTransformMatrix, TransformComponent_GetCameraTransformMatrix(transform->parent));
    //}
    transform->cameraTransformMatrix = Matrix3x3_Multiply(mx, TransformComponent_GetTransformMatrix(transform));
    //transform->transformMatrixDirty = false;
  }
  return &transform->cameraTransformMatrix;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Destroy(Component *self)
{
  free(self->data);
  free(self);
}

static void OnAttach(Component *self)
{
  TransformComponent *transform = self->data;
  Component *collider = (Component*)Entity_FindComponentByType(self->parent, COMPONENT_TYPE_COLLIDER);
  if (collider)
  {
    transform->collider = collider->data;
  }
  //Component *rigidbody = (Component*)Entity_FindComponentByType(self->parent, COMPONENT_TYPE_RIGIDBODY);
  //if (rigidbody)
  //{
  //  transform->rigidbody = collider->data;
  //}
}

static void OnOtherAttach(Component *self, Component *component)
{
  TransformComponent *transform = self->data;
  if (component->type == COMPONENT_TYPE_COLLIDER)
  {
    transform->collider = component->data;
  }
  //else if (component->type == COMPONENT_TYPE_RIGIDBODY)
  //{
  //  transform->rigidbody = component->data;
  //}
}

static Component* Copy(Component *self)
{
  return TransformComponent_Copy((TransformComponent*)self->data);
}

static void SetDirty(TransformComponent *transform)
{
  UNREFERENCED_PARAMETER(transform);
  //TransformComponent *child = transform->children;
  //transform->transformMatrixDirty = true;
  //while (child)
  //{
  //  SetDirty(child);
  //  child = child->nextBrother;
  //}
}

static void RegisterColliderInBroadphase(TransformComponent *transform)
{
  if (transform->collider)
  {
    if (ColliderComponent_GetRigidbody(transform->collider))
    {
      Physics_BroadPhase_RegisterRigidbody(ColliderComponent_GetRigidbody(transform->collider));
    }
    else
    {
      Physics_BroadPhase_RegisterCollider(transform->collider);
    }
  }
}