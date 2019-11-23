/***
 * Project: alphaphysics
 * File: PhysicsObject.c
 * Date: 1/2/2017
 * Created by Evan Kau
 * Description: 
***/

#include "PhysicsObject.h"
#include "PhysicsManager.h"

typedef struct _PhysicsObject
{
  AEMtx33 transformMatrix;
  AEVec2 halfSize;
	AEVec2 position;
	AEVec2 velocity;
  float drag;
	AEVec2 acceleration;
	float rotation;
	float angularVelocity;
  float angularDrag;
  float mass;
  float inertia;
  BOOL immobile;
  AEVec2 forceAccumulator;
  float torqueAccumulator;
	AEVec2 scale;
	AEGfxVertexList *mesh;
	AEGfxTexture *texture;
	float r, g, b, a;
  PhysicsObject *next;
  PhysicsObject *previous;
} PhysicsObject;

//Function
//Description: Creates a new PhysicsObject and returns a pointer to it.
//Inputs: Void.
//Output: A pointer to the new PhysicsObject, or NULL on fail.
PhysicsObject* PhysicsObject_New(void)
{
	PhysicsObject *newObject;

	newObject = (PhysicsObject*)malloc(sizeof(PhysicsObject));
	if (!newObject)
	{
		AESysPrintf("Could not allocate memory for a new PhysicsObject.\n");
		return NULL;
	}

  newObject->halfSize.x = 75;
  newObject->halfSize.y = 30;

	newObject->position.x = 0;
	newObject->position.y = 0;

	newObject->velocity.x = 0;
	newObject->velocity.y = 0;

  newObject->drag = 0.85f;

	newObject->acceleration.x = 0;
	newObject->acceleration.y = 0;

	newObject->rotation = 0;

	newObject->angularVelocity = 0;

  newObject->angularDrag = 0.95f;

  newObject->mass = 10;
  newObject->inertia = 900;
  newObject->immobile = FALSE;

  newObject->forceAccumulator.x = 0;
  newObject->forceAccumulator.y = 0;

  newObject->torqueAccumulator = 0;

	newObject->scale.x = 1;
	newObject->scale.y = 1;

  AEMtx33BuildFullTransform(&newObject->transformMatrix, newObject->position.x, newObject->position.y, newObject->rotation, newObject->scale.x, newObject->scale.y);

	AEGfxMeshStart();

	// This shape has 2 triangles
	AEGfxTriAdd(
		-newObject->halfSize.x, -newObject->halfSize.y, 0xFFA0FFFF, 0.0f, 1.0f,
    newObject->halfSize.x, -newObject->halfSize.y, 0xFFFFA0FF, 1.0f, 1.0f,
		-newObject->halfSize.x, newObject->halfSize.y, 0xFFFFA0FF, 0.0f, 0.0f);

	AEGfxTriAdd(
    newObject->halfSize.x, -newObject->halfSize.y, 0xFFFFA0FF, 1.0f, 1.0f,
    newObject->halfSize.x, newObject->halfSize.y, 0xFFA0FFFF, 1.0f, 0.0f,
    -newObject->halfSize.x, newObject->halfSize.y, 0xFFFFA0FF, 0.0f, 0.0f);

	newObject->mesh = AEGfxMeshEnd();
	if (!newObject->mesh)
	{
    free(newObject);
		AESysPrintf("Could not allocate memory for the mesh of a new PhysicsObject.\n");
		return NULL;
	}

  newObject->r = 1;
  newObject->g = 1;
  newObject->b = 1;
  newObject->a = 1;

  PhysicsManager_AddObject(newObject);

	return newObject;
}

//Function
//Description: Frees a given PhysicsObject.
//Inputs: object: The pointer to a PhysicsObject.
//Output: Void.
void PhysicsObject_Free(PhysicsObject *object)
{
  PhysicsManager_RemoveObject(object);
	AEGfxMeshFree(object->mesh);
	//AEGfxTextureUnload(object->texture);
	free(object);
}

PhysicsObject* PhysicsObject_GetNext(PhysicsObject *object)
{
  return object->next;
}

PhysicsObject* PhysicsObject_GetPrevious(PhysicsObject *object)
{
  return object->previous;
}

void PhysicsObject_SetNext(PhysicsObject *object, PhysicsObject *next)
{
  object->next = next;
}

void PhysicsObject_SetPrevious(PhysicsObject *object, PhysicsObject *previous)
{
  object->previous = previous;
}

