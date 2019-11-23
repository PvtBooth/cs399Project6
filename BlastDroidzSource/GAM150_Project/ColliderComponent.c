/******************************************************************************/
/*!
	\file   ColliderComponent.c
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "ColliderComponent.h"
#include "Vector2.h"
#include <stdlib.h>
#include <math.h>
#include "Matrix3x3.h"
#include "Entity.h"
#include "Physics_BroadPhase.h"
#include "PhysicsManager.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define PHYSICS_COLLIDER_COLLIDING_WITH_INIT_NUM 4

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct BoundingCircle
{
  float radius;
  Vector2 offset;
  Vector2 gridMin;
  Vector2 gridMax;
} BoundingCircle;

typedef struct PhysicsMaterial
{
  float restitution;
} PhysicsMaterial;

typedef struct ColliderComponent
{
  unsigned int id;
  COLLIDER_TYPE type;
  RigidbodyComponent *rigidbody;
  void *collider;
  BoundingCircle boundingCircle;
  TransformComponent *transform;
  unsigned int *collidingWith;
  int collidingWithMax;
  int collidingWithCount;
  PhysicsMaterial physicsMaterial;
  PhysicsLayer layer;
  bool isTrigger;
  bool proxy;

  Component *component;
} ColliderComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static unsigned int currentID = 0;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void CreateColliderBase(ColliderComponent *collider);

static void Update(Component *self, float dt);

static void Destroy(Component *self);

static void OnAttach(Component *self);

static void OnOtherAttach(Component *self, Component *component);

static Component* Copy(Component *self);

static float TransformBoxToAxis(TransformComponent *transform, const BoxCollider *box, Vector2 axis);

static float TransformCircleToAxis(TransformComponent *transform, const CircleCollider *circle);

static float TransformPolygonToAxis(TransformComponent *transform, const PolygonalCollider *circle, Vector2 axis, Vector2 toCenter);

static void RaycastAgainstBox(RaycastData *raycastData, ColliderComponent *collider);

static void RaycastAgainstCircle(RaycastData *raycastData, ColliderComponent *collider);

static void RaycastAgainstPolygon(RaycastData *raycastData, ColliderComponent *collider);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *  Creates a component with a box collider with a given half size and offset.
 * \param halfSize
 *   The half size of the box. [Vector2 = (half width, half height)]
 * \param offset
 *   The offset of the box on the object. Offsets have not been properly tested.
 * \param isTrigger
 *   Whether or not the given collider should be a trigger or not. A trigger does not physically
 *   interact with other objects but will create collision events.
 * \return
 *   A pointer to a new component with a box collider for data.
 */
Component* ColliderComponent_CreateBox(Vector2 halfSize, Vector2 offset, bool isTrigger)
{
  Component *component = Component_Create();
  ColliderComponent *collider = (ColliderComponent*)malloc(sizeof(ColliderComponent));
  BoxCollider *box = (BoxCollider*)malloc(sizeof(BoxCollider));

  CreateColliderBase(collider);

  box->halfSize = halfSize;
  box->offset = offset;

  collider->type = COLLIDER_TYPE_BOX;
  collider->collider = box;
  collider->boundingCircle.offset = offset;
  collider->boundingCircle.radius = sqrtf(halfSize.x * halfSize.x + halfSize.y * halfSize.y);
  collider->isTrigger = isTrigger;

  collider->component = component;

  component->data = collider;
  component->update = &Update;
  component->destroy = &Destroy;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;
  component->type = COMPONENT_TYPE_COLLIDER;

  return component;
}

/*!
 * \brief
 *  Creates a component with a circle collider with a given radius and offset.
 * \param radius
 *   The radius of the circle.
 * \param offset
 *   The offset of the box on the object. Offsets have not been properly tested.
 * \param isTrigger
 *   Whether or not the given collider should be a trigger or not. A trigger does not physically
 *   interact with other objects but will create collision events.
 * \return
 *   A pointer to a new component with a circle collider for data.
 */
Component* ColliderComponent_CreateCircle(float radius, Vector2 offset, bool isTrigger)
{
  Component *component = Component_Create();
  ColliderComponent *collider = (ColliderComponent*)malloc(sizeof(ColliderComponent));
  CircleCollider *circle = (CircleCollider*)malloc(sizeof(CircleCollider));

  CreateColliderBase(collider);

  circle->radius = radius;
  circle->offset = offset;

  collider->type = COLLIDER_TYPE_CIRCLE;
  collider->collider = circle;
  collider->boundingCircle.offset = offset;
  collider->boundingCircle.radius = radius;
  collider->isTrigger = isTrigger;

  collider->component = component;

  component->data = collider;
  component->update = &Update;
  component->destroy = &Destroy;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;
  component->type = COMPONENT_TYPE_COLLIDER;

  return component;
}

