/******************************************************************************/
/*!
	\file   KillAssignment.c
	\author Evan Kau
	\date   4/5/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "KillAssignment.h"
#include "Assignment.h"
#include <stdlib.h>
#include "Text.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct KillAssignment
{
  Assignment assignment;
  int killsRequired;
  int kills;
} KillAssignment;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void Update(Assignment *assignment, float dt);
static void Destroy(Assignment *assignment);
static void RecieveEvent(Assignment *assignment, ASSIGNMENT_EVENT type);
static void Render(Assignment *assignment);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

Assignment* KillAssignment_Create(int killsRequired)
{
  KillAssignment *assignment = (KillAssignment*)calloc(1, sizeof(KillAssignment));

  assignment->killsRequired = killsRequired;

  assignment->assignment.complete = false;
  assignment->assignment.Update = &Update;
  assignment->assignment.Destroy = &Destroy;
  assignment->assignment.RecieveEvent = &RecieveEvent;
  assignment->assignment.Render = &Render;

  return (Assignment*)assignment;
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void Update(struct Assignment *assignment, float dt)
{
    UNREFERENCED_PARAMETER(dt);
    UNREFERENCED_PARAMETER(assignment);
}

static void Destroy(Assignment *assignment)
{
  free((KillAssignment*)assignment);
}

static void RecieveEvent(struct Assignment *assignment, ASSIGNMENT_EVENT type)
{
  KillAssignment *killAssignment = (KillAssignment*)assignment;
  if (type == ASSIGNMENT_EVENT_SHIP_DIE || type == ASSIGNMENT_EVENT_STATION_DIE)
  {
    killAssignment->kills++;
    if (killAssignment->kills >= killAssignment->killsRequired)
    {
      assignment->complete = true;
    }
  }
}

static void Render(struct Assignment *assignment)
{
  KillAssignment *killAssignment = (KillAssignment*)assignment;

  char string[128] = "";
  char intBuffer[128] = "";

  if (!assignment->complete)
  {
    strcat_s(string, 128, "KILLS REMAINING ");
    _itoa_s(max(killAssignment->killsRequired - killAssignment->kills, 0), intBuffer, 128, 10);
    strcat_s(string, 128, intBuffer);
  }
  else
  {
    strcat_s(string, 128, "PROCEED TO JUMPGATE");
  }

  RECT temp = Graphics_GetWindowResolution();
  Vector2 position;
  position.x = -((float)strlen(string) / 2.f * 42);
  position.y = (float)(-(temp.bottom - temp.top) / 2 + 100);

  Text_Print(position, string, 42, RENDER_LAYER_WORLD_HUD);
}
