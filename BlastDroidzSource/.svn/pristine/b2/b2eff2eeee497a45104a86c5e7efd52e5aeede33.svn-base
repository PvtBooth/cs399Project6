/******************************************************************************/
/*!
	\file   Physics_NarrowPhase.c
	\author Evan Kau
	\date   1/17/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Physics_NarrowPhase.h"
#include "ColliderComponent.h"
#include <AEEngine.h>
#include "Physics_Resolver.h"
#include "CollisionSubject.h"

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

static bool CheckCollision(ColliderComponent *first, ColliderComponent *second);

static float ProjectOnBoxAxis(ColliderComponent *boxCollider, ColliderComponent *otherCollider, Vector2 toCenter, Vector2 *bestAxis);

static float ProjectOnCircleAxis(ColliderComponent *boxCollider, ColliderComponent *otherCollider, Vector2 toCenter, Vector2 *bestAxis);

static float ProjectOnPolygonAxis(ColliderComponent *boxCollider, ColliderComponent *otherCollider, Vector2 toCenter, Vector2 *bestAxis);

static float PenetrationOnAxis(ColliderComponent *first, ColliderComponent *second, Vector2 axis, Vector2 toCenter);

static void GenerateBoxContactData(ColliderComponent *primaryCollider, ColliderComponent *secondaryCollider, Vector2 toCenter, Vector2 axis, float penetration);

static void GenerateCircleContactData(ColliderComponent *primaryCollider, ColliderComponent *secondaryCollider, Vector2 toCenter, Vector2 axis, float penetration);

static void GeneratePolygonContactData(ColliderComponent *primaryCollider, ColliderComponent *secondaryCollider, Vector2 toCenter, Vector2 axis, float penetration);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/**
 * \brief 
 * \param collider1 
 * \param collider2 
 */
