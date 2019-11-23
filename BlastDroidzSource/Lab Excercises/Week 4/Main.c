/******************************************************************************/
/*!
\file   main.c
\author Doug Schilling
\par    Course: GAM150
\par    All content � 2015 DigiPen (USA) Corporation, all rights reserved.
\brief
Purpose: Sort and print an array of 10 predetermined values.

Objectives:
Identify and fix any defects in the program.
Eliminate the magic number ("10") specifying the array length.
Modify the functions to support arrays of any length.
Retain the original purpose of the program.

Note:
Consider this code to be part of a much larger project.  As a result,
you may not change any project settings as a means to solve any
defects that are identified.
*/
/******************************************************************************/

#include <stdio.h>
#include <Windows.h>

static void printArray(int * dataArray, int arrayLength);
static void sortArray(int * dataArray, int arrayLength);

int main(void)
{
  int dataArray[] = { 1000, 1, 10, 100, 500, 30, 22, 8, 50, 0 };

  SetConsoleTitle(L"GAM150Debug");

  printArray(dataArray, sizeof(dataArray) / sizeof(dataArray[0]));

  sortArray(dataArray, sizeof(dataArray) / sizeof(dataArray[0]));

  printArray(dataArray, sizeof(dataArray) / sizeof(dataArray[0]));

  printf("Press 'enter' to quit.");
  getchar();
}

/******************************************************************************/
/*!
\brief
Prints an array of ints to the output window.
\param dataArray
Pointer to the data array to be sorted.
\param arrayLength
The length of the data array.
*/
/******************************************************************************/

static void printArray(int * dataArray, int arrayLength)
{
  int i;

  for (i = 0; i < arrayLength; ++i)
  {
    printf("%6d ", dataArray[i]);
  }
  printf("\n");
}

/******************************************************************************/
/*!
\brief
Sorts an array of ints using bubble sort.
\param dataArray
Pointer to the data array to be sorted.
\param arrayLength
The length of the data array.
\note
"the bubble sort seems to have nothing to recommend it, except a catchy name
and the fact that it leads to some interesting theoretical problems"
Donald Knuth, The Art of Computer Programming
*/
/******************************************************************************/

static void sortArray(int * dataArray, int arrayLength)
{
  int i, j;

  for (i = 0; i < arrayLength; ++i)
  {
    for (j = 0; j < arrayLength - 1 - i; ++j)
    {
      if (dataArray[j] > dataArray[j + 1])
      {
        int temp;
        temp = dataArray[j];
        dataArray[j] = dataArray[j + 1];
        dataArray[j + 1] = temp;
      }
    }
  }
}