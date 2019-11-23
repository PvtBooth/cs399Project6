/******************************************************************************/
/*!
	\file   RigidbodyComponent.c
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "RigidbodyComponent.h"
#include "Vector2.h"
#include "PhysicsManager.h"
#include <math.h>
#include <stdlib.h>
#include "Entity.h"
#include "Physics_BroadPhase.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct RigidbodyComponent
{
  unsigned int id;
  TransformComponent *transform;
  ColliderComponent *collider;

  Vector2 velocity;
  //!Accumulates forces on the Rigidbody, which in an update will be applied to the velocity.
  Vector2 forceAccum;
  float angularVelocity;
  //!Accumulates torque on the Rigidbody, which in an update will be applied to the angular velocity.
  float torqueAccum;
  float mass;
  float inertia;
  float drag;
  float angularDrag;
  bool isKinematic;
  bool canLeave;

  Component *component;
  struct RigidbodyComponent *next;
  struct RigidbodyComponent *prev;
} RigidbodyComponent;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

//static void Update(Component *self, float dt);

static void Destroy(Component *self);

static void OnAttach(Component *self);

static void OnOtherAttach(Component *self, Component *component);

static Component* Copy(Component *self);

static void ClearAccumulators(RigidbodyComponent *rigidbody);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *   Creates a new rigidbody component.
 * \param mass
 *   The mass of the new rigidbody.
 * \param inertia
 *   The inertia of the new rigidbody. - Essentially mass for rotation
 * \param drag
 *   The drag of the new rigidbody.
 * \param angularDrag
 *   The angularDrag of the new rigidbody.
 * \param isKinematic
 *   Determines if the rigidbody is kinematic or dynamic.
 * \param canLeave
 *   Determines if the rigidbody can leave the grid of the current scene, or if it will be contained within it.
 * \return
 *   A pointer to a new component with a rigidbody attached.
 */
Component* RigidbodyComponent_Create(float mass, float inertia, float drag, float angularDrag, bool isKinematic, bool canLeave)
{
  Component *component = NULL;
  RigidbodyComponent *rigidbody;
  if (PhysicsManager_GetUnusedRigidbody(&component))
  {
    rigidbody = (RigidbodyComponent*)component->data;
    rigidbody->velocity.x = 0;
    rigidbody->velocity.y = 0;
    rigidbody->forceAccum.x = 0;
    rigidbody->forceAccum.y = 0;
    rigidbody->angularVelocity = 0;
    rigidbody->torqueAccum = 0;
  }
  else
  {
    rigidbody = (RigidbodyComponent*)calloc(1, sizeof(RigidbodyComponent));
    component = Component_Create();

    component->destroy = &Destroy;
    component->onAttach = &OnAttach;
    component->onOtherAttach = &OnOtherAttach;
    component->copy = &Copy;
    component->data = rigidbody;
    component->type = COMPONENT_TYPE_RIGIDBODY;
  }
  rigidbody->mass = mass;
  rigidbody->inertia = inertia;
  rigidbody->drag = drag;
  rigidbody->angularDrag = angularDrag;
  rigidbody->isKinematic = isKinematic;
  rigidbody->component = component;
  rigidbody->canLeave = canLeave;

  //Register this rigidbody with the PhysicsManager
  PhysicsManager_RegisterRigidbody(rigidbody);

  return component;
}

Component* RigidbodyComponent_Copy(RigidbodyComponent *rigidbody)
{
  return RigidbodyComponent_Create(rigidbody->mass, rigidbody->inertia, rigidbody->drag, rigidbody->angularDrag, rigidbody->isKinematic, rigidbody->canLeave);
}

/*!
 * \brief
 *   Special Update function only to be called by the Physics Manager.
 * \param rigidbody
 *   The rigidbody to update.
 * \param dt
 *   The delta time this frame.
 */
