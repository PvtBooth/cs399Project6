/******************************************************************************/
/*!
	\file   Scheme.h
	\author Cole Astaire
	\date   2/16/17
	\par    Course: GAM150
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once



/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Scheme
{

  void (*Lthumb_Angle_Bind)(int id, float angle, float magnitude);
  void (*Lthumb_Magnitude_Bind)(int id, float magnitude);
  void (*Rthumb_Angle_Bind)(int id, float angle);
  void (*Rthumb_Magnitude_Bind)(int id, float magnitude);
  void (*BAXY_Bind)(int id, int button);
  void (*Rtrigger_Magnitude_Bind)(int id, float magnitude);
  void (*Ltrigger_Magnitude_Bind)(int id, float magnitude);

} Scheme;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Scheme_Init(void);

void Scheme_Update(float dt);

void Scheme_Exit(void);

/*----------------------------------------------------------------------------*/


