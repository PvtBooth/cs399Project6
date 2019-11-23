/******************************************************************************/
/*!
	\file   Controls.c
	\author Cole Astaire
	\date   1/30/2017
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Controls.h"
#include "Input.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "RigidbodyComponent.h"
#include "Player.h"
#include "LaserComponent.h"
#include "Actions.h"

#include "AbilitySubject.h"
#include "AbilityComponent.h"

#define FORCE_SCALAR 7000
#define CONTROLS_TEMP_DEADZONE 0.40f

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

static void Lthumb_Angle_Bind(int id, float angle, float magnitude);
static void Lthumb_Magnitude_Bind(int id, float magnitude);
static void Rthumb_Angle_Bind(int id, float angle);
static void Rthumb_Magnitude_Bind(int id, float magnitude);
static void BAXY_Bind(int id, int button);
static void Rtrigger_Magnitude_Bind(int id, float magnitude);
static void Ltrigger_Magnitude_Bind(int id, float magnitude);
static void Rbumper_Bind(int id, bool state);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief This function sets all input callbacks
 */
void Controls_Init(void)
{
  //set left thumb stick angle
  Input_Set_Lthumb_Angle(Lthumb_Angle_Bind);

  //set left thumb stick magnitude
  Input_Set_Lthumb_Magnitude(Lthumb_Angle_Bind);

  //set right thumb stick angle
  Input_Set_Rthumb_Angle(Rthumb_Angle_Bind);

  //set right thumb stick magnitude
  Input_Set_Rthumb_Magnitude(Rthumb_Magnitude_Bind);

  //set baxy press
  Input_Set_BAXY(BAXY_Bind);

  //set right trigger magnitude
  Input_Set_Rtrigger_Magnitude(Rtrigger_Magnitude_Bind);

  //set left trigger magnitude
  Input_Set_Ltrigger_Magnitude(Ltrigger_Magnitude_Bind);

  //set right bumper press
  Input_Set_RBumper(&Rbumper_Bind);
}

/*!
 \brief
 \param	dt Change in time (in seconds) since last game loop.
 */
void Controls_Update(float dt)
{
}

/*!
 \brief
 */
void Controls_Exit(void)
{
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

/*!
\brief This function modifies the target rotation for the given players PID loop.
        this functionality will be moved to actions.
*/
static void Lthumb_Angle_Bind(int id, float angle, float magnitude)
{
  //primitive dead zone check, workaraound for engine proof
  if (magnitude < CONTROLS_TEMP_DEADZONE)
  {
    return;
  }

  //get player
  Player *player = Player_GetPlayer(id);

  //set the playrs PID loop
  Actions_Set_PID(player->rigidbodyComponent, angle);


  //pull the transform
  TransformComponent *transform = Entity_GetTransform(player->playerEntity);

  //pull the direction of the transform
  Vector2 force = TransformComponent_GetForward(transform);

  //modify the values based on teh current magnitude of the right trigger
  force.x *= magnitude * FORCE_SCALAR;
  force.y *= magnitude * FORCE_SCALAR;

  //set the force of the transform
  RigidbodyComponent_AddForce(player->rigidbodyComponent, force, FORCE_TYPE_FORCE);
}

/*!
\brief
*/
static void Lthumb_Magnitude_Bind(int id, float magnitude)
{
  //get the player, based on current id
  Player *player = Player_GetPlayer(id);
}

/*!
\brief
*/
static void Rthumb_Angle_Bind(int id, float angle)
{
  //get the player, based on current id
  Player *player = Player_GetPlayer(id);
}

/*!
\brief
*/
static void Rthumb_Magnitude_Bind(int id, float magnitude)
{
  //get the player, based on current id
  Player *player = Player_GetPlayer(id);
}

/*!
\brief This function call the actions specific to each baxy button press
*/
static void BAXY_Bind(int id, int button)
{
  //get the player, based on current id
  Player *player = Player_GetPlayer(id);

  //fire a basic shot when the a button is held
  //Component *shootComponent = player->shootComponent;

  button += EVENT_ABILITY_0; //button is 1-4, with 1 being B, 2=A, 3=X, 4=Y. Adds EVENT_ABILITY_0 to account for the 1 offset.

  Player_DeactivateAbilities(player);

  //Hank event test
  AbilitySubject_Notify(player->playerEntity, button);
  //Actions_ShootBasic(shootComponent, button);
}

/*!
\brief This function modifies the force of the player, propelling them
        in their current direction based off of the current magnitude of
        the right trigger. This functionality will be moved to actions.
*/
static void Rtrigger_Magnitude_Bind(int id, float magnitude)
{
  //propel the player in their current direction, at a force determined
  //by the magnitude of the right trigger

  //get the player, based on current id
  Player *player = Player_GetPlayer(id);

  ////pull the transform
  //TransformComponent *transform = Entity_GetTransform(player->playerEntity);
  //
  ////pull the direction of the transform
  //Vector2 force = TransformComponent_GetForward(transform);
  //
  ////modify the values based on teh current magnitude of the right trigger
  //force.x *= magnitude * FORCE_SCALAR;
  //force.y *= magnitude * FORCE_SCALAR;
  //
  ////set the force of the transform
  //RigidbodyComponent_AddForce(player->rigidbodyComponent, force);
}

/*!
\brief this function binds the left trigger magnitude to an action function
*/
static void Ltrigger_Magnitude_Bind(int id, float magnitude)
{
  
}

static void Rbumper_Bind(int id, bool state)
{
}