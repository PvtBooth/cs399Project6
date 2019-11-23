/******************************************************************************/
/*!
	\file   Camera.c
	\author Evan Kau
	\date   1/28/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Camera.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Matrix3x3.h"
#include <AEEngine.h>
#include "Entity.h"
#include "Graphics.h"
#include "RigidbodyComponent.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

#define CAMERA_INITIAL_TRACK_MAX 16
#define CAMERA_SIZE_TRACK_NUM 300

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/

typedef struct Camera
{
  Vector2 position;
  Vector2 targetPosition;
  float positionLerp;
  float size;
  float targetSize;
  float pastTargetSizes[CAMERA_SIZE_TRACK_NUM];
  int curTargetSizeIndex;
  float sizeLerp;
  Matrix3x3 matrix;
  bool matrixDirty;
} Camera;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/

static Camera camera = {0};

static Entity **trackedEntities;
static int trackedEntitiesCount = 0;
static int trackedEntitiesMax = CAMERA_INITIAL_TRACK_MAX;

static bool trackingBehavior = true;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief
 *   Initializes the camera, resetting its size and creating the array of tracked
 *   entities if needbe. This will not clear the tracked entities. Must be called
 *   before useage of the camera.
 */
void Camera_Init(void)
{
  int i;
  if (!trackedEntities)
    trackedEntities = (Entity**)calloc(CAMERA_INITIAL_TRACK_MAX, sizeof(Entity*));
  camera.position = Vector2_Zero();
  camera.targetPosition = Vector2_Zero();
  camera.positionLerp = 3.5f;
  camera.size = 1;
  camera.targetSize = 1;
  for (i = 0; i < CAMERA_SIZE_TRACK_NUM; i++)
  {
    camera.pastTargetSizes[i] = 0;
  }
  camera.curTargetSizeIndex = 0;
  camera.sizeLerp = 1;
  camera.matrixDirty = true;
}

/*!
 * \brief
 *   Updates the camera. If the camera is tracking entities, it will modify its
 *   targets on its own, otherwise, they will be left alone and only to position
 *   and size wil be modified [to approach the targets].
 * \param dt
 *   The time since the last frame.
 */
