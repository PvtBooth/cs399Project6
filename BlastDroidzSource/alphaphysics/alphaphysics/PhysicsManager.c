/***
 * Project: alphaphysics
 * File: PhysicsManager.c
 * Date: 1/8/2017
 * Created by Evan Kau
 * Description: 
***/

#include "PhysicsManager.h"
#include "Physics_Narrow.h"

static PhysicsObject *head;
static PhysicsObject *tail;

Contact contacts[100] = { 0 };
int contactsUsed = 0;

void PhysicsManager_AddObject(PhysicsObject *object)
{
  if (!head)
  {
    PhysicsObject_SetNext(object, NULL);
    PhysicsObject_SetPrevious(object, NULL);
    head = object;
    tail = object;
  }
  else
  {
    PhysicsObject_SetNext(object, NULL);
    PhysicsObject_SetPrevious(object, tail);
    PhysicsObject_SetNext(tail, object);
    tail = object;
  }
}

void PhysicsManager_RemoveObject(PhysicsObject *object)
{
  if (head != object)
  {
    PhysicsObject_SetNext(PhysicsObject_GetPrevious(object), PhysicsObject_GetNext(object));
  }
  else
  {
    head = PhysicsObject_GetNext(object);
  }

  if (tail != object)
  {
    PhysicsObject_SetPrevious(PhysicsObject_GetNext(object), PhysicsObject_GetPrevious(object));
  }
  else
  {
    tail = PhysicsObject_GetPrevious(object);
  }
}