void RigidbodyComponent_Update(RigidbodyComponent *rigidbody, float dt)
{
  //RigidbodyComponent *rigidbody = (RigidbodyComponent*)self->data;

  if (!rigidbody->isKinematic)
  {
    Vector2 velocityThisFrame;
    rigidbody->velocity = Vector2_Add(rigidbody->velocity, Vector2_Scale(rigidbody->forceAccum, dt / rigidbody->mass));

    rigidbody->angularVelocity += rigidbody->torqueAccum * dt / rigidbody->inertia;

    rigidbody->velocity = Vector2_Scale(rigidbody->velocity, powf(rigidbody->drag, dt));
    rigidbody->angularVelocity *= powf(rigidbody->angularDrag, dt);

    velocityThisFrame = Vector2_Scale(rigidbody->velocity, dt);
    TransformComponent_AddPosition(rigidbody->transform, velocityThisFrame);

    TransformComponent_AddRotation(rigidbody->transform, rigidbody->angularVelocity * dt);
  }

  if (!rigidbody->canLeave)
  {
    Vector2 transPos = TransformComponent_GetPosition(rigidbody->transform);
    if (Vector2_Length(transPos) > PhysicsManager_GetArenaRadius())
    {
      transPos = Vector2_Scale(Vector2_Normalize(transPos), PhysicsManager_GetArenaRadius());
      Vector2 velocityTowardsCenter = Vector2_Scale(rigidbody->velocity, Vector2_Dot(Vector2_Normalize(rigidbody->velocity), Vector2_Normalize(transPos)) * -0.1f);
      rigidbody->velocity = Vector2_Add(rigidbody->velocity, velocityTowardsCenter);
      TransformComponent_SetPosition(rigidbody->transform, transPos);
    }
  }

  //Physics_BroadPhase_RegisterRigidbody(rigidbody);

  ClearAccumulators(rigidbody);
}

/*!
 * \brief
 *   Translates a given rigidbody by a given amount.
 * \param rigidbody
 *   The rigidbody to translate.
 * \param deltaPosition
 *   The amount to move the rigidbody by.
 */
void RigidbodyComponent_Translate(RigidbodyComponent *rigidbody, Vector2 deltaPosition)
{
  TransformComponent_AddPosition(rigidbody->transform, deltaPosition);
}

void RigidbodyComponent_TranslateFloats(RigidbodyComponent *rigidbody, float deltaPositionX, float deltaPositionY)
{
  TransformComponent_AddPositionFloats(rigidbody->transform, deltaPositionX, deltaPositionY);
}

/*!
 * \brief
 *   Adds velocity directly to the rigidbody. Useful for impulses, but try to use AddForce instead.
 * \param rigidbody
 *   The rigidbody to change the velocity of.
 * \param deltaVelocity
 *   The amount to increase/decrease the velocity of the rigidbody by.
 */
void RigidbodyComponent_AddVelocity(RigidbodyComponent *rigidbody, Vector2 deltaVelocity)
{
  rigidbody->velocity = Vector2_Add(rigidbody->velocity, deltaVelocity);
}

void RigidbodyComponent_AddVelocityFloats(RigidbodyComponent *rigidbody, float deltaVelocityX, float deltaVelocityY)
{
  rigidbody->velocity = Vector2_AddFloats(rigidbody->velocity, deltaVelocityX, deltaVelocityY);
}

/*!
 * \brief
 *   Adds angular velocity directly to the rigidbody.
 * \param rigidbody
 *   The rigidbody to change the velocity of.
 * \param deltaAngularVelocity
 *   The amount to increase/decrease the angular velocity of the rigidbody by.
 */
void RigidbodyComponent_AddAngularVelocity(RigidbodyComponent *rigidbody, float deltaAngularVelocity)
{
  rigidbody->angularVelocity += deltaAngularVelocity;
}

/*!
 * \brief
 *   Applies a force at the origin of a given rigidbody.
 * \param rigidbody
 *   The rigidbody to apply the force to.
 * \param force
 *   The force vector to apply.
 */
