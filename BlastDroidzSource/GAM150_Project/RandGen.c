/******************************************************************************
filename    RandGen.c
author      Henry Brobeck
DP email    henry.brobeck@digipen.edu
course      GAM100
section     D
GAME PROJECT

Brief Description:
A unified random number generator for the whole game to use
******************************************************************************/

#include "RandGen.h"
#include "stdlib.h" /*srand, rand*/
#include <time.h> /* time */

static int is_seeded = 0;

float RandGen_Float(void)
{
	
	if (!is_seeded)
	{
		time_t t;
		srand((unsigned) time(&t));
		is_seeded = 1;
	}
	
	

	return (float)rand() / ((float)RAND_MAX + 1);
}

int RandGen_Int(int min, int max)
{
	if (!is_seeded)
	{
		time_t t;
		srand((unsigned)time(&t));
		is_seeded = 1;
	}

	double scaled = (double)rand() / RAND_MAX - 0.000001; //Temp fix

	return (int)((max - min + 1)*scaled + min);

	
}
