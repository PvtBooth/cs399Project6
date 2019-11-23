/******************************************************************************/
/*!
	\file   Assignment.c
	\author Evan Kau
	\date   4/5/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Assignment.h"
#include "Event.h"
#include <stddef.h>

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

static Assignment *assignment;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

void Assignment_Set(Assignment *newAssignment)
{
  if (assignment)
  {
    assignment->Destroy(assignment);
  }
  assignment = newAssignment;
}

void Assignment_Update(float dt)
{
  assignment->Update(assignment, dt);
}

void Assignment_Render()
{
  if (assignment)
  {
    assignment->Render(assignment);
  }
}

void Assignment_ReciveEvent(ASSIGNMENT_EVENT type)
{
  if (assignment)
  {
    assignment->RecieveEvent(assignment, type);
  }
}

bool Assignment_Complete()
{
  if (assignment)
  {
    return assignment->complete;
  }
  else
  {
    return true;
  }
}

void Assignment_Exit()
{
  if (assignment)
  {
    assignment->Destroy(assignment);
    assignment = NULL;
  }
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
