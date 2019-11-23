/***
 * Project: alphaphysics
 * File: Physics_Narrow.c
 * Date: 1/9/2017
 * Created by Evan Kau
 * Description: 
***/

#include "Physics_Narrow.h"

static float PenetrationOnAxis(PhysicsObject *object1, PhysicsObject *object2, AEVec2 *axis, AEVec2 *toCenter);

static void CreateContact(PhysicsObject *primaryObject, PhysicsObject *secondaryObject, Contact *contact, AEVec2 *toCenter, AEVec2(*axis)[4], float bestOverlap, int bestCaseIndex);

BOOL Physics_Narrow_GenerateContact(PhysicsObject *object1, PhysicsObject *object2, Contact *contact)
{
  AEVec2 toCenter;
  AEVec2 object1Position;
  AEVec2 object2Position;
  AEVec2 axis[4];
  float bestOverlap = 1000000.0f;
  int bestCaseIndex = 0;
  int i;

  PhysicsObject_GetPosition(object1, &object1Position);
  PhysicsObject_GetPosition(object2, &object2Position);

  //Get the vector pointing towards the second object from the first
  AEVec2Sub(&toCenter, &object1Position, &object2Position);

  //Get each axis of the first object
  PhysicsObject_GetAxis(object1, &axis[0], 0);
  PhysicsObject_GetAxis(object1, &axis[1], 1);

  //Get each axis of the second object
  PhysicsObject_GetAxis(object2, &axis[2], 0);
  PhysicsObject_GetAxis(object2, &axis[3], 1);

  for (i = 0; i < 4; i++)
  {
    float overlap;

    AEVec2Normalize(&axis[i], &axis[i]);

    overlap = PenetrationOnAxis(object1, object2, &axis[i], &toCenter);

    if (overlap < 0)
    {
      return FALSE;
    }
    if (overlap < bestOverlap)
    {
      bestOverlap = overlap;
      bestCaseIndex = i;
    }
  }

  //If the axis is on the first object,
  if (bestCaseIndex <= 1)
  {
    //The second object is the primary object, with its vertex protruding into the first object
    CreateContact(object2, object1, contact, &toCenter, (&axis), bestOverlap, bestCaseIndex);
  }
  //If the axis is on the second object,
  else
  {
    //The second object is the primary object, with its vertex protruding into the first object
    AEVec2Neg(&toCenter, &toCenter);
    CreateContact(object1, object2, contact, &toCenter, (&axis), bestOverlap, bestCaseIndex);
  }

  return TRUE;
}

static float PenetrationOnAxis(PhysicsObject *object1, PhysicsObject *object2, AEVec2 *axis, AEVec2 *toCenter)
{
  float firstProjection = PhysicsObject_TransformToAxis(object1, axis);
  float secondProjection = PhysicsObject_TransformToAxis(object2, axis);
  float distance = fabs(AEVec2DotProduct(toCenter, axis));

  return firstProjection + secondProjection - distance;
}

static void CreateContact(PhysicsObject *primaryObject, PhysicsObject *secondaryObject, Contact *contact, AEVec2 *toCenter, AEVec2 (*axis)[4], float bestOverlap, int bestCaseIndex)
{
  AEVec2 collisionNormal;
  AEVec2 collisionPoint;
  AEVec2 otherObjectAxis1;
  AEVec2 otherObjectAxis2;
  AEMtx33 primaryObjectTransform;

  PhysicsObject_GetTransform(primaryObject, &primaryObjectTransform);

  collisionNormal = (*axis)[bestCaseIndex];
  //If the normal is facing the toward the second object, reverse it
  if (AEVec2DotProduct(&collisionNormal, toCenter) > 0)
  {
    AEVec2Neg(&collisionNormal, &collisionNormal);
  }

  PhysicsObject_GetHalfSize(primaryObject, &collisionPoint);
  PhysicsObject_GetAxis(primaryObject, &otherObjectAxis1, 0);
  PhysicsObject_GetAxis(primaryObject, &otherObjectAxis2, 1);
  if (AEVec2DotProduct(&otherObjectAxis1, &collisionNormal) > 0)
  {
    collisionPoint.x *= -1;
  }
  if (AEVec2DotProduct(&otherObjectAxis2, &collisionNormal) > 0)
  {
    collisionPoint.y *= -1;
  }
  //We want the collision point to be in world coordinates.
  PhysicsObject_LocalToWorld(&collisionPoint, &collisionPoint, &primaryObjectTransform);

  contact->object1 = primaryObject;
  contact->object2 = secondaryObject;
  contact->penetration = bestOverlap;
  contact->contactNormal = collisionNormal;
  contact->contactPoint = collisionPoint;
}