void Camera_Update(float dt)
{
  if (trackingBehavior && trackedEntitiesCount > 0)
  {
    Vector2 farthestx1 = TransformComponent_GetPosition(trackedEntities[0]->transformComponent);
    Vector2 farthestx2 = TransformComponent_GetPosition(trackedEntities[0]->transformComponent);
    Vector2 farthesty1 = TransformComponent_GetPosition(trackedEntities[0]->transformComponent);
    Vector2 farthesty2 = TransformComponent_GetPosition(trackedEntities[0]->transformComponent);
    Vector2 average = { 0, 0 };
    float screenWidth = (float)(Graphics_GetWindowResolution().right - Graphics_GetWindowResolution().left);
    float screenHeight = (float)(Graphics_GetWindowResolution().bottom - Graphics_GetWindowResolution().top);
    float screenWidthHeightRatio = (float)screenWidth / screenHeight;
    float maxWidthDiff = 0.0f;
    float maxHeightDiff = 0.0f;
    int i, j;

    for (i = 0; i < trackedEntitiesCount - 1; i++)
    {
      for (j = i + 1; j < trackedEntitiesCount; j++)
      {
        Vector2 entity1Pos = TransformComponent_GetPosition(trackedEntities[i]->transformComponent);
        Vector2 entity2Pos = TransformComponent_GetPosition(trackedEntities[j]->transformComponent);
        Component *rigidbody1 = Entity_FindComponentByType(trackedEntities[i], COMPONENT_TYPE_RIGIDBODY);
        Component *rigidbody2 = Entity_FindComponentByType(trackedEntities[j], COMPONENT_TYPE_RIGIDBODY);
        if (rigidbody1)
        {
          entity1Pos = Vector2_Add(entity1Pos, Vector2_Scale(RigidbodyComponent_GetVelocity(rigidbody1->data), 0.35f));
        }
        if (rigidbody2)
        {
          entity2Pos = Vector2_Add(entity2Pos, Vector2_Scale(RigidbodyComponent_GetVelocity(rigidbody2->data), 0.35f));
        }
        float widthDiff = (float)fabs(entity1Pos.x - entity2Pos.x);
        float heightDiff = (float)fabs(entity1Pos.y - entity2Pos.y);
        if (widthDiff > maxWidthDiff)
        {
          farthestx1 = entity1Pos;
          farthestx2 = entity2Pos;
          maxWidthDiff = widthDiff;
        }
        if (heightDiff > maxHeightDiff)
        {
          farthesty1 = entity1Pos;
          farthesty2 = entity2Pos;
          maxHeightDiff = heightDiff;
        }
      }
    }

    average.x = Vector2_Average(farthestx1, farthestx2).x;
    average.y = Vector2_Average(farthesty1, farthesty2).y;

    maxWidthDiff = max(maxWidthDiff, screenWidth * 2.5f) + screenWidth / 2;
    maxHeightDiff = max(maxHeightDiff, screenHeight * 2.5f) + screenHeight / 2;
    maxWidthDiff *= 1.0f;
    maxHeightDiff *= 1.0f;
    if (maxWidthDiff > maxHeightDiff * screenWidthHeightRatio)
    {
      //camera.targetPosition = Vector2_Average(farthestx1, farthestx2);
      camera.targetSize = maxWidthDiff / screenWidth;
      //int testSize = (int)(camera.targetSize * camera.targetSize / 4 + camera.targetSize / 4);
      //camera.targetSize = sqrtf(testSize * 4);
    }
    else
    {
      //camera.targetPosition = Vector2_Average(farthesty1, farthesty2);
      camera.targetSize = maxHeightDiff / screenHeight;
      //int testSize = (int)(camera.targetSize * camera.targetSize / 4 + camera.targetSize / 4);
      //camera.targetSize = sqrtf(testSize * 4);
    }
    camera.pastTargetSizes[camera.curTargetSizeIndex] = camera.targetSize;
    camera.curTargetSizeIndex++;
    camera.curTargetSizeIndex %= CAMERA_SIZE_TRACK_NUM;
    for (i = 0; i < CAMERA_SIZE_TRACK_NUM; i++)
    {
      camera.targetSize = max(camera.targetSize, camera.pastTargetSizes[i]);
    }
    camera.targetPosition = average;
  }

  camera.position = Vector2_Lerp(camera.position, camera.targetPosition, camera.positionLerp * dt, true);
  camera.size += (camera.targetSize - camera.size) * min(camera.sizeLerp * dt, 1);
  camera.matrixDirty = true;
}

/*!
 * \brief
 *   Tells the camera to track a given entitiy. Will not track the same
 *   entity more than once.
 * \param entity
 *   The entity to track.
 * \return value
 */
void Camera_TrackEntity(Entity *entity)
{
  int i; //Loop variable

  //For all the tracked entities,
  for (i = 0; i < trackedEntitiesCount; i++)
  {
    //If the one we want to track is already here,
    if (trackedEntities[i] == entity)
    {
      return;
    }
  }

  //If we need more space for the tracked entities,
  if (trackedEntitiesCount + 1 == trackedEntitiesMax)
  {
    //Allocate more space
    trackedEntities = (Entity**)realloc(trackedEntities, trackedEntitiesMax * 2 * sizeof(Entity*));
    trackedEntitiesMax *= 2;
  }

  //Add the entity to the tracked list
  trackedEntities[trackedEntitiesCount] = entity;

  trackedEntitiesCount++;
}

/*!
 * \brief
 *   Removes an entity from the tracking list. If it is not there,
 *   nothing will happen.
 * \param entity
 *   The entity to remove from the list of tracked entities.
 */
void Camera_UntrackEntity(Entity *entity)
{
  bool found = false;
  int i;
  for (i = 0; i < trackedEntitiesCount; i++)
  {
    if (trackedEntities[i] == entity)
    {
      found = true;
      break;
    }
  }
  for (; i < trackedEntitiesCount - 1; i++)
  {
    trackedEntities[i] = trackedEntities[i + 1];
  }

  //If the entity was somewhere in the list,
  if (found)
    trackedEntitiesCount--;
}