void RigidbodyComponent_AddForce(RigidbodyComponent *rigidbody, Vector2 force, FORCE_TYPE type)
{
  if (type == FORCE_TYPE_FORCE)
    rigidbody->forceAccum = Vector2_Add(rigidbody->forceAccum, force);
  else //if(type == FORCE_TYPE_IMPULSE)
    rigidbody->velocity = Vector2_Add(rigidbody->velocity, Vector2_Scale(force, 1 / rigidbody->mass));
}

void RigidbodyComponent_AddForceFloats(RigidbodyComponent *rigidbody, float forceX, float forceY, FORCE_TYPE type)
{
  if (type == FORCE_TYPE_FORCE)
    rigidbody->forceAccum = Vector2_AddFloats(rigidbody->forceAccum, forceX, forceY);
  else
  {
    //if(type == FORCE_TYPE_IMPULSE)
    Vector2 force;
    force.x = forceX;
    force.y = forceY;
    rigidbody->velocity = Vector2_Add(rigidbody->velocity, Vector2_Scale(force, 1 / rigidbody->mass));
  }
}

/*!
 * \brief
 *   Applies a force to a rigidbody at a given point in world space.
 * \param rigidbody
 *   The rigidbody to apply the force to.
 * \param force
 *   The force vector to apply.
 * \param point
 *   The position in world space to apply the force at.
 */
void RigidbodyComponent_ApplyForceAtPoint(RigidbodyComponent *rigidbody, Vector2 force, Vector2 point, FORCE_TYPE type)
{
  Vector2 relative;
  relative.x = point.x - TransformComponent_GetPosition(rigidbody->transform).x; 
  relative.y = point.y - TransformComponent_GetPosition(rigidbody->transform).y;

  if (type == FORCE_TYPE_FORCE)
  {
    rigidbody->forceAccum.x += force.x;
    rigidbody->forceAccum.y += force.y;
    rigidbody->torqueAccum += Vector2_CrossVectorVector(relative, force);
  }
  else //if(type == FORCE_TYPE_IMPULSE)
  {
    rigidbody->velocity.x += force.x / rigidbody->mass;
    rigidbody->velocity.y += force.y / rigidbody->mass;
    rigidbody->angularVelocity += Vector2_CrossVectorVector(relative, force) / rigidbody->inertia;
  }
}

void RigidbodyComponent_ApplyForceAtPointFloats(RigidbodyComponent *rigidbody, float forceX, float forceY, float pointX, float pointY, FORCE_TYPE type)
{
  Vector2 relative;
  relative.x = pointX - TransformComponent_GetPosition(rigidbody->transform).x; 
  relative.y = pointY - TransformComponent_GetPosition(rigidbody->transform).y;

  if (type == FORCE_TYPE_FORCE)
  {
  rigidbody->forceAccum.x += forceX;
  rigidbody->forceAccum.y += forceY;
  rigidbody->torqueAccum += relative.x * forceY - relative.y * forceX;
  }
  else //if(type == FORCE_TYPE_IMPULSE)
  {
    rigidbody->velocity.x += forceX / rigidbody->mass;
    rigidbody->velocity.y += forceY / rigidbody->mass;
    Vector2 force;
    force.x = forceX;
    force.y = forceY;
    rigidbody->angularVelocity += Vector2_CrossVectorVector(relative, force) / rigidbody->inertia;
  }
}

/*!
* \brief
*   Applies a force to a rigidbody at a given point relative to the rigidbody.
* \param rigidbody
*   The rigidbody to apply the force to.
* \param force
*   The force vector to apply.
* \param point
*   The position in body space to apply the force at.
*/
void RigidbodyComponent_ApplyForceAtBodyPoint(RigidbodyComponent *rigidbody, Vector2 force, Vector2 point, FORCE_TYPE type)
{
  //A vector that will be transformed so it is in world coordinates
//  Vector2 worldPoint = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(rigidbody->transform), point);

  RigidbodyComponent_ApplyForceAtPoint(rigidbody, force, point, type);
}

