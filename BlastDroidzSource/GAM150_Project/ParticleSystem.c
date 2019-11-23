/******************************************************************************/
/*!
\file   ParticleSystem.c
\author Israel Zevenbergen
\date   2/23/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief
*/
/******************************************************************************/

#include "ParticleSystem.h"
#include "Vector2.h"
#include "TransformComponent.h"
#include <stdlib.h>
#include "Entity.h"
#include "Graphics.h"
#include "RandGen.h"
#include "GraphicsManager.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/
typedef struct Particle
{
  Vector2 position;
  Vector2 velocity;
  float lifetime, maxLifetime;
  Vector2 scale;
  Vector2 acceleration;
  float rotation;
  float rotationalVelocity;
  float rotationalAccelaration;
  float r, g, b, a;

}Particle;

typedef struct ParticleSystem
{
  Particle * particles; // dynamic array
  int maxParticles;
  int nextIndex; // next index of particles array
  float rate; // how fast each particles spawns
  float currentDelay; // time until next particles
  float rInitial, gInitial, bInitial, aInitial;
  float rFinal, gFinal, bFinal, aFinal;
  float lifetimeMin, lifetimeMax;
  Vector2 scaleInital, scaleFinal;
  Vector2 velocity;
  Vector2 acceleration;
  float rotationalVelocity, rotationalAcceleration;
  Vector2 offset;
  TransformComponent *transform;
  PARTICLE_DISPERSAL dispersal;
  float coneWidth;
  bool eventArray[EVENT_TYPE_MAX];

  //Hank's optimization test
  int minAlive;
  int maxAlive;

  bool isRunning, isLooping, isRegistered;

  RENDER_LAYER renderLayer;

}ParticleSystem;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Component *self, float dt);
static void Destroy(Component *self);
static void OnAttach(Component *self);
static void OnOtherAttach(Component *self, Component *component);
static void EventHandle(ParticleSystem * particleSystem, Component *self, EVENT_TYPE type, void *eventData, bool isLocal);
static void ParticleSystem_ActivateOnUpdate(ParticleSystem *particleSystem, EVENT_TYPE type, bool activate);
static void Render(Component* self);
static int CreateParticle(Vector2 positon, Vector2 velocity, float maxLifetime, Vector2 acceleration, float rotation,
                          float rotationalVelocity, float rotationalAccelaration, ParticleSystem * system);
void ParticleUpdate(ParticleSystem* system, int index, float dt);


/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/