void PhysicsObject_Update(PhysicsObject *object, float deltaTime)
{
  if (!object->immobile)
  {
    object->velocity.x += object->forceAccumulator.x * deltaTime / object->mass;
    object->velocity.y += object->forceAccumulator.y * deltaTime / object->mass;

    object->angularVelocity += object->torqueAccumulator * deltaTime / object->inertia;

    AEVec2Scale(&object->velocity, &object->velocity, pow(object->drag, deltaTime));
    object->angularVelocity *= pow(object->angularDrag, deltaTime);

    object->position.x += object->velocity.x * deltaTime;
    object->position.y += object->velocity.y * deltaTime;

    object->rotation += object->angularVelocity * deltaTime * 180 / 3.14159f;
  }

  AEMtx33BuildFullTransform(&object->transformMatrix, object->position.x, object->position.y, object->rotation, object->scale.x, object->scale.y);

  PhysicsObject_ClearAccumulators(object);
}

void PhysicsObject_ApplyForce(PhysicsObject *object, float forceX, float forceY)
{
  object->forceAccumulator.x += forceX;
  object->forceAccumulator.y += forceY;
}

void PhysicsObject_ApplyForceAtPoint(PhysicsObject *object, float forceX, float forceY, float pointX, float pointY)
{
  float relativeX = pointX - object->position.x;
  float relativeY = pointY - object->position.y;

  object->forceAccumulator.x += forceX;
  object->forceAccumulator.y += forceY;
  object->torqueAccumulator += relativeX * forceY - relativeY * forceX;
}

void PhysicsObject_ApplyForceAtBodyPoint(PhysicsObject *object, float forceX, float forceY, float pointX, float pointY)
{
  //A vector that will be transformed so it is in world coordinates
  AEVec2 worldPoint = { pointX, pointY };

  PhysicsObject_LocalToWorld(&worldPoint, &worldPoint, &object->transformMatrix);

  PhysicsObject_ApplyForceAtPoint(object, forceX, forceY, worldPoint.x, worldPoint.y);
}

void PhysicsObject_ApplyTorque(PhysicsObject *object, float torque)
{
  object->torqueAccumulator += torque;
}

void PhysicsObject_AddVelocity(PhysicsObject *object, float deltaX, float deltaY)
{
  object->velocity.x += deltaX;
  object->velocity.y += deltaY;
}

void PhysicsObject_AddAngularVelocity(PhysicsObject *object, float delta)
{
  object->angularVelocity += delta;
}

void PhysicsObject_ClearAccumulators(PhysicsObject *object)
{
  object->forceAccumulator.x = 0;
  object->forceAccumulator.y = 0;
  object->torqueAccumulator = 0;
}

void PhysicsObject_LocalToWorld(AEVec2 *result, AEVec2 *local, AEMtx33 *transform)
{
  AEMtx33MultVec(result, transform, local);
}

void PhysicsObject_WorldToLocal(AEVec2 *result, AEVec2 *world, AEMtx33 *transform)
{
  AEMtx33 inverseTransform;
  AEMtx33Inverse(&inverseTransform, transform);
  
  AEMtx33MultVec(result, &inverseTransform, world);
}

void PhysicsObject_GetHalfSize(PhysicsObject *object, AEVec2 *halfSize)
{
  halfSize->x = object->halfSize.x;
  halfSize->y = object->halfSize.y;
}

void PhysicsObject_GetPosition(PhysicsObject *object, AEVec2 *objectPosition)
{
  *objectPosition = object->position;
}

void PhysicsObject_GetVelocity(PhysicsObject *object, AEVec2 *objectVelocity)
{
  *objectVelocity = object->velocity;
}

float PhysicsObject_GetRotation(PhysicsObject *object)
{
  return object->rotation;
}

float PhysicsObject_GetAngularVelocity(PhysicsObject *object)
{
  return object->angularVelocity;
}

//Function
//Description: Description here.
//Inputs: Inputs here.
//Output: Output here.
void PhysicsObject_GetTransform(PhysicsObject *object, AEMtx33 *objectTransform)
{
  *objectTransform = object->transformMatrix;
}

//Function
//Description: Gets an axis of a PhysicsObject. Used for SAT collision detection.
//Inputs: object: The PhysicsObject to get the axis of.
//        axis: The returned axis. [Out]
//        index: The index to get. Either 1 or 2 for a rectangular object.
//Output: Output here.
void PhysicsObject_GetAxis(PhysicsObject *object, AEVec2 *axis, int index)
{
  AEMtx33GetCol(axis, index, &object->transformMatrix);
}

