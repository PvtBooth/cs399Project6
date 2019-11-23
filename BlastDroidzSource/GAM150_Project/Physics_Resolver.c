/******************************************************************************/
/*!
	\file   Physics_Resolver.c
	\author Evan Kau
	\date   1/19/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Physics_Resolver.h"
#include <stdlib.h>
#include <AEEngine.h>
#include "Camera.h"
#include "CollisionSubject.h"
#include "Log.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define PHYSICS_RESOLVER_INIT_CONTACT_NUM 32

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct Contact
{
  ColliderComponent *colliders[2];
  Vector2 position;
  Vector2 normal;
  float penetration;
} Contact;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static Contact *contacts;
static int contactMax;
static int contactCount;
static int debugCount;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void ResolveContact(Contact *contact);

static float GetRigidbodyDeltaVelocity(RigidbodyComponent *rigidbody, Vector2 contactToRigidbody, Vector2 contactNormal);

static void GenerateContactBaseis(Matrix3x3 *contactToWorld, Matrix3x3 *worldToContact, Vector2 contactNormal, Vector2 contactTangent);

static void ResolveRigidbody(RigidbodyComponent *rigidbody, Vector2 contactToRigidbody, Vector2 movementPerIMass, Vector2 impulse);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Physics_Resolver_Init()
{
  contacts = (Contact*)malloc(sizeof(Contact) * PHYSICS_RESOLVER_INIT_CONTACT_NUM);
  contactMax = PHYSICS_RESOLVER_INIT_CONTACT_NUM;
  contactCount = 0;
}

void Physics_Resolver_AddContact(ColliderComponent *first, ColliderComponent *second, Vector2 position, Vector2 normal, float penetration)
{
  if (contactCount == contactMax)
  {
    contacts = (Contact*)realloc(contacts, sizeof(Contact) * contactMax * 2);
    contactMax *= 2;
  }
  contacts[contactCount].colliders[0] = first;
  contacts[contactCount].colliders[1] = second;
  contacts[contactCount].position = position;
  contacts[contactCount].normal = normal;
  contacts[contactCount].penetration = penetration;
  contactCount++;

  CollisionSubject_Notify(ColliderComponent_GetEntity(first), ColliderComponent_GetEntity(second), position, normal, penetration);

  //LogDataContact logData = {{ColliderComponent_GetEntity(first), ColliderComponent_GetEntity(second)}, position, normal, penetration};
  //Log_LogData(LOG_TYPE_CONTACT, ((LogDataGeneric){"Physics_Resolver.c"}), &logData);
}

void Physics_Resolver_Update()
{
  int i;
  debugCount = contactCount;
#ifdef _DEBUG
  for (i = 0; i < contactCount; i++) //TODO: Remove for final version
  {
    int j;
    ColliderComponent *c = contacts[i].colliders[0];
    ColliderComponent *c2 = contacts[i].colliders[1];
    for (j = i + 1; j < contactCount; j++)
    {
      if ((c == contacts[j].colliders[0] && c2 == contacts[j].colliders[1]) || (c2 == contacts[j].colliders[0] && c == contacts[j].colliders[1]))
      {
        c = c; //Trap to see if any collider has a collision with any other spesific object more than once in a frame -- Lemme know if you hit this! --Evan
      }
    }
  }
#endif //_DEBUG
  for (i = 0; i < contactCount; i++)
  {
    ResolveContact(&contacts[i]); //TODO: Do velocity multiple times, if vel is seping, don't resolve, the resolve position
  }
  contactCount = 0;
}

void Physics_Resolver_Exit()
{
  free(contacts);
  contactMax = 0;
  contactCount = 0;
}

void Physics_Resolver_Debug()
{
  int i;
  for (i = 0; i < debugCount; i++)
  {
    Camera_DrawLine(contacts[i].position.x, contacts[i].position.y, 0, 1, 1, 1, contacts[i].position.x + contacts[i].normal.x * 100, contacts[i].position.y + contacts[i].normal.y * 100, 1, 1, 1, 0, true);
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void ResolveContact(Contact *contact)
{
  float restitution = (ColliderComponent_GetRestitution(contact->colliders[0]) + ColliderComponent_GetRestitution(contact->colliders[1])) / 2;
  TransformComponent *firstTransform  = ColliderComponent_GetTransform(contact->colliders[0]);
  TransformComponent *secondTransform = ColliderComponent_GetTransform(contact->colliders[1]);
  RigidbodyComponent *firstRigidbody  = ColliderComponent_GetRigidbody(contact->colliders[0]);
  RigidbodyComponent *secondRigidbody = ColliderComponent_GetRigidbody(contact->colliders[1]);
  Vector2 firstPosition  = TransformComponent_GetPosition(firstTransform);
  Vector2 secondPosition = TransformComponent_GetPosition(secondTransform);
  Vector2 contactToFirst  = Vector2_Sub(contact->position, firstPosition);
  Vector2 contactToSecond = Vector2_Sub(contact->position, secondPosition);
  float deltaVelocityPerImpulse = 0;
  float totalIMass = 0;

  if (firstRigidbody && !RigidbodyComponent_GetIsKinematic(firstRigidbody))
  {
    totalIMass += 1 / RigidbodyComponent_GetMass(firstRigidbody);
    deltaVelocityPerImpulse += GetRigidbodyDeltaVelocity(firstRigidbody, contactToFirst, contact->normal);
  }
  if (secondRigidbody && !RigidbodyComponent_GetIsKinematic(secondRigidbody))
  {
    totalIMass += 1 / RigidbodyComponent_GetMass(secondRigidbody);
    deltaVelocityPerImpulse += GetRigidbodyDeltaVelocity(secondRigidbody, contactToSecond, contact->normal);
  }

  if (totalIMass > 0)
  {
    Vector2 firstVelocity = (firstRigidbody) ? RigidbodyComponent_GetVelocity(firstRigidbody) : Vector2_Zero();
    Vector2 secondVelocity = (secondRigidbody) ? RigidbodyComponent_GetVelocity(secondRigidbody) : Vector2_Zero();
    float firstAngularVelocity = (firstRigidbody) ? RigidbodyComponent_GetAngularVelocity(firstRigidbody) : 0;
    float secondAngularVelocity = (secondRigidbody) ? RigidbodyComponent_GetAngularVelocity(secondRigidbody) : 0;
    Vector2 firstAngularClosingVelocity;
    Vector2 secondAngularClosingVelocity;
    Vector2 closingVelocity;
    Vector2 contactTangent = Vector2_GetOrthogonal(contact->normal);
    Matrix3x3 worldToContact = Matrix3x3_Identity();
    Matrix3x3 contactToWorld = Matrix3x3_Identity();
    Vector2 impulseContact;
    Vector2 impulse;
    float desiredVelocityChange;
    Vector2 movementPerIMass = contact->normal;

    contact->normal = Vector2_Normalize(contact->normal);
    contactTangent = Vector2_Normalize(contactTangent);

    //Generate baseis
    GenerateContactBaseis(&contactToWorld, &worldToContact, contact->normal, contactTangent);

    //Calculate fast the rigidbodies are rotation away from the point
    firstAngularClosingVelocity = Vector2_CrossFloatVector(firstAngularVelocity, contactToFirst);
    secondAngularClosingVelocity = Vector2_CrossFloatVector(secondAngularVelocity, contactToSecond);

    //Calculate the closing velocity
    closingVelocity = Vector2_Sub(firstVelocity, secondVelocity);
    closingVelocity = Vector2_Add(closingVelocity, firstAngularClosingVelocity);
    closingVelocity = Vector2_Sub(closingVelocity, secondAngularClosingVelocity);

    //Convert the closing velocity to contact coords
    closingVelocity = Matrix3x3_TransformVector(&worldToContact, closingVelocity);

    if (closingVelocity.x > 0)
      closingVelocity = Vector2_Zero();

    //Find the amount we want to change the closing velocity by -- it's amount + the restitution, in order to stop it and give it some bounce
    desiredVelocityChange = -closingVelocity.x * (1 + restitution);

    impulseContact.x = desiredVelocityChange / deltaVelocityPerImpulse;
    impulseContact.y = 0;

    impulse = Matrix3x3_TransformVector(&contactToWorld, impulseContact);

    movementPerIMass = Vector2_Scale(movementPerIMass, contact->penetration / totalIMass);

    if (firstRigidbody && !RigidbodyComponent_GetIsKinematic(firstRigidbody))
    {
      ResolveRigidbody(firstRigidbody, contactToFirst, movementPerIMass, impulse);
    }
    if (secondRigidbody && !RigidbodyComponent_GetIsKinematic(secondRigidbody))
    {
      movementPerIMass = Vector2_Scale(movementPerIMass, -1);
      impulse = Vector2_Scale(impulse, -1);
      ResolveRigidbody(secondRigidbody, contactToSecond, movementPerIMass, impulse);
    }
  }
}

static float GetRigidbodyDeltaVelocity(RigidbodyComponent *rigidbody, Vector2 contactToRigidbody, Vector2 contactNormal)
{
  float torquePerUnitImpulse = Vector2_CrossVectorVector(contactToRigidbody, contactNormal);
  float angularVelocityPerUnitImpulse = torquePerUnitImpulse / RigidbodyComponent_GetInertia(rigidbody);
  Vector2 velocityPerUnitImpulse = Vector2_CrossFloatVector(angularVelocityPerUnitImpulse, contactToRigidbody);
  float deltaVelocity = Vector2_Dot(velocityPerUnitImpulse, contactNormal) + 1 / RigidbodyComponent_GetMass(rigidbody);

  return deltaVelocity;
}

static void GenerateContactBaseis(Matrix3x3 *contactToWorld, Matrix3x3 *worldToContact, Vector2 contactNormal, Vector2 contactTangent)
{
  AEMtx33 mtx;
  //Generate contact->world basis
  Matrix3x3_SetColumn(contactToWorld, contactNormal, 0);
  Matrix3x3_SetColumn(contactToWorld, contactTangent, 1);
  mtx.m[0][0] = contactToWorld->data[0][0];
  mtx.m[0][1] = contactToWorld->data[0][1];
  mtx.m[0][2] = contactToWorld->data[0][2];
  mtx.m[1][0] = contactToWorld->data[1][0];
  mtx.m[1][1] = contactToWorld->data[1][1];
  mtx.m[1][2] = contactToWorld->data[1][2];
  mtx.m[2][0] = contactToWorld->data[2][0];
  mtx.m[2][1] = contactToWorld->data[2][1];
  mtx.m[2][2] = contactToWorld->data[2][2];
  AEMtx33Inverse(&mtx, &mtx);
  //Generate the world->contact basis the inverting the contact->world basis
  *worldToContact = Matrix3x3_Inverse(contactToWorld);
}

static void ResolveRigidbody(RigidbodyComponent *rigidbody, Vector2 contactToRigidbody, Vector2 movementPerIMass, Vector2 impulse)
{
  float angularVelocityChange = Vector2_CrossVectorVector(contactToRigidbody, impulse) / RigidbodyComponent_GetInertia(rigidbody);

  RigidbodyComponent_TranslateFloats(rigidbody, movementPerIMass.x / RigidbodyComponent_GetMass(rigidbody), movementPerIMass.y / RigidbodyComponent_GetMass(rigidbody));
  RigidbodyComponent_AddVelocityFloats(rigidbody, impulse.x / RigidbodyComponent_GetMass(rigidbody), impulse.y / RigidbodyComponent_GetMass(rigidbody));
  RigidbodyComponent_AddAngularVelocity(rigidbody, angularVelocityChange);
}