Component* ParticleSystem_Create(int maxParticles, float rate, float r, float g, float b, float a, float lifetime,
  Vector2 scale, Vector2 velocity, Vector2 acceleration, float roationalVelocity, float rotationalAcceleration, RENDER_LAYER renderLayer)
{
  ParticleSystem *system = calloc(1, sizeof(ParticleSystem));
  Particle *newParticle = calloc(maxParticles, sizeof(Particle));
  system->particles = newParticle;
  system->maxParticles = maxParticles;
  system->nextIndex = 0;
  system->rate = rate;
  system->lifetimeMin = lifetime;
  system->lifetimeMax = lifetime;
  system->aInitial = a;
  system->rInitial = r;
  system->bInitial = b;
  system->gInitial = g;
  system->rFinal = r;
  system->gFinal = g;
  system->bFinal = b;
  system->aFinal = a;
  system->scaleInital = scale;
  system->scaleFinal = scale;
  system->velocity = velocity;
  system->acceleration = acceleration;
  system->rotationalVelocity = roationalVelocity;
  system->rotationalAcceleration = rotationalAcceleration;
  system->coneWidth = 0;
  system->dispersal = PARTICLE_DISPERSAL_VECTOR;
  system->minAlive = 0;
  system->maxAlive = maxParticles;
  system->renderLayer = renderLayer;

  system->isRunning = true;
  system->isLooping = true;


  Component * component = Component_Create();
  component->update = &Update;
  component->destroy = &Destroy;
  component->onAttach = &OnAttach;
  component->onOtherAttach = &OnOtherAttach;
  component->data = system;
  

  for (int i = 0; i < system->maxParticles; i++)
  {

    system->particles[i].lifetime = lifetime;
    
  }

  return component;
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
static void Update(Component *self, float dt)
{
  ParticleSystem * data = (ParticleSystem*)self->data;


  GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);
  if (graphicsManager && !data->isRegistered)
  {
    GraphicsManager_RegisterComponentRender(graphicsManager, Render, self, data->renderLayer);
    data->isRegistered = true;
  }
  
  
  data->currentDelay -= dt;
  for (int i = 0; i < data->maxParticles; i++)
  {

    ParticleUpdate(data, i, dt);

  }

  if (!ParticleSystem_GetisRunning(data))
  {
    return;
  }
  

  while (data->currentDelay <= 0)
  {
    float magnitude = Vector2_Length(data->velocity);
    float angle = TransformComponent_GetRotation(data->transform);
    float vec_x, vec_y;
    Vector2 velocity;

    Vector2 acceleration;
    float acc_x, acc_y;

    switch (data->dispersal)
    {
    case PARTICLE_DISPERSAL_CIRCLE:
      angle = RandGen_Float() * 2.0f * PI;
      vec_x = cosf(angle);
      vec_y = sinf(angle);
      velocity.x = vec_x * magnitude;
      velocity.y = vec_y * magnitude;
      break;

    case PARTICLE_DISPERSAL_VECTOR:
      vec_x = data->velocity.x * cosf(angle) - data->velocity.y * sinf(angle);
      vec_y = data->velocity.x * sinf(angle) + data->velocity.y * cosf(angle);
      velocity.x = vec_x;
      velocity.y = vec_y;
      break;
    case PARTICLE_DISPERSAL_CONE:
      angle -= data->coneWidth / 2;
      angle += RandGen_Float() * data->coneWidth;
      vec_x = data->velocity.x * cosf(angle) - data->velocity.y * sinf(angle);
      vec_y = data->velocity.x * sinf(angle) + data->velocity.y * cosf(angle);
      velocity.x = vec_x * magnitude;
      velocity.y = vec_y * magnitude;
      break;
    default: return;
    }

    acc_x = data->acceleration.x * cosf(angle) - data->acceleration.y * sinf(angle);
    acc_y = data->acceleration.x * sinf(angle) + data->acceleration.y * cosf(angle);
    acceleration.x = acc_x;
    acceleration.y = acc_y;

    float lifetime = data->lifetimeMin;
    lifetime += (RandGen_Float() * (data->lifetimeMax - data->lifetimeMin));

    Vector2 offset;
    offset.x = data->offset.x * cosf(angle) - data->offset.y * sinf(angle);
    offset.y = data->offset.x * sinf(angle) + data->offset.y * cosf(angle);
    Vector2 position = Vector2_Add(TransformComponent_GetPosition(data->transform), offset);

    

    int index = CreateParticle(position, velocity,
      lifetime, acceleration,
      TransformComponent_GetRotation(data->transform), data->rotationalVelocity,
      data->rotationalAcceleration, data);
    ParticleUpdate(data, index, -data->currentDelay);
    data->currentDelay += 1.0f / data->rate;

    if (!ParticleSystem_GetisRunning(data))
    {
      return;
    }
  }

  
}
static void Destroy(Component *self)
{
  ParticleSystem * data = (ParticleSystem*)self->data;
  GraphicsManager_RemoveComponentRender(Entity_GetGraphicsManager(self->parent), self, data->renderLayer);
  free(data->particles);
  free(data);
  free(self);
}
static void OnAttach(Component *self)
{
  ParticleSystem * data = (ParticleSystem*)self->data;
  GraphicsManager *graphicsManager = Entity_GetGraphicsManager(self->parent);
  if (graphicsManager)
  {
    GraphicsManager_RegisterComponentRender(graphicsManager, Render, self, data->renderLayer);
    data->isRegistered = true;
  }
  if (data->transform)
  {
    return;
  }

  data->transform = Entity_GetTransform(self->parent);
}
static void OnOtherAttach(Component *self, Component *component)
{
  ParticleSystem * data = (ParticleSystem*)self->data;
  if (data->transform)
  {
    return;
  }
  if (component->type == COMPONENT_TYPE_TRANSFORM)
  {
    data->transform = component->data;
  }
}