/*!
* \brief
*  Creates a component with a convex polygonal collider with a given outer mesh.
* \param mesh
*   An array of verticies that form the polygonal collider. It is assumed that they are in order, though the direction does not matter.
* \param vertexCount
*   The number of verticies in the mesh.
* \param isTrigger
*   Whether or not the given collider should be a trigger or not. A trigger does not physically
*   interact with other objects but will create collision events.
* \return
*   A pointer to a new component with a polygonal collider for data.
*/
Component* ColliderComponent_CreatePolygon(const Vector2 *mesh, int vertexCount, bool isTrigger)
{
  Component *component = Component_Create();
  ColliderComponent *collider = (ColliderComponent*)malloc(sizeof(ColliderComponent));
  PolygonalCollider *polygon = (PolygonalCollider*)malloc(sizeof(PolygonalCollider));
  int i;
  float farthestVertexDistance = 0;

  CreateColliderBase(collider);

  polygon->mesh = (Vector2*)malloc(sizeof(Vector2) * vertexCount);
  for (i = 0; i < vertexCount; i++)
    polygon->mesh[i] = mesh[i];
  polygon->vertexCount = vertexCount;
  polygon->axisList = (PolygonColliderEdge*)malloc(sizeof(PolygonColliderEdge) * vertexCount);

  for (i = 0; i < vertexCount; i++)
  {
    Vector2 point1ToOrigin;
    Vector2 nearestToOrigin;
    Vector2 direction;
    float distance = Vector2_Distance(mesh[i], Vector2_Zero());

    if (distance > farthestVertexDistance)
    {
      farthestVertexDistance = distance;
    }
    polygon->axisList[i].axis = Vector2_Sub(mesh[(i + 1) % vertexCount], mesh[i]);
    polygon->axisList[i].axis = Vector2_Normalize(polygon->axisList[i].axis);
    polygon->axisList[i].axis = Vector2_GetOrthogonal(polygon->axisList[i].axis);

    polygon->axisList[i].point1 = mesh[i];
    polygon->axisList[i].point2 = mesh[(i + 1) % vertexCount];
    polygon->axisList[i].length = Vector2_Distance(mesh[(i + 1) % vertexCount], mesh[i]);

    if (Vector2_CrossVectorVector(polygon->axisList[i].point1, polygon->axisList[i].point2) > 0)
      polygon->axisList[i].axis = Vector2_Scale(polygon->axisList[i].axis, -1);

    point1ToOrigin = Vector2_Scale(polygon->axisList[i].point1, -1); //Vector pointing from point1 to (0, 0)
    direction = Vector2_Sub(polygon->axisList[i].point2, polygon->axisList[i].point1);
    direction = Vector2_Normalize(direction);
    nearestToOrigin = Vector2_Scale(direction, Vector2_Dot(point1ToOrigin, direction));

    if (polygon->axisList[i].length < Vector2_Dot(point1ToOrigin, direction)) //If past point2,
    {
      nearestToOrigin = polygon->axisList[i].point2; //Set to point2
    }
    else if (Vector2_Dot(point1ToOrigin, direction) < 0) //If before point1
    {
      nearestToOrigin = polygon->axisList[i].point1; //Set to point1
    }

    polygon->axisList[i].nearestToOrigin = nearestToOrigin;
  }

  collider->type = COLLIDER_TYPE_CONVEX;
  collider->collider = polygon;
  collider->boundingCircle.offset = Vector2_Zero();
  collider->boundingCircle.radius = farthestVertexDistance;
  collider->isTrigger = isTrigger;

  collider->component = component;

  component->data = collider;
  component->update = &Update;
  component->destroy = &Destroy;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->copy = &Copy;
  component->type = COMPONENT_TYPE_COLLIDER;

  return component;
}

