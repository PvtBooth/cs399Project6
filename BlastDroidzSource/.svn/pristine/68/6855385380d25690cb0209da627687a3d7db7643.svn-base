/***
 * Project: alphaphysics
 * File: Physics_Narrow.h
 * Date: 1/9/2017
 * Created by Evan Kau
 * Description: 
***/

#pragma once
#include "PhysicsObject.h"

typedef struct _Contact
{
  PhysicsObject *object1;
  PhysicsObject *object2;
  AEVec2 contactPoint;
  AEVec2 contactNormal;
  float penetration;
} Contact;

BOOL Physics_Narrow_GenerateContact(PhysicsObject *object1, PhysicsObject *object2, Contact *contact);