bool Physics_NarrowPhase_CheckCollision(ColliderComponent *collider1, ColliderComponent *collider2)
{
  Vector2 collider1Pos = ColliderComponent_GetBoundingCircleWorldPos(collider1);
  Vector2 collider2Pos = ColliderComponent_GetBoundingCircleWorldPos(collider2);
  Vector2 collider1ToCollider2 = Vector2_Sub(collider2Pos, collider1Pos);
  float distanceSquared = Vector2_LengthSquared(collider1ToCollider2);

  if (Physics_Layers_Check(ColliderComponent_GetLayer(collider1), ColliderComponent_GetLayer(collider2)))
  {
    float sumColliderRadii = ColliderComponent_GetBoundingCircleRadius(collider1) + ColliderComponent_GetBoundingCircleRadius(collider2);
    if (sumColliderRadii * sumColliderRadii >= distanceSquared)
    {
      //Check to see that they aren't proxy colliders
      if (!ColliderComponent_IsProxy(collider1) && !ColliderComponent_IsProxy(collider2))
      {
        //Check to see if these have already been checked
        if (ColliderComponent_CheckedCollisionWith(collider1, collider2))
          return false;
        //If not, indicate that they have now been checked
        ColliderComponent_AddCollisionWith(collider1, collider2);
        //Check collision
        return CheckCollision(collider1, collider2);
      }
      else //If one or both is a proxy collider,
      {
        //Check collision
        return CheckCollision(collider1, collider2);
      }
    }
  }
  return false;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static bool CheckCollision(ColliderComponent *first, ColliderComponent *second)
{
  TransformComponent *transform1 = ColliderComponent_GetTransform(first);
  TransformComponent *transform2 = ColliderComponent_GetTransform(second);
  Vector2 toCenter;
  Vector2 position1;
  Vector2 position2;
  Vector2 bestAxis1;
  Vector2 bestAxis2;
  float penetration1;
  float penetration2;

  //Get positions
  if (ColliderComponent_GetType(first) == COLLIDER_TYPE_BOX)
  {
    position1 = ((BoxCollider*)ColliderComponent_GetCollider(first))->offset;
    position1 = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(transform1), position1);
  }
  else if (ColliderComponent_GetType(first) == COLLIDER_TYPE_CIRCLE)
  {
    position1 = ((CircleCollider*)ColliderComponent_GetCollider(first))->offset;
    position1 = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(transform1), position1);
  }
  else if (ColliderComponent_GetType(first) == COLLIDER_TYPE_CONVEX)
    position1 = TransformComponent_GetPosition(transform1);
  else
    return false; //Bad collider, exit early

  if (ColliderComponent_GetType(second) == COLLIDER_TYPE_BOX)
  {
    position2 = ((BoxCollider*)ColliderComponent_GetCollider(second))->offset;
    position2 = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(transform2), position2);
  }
  else if (ColliderComponent_GetType(second) == COLLIDER_TYPE_CIRCLE)
  {
    position2 = ((CircleCollider*)ColliderComponent_GetCollider(second))->offset;
    position2 = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(transform2), position2);
  }
  else if (ColliderComponent_GetType(second) == COLLIDER_TYPE_CONVEX)
    position2 = TransformComponent_GetPosition(transform2);
  else
    return false; //Bad collider, exit early

  //Get toCenter
  toCenter = Vector2_Sub(position1, position2);

  //Get penetrations and axis
  if (ColliderComponent_GetType(first) == COLLIDER_TYPE_BOX)
    penetration1 = ProjectOnBoxAxis(first, second, toCenter, &bestAxis1);
  else if (ColliderComponent_GetType(first) == COLLIDER_TYPE_CIRCLE)
    penetration1 = ProjectOnCircleAxis(first, second, toCenter, &bestAxis1);
  else //if (ColliderComponent_GetType(first) == COLLIDER_TYPE_CONVEX)
    penetration1 = ProjectOnPolygonAxis(first, second, toCenter, &bestAxis1);
  if (penetration1 <= 0)
    return false;

  //Reverse toCenter for the second collider
  toCenter = Vector2_Scale(toCenter, -1);

  if (ColliderComponent_GetType(second) == COLLIDER_TYPE_BOX)
    penetration2 = ProjectOnBoxAxis(second, first, toCenter, &bestAxis2);
  else if (ColliderComponent_GetType(second) == COLLIDER_TYPE_CIRCLE)
    penetration2 = ProjectOnCircleAxis(second, first, toCenter, &bestAxis2);
  else //if (ColliderComponent_GetType(second) == COLLIDER_TYPE_CONVEX)
    penetration2 = ProjectOnPolygonAxis(second, first, toCenter, &bestAxis2);
  if (penetration2 <= 0)
    return false;

  if (!ColliderComponent_GetIsTrigger(first) && !ColliderComponent_GetIsTrigger(second))
  {
    //Generate data based on which object has its vertex in the other
    if (penetration1 < penetration2)
    {
      //Reverse toCenter again
      toCenter = Vector2_Scale(toCenter, -1);
      if (ColliderComponent_GetType(second) == COLLIDER_TYPE_BOX)
        GenerateBoxContactData(second, first, toCenter, bestAxis1, penetration1);
      else if (ColliderComponent_GetType(second) == COLLIDER_TYPE_CIRCLE)
        GenerateCircleContactData(second, first, toCenter, bestAxis1, penetration1);
      else //if (ColliderComponent_GetType(second) == COLLIDER_TYPE_CONVEX)
        GeneratePolygonContactData(second, first, toCenter, bestAxis1, penetration1);
    }
    else
    {
      if (ColliderComponent_GetType(first) == COLLIDER_TYPE_BOX)
        GenerateBoxContactData(first, second, toCenter, bestAxis2, penetration2);
      else if (ColliderComponent_GetType(first) == COLLIDER_TYPE_CIRCLE)
        GenerateCircleContactData(first, second, toCenter, bestAxis2, penetration2);
      else //if (ColliderComponent_GetType(first) == COLLIDER_TYPE_CONVEX)
        GeneratePolygonContactData(first, second, toCenter, bestAxis2, penetration2);
    }
  }
  else
  {
    //CollisionSubject_NotifyTigger(ColliderComponent_GetEntity(first), ColliderComponent_GetEntity(second)); //TODO: Add a notification for a trigger
  }
  return true;
}