Component* ColliderComponent_Copy(ColliderComponent *collider)
{
  Component *newComponent;
  ColliderComponent *newCollider;

  if (collider->type == COLLIDER_TYPE_BOX)
  {
    BoxCollider *box = collider->collider;
    newComponent = ColliderComponent_CreateBox(box->halfSize, box->offset, collider->isTrigger);
  }
  else if (collider->type == COLLIDER_TYPE_CIRCLE)
  {
    CircleCollider *circle = collider->collider;
    newComponent = ColliderComponent_CreateCircle(circle->radius, circle->offset, collider->isTrigger);
  }
  else //if (collider->type == COLLIDER_TYPE_CONVEX)
  {
    PolygonalCollider *polygon = collider->collider;
    newComponent = ColliderComponent_CreatePolygon(polygon->mesh, polygon->vertexCount, collider->isTrigger);
  }

  newCollider = (ColliderComponent*)newComponent->data;
  newCollider->layer = collider->layer;
  newCollider->physicsMaterial.restitution = collider->physicsMaterial.restitution;

  return newComponent;
}

void ColliderComponent_CreateProxyBox(ColliderComponent *proxyCollider, BoxCollider *proxyBox, TransformComponent *proxyTransform, Vector2 halfSize, Vector2 offset)
{
  proxyBox->halfSize = halfSize;
  proxyBox->offset = offset;

  proxyCollider->type = COLLIDER_TYPE_BOX;
  proxyCollider->collider = proxyBox;
  proxyCollider->boundingCircle.offset = offset;
  proxyCollider->boundingCircle.radius = Vector2_Length(halfSize);
  proxyCollider->isTrigger = true;
  proxyCollider->transform = proxyTransform;
  proxyCollider->id = currentID++;
  proxyCollider->layer = PHYSICS_LAYER_DEFAULT;
  proxyCollider->proxy = true;
}

void ColliderComponent_CreateProxyCircle(ColliderComponent *proxyCollider, CircleCollider *proxyCircle, TransformComponent *proxyTransform, float radius, Vector2 offset)
{
  proxyCircle->radius = radius;
  proxyCircle->offset = offset;

  proxyCollider->type = COLLIDER_TYPE_CIRCLE;
  proxyCollider->collider = proxyCircle;
  proxyCollider->boundingCircle.offset = offset;
  proxyCollider->boundingCircle.radius = radius;
  proxyCollider->isTrigger = true;
  proxyCollider->transform = proxyTransform;
  proxyCollider->id = currentID++;
  proxyCollider->layer = PHYSICS_LAYER_DEFAULT;
  proxyCollider->proxy = true;
}

/*!
 * \brief
 *   Projects a given collider of any type onto a given axis.
 * \param collider
 *   The collider to project.
 * \param axis
 *   The axis to project the collider onto.
 * \param toCenter
 *   The direction from this collider to the other relavant collider. Only used for polygonal colliders.
 * \return
 *   The amount that the given object is projecting towards the other object, as defined by toCenter.
 */
float ColliderComponent_TransformToAxis(const ColliderComponent *collider, Vector2 axis, Vector2 toCenter)
{
  if (collider->type == COLLIDER_TYPE_BOX)
  {
    return TransformBoxToAxis(ColliderComponent_GetTransform(collider), collider->collider, axis);
  }
  else if (collider->type == COLLIDER_TYPE_CIRCLE)
  {
    return TransformCircleToAxis(ColliderComponent_GetTransform(collider), collider->collider);
  }
  else if (collider->type == COLLIDER_TYPE_CONVEX)
  {
    return TransformPolygonToAxis(ColliderComponent_GetTransform(collider), collider->collider, axis, toCenter);
  }
  else
  {
    return 0;
  }
}

/*!
 * \brief
 *   Runs a raycast check against a given collider of any type.
 * \param raycastData
 *   The information about the raycast, which may be modified if it hits the collider.
 * \param collider
 *   The collider to check against.
 */
void ColliderComponent_RaycastAgainstCollider(RaycastData *raycastData, ColliderComponent *collider)
{
  if (collider->isTrigger)
    return;
  if (ColliderComponent_GetType(collider) == COLLIDER_TYPE_BOX)
  {
    RaycastAgainstBox(raycastData, collider);
  }
  else if (ColliderComponent_GetType(collider) == COLLIDER_TYPE_CIRCLE)
  {
    RaycastAgainstCircle(raycastData, collider);
  }
  else if (ColliderComponent_GetType(collider) == COLLIDER_TYPE_CONVEX)
  {
    RaycastAgainstPolygon(raycastData, collider);
  }
}

