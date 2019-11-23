/******************************************************************************/
/*!
	\file   PlayerHUD.c
	\author Henry Brobeck
	\date   3/30/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "PlayerHUD.h"
#include "PlayerHUDControllerComponent.h"
#include "BarComponent.h"
#include "TransformComponent.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "Player.h"


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
static Vector2 positions[4];
static Entity *playerHUDs[4];
static GraphicsManager *graphicsManager;
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
void PlayerHUD_Init(StackState *state)
{

  
  RECT window = Graphics_GetWindowResolution();
  positions[0].x = 0.0f;
  positions[0].y = 30.0f;

  positions[1].x = (float)(window.right - window.left - 500.0f);
  positions[1].y = 30.0f;

  positions[2].x = 0.0f;
  positions[2].y = (float)(window.bottom - window.top - 100);

  positions[3].x = (float)(window.right - window.left - 500);
  positions[3].y = (float)(window.bottom - window.top - 100);

  for (int i = 0; i < 4; i++)
  {
    playerHUDs[i] = NULL;
    if(Player_GetPlayer(i)->playerEntity)
    {
      Component *hudBarComponent = BarComponent_Create(Vector2_Zero());
      

      Component *hudTransCom = TransformComponent_Create();
      TransformComponent *hudTransform = (TransformComponent*)hudTransCom->data;
      TransformComponent_SetPosition(hudTransform, positions[i]);

      Entity *playerHUD = EntityManager_CreateEntity(state->entityManager, "CornerHUD");
      Entity_AttachComponent(playerHUD, hudTransCom);
      Entity_AttachComponent(playerHUD, hudBarComponent);
      Entity_AttachComponent(playerHUD, PlayerHUDControllerComponent_Create(i));
      BarComponent_SetWorld(hudBarComponent, false);
      playerHUDs[i] = playerHUD;
    }
  }
}

/*!
 \brief
 \param	dt Change in time (in seconds) since last game loop.
 */
void PlayerHUD_Update(float dt)
{
  UNREFERENCED_PARAMETER(dt);
  RECT window = Graphics_GetWindowResolution();
  positions[0].x = 0.0f;
  positions[0].y = 30.0f;

  positions[1].x = (float)(window.right - window.left - 500.0f);
  positions[1].y = 30.0f;

  positions[2].x = 0.0f;
  positions[2].y = (float)(window.bottom - window.top - 100);

  positions[3].x = (float)(window.right - window.left - 500);
  positions[3].y = (float)(window.bottom - window.top - 100);

  for (int i = 0; i < 4; i++)
  {
    playerHUDs[i] = NULL;
    if (Player_GetPlayer(i)->playerEntity)
    {
      
      Entity *entity = playerHUDs[i];
      TransformComponent_SetPosition(entity->transformComponent, positions[i]);
    }
  }
}

/*!
 \brief
 */
void PlayerHUD_Exit(StackState *state)
{
  UNREFERENCED_PARAMETER(state);
  for(int i = 0; i < 4; i++)
  {
    if(playerHUDs[i])
    {
      Entity_DestroyOrphan(playerHUDs[i]);
    }
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