//bestAxis is an output
static float ProjectOnBoxAxis(ColliderComponent *boxCollider, ColliderComponent *otherCollider, Vector2 toCenter, Vector2 *bestAxis)
{
  TransformComponent *boxTransform = ColliderComponent_GetTransform(boxCollider);
  Vector2 boxAxis[2];
  float bestOverlap = ColliderComponent_GetBoundingCircleRadius(boxCollider) + ColliderComponent_GetBoundingCircleRadius(otherCollider) + 1; //Set it too high to be achievable to start with
  int bestCaseIndex = 0;
  int i;

  boxAxis[0] = Matrix3x3_GetColumn(TransformComponent_GetTransformMatrix(boxTransform), 0);
  boxAxis[1] = Matrix3x3_GetColumn(TransformComponent_GetTransformMatrix(boxTransform), 1);

  for (i = 0; i < 2; i++)
  {
    float overlap;

    boxAxis[i] = Vector2_Normalize(boxAxis[i]);

    overlap = PenetrationOnAxis(boxCollider, otherCollider, boxAxis[i], toCenter);

    //If the boxes don't intersect,
    if (overlap <= 0)
    {
      //Exit early
      return overlap;
    }
    //If this overlap is lower than the others,
    if (overlap < bestOverlap)
    {
      //It's now the axis the penetration is happening on
      bestOverlap = overlap;
      bestCaseIndex = i;
    }
  }

  *bestAxis = boxAxis[bestCaseIndex];
  return bestOverlap;
}

//bestAxis is an output
static float ProjectOnCircleAxis(ColliderComponent *circleCollider, ColliderComponent *otherCollider, Vector2 toCenter, Vector2 *bestAxis)
{
  Vector2 axis = Vector2_Normalize(toCenter);
  float overlap = ColliderComponent_GetBoundingCircleRadius(circleCollider) + ColliderComponent_GetBoundingCircleRadius(otherCollider) + 1; //Set it too high to be achievable to start with

  overlap = PenetrationOnAxis(circleCollider, otherCollider, axis, toCenter);

  //If the boxes don't intersect,
  if (overlap <= 0)
  {
    //Exit early
    return overlap;
  }

  *bestAxis = axis;
  return overlap;
}

//bestAxis is an output
static float ProjectOnPolygonAxis(ColliderComponent *polygonCollider, ColliderComponent *otherCollider, Vector2 toCenter, Vector2 *bestAxis)
{
  TransformComponent *polygonTransform = ColliderComponent_GetTransform(polygonCollider);
  PolygonalCollider *polygon = ColliderComponent_GetCollider(polygonCollider);
  float bestOverlap = ColliderComponent_GetBoundingCircleRadius(polygonCollider) + ColliderComponent_GetBoundingCircleRadius(otherCollider) + 1; //Set it too high to be achievable to start with
  //int bestCaseIndex = 0;
  int i;

  for (i = 0; i < polygon->vertexCount; i++)
  {
    float overlap;

    Vector2 axis = Vector2_Normalize(Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(polygonTransform), polygon->axisList[i].axis));

    overlap = PenetrationOnAxis(polygonCollider, otherCollider, axis, toCenter);

    //If the boxes don't intersect,
    if (overlap <= 0)
    {
      //Exit early
      return overlap;
    }
    //If this overlap is lower than the others,
    if (overlap < bestOverlap)
    {
      //It's now the axis the penetration is happening on
      bestOverlap = overlap;
      *bestAxis = axis;
    }
  }

  return bestOverlap;
}

static float PenetrationOnAxis(ColliderComponent *first, ColliderComponent *second, Vector2 axis, Vector2 toCenter)
{
  Vector2 negToCenter = Vector2_Scale(toCenter, -1);
  float firstProjection = ColliderComponent_TransformToAxis(first, axis, negToCenter);
  float secondProjection = ColliderComponent_TransformToAxis(second, axis, toCenter);
  float distance = (float)fabs(Vector2_Dot(toCenter, axis));

  return firstProjection + secondProjection - distance;
}

