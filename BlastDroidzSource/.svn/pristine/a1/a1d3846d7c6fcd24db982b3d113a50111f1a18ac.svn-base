/******************************************************************************/
/*!
	\file   BlackHoleComponent.c
	\author Henry Brobeck
	\date   3/27/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/
#include "Unreferenced_Parameter.h"
#include "BlackHoleComponent.h"
#include "ColliderComponent.h"
#include <stdlib.h>
#include "Sound.h"


/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct BlackHoleData
{
  float radius;
  float velocity;
  bool effectsPlayer;
}BlackHoleData;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt);
static void Destroy(Component * self);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Component* BlackHoleComponent_Create(float radius, float velocity, bool effectsPlayer)
{
  Component* component = Component_Create();
  component->update = Update;
  component->destroy = Destroy;
  BlackHoleData *data = calloc(1, sizeof(BlackHoleData));
  data->radius = radius;
  data->velocity = velocity;
  data->effectsPlayer = effectsPlayer;

  component->data = data;


  return component;
  
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

//TODO DELETE UNECESSARY

static void Update(Component* self, float dt)
{
  UNREFERENCED_PARAMETER(dt);
  BlackHoleData *data = (BlackHoleData*)self->data;
  Vector2 pos = TransformComponent_GetPosition(self->parent->transformComponent);
  int numHit = 0;
  const ColliderComponent **col = PhysicsManager_CircleCast(pos, data->radius, &numHit);

  for(int i = 0; i < numHit; i++)
  {
    const ColliderComponent *collider = col[i];
    RigidbodyComponent *rigidbody = ColliderComponent_GetRigidbody(collider);
    if(!data->effectsPlayer)
    {
      if(ColliderComponent_GetLayer(collider) == PHYSICS_LAYER_PLAYER)
      {
        continue;
      }
    }
    if(rigidbody)
    {
      TransformComponent *transform = RigidbodyComponent_GetTransform(rigidbody);
      Vector2 hitPos = TransformComponent_GetPosition(transform);
      Vector2 toCenter = Vector2_Sub(pos, hitPos);
      toCenter = Vector2_Normalize(toCenter);
      toCenter.x *= data->velocity;
      toCenter.y *= data->velocity;
      RigidbodyComponent_AddForce(rigidbody, toCenter, FORCE_TYPE_IMPULSE);
    }
  }
}

static void Destroy(Component* self)
{
  free(self->data);
  free(self);
}