void RigidbodyComponent_ApplyForceAtBodyPointFloats(RigidbodyComponent *rigidbody, float forceX, float forceY, float pointX, float pointY, FORCE_TYPE type)
{
  //A vector that will be transformed so it is in world coordinates
  Vector2 worldPoint;
  worldPoint.x = pointX;
  worldPoint.y = pointY;

  worldPoint = Matrix3x3_TransformPoint(TransformComponent_GetTransformMatrix(rigidbody->transform), worldPoint);

  RigidbodyComponent_ApplyForceAtPointFloats(rigidbody, forceX, forceY, worldPoint.x, worldPoint.y, type);
}

/*!
* \brief
*   Applies a torque to a rigidbody.
* \param rigidbody
*   The rigidbody to apply the force to.
* \param torque
*   The amount of torque to apply. Counterclockwise is positive.
*/
void RigidbodyComponent_ApplyTorque(RigidbodyComponent *rigidbody, float torque)
{
  rigidbody->torqueAccum += torque;
}

/*!
 * \brief
 *   Uses a PID loop to apply torque to a rigidbody, taking in a point in world space to look towards.
 *   Call this every frame if you want a smooth physics-enabled lookat.
 * \param rigidbody
 *   The rigidbody to apply torque to.
 * \param point
 *   The point in world space to look at.
 * \param rate
 *   The speed at which the rigidbody will roate.
 * \param tightness
 *   The snappyness of the rotation. Higher values reduce overshoot.
 */
void RigidbodyComponent_PIDLoopToPoint(RigidbodyComponent *rigidbody, Vector2 point, float rate, float tightness)
{
  Vector2 forward = TransformComponent_GetForward(rigidbody->transform);
  Vector2 toPoint = Vector2_SubFloats(point, TransformComponent_GetPosition(rigidbody->transform).x, TransformComponent_GetPosition(rigidbody->transform).y);
  float angle = Vector2_GetSignedAngle(forward, toPoint);
  float torque = angle * rigidbody->inertia * rate;
  torque -= rigidbody->angularVelocity * rigidbody->inertia * tightness;
  RigidbodyComponent_ApplyTorque(rigidbody, torque);
}

/*!
 * \brief
 *   Uses a PID loop to apply torque to a rigidbody, taking in an point relative to the rigidbody to look towards.
 *   Call this every frame if you want a smooth physics-enabled lookat.
 * \param rigidbody
 *   The rigidbody to apply torque to.
 * \param direction
 *   The point in relative to the rigidbody to look at.
 * \param rate
 *   The speed at which the rigidbody will roate.
 * \param tightness
 *   The snappyness of the rotation. Higher values reduce overshoot.
 */
void RigidbodyComponent_PIDLoopToDirection(RigidbodyComponent *rigidbody, Vector2 direction, float rate, float tightness)
{
  Vector2 forward = TransformComponent_GetForward(rigidbody->transform);
  float angle = Vector2_GetSignedAngle(forward, direction);
  float torque = angle * rigidbody->inertia * rate;
  torque -= rigidbody->angularVelocity * rigidbody->inertia * tightness;
  RigidbodyComponent_ApplyTorque(rigidbody, torque);
}

/*!
 * \brief
 *   Uses a PID loop to apply torque to a rigidbody, taking in an angle to look towards.
 *   Call this every frame if you want a smooth physics-enabled lookat.
 * \param rigidbody
 *   The rigidbody to apply torque to.
 * \param targetAngle
 *   The angle to look towards
 * \param rate
 *   The speed at which the rigidbody will roate.
 * \param tightness
 *   The snappyness of the rotation. Higher values reduce overshoot.
 */
void RigidbodyComponent_PIDLoopToAngle(RigidbodyComponent *rigidbody, float targetAngle, float rate, float tightness)
{
  if(!rigidbody)
  {
    return;
  }
  Vector2 forward = TransformComponent_GetForward(rigidbody->transform);
  Vector2 toPoint;
  toPoint.x = cosf(targetAngle);
  toPoint.y = sinf(targetAngle);
  float angle = Vector2_GetSignedAngle(forward, toPoint);
  float torque = angle * rigidbody->inertia * rate;
  torque -= rigidbody->angularVelocity * rigidbody->inertia * tightness;
  RigidbodyComponent_ApplyTorque(rigidbody, torque);
}

