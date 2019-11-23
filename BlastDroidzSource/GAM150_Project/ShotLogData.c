/******************************************************************************/
/*!
	\file   ShotLogData.c
	\author Evan Kau
	\date   3/12/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ShotLogData.h"
#include "Entity.h"
#include "Log.h"

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

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
void ShotLogData_Init(ShotLogData *data, Entity *sourceEntity, bool isPlayer, COMPONENT_TYPE weaponType)
{
  data->sourceEntity = *sourceEntity;
  data->isPlayer = isPlayer;
  //data->typeID = typeID;
  data->weaponType = weaponType;
}

void ShotLogData_LogDamageDealt(ShotLogData *data, float damage, Vector2 position, Entity *hitEntity)
{
  LogDataGeneric genericData = {"ShotLogData.c"};
  LogDataDamage damageData;
  damageData.position = position;
  damageData.isPlayer = data->isPlayer;
  damageData.damage = damage;
  damageData.hitEntity = hitEntity;
  damageData.sourceEntity = &data->sourceEntity;
  damageData.weaponType = data->weaponType;
  //damageData.sourceEntityType = data->typeID;
  Log_LogData(LOG_TYPE_DAMAGE, genericData, &damageData);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
