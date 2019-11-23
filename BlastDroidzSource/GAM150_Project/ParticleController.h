/******************************************************************************/
/*!
	\file   ParticleController.h
	\author Henry Brobeck
	\date   2/15/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"


/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/
typedef enum PARTICLE_AREA
{
  PARTICLE_AREA_CIRCLE,
  PARTICLE_AREA_CONE,
  PARTICLE_AREA_VECTOR
}PARTICLE_AREA;



/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/
typedef struct Particle
{
  Vector2 direction_;
  Vector2 position_;
  float lifetime_;
}Particle;
typedef struct ParticleSystem
{
  PARTICLE_AREA area_;
  Vector2 direction_;
  float angle_;
 
  float rate_;
  float delay_;
  float speed_;
  float lifetime_;

  int particleMax_; 
  int particleCurrent_;
  Particle *particleList_;
}ParticleSystem;
/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

ParticleSystem *ParticleController_CreateSystem(PARTICLE_AREA area, Vector2 direction, float angle, float rate, int max, float speed, float lifetime, bool loop);

void ParticleController_Update(ParticleSystem *system, float dt);

void ParticleController_FreeSystem(ParticleSystem *system);

/*----------------------------------------------------------------------------*/


