/******************************************************************************/
/*!
	\file   Colors.h
	\author Evan Kau
	\date   4/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum TEAMCOLOR
{
  COLORT_PRIMARY,
  COLORT_SECONDARY,
  COLORT_TERTIARY,
  COLORT_TINT,
  COLORT_SHADE,

  COLORT_MIN = COLORT_PRIMARY,
  COLORT_MAX = COLORT_SHADE,
  COLORT_LIMIT = COLORT_SHADE + 1,
} TEAMCOLOR;

typedef enum GENERALCOLOR
{
  COLOR_ASTEROID,
  COLOR_PLANET,
  COLOR_DEFAULT
} GENERALCOLOR;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Color
{
  float r;
  float g;
  float b;
  float a;
} Color;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Color Colors_GetPlayerColor(int player, TEAMCOLOR color);

Color Colors_GetEnemyColor(int type, TEAMCOLOR color);

Color Colors_GetGeneralColor(GENERALCOLOR color);