float PhysicsObject_TransformToAxis(PhysicsObject *object, AEVec2 *axis)
{
  AEVec2 objectAxis1;
  AEVec2 objectAxis2;
  PhysicsObject_GetAxis(object, &objectAxis1, 0);
  PhysicsObject_GetAxis(object, &objectAxis2, 1);
  return object->halfSize.x * fabs(AEVec2DotProduct(&objectAxis1, axis)) + object->halfSize.y * fabs(AEVec2DotProduct(&objectAxis2, axis));
}

float PhysicsObject_GetMass(PhysicsObject *object)
{
  return object->mass;
}

float PhysicsObject_GetInertia(PhysicsObject *object)
{
  return object->inertia;
}

BOOL PhysicsObject_GetImmobile(PhysicsObject *object)
{
  return object->immobile;
}

void PhysicsObject_Translate(PhysicsObject *object, float deltaX, float deltaY)
{
  object->position.x += deltaX;
  object->position.y += deltaY;
  AEMtx33BuildFullTransform(&object->transformMatrix, object->position.x, object->position.y, object->rotation, object->scale.x, object->scale.y);
}

//Function
//Description: Sets the position of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xPosition: The new X position of the PhysicsObject.
//        yPosition: The new Y position of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetPosition(PhysicsObject *object, float xPosition, float yPosition)
{
  object->position.x = xPosition;
  object->position.y = yPosition;
}

//Function
//Description: Sets the velocity of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xVelocity: The new X velocity of the PhysicsObject.
//        yVelocity: The new Y velocity of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetVelocity(PhysicsObject *object, float xVelocity, float yVelocity)
{
  object->velocity.x = xVelocity;
  object->velocity.y = yVelocity;
}

//Function
//Description: Sets the acceleration of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xAcceleration: The new X acceleration of the PhysicsObject.
//        yAcceleration: The new Y acceleration of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetAcceleration(PhysicsObject *object, float xAcceleration, float yAcceleration)
{
  object->acceleration.x = xAcceleration;
  object->acceleration.y = yAcceleration;
}

//Function
//Description: Sets the rotation of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        rotation: The new rotation of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetRotation(PhysicsObject *object, float rotation)
{
  object->rotation = rotation;
}

//Function
//Description: Sets the position of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        angularVelocity: The new angular velocity of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetRotationalVelocity(PhysicsObject *object, float angularVelocity)
{
  object->angularVelocity = angularVelocity;
}

void PhysicsObject_SetMass(PhysicsObject *object, float mass)
{
  object->mass = mass;
}

void PhysicsObject_SetImmobile(PhysicsObject *object, BOOL isImmobile)
{
  object->immobile = isImmobile;
}

//Function
//Description: Sets the scale of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        xScale: The new X scale of the PhysicsObject.
//        yScale: The new Y scale of the PhysicsObject.
//Output: Void.
void PhysicsObject_SetScale(PhysicsObject *object, float xScale, float yScale)
{
  object->scale.x = xScale;
  object->scale.y = yScale;
}

//Function
//Description: Sets the color of a given PhysicsObject.
//Inputs: object: The pointer to the PhysicsObject to change.
//        r: The new red channel value of the PhysicsObject's color.
//        g: The new green channel value of the PhysicsObject's color.
//        b: The new blue channel value of the PhysicsObject's color.
//        a: The new alpha channel value of the PhysicsObject's color.
//Output: Void.
void PhysicsObject_SetColor(PhysicsObject *object, float r, float g, float b, float a)
{
  object->r = r;
  object->g = g;
  object->b = b;
  object->a = a;
}

//Function
//Description: Draws a given PhysicsObject onto the screen given its internal parameters.
//Inputs: object: The pointer to the PhysicsObject to change.
//Output: Void.
void PhysicsObject_Draw(PhysicsObject *object)
{
  //Set the transform of the object
	AEGfxSetTransform(&object->transformMatrix);
  //Set the texture to be NULL for the object
  AEGfxTextureSet(NULL, 0, 0);
  AEGfxSetTintColor(object->r, object->g, object->b, object->a);
  //Draw the mesh
	AEGfxMeshDraw(object->mesh, AE_GFX_MDM_TRIANGLES);
}