/*!
 * \brief
 *   Indicates that the first collider is colliding with the second.
 * \param this
 *   The collider to modify, and add the other to its list of collisions.
 * \param other
 *   The collider to add to the first's list of collisions.
 */
void ColliderComponent_AddCollisionWith(ColliderComponent *this, const ColliderComponent *other)
{
  if (this->collidingWithCount == this->collidingWithMax)
  {
    this->collidingWith = (unsigned int*)realloc(this->collidingWith, sizeof(unsigned int) * this->collidingWithMax * 2);
    this->collidingWithMax *= 2;
  }
  this->collidingWith[this->collidingWithCount] = other->id;
  this->collidingWithCount++;
}

/*!
 * \brief
 *   Checks against both colliders' list of collisions to determine if they have already been checked against each other.
 * \param this
 *   The first collider being checked.
 * \param other
 *   The second collider being checked.
 * \return
 *   Returns true if the two are already colliding with each other, false otherwise.
 */
bool ColliderComponent_CheckedCollisionWith(const ColliderComponent *this, const ColliderComponent *other)
{
  int i;
  for (i = 0; i < this->collidingWithCount; i++)
  {
    if (other->id == this->collidingWith[i])
    {
      return true;
    }
  }
  for (i = 0; i < other->collidingWithCount; i++)
  {
    if (this->id == other->collidingWith[i])
    {
      return true;
    }
  }
  return false;
}

/*!
 * \brief
 *   Gets the ID of a given collider.
 * \param collider
 *   The collider to get the ID of.
 * \return
 *   The ID of that collider.
 */
unsigned int ColliderComponent_GetID(const ColliderComponent *collider)
{
  return collider->id;
}

/*!
* \brief
*   Gets the type of a given collider.
* \param collider
*   The collider to get the type of.
* \return
*   The type of that collider.
*/
COLLIDER_TYPE ColliderComponent_GetType(const ColliderComponent *collider)
{
  return collider->type;
}

/*!
* \brief
*   Gets the transform component of a given collider.
* \param collider
*   The collider to get the transform component of.
* \return
*   The transform component of that collider.
*/
TransformComponent* ColliderComponent_GetTransform(const ColliderComponent *collider)
{
  return collider->transform;
}

/*!
* \brief
*   Gets the bounding circle of a given collider.
* \param collider
*   The collider to get the bounding circle of.
* \return
*   The bounding circle of that collider.
*/
const BoundingCircle* ColliderComponent_GetBoundingCircle(const ColliderComponent *collider)
{
  return &collider->boundingCircle;
}

/*!
* \brief
*   Gets the rigidbody component of a given collider.
* \param collider
*   The collider to get the rigidbody component of.
* \return
*   The rigidbody component of that collider.
*/
RigidbodyComponent* ColliderComponent_GetRigidbody(const ColliderComponent *collider)
{
  return collider->rigidbody;
}

/*!
* \brief
*   Gets the collider data of a given collider.
* \param collider
*   The collider to get the collider data of.
* \return
*   The a void pointer to the collider data of that collider.
*/
void* ColliderComponent_GetCollider(const ColliderComponent *collider)
{
  return (void*)collider->collider;
}

/*!
* \brief
*   Gets the world position of the bounding circle of a given collider.
* \param collider
*   The collider to get the world position of the bounding circle of.
* \return
*   The world position of the bounding circle of that collider.
*/
Vector2 ColliderComponent_GetBoundingCircleWorldPos(const ColliderComponent *collider)
{
  Vector2 position = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(collider->transform), collider->boundingCircle.offset);
  return position;
}

/*!
* \brief
*   Gets the radius of the bounding circle of a given collider.
* \param collider
*   The collider to get the radius of the bounding circle of.
* \return
*   The radius of the bounding circle of that collider.
*/
float ColliderComponent_GetBoundingCircleRadius(const ColliderComponent *collider)
{
  if(!collider)
  {
    return 0.0f;
  }
  float scale = max(TransformComponent_GetScale(collider->transform).x, TransformComponent_GetScale(collider->transform).y);
  return collider->boundingCircle.radius * scale;
}

/*!
* \brief
*   Gets the minimum position of the bounding circle in the grid of a given collider.
* \param collider
*   The collider to get the minimum position of the bounding circle in the grid of.
* \return
*   The minimum position of the bounding circle in the grid of that collider.
*/
Vector2* ColliderComponent_GetBoundingCircleMin(ColliderComponent *collider)
{
  return &collider->boundingCircle.gridMin;
}

