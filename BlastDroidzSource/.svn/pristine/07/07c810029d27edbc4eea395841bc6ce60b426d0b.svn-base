/***
 * Project: alphaphysics
 * File: PhysicsObject.h
 * Date: 1/2/2017
 * Created by Evan Kau
 * Description: 
***/

#pragma once

#include "AEEngine.h"

typedef struct _PhysicsObject PhysicsObject;

//Function
//Description: Creates a new PhysicsObject and returns a pointer to it.
//Inputs: Void.
//Output: A pointer to the new PhysicsObject, or NULL on fail.
PhysicsObject* PhysicsObject_New(void);

//Function
//Description: Frees a given PhysicsObject.
//Inputs: object: The pointer to a PhysicsObject.
//Output: Void.
void PhysicsObject_Free(PhysicsObject *object);

PhysicsObject* PhysicsObject_GetNext(PhysicsObject *object);

PhysicsObject* PhysicsObject_GetPrevious(PhysicsObject *object);

void PhysicsObject_SetNext(PhysicsObject *object, PhysicsObject *next);

void PhysicsObject_SetPrevious(PhysicsObject *object, PhysicsObject *previous);

void PhysicsObject_Update(PhysicsObject *object, float deltaTime);

void PhysicsObject_ApplyForce(PhysicsObject *object, float forceX, float forceY);

void PhysicsObject_ApplyForceAtPoint(PhysicsObject *object, float forceX, float forceY, float pointX, float pointY);

void PhysicsObject_ApplyForceAtBodyPoint(PhysicsObject *object, float forceX, float forceY, float pointX, float pointY);

void PhysicsObject_ApplyTorque(PhysicsObject *object, float torque);

void PhysicsObject_AddVelocity(PhysicsObject *object, float deltaX, float deltaY);

void PhysicsObject_AddAngularVelocity(PhysicsObject *object, float delta);

void PhysicsObject_ClearAccumulators(PhysicsObject *object);

void PhysicsObject_LocalToWorld(AEVec2 *result, AEVec2 *local, AEMtx33 *transform);

void PhysicsObject_WorldToLocal(AEVec2 *result, AEVec2 *world, AEMtx33 *transform);

void PhysicsObject_GetHalfSize(PhysicsObject *object, AEVec2 *halfSize);

void PhysicsObject_GetPosition(PhysicsObject *object, AEVec2 *objectPosition);

void PhysicsObject_GetVelocity(PhysicsObject *object, AEVec2 *objectVelocity);

float PhysicsObject_GetRotation(PhysicsObject *object);

float PhysicsObject_GetAngularVelocity(PhysicsObject *object);

void PhysicsObject_GetTransform(PhysicsObject *object, AEMtx33 *objectTransform);

//Function
//Description: Gets an axis of a PhysicsObject. Used for SAT collision detection.
//Inputs: object: The PhysicsObject to get the axis of.
//        axis: The returned axis. [Out]
//        index: The index to get. Either 1 or 2 for a rectangular object.
//Output: Output here.
void PhysicsObject_GetAxis(PhysicsObject *object, AEVec2 *axis, int index);

float PhysicsObject_TransformToAxis(PhysicsObject *object, AEVec2 *axis);

float PhysicsObject_GetMass(PhysicsObject *object);

float PhysicsObject_GetInertia(PhysicsObject *object);

BOOL PhysicsObject_GetImmobile(PhysicsObject *object);

void PhysicsObject_Translate(PhysicsObject *object, float deltaX, float deltaY);

//Function
//Description: Sets the position of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xPosition: The new X position of the PhysicsObject.
//        yPosition: The new Y position of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetPosition(PhysicsObject *object, float xPosition, float yPosition);

//Function
//Description: Sets the velocity of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xVelocity: The new X velocity of the PhysicsObject.
//        yVelocity: The new Y velocity of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetVelocity(PhysicsObject *object, float xVelocity, float yVelocity);

//Function
//Description: Sets the acceleration of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xAcceleration: The new X acceleration of the PhysicsObject.
//        yAcceleration: The new Y acceleration of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetAcceleration(PhysicsObject *object, float xAcceleration, float yAcceleration);

//Function
//Description: Sets the rotation of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        rotation: The new rotation of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetRotation(PhysicsObject *object, float rotation);

//Function
//Description: Sets the position of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        rotationalVelocity: The new rotationonal velocity of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetRotationalVelocity(PhysicsObject *object, float rotationalVelocity);

void PhysicsObject_SetMass(PhysicsObject *object, float mass);

void PhysicsObject_SetImmobile(PhysicsObject *object, BOOL isImmobile);

//Function
//Description: Sets the scale of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xScale: The new X scale of the PhysicsObject.
//        yScale: The new Y scale of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetScale(PhysicsObject *object, float xScale, float yScale);

//Function
//Description: Sets the color of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        r: The new red channel value of the PhysicsObject's color.
//        g: The new green channel value of the PhysicsObject's color.
//        b: The new blue channel value of the PhysicsObject's color.
//        a: The new alpha channel value of the PhysicsObject's color.
//Output: Void.
void PhysicsObject_SetColor(PhysicsObject *object, float r, float g, float b, float a);

//Function
//Description: Draws a given PhysicsObject onto the screen given its internal parameters.
//Inputs: object: The pointer to the PhysicsObject to change.
//Output: Void.
void PhysicsObject_Draw(PhysicsObject *object);