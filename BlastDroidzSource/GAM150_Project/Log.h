/******************************************************************************/
/*!
	\file   Log.h
	\author Evan Kau
	\date   2/3/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "ColliderComponent.h"
#include "Time.h"
#include "StackState.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef enum LOG_TYPE
{
  LOG_TYPE_STRING = 0,
  LOG_TYPE_CONTACT,
  LOG_TYPE_SHOT,
  LOG_TYPE_SPAWN,
  LOG_TYPE_DEATH,
  LOG_TYPE_DAMAGE,
  LOG_TYPE_TRANSFORM,
  LOG_TYPE_TIME,
  LOG_TYPE_STATEPUSH,
  LOG_TYPE_STATEPOP,

  LOG_TYPE_MAX
} LOG_TYPE;

typedef struct LogDataGeneric
{
  const char *sourceFile;
  //Anything else
} LogDataGeneric;

typedef struct LogDataContact
{
  Entity *entities[2];
  Vector2 position;
  Vector2 normal;
  float penetration;
} LogDataContact;

typedef struct LogDataShot
{
  Entity *entity;
  bool isPlayer;
  Vector2 position;
  Vector2 direction;
} LogDataShot;

typedef struct LogDataSpawn
{
  Entity *entity;
  bool isPlayer;
  Vector2 position;
} LogDataSpawn;

typedef struct LogDataDeath //Log who killed it, need to track damage sources
{
  Entity *entity;
  bool isPlayer;
  Vector2 position;
} LogDataDeath;

typedef struct LogDataDamage //Track source of shot
{
  Entity *sourceEntity;
  //int sourceEntityType;
  COMPONENT_TYPE weaponType;
  Entity *hitEntity;
  bool isPlayer;
  float damage;
  Vector2 position;
} LogDataDamage;

typedef struct LogDataTransform
{
  Entity *entity;
  TransformComponent *transform;
} LogDataTransform;

typedef struct LogDataTime
{
  Time *time;
} LogDataTime;

typedef struct LogDataStatePush
{
  StackState *state;
} LogDataStatePush;

typedef struct LogDataStatePop
{
  StackState *state;
} LogDataStatePop;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Log_Init(HINSTANCE instanceH);

void Log_Reset();

void Log_LogData(LOG_TYPE logType, LogDataGeneric genericLogData, void *excessData);

void Log_Exit(void);

//void Log_Debug(void);
