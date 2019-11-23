/******************************************************************************/
/*!
  \file   ParticleSystem.h
  \author Israel Zevenbergen
	\date   2/23/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Component.h"
#include "Vector2.h"
#include "GraphicsManager.h"

typedef struct Particle Particle;
typedef struct ParticleSystem ParticleSystem;
/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/
typedef enum PARTICLE_DISPERSAL
{
  PARTICLE_DISPERSAL_CIRCLE,
  PARTICLE_DISPERSAL_VECTOR,
  PARTICLE_DISPERSAL_CONE
}PARTICLE_DISPERSAL;
/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/
Component* ParticleSystem_Create(int maxParticles, float rate, float r, float g, float b, float a, float lifetime,
  Vector2 scale, Vector2 velocity, Vector2 acceleration, float roationalVelocity, float rotationalAcceleration, RENDER_LAYER renderLayer);


// Getter functions
float ParticleSystem_GetLifetimeMin(ParticleSystem *particleSystem);
float ParticleSystem_GetLifetimemax(ParticleSystem *particleSystem);
Vector2 ParticleSystem_GetScaleInitial(ParticleSystem *particleSystem);
Vector2 ParticleSystem_GetScaleFinal(ParticleSystem *particleSystem);
float ParticleSystem_GetConeWidth(ParticleSystem *particleSystem);
PARTICLE_DISPERSAL ParticleSystem_GetDispersal(ParticleSystem *particleSystem);
Vector2 ParticleSystem_GetOffset(ParticleSystem *particleSystem);
bool ParticleSystem_GetisLooping(ParticleSystem *particleSystem);
bool ParticleSystem_GetisRunning(ParticleSystem *particleSystem);

// Setter functions
void ParticleSystem_SetLifetimeMin(ParticleSystem *particleSystem, float Lifetime);
void ParticleSystem_SetLifetimeMax(ParticleSystem *particleSystem, float lifetime);
void ParticleSystem_SetRGBAInitial(ParticleSystem *particleSystem, float r, float g, float b, float a);
void ParticleSystem_SetRGBAFinal(ParticleSystem *particleSystem, float r, float g, float b, float a);
void ParticleSystem_SetScaleInitial(ParticleSystem *particleSystem, float scaleX, float scaleY);
void ParticleSystem_SetScaleFinal(ParticleSystem *particleSystem, float scaleX, float scaleY);
void ParticleSystem_SetConeWidth(ParticleSystem *particleSystem, float coneWidth);
void ParticleSystem_SetDispersal(ParticleSystem *particleSystem, PARTICLE_DISPERSAL dispersal);
void ParticleSystem_SetOffset(ParticleSystem *particleSystem, float x, float y);
void ParticleSystem_SetisLooping(ParticleSystem *particleSystem, bool loop);
void ParticleSystem_SetisRunning(ParticleSystem *particleSystem, bool run);

/*----------------------------------------------------------------------------*/


