/***
 * Project: alphaphysics
 * File: GameWorld.c
 * Date: 12/31/2016
 * Created by Evan Kau
 * Description: 
***/

#include "GameWorld.h"
#include "PhysicsManager.h"

float mouseX, mouseY; //X and Y coordinates of the mouse in the world

void GameWorld_Start(void)
{
  objects[0] = PhysicsObject_New();
  PhysicsObject_SetPosition(objects[0], 0, 150);
  PhysicsObject_SetImmobile(objects[0], FALSE);
  objects[1] = PhysicsObject_New();
  PhysicsObject_SetPosition(objects[1], 0, 0);
  PhysicsObject_SetImmobile(objects[1], TRUE);
  objects[2] = PhysicsObject_New();
  PhysicsObject_SetPosition(objects[2], 0, -150);
}

void GameWorld_Update(float deltaTime)
{
  AEVec2 force;
  float springDistance = 250;
  float springConstant = 9;
  float springLength;
  s32 screenMouseX, screenMouseY;
  AEVec2 springPosition = { -0, 0 };
  AEVec2 springPosition2 = { 0, 0 };
  AEMtx33 objectTransform;
  AEVec2 object3Pos;

  //Get the mouse coordinates on the screen
  AEInputGetCursorPosition(&screenMouseX, &screenMouseY);
  //Get the mouse coordinates in the world
  AEGfxConvertScreenCoordinatesToWorld(screenMouseX, screenMouseY, &mouseX, &mouseY);


  PhysicsObject_GetTransform(objects[0], &objectTransform);
  PhysicsObject_LocalToWorld(&springPosition, &springPosition, &objectTransform);
  PhysicsObject_LocalToWorld(&springPosition2, &springPosition2, &objectTransform);

  force.x = springPosition.x - mouseX;
  force.y = springPosition.y - mouseY;

  springLength = AEVec2Length(&force);

  AEVec2Normalize(&force, &force);

  AEVec2Scale(&force, &force, (springDistance - springLength) * springConstant);

  PhysicsObject_ApplyForceAtBodyPoint(objects[0], force.x, force.y, 0, 0);

  force.x = springPosition2.x - mouseX;
  force.y = springPosition2.y - mouseY;

  springLength = AEVec2Length(&force);

  AEVec2Normalize(&force, &force);

  AEVec2Scale(&force, &force, (springDistance - springLength) * springConstant);

  PhysicsObject_ApplyForceAtBodyPoint(objects[0], force.x, force.y, 0, 0);

  PhysicsObject_ApplyForce(objects[0], 0, -2000);
  PhysicsObject_ApplyForce(objects[2], 0, -2000);
  PhysicsObject_Update(objects[0], deltaTime);

  if (AEInputCheckCurr('W'))
  {
    PhysicsObject_ApplyForce(objects[2], 0, 7500);
  }
  if (AEInputCheckCurr('S'))
  {
    PhysicsObject_ApplyForce(objects[2], 0, -5500);
  }
  if (AEInputCheckCurr('D'))
  {
    PhysicsObject_ApplyForce(objects[2], 5500, 0);
  }
  if (AEInputCheckCurr('A'))
  {
    PhysicsObject_ApplyForce(objects[2], -5500, 0);
  }
  if (AEInputCheckCurr('Q'))
  {
    PhysicsObject_ApplyTorque(objects[2], 6000);
  }
  if (AEInputCheckCurr('E'))
  {
    PhysicsObject_ApplyTorque(objects[2], -6000);
  }
  PhysicsObject_Update(objects[1], deltaTime);
  PhysicsObject_Update(objects[2], deltaTime);

  PhysicsManager_Update();
}

void GameWorld_Draw(void)
{
  AEVec2 springPosition = { -0, 0 };
  AEVec2 springPosition2 = { 0, 0 };
  AEMtx33 objectTransform;

  PhysicsObject_GetTransform(objects[0], &objectTransform);

  PhysicsObject_LocalToWorld(&springPosition, &springPosition, &objectTransform);
  PhysicsObject_LocalToWorld(&springPosition2, &springPosition2, &objectTransform);

  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  PhysicsObject_Draw(objects[0]);
  PhysicsObject_Draw(objects[2]);
  PhysicsObject_Draw(objects[1]);

  AEGfxSetPosition(0, 0);
  AEGfxLine(mouseX, mouseY, 0, 1, 0, 0, 1, springPosition.x, springPosition.y, 0, 1, 0, 0, 1);
  AEGfxLine(mouseX, mouseY, 0, 1, 0, 0, 1, springPosition2.x, springPosition2.y, 0, 1, 0, 0, 1);

  PhysicsManager_Draw();
}

void GameWorld_Shutdown(void)
{
  PhysicsObject_Free(objects[0]);
}