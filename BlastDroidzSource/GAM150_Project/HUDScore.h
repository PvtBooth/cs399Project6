/******************************************************************************/
/*!
\file   HUDScore.h
\author Ryan Booth
\date   4/01/2017
\par    Project: BlastDroids
\par    Copyright © 2017 DigiPen (USA) Corporation.
\brief  This file includes the function declarations that handle score and text
        on the HUD.
*/
/******************************************************************************/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void HUDScore_Init();

void HUDScore_Update();

void HUDScore_AddScore(int score);

void HUDScore_ResetScore();

void HUDScore_RenderHealth(int player_id, float max, float current);void HUDScore_ResetScore();

int HUDScore_GetScore();