/*!
* \brief
*   Gets the maximum position of the bounding circle in the grid of a given collider.
* \param collider
*   The collider to get the maximum position of the bounding circle in the grid of.
* \return
*   The maximum position of the bounding circle in the grid of that collider.
*/
Vector2* ColliderComponent_GetBoundingCircleMax(ColliderComponent *collider)
{
  return &collider->boundingCircle.gridMax;
}

/*!
* \brief
*   Gets the layer of a given collider.
* \param collider
*   The collider to get the layer of.
* \return
*   The layer of that collider.
*/
PhysicsLayer ColliderComponent_GetLayer(const ColliderComponent *collider)
{
  return collider->layer;
}

/*!
* \brief
*   Sets the layer of a given collider.
* \param collider
*   The collider to set the layer of.
* \param physicsLayer
*   The layer to set the collider to.
*/
void ColliderComponent_SetLayer(ColliderComponent *collider, PhysicsLayer physicsLayer)
{
  collider->layer = physicsLayer;
}

/*!
* \brief
*   Gets if the collider is a trigger or not.
* \param collider
*   The collider to get the isTrigger value of of.
* \return
*   Returns true if the collider is a trigger, or false if it isn't.
*/
bool ColliderComponent_GetIsTrigger(const ColliderComponent *collider)
{
  return collider->isTrigger;
}

/*!
* \brief
*   Gets the entity of a given collider.
* \param collider
*   The collider to get the entity of.
* \return
*   The entity of that collider.
*/
Entity* ColliderComponent_GetEntity(const ColliderComponent *collider)
{
  return collider->component->parent;
}

/*!
* \brief
*   Sets the rigidbody of a given collider.
* \param collider
*   The collider to set the rigidbody of.
* \param rigidbody
*   The rigidbody to set to the collider.
*/
void ColliderComponent_SetRigidbody(ColliderComponent *collider, RigidbodyComponent *rigidbody)
{
  collider->rigidbody = rigidbody;
}

/*!
* \brief
*   Set the trigger value of the given collider.
* \param collider
*   The collider to set the isTrigger value of.
* \param isTrigger
*   The value to set the collider's isTrigger value to.
*/
void ColliderComponent_SetIsTrigger(ColliderComponent *collider, bool isTrigger)
{
  collider->isTrigger = isTrigger;
}

/*!
 * \brief
 *   Gets the restitution from the physics material of the collider.
 * \param collider
 *   The collider to get the restitution from.
 * \return
 *   The restitution of the physics material on the collider.
 */
float ColliderComponent_GetRestitution(const ColliderComponent *collider)
{
  return collider->physicsMaterial.restitution;
}

/*!
 * \brief
 *   Sets the restitution on the physics material of the collider. It is highly reccomended that you keep
 *   the new values between 0 and 1.
 * \param collider
 *   The collider to set the restitution of.
 * \param restitution
 *   The new restitution to use.
 */
void ColliderComponent_SetRestitution(ColliderComponent *collider, float restitution)
{
  collider->physicsMaterial.restitution = restitution;
}

/*!
 * \brief
 *   Returns if a collider is a proxy collider or not.
 * \param collider
 *   The collider to check.
 * \return
 *   true if the collider is a proxy, false if it isn't a proxy.
 */