void PhysicsManager_Update(void)
{
  PhysicsObject *currentObject = head;
  int i;

  contactsUsed = 0;

  //Generate contacts
  while (currentObject)
  {
    PhysicsObject_SetColor(currentObject, 1, 1, 1, 1);
    PhysicsObject *otherObject = PhysicsObject_GetNext(currentObject);
    while (otherObject)
    {
      if (Physics_Narrow_GenerateContact(currentObject, otherObject, &contacts[contactsUsed]))
      {
        PhysicsObject_SetColor(currentObject, 1, 0, 0, 1);
        contactsUsed++;
      }
      otherObject = PhysicsObject_GetNext(otherObject);
    }
    currentObject = PhysicsObject_GetNext(currentObject);
  }

  //Process contacts
  for (i = 0; i < contactsUsed; i++)
  {
    float restitution = 0.0f; //How bouncy the objects are
    AEVec2 movementPerIMass = contacts[i].contactNormal;
    float totalInverseIMass = 0;
    AEVec2 contactTangent = { -contacts[i].contactNormal.y, contacts[i].contactNormal.x };
    AEVec2 zero = {0, 0};
    AEMtx33 worldToContactBasis;
    AEMtx33 contactToWorldBasis;
    AEVec2 contactToObject1;
    AEVec2 contactToObject2;
    float torquePerUnitImpulse;
    float angularVelocityPerUnitImpulse;
    AEVec2 velocityPerUnitImpulse;
    float deltaVelocity = 0; //This is the change in velocity per unit of impulse
    AEVec2 object1Velocity;
    AEVec2 object2Velocity;
    AEVec2 closingVelocity;
    float desiredVelocityChange;
    AEVec2 impulseContact;
    AEVec2 impulse;

    //Get the relative position of the contact to the first object
    PhysicsObject_GetPosition(contacts[i].object1, &contactToObject1);
    contactToObject1.x = contacts[i].contactPoint.x - contactToObject1.x;
    contactToObject1.y = contacts[i].contactPoint.y - contactToObject1.y;
    //AEVec2Normalize(&contactToObject1, &contactToObject1);
    //Get the relative position of the contact to the second object
    PhysicsObject_GetPosition(contacts[i].object2, &contactToObject2);
    contactToObject2.x = contacts[i].contactPoint.x - contactToObject2.x;
    contactToObject2.y = contacts[i].contactPoint.y - contactToObject2.y;
    //AEVec2Normalize(&contactToObject2, &contactToObject2);

    //For each object, if it can move, get the amount a unit of impulse would change the velocity of it at the point of contact in the direction of the contact normal
    if (!PhysicsObject_GetImmobile(contacts[i].object1))
    {
      torquePerUnitImpulse = contactToObject1.x * contacts[i].contactNormal.y - contactToObject1.y * contacts[i].contactNormal.x;
      angularVelocityPerUnitImpulse = torquePerUnitImpulse / PhysicsObject_GetInertia(contacts[i].object1);
      velocityPerUnitImpulse.x = -angularVelocityPerUnitImpulse * contactToObject1.y;
      velocityPerUnitImpulse.y = angularVelocityPerUnitImpulse * contactToObject1.x;

      deltaVelocity += AEVec2DotProduct(&velocityPerUnitImpulse, &contacts[i].contactNormal);

      deltaVelocity += 1 / PhysicsObject_GetMass(contacts[i].object1);
    }
    if (!PhysicsObject_GetImmobile(contacts[i].object2))
    {
      torquePerUnitImpulse = contactToObject2.x * contacts[i].contactNormal.y - contactToObject2.y * contacts[i].contactNormal.x;
      angularVelocityPerUnitImpulse = torquePerUnitImpulse / PhysicsObject_GetInertia(contacts[i].object2);
      velocityPerUnitImpulse.x = -angularVelocityPerUnitImpulse * contactToObject2.y;
      velocityPerUnitImpulse.y = angularVelocityPerUnitImpulse * contactToObject2.x;

      deltaVelocity += AEVec2DotProduct(&velocityPerUnitImpulse, &contacts[i].contactNormal);

      deltaVelocity += 1 / PhysicsObject_GetMass(contacts[i].object2);
    }

    //Generate contact->world basis
    AEMtx33SetCol(&contactToWorldBasis, 0, &contacts[i].contactNormal);
    AEMtx33SetCol(&contactToWorldBasis, 1, &contactTangent);
    AEMtx33SetCol(&contactToWorldBasis, 2, &zero);

    //Generate the world->contact basis the inverting the contact->world basis
    AEMtx33Inverse(&worldToContactBasis, &contactToWorldBasis);

    //Get the closing velocity
    PhysicsObject_GetVelocity(contacts[i].object1, &object1Velocity);
    PhysicsObject_GetVelocity(contacts[i].object2, &object2Velocity);
    closingVelocity.x = object1Velocity.x - object2Velocity.x - (PhysicsObject_GetAngularVelocity(contacts[i].object1) * contactToObject1.y) + (PhysicsObject_GetAngularVelocity(contacts[i].object2) * contactToObject2.y);
    closingVelocity.y = object1Velocity.y - object2Velocity.y + (PhysicsObject_GetAngularVelocity(contacts[i].object1) * contactToObject1.x) - (PhysicsObject_GetAngularVelocity(contacts[i].object2) * contactToObject2.x);

    //Convert the closing velocity to contact coords
    AEMtx33MultVec(&closingVelocity, &worldToContactBasis, &closingVelocity);

    //Find the amount we want to change the closing velocity by - it's amount + the restitution, in order to stop it and give it some bounce
    desiredVelocityChange = -closingVelocity.x * (1 + restitution);

    //Put that into a vector
    impulseContact.x = desiredVelocityChange / deltaVelocity;
    impulseContact.y = 0;

    //Convert that vector to world coords
    AEMtx33MultVec(&impulse, &contactToWorldBasis, &impulseContact);

    //If the first/second object can move, get its inverse mass and total it
    if (!PhysicsObject_GetImmobile(contacts[i].object1))
    {
      totalInverseIMass += 1 / PhysicsObject_GetMass(contacts[i].object1);
    }
    if (!PhysicsObject_GetImmobile(contacts[i].object2))
    {
      totalInverseIMass += 1 / PhysicsObject_GetMass(contacts[i].object2);
    }
    if (totalInverseIMass <= 0)
      continue;

    //AESysPrintf("Impulse: %10.3f, %10.3f\nRelative Velocity = %10.3f, %10.3f\n", impulse.x, impulse.y, object1Velocity.x - object2Velocity.x, object1Velocity.y - object2Velocity.y);

    AEVec2Scale(&movementPerIMass, &movementPerIMass, contacts[i].penetration / totalInverseIMass);
    //If both objects can move,
    if (!PhysicsObject_GetImmobile(contacts[i].object1) && !PhysicsObject_GetImmobile(contacts[i].object2))
    {
      float angularVelocityChangeObject1 = (contactToObject1.x * impulse.y - contactToObject1.y * impulse.x) / PhysicsObject_GetInertia(contacts[i].object1);
      float angularVelocityChangeObject2 = -(contactToObject2.x * impulse.y - contactToObject2.y * impulse.x) / PhysicsObject_GetInertia(contacts[i].object2);

      PhysicsObject_Translate(contacts[i].object1, movementPerIMass.x / PhysicsObject_GetMass(contacts[i].object1), movementPerIMass.y / PhysicsObject_GetMass(contacts[i].object1));
      PhysicsObject_AddVelocity(contacts[i].object1, impulse.x / PhysicsObject_GetMass(contacts[i].object1), impulse.y / PhysicsObject_GetMass(contacts[i].object1));
      PhysicsObject_AddAngularVelocity(contacts[i].object1, angularVelocityChangeObject1);

      PhysicsObject_Translate(contacts[i].object2, -movementPerIMass.x / PhysicsObject_GetMass(contacts[i].object2), -movementPerIMass.y / PhysicsObject_GetMass(contacts[i].object2));
      PhysicsObject_AddVelocity(contacts[i].object2, -impulse.x / PhysicsObject_GetMass(contacts[i].object2), -impulse.y / PhysicsObject_GetMass(contacts[i].object2));
      PhysicsObject_AddAngularVelocity(contacts[i].object2, angularVelocityChangeObject2);
    }
    //If only the first object can move,
    else if (!PhysicsObject_GetImmobile(contacts[i].object1))
    {
      float angularVelocityChangeObject1 = (contactToObject1.x * impulse.y - contactToObject1.y * impulse.x) / PhysicsObject_GetInertia(contacts[i].object1);

      PhysicsObject_Translate(contacts[i].object1, movementPerIMass.x / PhysicsObject_GetMass(contacts[i].object1), movementPerIMass.y / PhysicsObject_GetMass(contacts[i].object1));
      PhysicsObject_AddVelocity(contacts[i].object1, impulse.x / PhysicsObject_GetMass(contacts[i].object1), impulse.y / PhysicsObject_GetMass(contacts[i].object1));
      PhysicsObject_AddAngularVelocity(contacts[i].object1, angularVelocityChangeObject1);
    }
    //If only the second object can move,
    else if (!PhysicsObject_GetImmobile(contacts[i].object2))
    {
      float angularVelocityChangeObject2 = -(contactToObject2.x * impulse.y - contactToObject2.y * impulse.x) / PhysicsObject_GetInertia(contacts[i].object2);

      PhysicsObject_Translate(contacts[i].object2, -movementPerIMass.x / PhysicsObject_GetMass(contacts[i].object2), -movementPerIMass.y / PhysicsObject_GetMass(contacts[i].object2));
      PhysicsObject_AddVelocity(contacts[i].object2, -impulse.x / PhysicsObject_GetMass(contacts[i].object2), -impulse.y / PhysicsObject_GetMass(contacts[i].object2));
      PhysicsObject_AddAngularVelocity(contacts[i].object2, angularVelocityChangeObject2);
    }
  }
}

void PhysicsManager_Draw(void)
{
  int i;
  AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
  for (i = 0; i < contactsUsed; i++)
  {
    AEGfxLine(contacts[i].contactPoint.x, contacts[i].contactPoint.y, 0, 1.0f, 1.0f, 0.0f, 1.0f, contacts[i].contactPoint.x + contacts[i].contactNormal.x * 30, contacts[i].contactPoint.y + contacts[i].contactNormal.y * 30, 0, 1.0f, 1.0f, 0.0f, 1.0f);
  }
}
