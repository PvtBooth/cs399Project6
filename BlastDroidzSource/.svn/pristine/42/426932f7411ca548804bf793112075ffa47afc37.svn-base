/******************************************************************************/
/*!
	\file   ScoreLog.c
	\author Israel Zevenbergen
	\date   4/1/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "ScoreLog.h"
#include <stdio.h>
#include "HUDScore.h"
#include <stdlib.h>
#include <windows.h>

#define MAX_STRING_SCORE 15
/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

FILE *pFile;

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static char scoreBuffer[MAX_STRING_SCORE] = {0};
/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief
 */
char * ScoreLog_Init(void)
{

  fopen_s(&pFile, ".\\Logs\\score\\Highscore.txt", "r");

  if (pFile)
  {

    fgets(scoreBuffer, MAX_STRING_SCORE, pFile);

    fclose(pFile);

    return scoreBuffer;

  }
  else
    scoreBuffer[0] = '\0';
    return scoreBuffer;

}

/*!
 \brief
 \param	dt Change in time (in seconds) since last game loop.
 */
void ScoreLog_Update()
{

  if (HUDScore_GetScore() > atoi(scoreBuffer))
  {
    int Highscore = HUDScore_GetScore();

    fopen_s(&pFile, ".\\Logs\\score\\Highscore.txt", "w");

    if (pFile)
    {
      fprintf(pFile, "%d", Highscore);

      fclose(pFile);
    }
    else
      return;
    
  }
}


/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
