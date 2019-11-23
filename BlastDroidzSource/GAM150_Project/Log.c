/******************************************************************************/
/*!
	\file   Log.c
	\author Evan Kau
	\date   2/3/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Log.h"
#include "Time.h"
#include <time.h>

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef void LogFunction(void *);

typedef struct LogTypeData
{
  const char *name;
  LogFunction *function;
} LogTypeData;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static char logPath[256] = { ".\\Logs\\" };
static char logFilePath[256] = { ".\\Logs\\" };
static char logFileID[256] = {"GAM150_Project_"};

static FILE *logFile;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

static void LogString(void *string);

static void LogContact(void *contact);

static void LogShot(void *shot);

static void LogSpawn(void *spawn);

static void LogDeath(void *spawn);

static void LogDamage(void *damage);

static void LogTransform(void *transform);

static void LogTime(void *time);

static void LogStatePush(void *state);

static void LogStatePop(void *state);

static const char* GetTeam(bool isPlayer);

//static void GetProgramPath(HINSTANCE instanceH);

static void SpawnWatcher(HINSTANCE instanceH);

/*------------------------------------------------------------------------------
// Log Function Table
//----------------------------------------------------------------------------*/

static LogTypeData logTable[LOG_TYPE_MAX] =
{
  {"L_string", &LogString},
  {"L_contact", &LogContact},
  {"L_shot", &LogShot},
  {"L_spawn", &LogSpawn},
  {"L_death", &LogDeath},
  {"L_damage", &LogDamage},
  {"L_transform", &LogTransform},
  {"L_time", &LogTime},
  {"L_StatePush", &LogStatePush},
  {"L_StatePop", &LogStatePop}
};

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *   Opens the log file, and starts the Watcher program. Must be run before any
 *   data is logged.
 * \param instanceH
 *   The handle to the instance of the program. It's a Windows thing, ask them.
 */
void Log_Init(HINSTANCE instanceH)
{
  UNREFERENCED_PARAMETER(instanceH);
  char logIDNumber[128] = {0};
  time_t currentTime = time(NULL);
  long timeLong = (long)currentTime;

  _ltoa_s(timeLong, logIDNumber, 128, 10);

  strcat_s(logFileID, 256, logIDNumber);
  strcat_s(logFileID, 256, ".log");

  //We no longer want to spawn the watcher program.
  //SpawnWatcher(instanceH);

  strcat_s(logFilePath, 256, logFileID);

  fopen_s(&logFile, logFilePath, "wt");
}

/*!
 * \brief
 *   Logs a given type of data to the log file.
 * \param logType
 *   The type of data being logged.
 * \param genericLogData
 *   A set of generic values that will be logged too.
 * \param excessData
 *   A pointer to a struct that contains the remaining data to be logged. Must be of the
 *   corresponding type, or it could cause a crash.
 */
void Log_LogData(LOG_TYPE logType, LogDataGeneric genericLogData, void *excessData)
{
  if (logFile)
  {
    fprintf_s(logFile, "%f,", Time_Get()->sinceGameStart);
    fprintf_s(logFile, "%s,", genericLogData.sourceFile);
    fprintf_s(logFile, "%s,", logTable[logType].name);
    (*logTable[logType].function)(excessData);
    fprintf_s(logFile, ",END\n");
  }
}

/*!
 * \brief
 *   Closes the log file.
 */
void Log_Exit(void)
{
  if (logFile)
  {
    fclose(logFile);
  }
}

//void Log_Debug(void)
//{
//  char logPath[1024] = { 0 };
//
//  strcpy_s(logPath, 1024, logPath);
//  strcat_s(logPath, 1024, logFileID);
//
//  printf("PROGRAM PATH: %s\n", logPath);
//  printf("    LOG PATH: %s\n", logPath);
//}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void LogString(void *string)
{
  fprintf_s(logFile, "%s", (char*)string);
}

static void LogContact(void *contact)
{
  LogDataContact *data = (LogDataContact*)contact;
  fprintf_s(logFile, "%s,%i,%s,%i,%g,%g,%g,%g,%g", data->entities[0]->name, data->entities[0]->id, data->entities[1]->name, data->entities[1]->id, data->position.x, data->position.y, data->normal.x, data->normal.y, data->penetration);
}