static int CreateParticle(Vector2 position, Vector2 velocity, float maxLifetime, Vector2 acceleration, float rotation,
                          float rotationalVelocity, float rotationalAccelaration, ParticleSystem * system)
{
  
  Particle newParticle;
  newParticle.velocity = velocity;

  newParticle.lifetime = 0;
  newParticle.maxLifetime = maxLifetime;
  newParticle.acceleration = acceleration;
  newParticle.rotation = rotation;
  newParticle.rotationalVelocity = rotationalVelocity;
  newParticle.rotationalAccelaration = rotationalAccelaration;
  newParticle.position = position;
  int index = system->nextIndex;
  system->particles[index] = newParticle;
  ++system->nextIndex;

  system->nextIndex %= system->maxParticles;

  if (system->nextIndex == 0 && !ParticleSystem_GetisLooping(system))
  {
	  ParticleSystem_SetisRunning(system, false);
  }

  return index;

}


static void Render(Component* self)
{
  if(Entity_GetFlag(self->parent,EFLAG_FREEZE))
  {
    return;
  }

  ParticleSystem * data = (ParticleSystem*)self->data;
  for (int i = 0; i < data->maxParticles; i++)
  {
    Particle particle = data->particles[i];
    if (particle.lifetime > particle.maxLifetime)
    {
      continue;
    }
    Graphics_DrawRect(particle.position, particle.scale, particle.rotation, particle.r, particle.g, particle.b, particle.a);
  }
}

void ParticleUpdate(ParticleSystem *system, int index, float dt)
{
  Particle *particle = &system->particles[index];
  particle->lifetime += dt;
  if (particle->lifetime > particle->maxLifetime)
  {
    return;
  }

  particle->velocity = Vector2_Add(particle->velocity, particle->acceleration);
  particle->position = Vector2_Add(particle->velocity, particle->position);
  particle->rotationalVelocity += particle->rotationalAccelaration;
  particle->rotation += particle->rotationalVelocity;

  float ratioLifetime = particle->lifetime / particle->maxLifetime;
  particle->r = system->rInitial + ((system->rFinal - system->rInitial) * ratioLifetime);
  particle->g = system->gInitial + ((system->gFinal - system->gInitial) * ratioLifetime);
  particle->b = system->bInitial + ((system->bFinal - system->bInitial) * ratioLifetime);
  particle->a = system->aInitial + ((system->aFinal - system->aInitial) * ratioLifetime);
  Vector2 newScale = Vector2_Sub(system->scaleFinal, system->scaleInital);
  newScale.x *= ratioLifetime;
  newScale.y *= ratioLifetime;
  newScale = Vector2_Add(newScale, system->scaleInital);
  particle->scale = newScale;

}

static void EventHandle(ParticleSystem * particleSystem, Component *self, EVENT_TYPE type, void *eventData, bool isLocal)
{
    UNREFERENCED_PARAMETER(eventData);
	if (particleSystem->eventArray[type] && isLocal)
	{
		ParticleSystem_SetisRunning(self->data, true);
	}
}

static void ParticleSystem_ActivateOnUpdate(ParticleSystem *particleSystem, EVENT_TYPE type, bool activate)
{
	particleSystem->eventArray[type] = activate;
}

/*------------------------------------------------------------------------------
// Getter functions:
//----------------------------------------------------------------------------*/

