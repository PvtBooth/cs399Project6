/******************************************************************************/
/*!
\file   HUDScore.c
\author Ryan Booth
\date   4/01/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes the functions that handle score and text on the
        HUD.
*/
/******************************************************************************/

#include "Text.h"
#include "HUDScore.h"
#include "Player.h"
#include "PlayerHealthComponent.h"
#include <stdlib.h>

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
static int totalScore = 0;
static Vector2 scorePosition;
static Vector2 scoreScale = { 80.0f, 80.0f };
static Vector2 namePosition1;
static Vector2 namePosition2;
static Vector2 namePosition3;
static Vector2 namePosition4;
static char scoreGraphic[33]; //33 bytes (number of digits available)
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/
static void HUDScore_Render();
static void HUDScore_RenderNames();
/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
\brief Initializes the score position and name position
*/
void HUDScore_Init()
{
    RECT temp = Graphics_GetWindowResolution();

    scorePosition.x = 0.0f;
    scorePosition.y = ((temp.bottom - temp.top) / 2) - scoreScale.y;

    namePosition1.x = -((temp.right - temp.left) / 2) + 140.0f;
    namePosition1.y = ((temp.bottom - temp.top) / 2) - 60.0f;

    namePosition2.x = ((temp.right - temp.left) / 2) - (160.0f * 2.0f);
    namePosition2.y = ((temp.bottom - temp.top) / 2) - 60.0f;

    namePosition3.x = -((temp.right - temp.left) / 2) + 140.0f;
    namePosition3.y = -((temp.bottom - temp.top) / 2) + 90.0f;

    namePosition4.x = ((temp.right - temp.left) / 2) - (160.0f * 2.0f);
    namePosition4.y = -((temp.bottom - temp.top) / 2) + 90.0f;

    scoreGraphic[0] = 0;
}
/*!
\brief Updates the positions of the names and scores if the screen size changes
*/
void HUDScore_Update()
{
    //This is incase someone resizes the window during gameplay
    RECT temp = Graphics_GetWindowResolution();

    scorePosition.y = ((temp.bottom - temp.top) / 2) - scoreScale.y; //Put the score at the top of the window

    //Offset for # of digits
    int digits = strlen(scoreGraphic);
    if (digits > 1)
    {
        scorePosition.x = -(scoreScale.x * digits) / 2.0f;
    }

    namePosition1.x = -((temp.right - temp.left) / 2) + 140.0f;
    namePosition1.y = ((temp.bottom - temp.top) / 2) - 70.0f;

    namePosition2.x = ((temp.right - temp.left) / 2) - (160.0f * 2.0f);
    namePosition2.y = ((temp.bottom - temp.top) / 2) - 70.0f;

    namePosition3.x = -((temp.right - temp.left) / 2) + 140.0f;
    namePosition3.y = -((temp.bottom - temp.top) / 2) + 90.0f;

    namePosition4.x = ((temp.right - temp.left) / 2) - (160.0f * 2.0f);
    namePosition4.y = -((temp.bottom - temp.top) / 2) + 90.0f;
    HUDScore_Render();
    HUDScore_RenderNames();
}

/*!
\brief Adds score to the total score
*/
void HUDScore_AddScore(int score)
{
    totalScore += score;
}
/*!
\brief Resets the score to 0
*/
void HUDScore_ResetScore()
{
    totalScore = 0;
}
/*!
\brief Renders the health of each player using text printing
*/
void HUDScore_RenderHealth(int player_id, float max, float current)
{
    char player_healths[10];

    sprintf_s(player_healths, 10, "%i / %i", (int)current, (int)max);

    Vector2 healthPosition;

    if (player_id == 0)
    {
        healthPosition.x = namePosition1.x + 10.0f;
        healthPosition.y = namePosition1.y + 33.0f;
        Text_Print(healthPosition, player_healths, 20.0f, RENDER_LAYER_PARTICLE);
        return;
    }
        
    if (player_id == 1)
    {
        healthPosition.x = namePosition2.x - 10.0f;
        healthPosition.y = namePosition2.y + 33.0f;
        Text_Print(healthPosition, player_healths, 20.0f, RENDER_LAYER_PARTICLE);
        return;
    }
        
    if (player_id == 2)
    {
        healthPosition.x = namePosition3.x + 10.0f;
        healthPosition.y = namePosition3.y - 39.0f;
        Text_Print(healthPosition, player_healths, 25.0f, RENDER_LAYER_PARTICLE);
        return;
    }
        
    if (player_id == 3)
    {
        healthPosition.x = namePosition4.x - 10.0f;
        healthPosition.y = namePosition4.y - 39.0f;
        Text_Print(healthPosition, player_healths, 25.0f, RENDER_LAYER_PARTICLE);
        return;
    }
        
}
/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
/*!
\brief Renders the total score
*/
static void HUDScore_Render()
{
    //Convery integer to string
    _itoa_s(totalScore, scoreGraphic, 33, 10);

    Text_Print(scorePosition, scoreGraphic, scoreScale.x, RENDER_LAYER_PARTICLE);
}
/*!
\brief Renders the names of each player
*/
static void HUDScore_RenderNames()
{
    int count = Player_PlayerCount();

    char player_names[9];

    for (int i = 0; i < count; i++)
    {
        sprintf_s(player_names, 9, "PLAYER %i", i + 1);
        if(i == 0)
            Text_Print(namePosition1, player_names, 30.0f, RENDER_LAYER_PARTICLE);
        if (i == 1)
            Text_Print(namePosition2, player_names, 30.0f, RENDER_LAYER_PARTICLE);
        if (i == 2)
            Text_Print(namePosition3, player_names, 30.0f, RENDER_LAYER_PARTICLE);
        if(i == 3)
            Text_Print(namePosition4, player_names, 30.0f, RENDER_LAYER_PARTICLE);
    }
}


// Getter

int HUDScore_GetScore()
{
  return totalScore;
}