static void LogShot(void *shot)
{
  LogDataShot *data = (LogDataShot*)shot;
  fprintf_s(logFile, "%s,%i,%s,%g,%g,%g,%g", data->entity->name, data->entity->id, GetTeam(data->isPlayer), data->position.x, data->position.y, data->direction.x, data->direction.y);
}

static void LogSpawn(void *spawn)
{
  LogDataSpawn *data = (LogDataSpawn*)spawn;
  fprintf_s(logFile, "%s,%i,%s,%g,%g", data->entity->name, data->entity->id, GetTeam(data->isPlayer), data->position.x, data->position.y);
}

static void LogDeath(void *death)
{
  LogDataDeath *data = (LogDataDeath*)death;
  fprintf_s(logFile, "%s,%i,%s,%g,%g", data->entity->name, data->entity->id, GetTeam(data->isPlayer), data->position.x, data->position.y);
}

static void LogDamage(void *damage)
{
  LogDataDamage *data = (LogDataDamage*)damage;
  //Source entity, source entity name, source entity id, /*source entity typeID*/, source entity team, source weapon type, damage dealt, hit entity name, hit entity id, positionX, positionY
  fprintf_s(logFile, "%s,%i,%s,%i,%f,%s,%i,%f,%f", data->sourceEntity->name, data->sourceEntity->id, GetTeam(data->isPlayer), data->weaponType, data->damage,
                                                   data->hitEntity->name, data->hitEntity->id,  data->position.x, data->position.y);
}

static void LogTransform(void *transform)
{
  LogDataTransform *data = (LogDataTransform*)transform;
  fprintf_s(logFile, "%s,%i,%f,%f,%f", data->entity->name, data->entity->id, TransformComponent_GetPosition(data->transform).x, TransformComponent_GetPosition(data->transform).y, TransformComponent_GetRotation(data->transform));
}

static void LogTime(void *time)
{
  LogDataTime *data = (LogDataTime*)time;
  fprintf_s(logFile, "%f,%f,%f,%f", data->time->sinceGameStart, data->time->sinceStateStart, data->time->dtCur, data->time->timeScale);
}

static void LogStatePush(void *state)
{
  LogDataStatePush *data = (LogDataStatePush*)state;
  fprintf_s(logFile, "%s", data->state->name);
}

static void LogStatePop(void *state)
{
  LogDataStatePop *data = (LogDataStatePop*)state;
  fprintf_s(logFile, "%s", data->state->name);
}

static const char* GetTeam(bool isPlayer)
{
  return isPlayer ? "L_TeamPlayer" : "L_TeamEnemy";
}

//static void GetProgramPath(HINSTANCE instanceH)
//{
//  int i;
//  GetModuleFileName(instanceH, path, 1024); //Get the full path of the program
//
//  i = strlen(path) - 1; //Get the end of the full path
//  while (path[i - 1] != '\\') //backtrack until a backslash is found
//  {
//    i--;
//  }
//  path[i] = '\0'; //Null terminate it
//}

static void SpawnWatcher(HINSTANCE instanceH)
{
    UNREFERENCED_PARAMETER(instanceH);
  int id = GetCurrentProcessId();
  char watcherPath[1024] = { 0 }; //Use this while in VS, as it changes where the directory is
  char watcherName[] = { "WATCHR.exe" };
  char idString[128] = { 0 };
  char args[1024] = { " " };
  // additional information
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  _itoa_s(id, idString, 128, 10); //Get the ID of this process
  strcat_s(args, 1024, idString);  //Add it onto the args string
  strcat_s(args, 1024, " ");  //Add a space into the args string
  strcat_s(args, 1024, logFilePath);  //Add the log file path onto the args string
  strcat_s(args, 1024, logFileID);  //Add the log file name onto the args string

  strcpy_s(watcherPath, 1024, logPath);
  strcat_s(watcherPath, 1024, watcherName);

  // set the size of the structures
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  //Create the program on the side, with this process's ID as an argument
  CreateProcess(watcherPath,   // the path
    args,           // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
  );
  // Close process and thread handles.
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}