void Camera_UntrackAll()
{
  int i;
  for (i = 0; i < trackedEntitiesCount; i++)
  {
    trackedEntities[i] = NULL;
  }
  trackedEntitiesCount = 0;
}

/*!
 * \brief
 *   Tells the camera to either start or stop tracking.
 * \param isTracking
 *   If true, the camera will start tracking, if false it will stop tracking.
 */
void Camera_SetTracking(bool isTracking)
{
  trackingBehavior = isTracking;
}

/*!
 * \brief
 *   Sets the position and target position of the camera.
 * \param x
 *   The X coordinate to use.
 * \param y
 *   The Y coordinate to use.
 */
void Camera_SetPosition(float x, float y)
{
  camera.position.x = x;
  camera.position.y = y;
  camera.targetPosition.x = x;
  camera.targetPosition.y = y;
  camera.matrixDirty = true;
}

/*!
 * \brief
 *   Adds a given offset to the current position. Does not update the
 *   target position.
 * \param x
 *   The X offset to use.
 * \param y
 *   The Y offset to use.
 */
void Camera_AddPosition(float x, float y)
{
  camera.position.x += x;
  camera.position.y += y;
  camera.matrixDirty = true;
}

/*!
 * \brief
 *   Sets the target position of the camera.
 * \param x
 *   The X coordinate to use.
 * \param y
 *   The Y coordinate to use.
 */
void Camera_SetTargetPosition(float x, float y)
{
  camera.targetPosition.x = x;
  camera.targetPosition.y = y;
}

/*!
 * \brief
 *   Adds an offset to the target position of the camera.
 * \param x
 *   The X offset to use.
 * \param y
 *   The Y offset to use.
 */
void Camera_AddTargetPosition(float x, float y)
{
  camera.targetPosition.x += x;
  camera.targetPosition.y += y;
}

/*!
 * \brief
 *   Sets the size and target size of the camera. Limits it to >= 0.01.
 * \param size
 *   The size to use.
 */
void Camera_SetSize(float size)
{
  if (size < 0.01f)
    size = 0.01f;
  camera.size = size;
  camera.targetSize = size;
  camera.matrixDirty = true;
}

/*!
 * \brief
 *   Adds to the size of the camera. Limits it to >= 0.01. Does not
 *   modify the target size.
 * \param size
 *   The size to add.
 */
void Camera_AddSize(float size)
{
  camera.matrixDirty = true;
  camera.size += size;
  if (camera.size < 0.01f)
    camera.size = 0.01f;
}

/*!
 * \brief
 *   Sets the target size of the camera. Limits it to >= 0.01.
 * \param size
 *   The size to use.
 */
void Camera_SetTargetSize(float size)
{
  if (size < 0.01f)
    size = 0.01f;
  camera.targetSize = size;
}

/*!
 * \brief
 *   Adds to the target size of the camera. Limits it to >= 0.01.
 * \param size
 *   The size to add.
 */
void Camera_AddTargetSize(float size)
{
  camera.targetSize += size;
  if (camera.targetSize < 0.01f)
    camera.targetSize = 0.01f;
}

/*!
 * \brief
 *   Gets the current size of the camera.
 * \return
 *   The current size of the camera.
 */
float Camera_GetSize(void)
{
  return camera.size;
}

/*!
 * \brief
 *   Gets the current target size of the camera.
 * \return
 *   The current target size of the camera.
 */
float Camera_GetTargetSize(void)
{
  return camera.targetSize;
}

/*!
 * \brief
 *   Gets the current position of the camera.
 * \return
 *   The current position of the camera.
 */
Vector2 Camera_GetPosition(void)
{
  return camera.position;
}

/*!
 * \brief
 *   Gets the current target position of the camera.
 * \return
 *   The current target position of the camera.
 */
Vector2 Camera_GetTargetPosition(void)
{
  return camera.targetPosition;
}

/*!
 * \brief
 *   Gets the current render scale factor of the camera, which is 1/size.
 * \return
 *   The current inverse size of the camera.
 */
float Camera_GetRenderScaleFactor(void)
{
  return 1 / camera.size;
}

/*!
 * \brief
 *   Gets the transformation matrix of the camera - CURRENTLY FLAWED. ONLY GIVES SCALE, NO TRANSLATION.
 * \return
 *   The transformaiton matrix of the camera.
 */