static void GenerateBoxContactData(ColliderComponent *primaryCollider, ColliderComponent *secondaryCollider, Vector2 toCenter, Vector2 axis, float penetration)
{
  TransformComponent *primaryColliderTransform = ColliderComponent_GetTransform(primaryCollider);
  Matrix3x3 *primaryColliderTransformMatrix = TransformComponent_GetTransformMatrix(primaryColliderTransform);
  Vector2 primaryColliderAxis1 = Matrix3x3_GetColumn(primaryColliderTransformMatrix, 0);
  Vector2 primaryColliderAxis2 = Matrix3x3_GetColumn(primaryColliderTransformMatrix, 1);
  Vector2 collisionNormal = axis;
  Vector2 collisionPoint = ((BoxCollider*)ColliderComponent_GetCollider(primaryCollider))->halfSize;

  //If the normal is facing the toward the second object, reverse it
  if (Vector2_Dot(collisionNormal, toCenter) > 0)
  {
    collisionNormal = Vector2_Scale(collisionNormal, -1);
  }

  if (Vector2_Dot(primaryColliderAxis1, collisionNormal) > 0)
  {
    collisionPoint.x *= -1;
  }
  if (Vector2_Dot(primaryColliderAxis2, collisionNormal) > 0)
  {
    collisionPoint.y *= -1;
  }

  //We want the collision point to be in world coordinates.
  collisionPoint = Matrix3x3_TransformPoint(primaryColliderTransformMatrix, collisionPoint);

  Physics_Resolver_AddContact(primaryCollider, secondaryCollider, collisionPoint, collisionNormal, penetration);
}

static void GenerateCircleContactData(ColliderComponent *primaryCollider, ColliderComponent *secondaryCollider, Vector2 toCenter, Vector2 axis, float penetration)
{
  TransformComponent *primaryColliderTransform = ColliderComponent_GetTransform(primaryCollider);
  TransformComponent *secondaryColliderTransform = ColliderComponent_GetTransform(secondaryCollider);
  Vector2 primaryPosition = TransformComponent_GetPosition(primaryColliderTransform);
  Vector2 secondaryPosition = TransformComponent_GetPosition(secondaryColliderTransform);
  Vector2 collisionNormal = axis;
  Vector2 collisionPoint = Vector2_Average(primaryPosition, secondaryPosition);

  //If the normal is facing the toward the second object, reverse it
  if (Vector2_Dot(collisionNormal, toCenter) > 0)
  {
    collisionNormal = Vector2_Scale(collisionNormal, -1);
  }

  Physics_Resolver_AddContact(primaryCollider, secondaryCollider, collisionPoint, collisionNormal, penetration);
}

//Assumes the primary collider is the polygon
static void GeneratePolygonContactData(ColliderComponent *primaryCollider, ColliderComponent *secondaryCollider, Vector2 toCenter, Vector2 axis, float penetration)
{
  TransformComponent *primaryColliderTransform = ColliderComponent_GetTransform(primaryCollider);
//  TransformComponent *secondaryColliderTransform = ColliderComponent_GetTransform(secondaryCollider);
  PolygonalCollider *polygon = ColliderComponent_GetCollider(primaryCollider);
  Vector2 primaryPosition = TransformComponent_GetPosition(primaryColliderTransform);
  //Vector2 secondaryPosition = TransformComponent_GetPosition(secondaryColliderTransform);
  Vector2 collisionNormal = (Vector2_Dot(axis, toCenter) > 0) ? Vector2_Scale(axis, -1) : axis;
  Vector2 collisionPoint = {0, 0};// = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(primaryColliderTransform), &polygon->mesh[0]);
  float distance = 10000;// = Vector2_Dot(&collisionPoint, &collisionNormal);
  int i;

  //If the normal is facing the toward the second object, reverse it
  if (Vector2_Dot(collisionNormal, toCenter) > 0)
  {
    collisionNormal = Vector2_Scale(collisionNormal, -1);
  }

  for (i = 0; i < polygon->vertexCount; i++)
  {
    Vector2 point = Matrix3x3_TransformVector(TransformComponent_GetTransformMatrix(primaryColliderTransform), polygon->mesh[i]);
    float length = Vector2_Dot(point, collisionNormal);
    if (length < distance)
    {
      collisionPoint = point;
      distance = length;
    }
  }

  //Translate the point to world coordinates
  collisionPoint = Vector2_Add(collisionPoint, primaryPosition);

  Physics_Resolver_AddContact(primaryCollider, secondaryCollider, collisionPoint, collisionNormal, penetration);
}