/*!
 * \brief
 *   Sets the ID of a rigidbody.
 * \param rigidbody
 *   The rigidbody to set the ID of.
 * \param id
 *   The number to use for the ID.
 */
void RigidbodyComponent_SetID(RigidbodyComponent *rigidbody, unsigned int id)
{
  rigidbody->id = id;
}

/*!
 * \brief
 *   Gets the parent component of the rigidbody.
 * \param rigidbody
 *   The rigidbody get the parent component of.
 * \return
 *   The a pointer to the parent component.
 */
Component *RigidbodyComponent_GetComponent(const RigidbodyComponent *rigidbody)
{
  return rigidbody->component;
}

/*!
 * \brief
 *   Sets the collider of a rigidbody.
 * \param rigidbody
 *   The rigidbody to set the collider of.
 * \param collider
 *   The collider to use.
 */
void RigidbodyComponent_SetCollider(RigidbodyComponent *rigidbody, ColliderComponent *collider)
{
  rigidbody->collider = collider;
}

/*!
 * \brief
 *   Gets the transform component from the entity the rigidbody is on.
 * \param rigidbody
 *   The rigidbody get the related transform of.
 * \return
 *   The a pointer to the related transform.
 */
TransformComponent* RigidbodyComponent_GetTransform(const RigidbodyComponent *rigidbody)
{
  return rigidbody->transform;
}

/*!
 * \brief
 *   Gets the collider component from the entity the rigidbody is on.
 * \param rigidbody
 *   The rigidbody get the related collider of.
 * \return
 *   The a pointer to the related collider.
 */
ColliderComponent* RigidbodyComponent_GetCollider(const RigidbodyComponent *rigidbody)
{
  return rigidbody->collider;
}

/*!
 * \brief
 *   Gets the layer the rigidbody is on.
 * \param rigidbody
 *   The rigidbody get the layer of.
 * \return
 *   The layer the related collider is on, or PHYSICS_LAYER_DEFAULT if there isn't one.
 */
PhysicsLayer RigidbodyComponent_GetLayer(const RigidbodyComponent *rigidbody)
{
  if (!rigidbody->collider)
    return Physics_Layers_GetLayerMask(PHYSICS_LAYER_DEFAULT);
  return ColliderComponent_GetLayer(rigidbody->collider);
}

/*!
 * \brief
 *   Sets the layer of the rigidbody. The rigidbody must have a collider too.
 * \param rigidbody
 *   The rigidbody set the layer of.
 * \param physicsLayer
 *   The layer to set the rigidbody to.
 */
void RigidbodyComponent_SetLayer(RigidbodyComponent *rigidbody, PhysicsLayer physicsLayer)
{
  if (!rigidbody->collider)
    ColliderComponent_SetLayer(rigidbody->collider, physicsLayer);
}

/*!
 * \brief
 *   Gets the mass of the rigidbody.
 * \param rigidbody
 *   The rigidbody to get the mass from.
 * \return
 *   The mass of the rigidbody.
 */
float RigidbodyComponent_GetMass(const RigidbodyComponent *rigidbody)
{
  return rigidbody->mass;
}

/*!
 * \brief
 *   Gets the mass of the rigidbody.
 * \param rigidbody
 *   The rigidbody to get the mass from.
 * \return
 *   The mass of the rigidbody.
 */
float RigidbodyComponent_GetInertia(const RigidbodyComponent *rigidbody)
{
  return rigidbody->inertia;
}

/*!
 * \brief
 *   Gets the velocity of the rigidbody.
 * \param rigidbody
 *   The rigidbody to get the velocity from.
 * \return
 *   The velocity of the rigidbody.
 */
Vector2 RigidbodyComponent_GetVelocity(const RigidbodyComponent *rigidbody)
{
  //if (TransformComponent_GetParent(rigidbody->transform))
  //{
  //  RigidbodyComponent *parent = TransformComponent_GetRigidbody(rigidbody->transform);
  //  if (parent)
  //  {
  //    return Vector2_Add(rigidbody->velocity, parent->velocity);
  //  }
  //}
  return rigidbody->velocity;
}