Matrix3x3* Camera_GetMatrix(void)
{
  if (camera.matrixDirty)
  {
    //Vector2 negPos = Vector2_Scale(&camera.position, -1);
    //camera.matrix = Matrix3x3_BuildCamera(&negPos, 1 / camera.size); //TODO: Try to make the matrix account for rotation?
    camera.matrix.data[0][0] = 1 / camera.size;
    camera.matrix.data[1][1] = 1 / camera.size;
    camera.matrix.data[2][2] = 1 / camera.size;
    camera.matrix.data[0][2] = -camera.position.x / camera.size;
    camera.matrix.data[1][2] = -camera.position.y / camera.size;
    camera.matrixDirty = false;
  }
  return &camera.matrix;
}

/*!
 * \brief
 *   Gets the mouse position in the world.
 * \return
 *   A vector2 representing the mouse's position in the world.
 */
Vector2 Camera_MouseToWorld(void)
{
  s32 x, y;
  float worldX, worldY;
  AEGfxSetCamPosition(0, 0);
  AEInputGetCursorPosition(&x, &y);
  AEGfxConvertScreenCoordinatesToWorld((float)x, (float)y, &worldX, &worldY);

  float mouseX = worldX * camera.size + camera.position.x;
  float mouseY = worldY * camera.size + camera.position.y;

  Vector2 mouse;
  mouse.x = mouseX;
  mouse.y = mouseY;

  return mouse;
}

/*!
 * \brief
 *   Draws a line in either world space or screen space. Works as AEGfxLine.
 * \param x1
 *   The first X coordinate to use.
 * \param y1
 *   The first Y coordinate to use.
 * \param r1
 *   The red component of the first vertex of the line.
 * \param g1
 *   The green component of the first vertex of the line.
 * \param b1
 *   The blue component of the first vertex of the line.
 * \param a1
 *   The alpha component of the first vertex of the line.
 * \param x2
 *   The second X coordinate to use.
 * \param y2
 *   The second Y coordinate to use.
 * \param r2
 *   The red component of the second vertex of the line.
 * \param g2
 *   The green component of the second vertex of the line.
 * \param b2
 *   The blue component of the second vertex of the line.
 * \param a2
 *   The alpha component of the second vertex of the line.
 * \param worldSpace
 *   If true, the line will be drawn in world space. If false, it will be
 *   drawn in screen space.
 */
void Camera_DrawLine(float x1, float y1, float r1, float g1, float b1, float a1, float x2, float y2, float r2, float g2, float b2, float a2, bool worldSpace)
{
  AEGfxSetPosition(0, 0);
  if (worldSpace)
  {
    x1 -= camera.position.x;
    x1 *= 1 / camera.size;
    y1 -= camera.position.y;
    y1 *= 1 / camera.size;

    x2 -= camera.position.x;
    x2 *= 1 / camera.size;
    y2 -= camera.position.y;
    y2 *= 1 / camera.size;
  }
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  AEGfxLine(x1, y1, 0, r1, g1, b1, a1, x2, y2, 0, r2, g2, b2, a2);
}

/*!
 * \brief
 *   Draws a box out of lines, starting a given center, with a given width and height and color.
 */
void Camera_DrawLineBox(Vector2 center, float halfWidth, float halfHeight, float r, float g, float b, float a)
{
  Camera_DrawLine(center.x - halfWidth, center.y - halfHeight, r, g, b, a, center.x + halfWidth, center.y - halfWidth, r, g, b, a, true);
  Camera_DrawLine(center.x - halfWidth, center.y + halfHeight, r, g, b, a, center.x + halfWidth, center.y + halfWidth, r, g, b, a, true);
  Camera_DrawLine(center.x - halfWidth, center.y - halfHeight, r, g, b, a, center.x - halfWidth, center.y + halfWidth, r, g, b, a, true);
  Camera_DrawLine(center.x + halfWidth, center.y - halfHeight, r, g, b, a, center.x + halfWidth, center.y + halfWidth, r, g, b, a, true);
}

void Camera_Exit()
{
  if (trackedEntities)
    free(trackedEntities);
  trackedEntitiesCount = 0;
  trackedEntitiesMax = 0;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