bool ColliderComponent_IsProxy(ColliderComponent *collider)
{
  return collider->proxy;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void CreateColliderBase(ColliderComponent *collider)
{
  collider->id = currentID++;
  collider->boundingCircle.gridMin.x = 0;
  collider->boundingCircle.gridMin.y = 0;
  collider->boundingCircle.gridMax.x = 0;
  collider->boundingCircle.gridMax.y = 0;
  collider->collidingWith = (unsigned int*)malloc(sizeof(unsigned int) * PHYSICS_COLLIDER_COLLIDING_WITH_INIT_NUM);
  collider->collidingWithMax = PHYSICS_COLLIDER_COLLIDING_WITH_INIT_NUM;
  collider->collidingWithCount = 0;
  collider->physicsMaterial.restitution = 0.25f;
  collider->layer = PHYSICS_LAYER_DEFAULT;
  collider->proxy = false;
}

static void Update(Component *self, float dt)
{
    UNREFERENCED_PARAMETER(dt);
  ColliderComponent *collider = (ColliderComponent*)self->data;
  collider->collidingWithCount = 0;
  //If this collider is static,
  if (!collider->rigidbody)
  {
    //Put it in the broad phase grid
    Physics_BroadPhase_RegisterCollider(collider);
  }
}

static void Destroy(Component *self)
{
  ColliderComponent *collider = self->data;
  if (!collider->rigidbody)
  {
    Physics_BroadPhase_RemoveCollider(collider);
  }
  else
  {
    Physics_BroadPhase_RemoveRigidbody(collider->rigidbody);
  }
  free(collider->collidingWith);
  if (collider->type == COLLIDER_TYPE_CONVEX)
  {
    PolygonalCollider *polygon = collider->collider;
    free(polygon->mesh);
    free(polygon->axisList);
  }
  free(collider->collider);
  free(collider);
  free(self);
}

static void OnAttach(Component *self)
{
  Component *transComponent = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  ColliderComponent *collider = self->data;
  Component *rigidbody;

  if (transComponent)
  {
    collider->transform = (TransformComponent*)transComponent->data;
  }

  rigidbody = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_RIGIDBODY);
  if (rigidbody)
  {
    collider->rigidbody = rigidbody->data;
    RigidbodyComponent_SetCollider(collider->rigidbody, collider);
  }
  else
  {
    collider->rigidbody = NULL;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    ColliderComponent *collider = (ColliderComponent*)self->data;
    collider->transform = (TransformComponent*)component->data;
  }
}

static Component* Copy(Component *self)
{
  return ColliderComponent_Copy((ColliderComponent*)self->data);
}

static float TransformBoxToAxis(TransformComponent *transform, const BoxCollider *box, Vector2 axis)
{
  Vector2 boxAxis1 = Matrix3x3_GetColumn(TransformComponent_GetTransformMatrix(transform), 0);
  Vector2 boxAxis2 = Matrix3x3_GetColumn(TransformComponent_GetTransformMatrix(transform), 1);
  return box->halfSize.x * fabsf(Vector2_Dot(boxAxis1, axis)) + box->halfSize.y * fabsf(Vector2_Dot(boxAxis2, axis));
}

static float TransformCircleToAxis(TransformComponent *transform, const CircleCollider *circle)
{
  return circle->radius * (float)max(TransformComponent_GetScale(transform).x, TransformComponent_GetScale(transform).y);
}

static float TransformPolygonToAxis(TransformComponent *transform, const PolygonalCollider *polygon, Vector2 axis, Vector2 toCenter)
{
  float projectionLengthMin = 0;
  float projectionLengthMax = 0;
  //float projectionLength = 0;
  int i;
  if (Vector2_Dot(axis, toCenter) > 0)
  {
    for (i = 0; i < polygon->vertexCount; i++)
    {
      Vector2 transformedPoint = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(transform), polygon->mesh[i]);
      float point = Vector2_Dot(transformedPoint, axis);
      if (point > projectionLengthMax)
        projectionLengthMax = point;
    }
    return fabsf(projectionLengthMax);
  }
  else
  {
    for (i = 0; i < polygon->vertexCount; i++)
    {
      Vector2 transformedPoint = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(transform), polygon->mesh[i]);
      float point = Vector2_Dot(transformedPoint, axis);
      if (point < projectionLengthMin)
        projectionLengthMin = point;
    }
    return fabsf(projectionLengthMin);
  }

  //if (Vector2_Dot(axis, toCenter) > 0)
  //  return (float)fabs(projectionLengthMax);
  //else
  //  return (float)fabs(projectionLengthMin);

  //return projectionLength;
}

