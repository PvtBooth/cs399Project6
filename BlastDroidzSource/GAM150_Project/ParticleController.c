/******************************************************************************/
/*!
	\file   ParticleController.c
	\author Henry Brobeck
	\date   2/15/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ParticleController.h"
#include <stdlib.h>
#include "RandGen.h"
#include <math.h>

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


static Particle createParticle(ParticleSystem *system);
static Vector2 GetDirection(float angle);
static float GetAngle(Vector2 direction);
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/





/**
 * \brief Create a new particle system
 * \param area Area type
 * \param direction Direction if area is cone or vector
 * \param angle spread if area is cone
 * \param rate delay between particle spawns
 * \param max max number of particles
 * \param speed speed of particles
 * \param loop set whether the particle system loops after it hit max particles
 * \param lifetime Lifetime of each particle
 * \return 
 */
ParticleSystem *ParticleController_CreateSystem(PARTICLE_AREA area, Vector2 direction, float angle, float rate, int max, float speed, float lifetime, bool loop)
{
  ParticleSystem *system = calloc(1, sizeof(ParticleSystem));
  if(!system)
  {
    return NULL;
  }
  //initialize constructor arguments
  system->area_ = area;
  system->direction_ = direction;
  system->angle_ = angle;
  system->particleMax_ = max;

  //initialize base members
  system->particleCurrent_ = 0;
  system->particleList_ = calloc(1, (sizeof(Particle) * max));
  system->delay_ = 0;
  system->rate_ = rate;

  return system;
}

/**
 * \brief Updates a particle system, creating new particles if necessary
 * \param system The system to update
 * \param dt time since last update
 */
void ParticleController_Update(ParticleSystem* system, float dt)
{
  system->delay_ -= dt; //update delay until new particle

  if(system->delay_ <= 0) //should system create a new particle?
  {
    system->delay_ = system->rate_;
    
  }
}

void ParticleController_FreeSystem(ParticleSystem* system)
{
  free(system->particleList_);
  free(system);
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static Particle createParticle(ParticleSystem *system)
{
  Particle particle;
  particle.position_ = (Vector2) { 0,0 };
  particle.direction_ = (Vector2) { 0, 0 };
  Vector2 direction;
  direction.x = 0;
  direction.y = 0;
  float offset;
  float systemAngle = GetAngle(system->direction_);;
  switch(system->area_)
  {
  case PARTICLE_AREA_CIRCLE:
    direction = GetDirection(RandGen_Float() * 360.0f);
    break;
  case PARTICLE_AREA_CONE: 
    
    offset = RandGen_Float() * system->angle_; //find a random angle in the cone
    offset -= system->angle_/2.0f; //bring the angle to center
    direction = GetDirection(systemAngle + offset);
    break;
  case PARTICLE_AREA_VECTOR:
    direction = system->direction_;
    break;
  default: return particle;
  }
  particle.direction_ = direction;
  return particle;
}
static Vector2 GetDirection(float angle)
{
  Vector2 dir;
  dir.x = cos(angle);
  dir.y = sin(angle);
}
static float GetAngle(Vector2 direction)
{
  return atan2(direction.y, direction.x);
}