float ParticleSystem_GetLifetimeMin(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return 0;
  }
  return particleSystem->lifetimeMin;
}
float ParticleSystem_GetLifetimemax(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return 0;
  }
  return particleSystem->lifetimeMax;
}
Vector2 ParticleSystem_GetScaleInitial(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return Vector2_Zero();
  }
  return particleSystem->scaleInital;
}
Vector2 ParticleSystem_GetScaleFinal(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return Vector2_Zero();
  }
  return particleSystem->scaleFinal;
}

PARTICLE_DISPERSAL ParticleSystem_GetDispersal(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return -1;
  }
  return particleSystem->dispersal;
}

Vector2 ParticleSystem_GetOffset(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return Vector2_Zero();
  }
  return particleSystem->offset;
}

bool ParticleSystem_GetisLooping(ParticleSystem *particleSystem)
{
	if (!particleSystem)
	{
		return false;
	}
	return particleSystem->isLooping;
}

bool ParticleSystem_GetisRunning(ParticleSystem *particleSystem)
{
	if (!particleSystem)
	{
		return false;
	}
	return particleSystem->isRunning;
}


float ParticleSystem_GetConeWidth(ParticleSystem *particleSystem)
{
  if (!particleSystem)
  {
    return 0;
  }

  return particleSystem->coneWidth;
}

/*------------------------------------------------------------------------------
// Setter functions:
//----------------------------------------------------------------------------*/

void ParticleSystem_SetLifetimeMin(ParticleSystem *particleSystem, float Lifetime)
{
  if (!particleSystem)
  {
    return;
  }
  particleSystem->lifetimeMin = Lifetime;
}

void ParticleSystem_SetLifetimeMax(ParticleSystem *particleSystem, float lifetime)
{
  if (!particleSystem)
  {
    return;
  }
  particleSystem->lifetimeMax = lifetime;
}

void ParticleSystem_SetRGBAInitial(ParticleSystem *particleSystem, float r, float g, float b, float a)
{
  if (!particleSystem)
  {
    return;
  }
  particleSystem->rInitial = r;
  particleSystem->gInitial = g;
  particleSystem->bInitial = b;
  particleSystem->aInitial = a;
}

void ParticleSystem_SetRGBAFinal(ParticleSystem *particleSystem, float r, float g, float b, float a)
{
  if (particleSystem == NULL)
  {
    return;
  }
  particleSystem->rFinal = r;
  particleSystem->gFinal = g;
  particleSystem->bFinal = b;
  particleSystem->aFinal = a;
}

void ParticleSystem_SetScaleInitial(ParticleSystem *particleSystem, float scaleX, float scaleY)
{
  if (particleSystem == NULL)
  {
    return;
  }
  particleSystem->scaleInital.x = scaleX;
  particleSystem->scaleInital.y = scaleY;
}

void ParticleSystem_SetScaleFinal(ParticleSystem *particleSystem, float scaleX, float scaleY)
{
  if (particleSystem == NULL)
  {
    return;
  }
  particleSystem->scaleFinal.x = scaleX;
  particleSystem->scaleFinal.y = scaleY;
}

void ParticleSystem_SetConeWidth(ParticleSystem *particleSystem, float coneWidth)
{
  if (particleSystem == NULL)
  {
    return;
  }
  particleSystem->coneWidth = coneWidth;
}

void ParticleSystem_SetDispersal(ParticleSystem *particleSystem, PARTICLE_DISPERSAL dispersal)
{
  if (particleSystem == NULL)
  {
    return;
  }
  particleSystem->dispersal = dispersal;
}

void ParticleSystem_SetOffset(ParticleSystem *particleSystem, float x, float y)
{
  if (particleSystem == NULL)
  {
    return;
  }
  particleSystem->offset.x = x;
  particleSystem->offset.y = y;
}
void ParticleSystem_SetisLooping(ParticleSystem *particleSystem, bool loop)
{
	if (particleSystem == NULL)
	{
		return;
	}
	particleSystem->isLooping = loop;
}

void ParticleSystem_SetisRunning(ParticleSystem *particleSystem, bool run)
{
	if (particleSystem == NULL)
	{
		return;
	}
	particleSystem->isRunning = run;
}