/*!
 * \brief
 *   Gets the angular velocity of the rigidbody.
 * \param rigidbody
 *   The rigidbody to get the angular velocity from.
 * \return
 *   The angular velocity of the rigidbody.
 */
float RigidbodyComponent_GetAngularVelocity(const RigidbodyComponent *rigidbody)
{
  return rigidbody->angularVelocity;
}

/*!
 * \brief
 *   Gets if the rigidbody is kinematic or not.
 * \param rigidbody
 *   The rigidbody to check.
 * \return
 *   Returns true if the rigidbody is kinematic, false otherwise.
 */
bool RigidbodyComponent_GetIsKinematic(const RigidbodyComponent *rigidbody)
{
  return rigidbody->isKinematic;
}

/*!
 * \brief
 *   Gets the ID of the rigidbody.
 * \param rigidbody
 *   The rigidbody to get the ID from.
 * \return
 *   The ID of the rigidbody.
 */
unsigned int RigidbodyComponent_GetID(const RigidbodyComponent *rigidbody)
{
  return rigidbody->id;
}

/*!
 * \brief
 *   Sets the next rigidbody of a rigidbody.
 * \param rigidbody
 *   The rigidbody to modify.
 * \param next
 *   The rigidbody to use as the other's next.
 */
void RigidbodyComponent_SetNext(RigidbodyComponent *rigidbody, RigidbodyComponent *next)
{
  rigidbody->next = next;
}

/*!
 * \brief
 *   Sets the previous rigidbody of a rigidbody.
 * \param rigidbody
 *   The rigidbody to modify.
 * \param prev
 *   The rigidbody to use as the other's previous.
 */
void RigidbodyComponent_SetPrev(RigidbodyComponent *rigidbody, RigidbodyComponent *prev)
{
  rigidbody->prev = prev;
}

/*!
 * \brief
 *   Gets the next rigidbody of a rigidbody.
 * \param rigidbody
 *   The rigidbody to get the next of.
 * \return
 *   The rigidbody's next rigidbody.
 */
RigidbodyComponent* RigidbodyComponent_GetNext(const RigidbodyComponent *rigidbody)
{
  return rigidbody->next;
}

/*!
 * \brief
 *   Gets the previous rigidbody of a rigidbody.
 * \param rigidbody
 *   The rigidbody to get the previous of.
 * \return
 *   The rigidbody's previous rigidbody.
 */
RigidbodyComponent* RigidbodyComponent_GetPrev(const RigidbodyComponent *rigidbody)
{
  return rigidbody->prev;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Destroy(Component *self)
{
  self->parent = NULL;
  RigidbodyComponent *rigidbody = self->data;
  PhysicsManager_RemoveRigidbody(rigidbody);
}

static void OnAttach(Component *self)
{
  Component *transComponent = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_TRANSFORM);
  RigidbodyComponent *rigidbody = self->data;
  Component *collider;

  if (transComponent)
  {
    rigidbody->transform = (TransformComponent*)transComponent->data;
  }

  collider = Entity_FindComponentByType(self->parent, COMPONENT_TYPE_COLLIDER);
  if (collider)
  {
    rigidbody->collider = (ColliderComponent*)collider->data;
    ColliderComponent_SetRigidbody(rigidbody->collider, rigidbody);
  }
  else
  {
    rigidbody->collider = NULL;
  }
}

static void OnOtherAttach(Component *self, Component *component)
{
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    RigidbodyComponent *rigidbody = (RigidbodyComponent*)self->data;
    rigidbody->transform = (TransformComponent*)component->data;
  }
}

static Component* Copy(Component *self)
{
  return RigidbodyComponent_Copy((RigidbodyComponent*)self->data);
}

static void ClearAccumulators(RigidbodyComponent *rigidbody)
{
  rigidbody->forceAccum.x = 0;
  rigidbody->forceAccum.y = 0;
  rigidbody->torqueAccum = 0;
}