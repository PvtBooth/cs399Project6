/******************************************************************************/
/*!
	\file   Actions.c
	\author Cole Astaire
	\date   2/11/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief  This file contains all actions which maipulate entities. Each 
          function takes an entitiy as a parameter, then the following parameters
          vary based on the action that is to be taken.
 */
/******************************************************************************/

#include "Actions.h"
#include "Entity.h"
#include <stddef.h>
#include "RigidbodyComponent.h"
#include "ShootComponent.h"
#include "AbilitySubject.h"

#define PID_DEFAULT_RATE 50
#define PID_DEFAULT_TIGHTNESS 10

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
void Actions_Init(void)
{
}

/*!
 \brief
 \param	dt Change in time (in seconds) since last game loop.
 */
void Actions_Update(float dt)
{
}

/*!
 \brief
 */
void Actions_Exit(void)
{
}

/*------------------------------------------------------------------------------
// Movement Actions
//----------------------------------------------------------------------------*/

/*!
\brief This function sets the current PID loop of the entity.
*/
void Actions_Set_PID(RigidbodyComponent *rigidbody, float angle)
{
  //safety check
  if (rigidbody == NULL)
    return;

  //set the PID of the given rigidbody
  RigidbodyComponent_PIDLoopToAngle(rigidbody, angle, PID_DEFAULT_RATE, PID_DEFAULT_TIGHTNESS);
}

/*!
\brief This function sets the current force in the forward direction of the entity
*/
void Actions_Set_Force(TransformComponent *transform, float force)
{
  //TODO: place all of the required code to modify the transform's force here

}

/*------------------------------------------------------------------------------
// Basic Attacks
//----------------------------------------------------------------------------*/

/*!
\brief This function sets the current force in the forward direction of the entity
*/
void Actions_ShootBasic(Component *shootComponent, int button)
{
  //TODO: place all of the required code to modify the shootcomponent here
  //safety check
  if(shootComponent == NULL)
  {
    return;
  }

  //if pressing the A button, call shoot funcion
  if (button == 2)
  {
    
    ShootComponent_Shoot(shootComponent);
  }
}


void Actions_UseAbility(Entity* entity, int actionNumber, float angle)
{
  if(entity == NULL)
  {
    return;
  }

  switch (actionNumber)
  {
  case 1:
    AbilitySubject_Notify(entity, EVENT_ABILITY_1);
    break;
  case 2:
    AbilitySubject_Notify(entity, EVENT_ABILITY_2);
    break;
  case 3:
    AbilitySubject_Notify(entity, EVENT_ABILITY_3);
    break;
  case 4:
    AbilitySubject_Notify(entity, EVENT_ABILITY_4);
    break;
  default:
    return;
  }


}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
