/******************************************************************************/
/*!
	\file   Assignment.h
	\author Evan Kau
	\date   4/5/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include <stdbool.h>

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

typedef enum ASSIGNMENT_EVENT
{
  ASSIGNMENT_EVENT_SHIP_DIE,
  ASSIGNMENT_EVENT_STATION_DIE
} ASSIGNMENT_EVENT;

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Assignment
{
  bool complete;
  void(*Update)(struct Assignment *assignment, float dt);
  void(*Destroy)(struct Assignment *assignment);
  void(*RecieveEvent)(struct Assignment *assignment, ASSIGNMENT_EVENT type);
  void(*Render)(struct Assignment *assignment);
} Assignment;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Assignment_Set(Assignment *newAssignment);

void Assignment_Update(float dt);

void Assignment_Render();

void Assignment_ReciveEvent(ASSIGNMENT_EVENT type);

bool Assignment_Complete();

void Assignment_Exit();