static void RaycastAgainstBox(RaycastData *raycastData, ColliderComponent *collider)
{
  BoxCollider *box = collider->collider;
  float firstPoint = 0;
  float lastPoint = 1;
  Vector2 axis[4];
  Vector2 points[4];
  Vector2 rayDirection = Vector2_Sub(raycastData->point, raycastData->origin);
  int bestIndex = 0;
  int i;

  axis[0].x = 1;
  axis[0].y = 0;
  axis[1].x = 0;
  axis[1].y = 1;
  axis[2].x = -1;
  axis[2].y = 0;
  axis[3].x = 0;
  axis[4].y = -1;
  points[0] = box->halfSize;
  points[1] = box->halfSize;
  points[2] = Vector2_Scale(box->halfSize, -1);
  points[3] = Vector2_Scale(box->halfSize, -1);
  for (i = 0; i < 4; i++)
  {
    axis[i] = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(ColliderComponent_GetTransform(collider)), axis[i]);
    axis[i] = Vector2_Normalize(axis[i]);
    points[i] = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(ColliderComponent_GetTransform(collider)), points[i]);
  }
  
  for (i = 0; i < 4; i++)
  {
    float lineDotProduct = Vector2_Dot(axis[i], points[i]);
    float denominator = Vector2_Dot(axis[i], rayDirection);
    float distance = lineDotProduct - Vector2_Dot(axis[i], raycastData->origin);
    if (denominator == 0)
    {
      if (distance < 0)
      {
        return;
      }
    }
    else
    {
      float hitPoint = distance / denominator; // 0-1, 1 being the end of the ray
      if (denominator < 0)
      {
        if (hitPoint > firstPoint)
        {
          firstPoint = hitPoint;
          bestIndex = i;
        }
      }
      else
      {
        if (hitPoint < lastPoint)
        {
          lastPoint = hitPoint;
        }
      }
    }
    if (firstPoint > lastPoint)
    {
      return;
    }
  }
  //If we hit the object,
  raycastData->point = Vector2_Scale(rayDirection, firstPoint);
  raycastData->point = Vector2_Add(raycastData->point, raycastData->origin);
  raycastData->transform = collider->transform;
  raycastData->rigidbody = collider->rigidbody;
  raycastData->collider = collider;
  raycastData->entity = ColliderComponent_GetEntity(raycastData->collider);
  raycastData->normal = axis[bestIndex];
  raycastData->reflection = Vector2_GetOrthogonal(raycastData->normal);
  raycastData->reflection = Vector2_Scale(raycastData->reflection, Vector2_Dot(raycastData->reflection, raycastData->direciton) * 2);
  raycastData->reflection = Vector2_Sub(raycastData->reflection, raycastData->direciton);
  raycastData->distance = raycastData->distance * firstPoint;
}

static void RaycastAgainstCircle(RaycastData *raycastData, ColliderComponent *collider)
{
    UNREFERENCED_PARAMETER(collider);
    UNREFERENCED_PARAMETER(raycastData);
  return;
}

static void RaycastAgainstPolygon(RaycastData *raycastData, ColliderComponent *collider)
{
  PolygonalCollider *polygon = collider->collider;
  float firstPoint = 0;
  float lastPoint = 1;
  Vector2 rayDirection = Vector2_Sub(raycastData->point, raycastData->origin);
  int bestIndex = 0;
  int i;

  for (i = 0; i < polygon->vertexCount; i++)
  {
    Vector2 axis = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(ColliderComponent_GetTransform(collider)), polygon->axisList[i].axis);
    Vector2 point = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(ColliderComponent_GetTransform(collider)), polygon->axisList[i].point1);
    float lineDotProduct = Vector2_Dot(axis, point);
    float denominator = Vector2_Dot(axis, rayDirection);
    float distance = lineDotProduct - Vector2_Dot(axis, raycastData->origin);

    if (denominator == 0)
    {
      if (distance < 0)
      {
        return;
      }
    }
    else
    {
      float hitPoint = distance / denominator; // 0-1, 1 being the end of the ray
      if (denominator < 0)
      {
        if (hitPoint > firstPoint)
        {
          firstPoint = hitPoint;
          bestIndex = i;
        }
      }
      else
      {
        if (hitPoint < lastPoint)
        {
          lastPoint = hitPoint;
        }
      }
    }
    if (firstPoint > lastPoint)
    {
      return;
    }
  }
  //If we hit the object,
  raycastData->point = Vector2_Scale(rayDirection, firstPoint);
  raycastData->point = Vector2_Add(raycastData->point, raycastData->origin);
  raycastData->transform = collider->transform;
  raycastData->rigidbody = collider->rigidbody;
  raycastData->collider = collider;
  raycastData->entity = ColliderComponent_GetEntity(raycastData->collider);
  raycastData->normal = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(ColliderComponent_GetTransform(collider)), polygon->axisList[bestIndex].axis);
  raycastData->reflection = Vector2_GetOrthogonal(raycastData->normal);
  raycastData->reflection = Vector2_Scale(raycastData->reflection, Vector2_Dot(raycastData->reflection, raycastData->direciton) * 2);
  raycastData->reflection = Vector2_Sub(raycastData->reflection, raycastData->direciton);
  raycastData->distance = raycastData->distance * firstPoint;
}