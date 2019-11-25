// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	main entry point2 for the test program
// History			:
// - 2012/01/11		:	- initial implementation
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Engine.h"
#include "Game.h"
#include "Log.h"
#include "GameStateStack.h"
#include "TestStackState.h"
#include "BarTestState.h"
#include "DigiPenLogoStackState.h"
#include "LevelState.h"
#include "MainMenuStackState.h"
#include "StandardMeshGroups.h"
#include "AssetTable.h"
#include "RandGen.h"
#include "ShipSelectStackState.h"


// ---------------------------------------------------------------------------

// Libraries
#ifdef _DEBUG
#pragma comment (lib, "Alpha_Engine_D.lib")
#else
#pragma comment (lib, "Alpha_Engine.lib")
#endif


// ---------------------------------------------------------------------------
// globals

int gGameRunning = 1;


// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------



//main

//int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
//{
//  Log_Init(instanceH);
//
//  Log_LogData(LOG_TYPE_STRING, ((LogDataGeneric){"Main.c"}), "This is a test to see if this works well...");
//
//  Engine_Init(instanceH,prevInstanceH,command_line, show);
//  Game_Init();
//
//  float delta;
//  
//
//  while(Game_IsRunning())
//  {
//    delta = Engine_UpdateStart();
//    Game_Update(delta);
//    Engine_UpdateEnd();
//  }
//
//  Game_Exit();
//  Engine_Exit();
//
//  Log_LogData(LOG_TYPE_STRING, ((LogDataGeneric){"Main.c"}), "Ending the game now");
//
//  Log_Exit();
//  
//  return 1;
//}
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  Log_Init(instanceH);

  Log_LogData(LOG_TYPE_STRING, ((LogDataGeneric){"Main.c"}), "Game Start");

  Engine_Init(instanceH, prevInstanceH, command_line, show);
  StandardMeshGroups_Init();
  GameStateStack_Init();
  /*GameStateStack_PushState(BarTestState_Create());*/
  //GameStateStack_PushState(TestStackState_Create());
  GameStateStack_PushState(ShipSelectStackState_Create());
  //GameStateStack_PushState(LevelState_Create());
  //GameStateStack_PushState(MainMenuStackState_Create());
  GameStateStack_Update(0.0f);

  float delta;


  while (!GameStateStack_StackEmpty())
  {
    delta = Engine_UpdateStart();
    GameStateStack_Update(delta);
    Engine_UpdateEnd();
  }

  
  GameStateStack_Exit();
  Engine_Exit();
  AssetTable_Exit();

  Log_LogData(LOG_TYPE_STRING, ((LogDataGeneric){"Main.c"}), "Game Clean Exit");

  Log_Exit();

  return 0;
}

// ---------------------------------------------------------------------------
