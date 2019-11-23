/******************************************************************************/
/*!
	\file   Vector2.c
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Vector2.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

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

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief Returns a Vector2 with 0 for both components.
 * \return A Vector2 with 0 for both its X and Y componenents.
 */
Vector2 Vector2_Zero()
{
  Vector2 zeroVector = { 0, 0 };
  return zeroVector;
}

/*!
 * \brief Adds two Vector2s together.
 * \param first
 *   The first Vector2 to add.
 * \param second
 *   The second Vector2 to add.
 * \return A new Vector2 struct that is the sum of the Vector2s passed in.
 */
Vector2 Vector2_Add(Vector2 first, Vector2 second)
{
  first.x += second.x;
  first.y += second.y;
  return first;
}

/*!
 * \brief Adds two Vector2s together, with the second being passed in by components.
 * \param vector
 *   The first Vector2 to add.
 * \param x
 *   The X component of the second Vector2 to add.
 * \param y
 *   The Y component of the second Vector2 to add.
 * \return A new Vector2 struct that is the sum of the Vector2 and floats passed in.
 */
Vector2 Vector2_AddFloats(Vector2 vector, float x, float y)
{
  vector.x += x;
  vector.y += y;
  return vector;
}

/*!
 * \brief Subtracts two Vector2s from each other [first - second].
 * \param first
 *   The initial Vector2.
 * \param second
 *   The second Vector2 to subtract from the first Vector2.
 * \return A new Vector2 struct that is the difference of the first and second Vector2s passed in.
 */
Vector2 Vector2_Sub(Vector2 first, Vector2 second)
{
  first.x -= second.x;
  first.y -= second.y;
  return first;
}

/*!
 * \brief Subtracts two Vector2s from each other [first - second], with the second being passed in by components.
 * \param vector
 *   The initial Vector2.
 * \param x
 *   The X component of the second Vector2 to subtract from the first Vector2.
 * \param y
 *   The Y component of the second Vector2 to subtract from the first Vector2.
 * \return A new Vector2 struct that is the difference of the Vector2 and floats passed in.
 */
Vector2 Vector2_SubFloats(Vector2 vector, float x, float y)
{
  vector.x -= x;
  vector.y -= y;
  return vector;
}

/*!
 * \brief Creates a normalized Vector2 from a given Vector2.
 * \param vector
 *   The Vector2 to normalize.
 * \return A normalized Vector2 in the direction of the given Vector2.
 */
Vector2 Vector2_Normalize(Vector2 vector)
{
  float length = sqrtf(vector.x * vector.x + vector.y * vector.y);
  if (length != 0)
  {
    vector.x /= length;
    vector.y /= length;
  }
  else
  {
    vector.x = 1;
    vector.y = 0;
  }
  return vector;
}

/*!
 * \brief Calculates the length of a Vector2.
 * \param vector
 *   The Vector2 to get the length of.
 * \return The length of the given Vector2.
 */
float Vector2_Length(Vector2 vector)
{
  return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

/*!
 * \brief Calculates the length squared of a Vector2.
 * \param vector
 *   The Vector2 to get the length squared of.
 * \return The length squared of the given Vector2.
 */
float Vector2_LengthSquared(Vector2 vector)
{
  return vector.x * vector.x + vector.y * vector.y;
}

/*!
 * \brief Scales a Vector2 by a given scalar value.
 * \param vector
 *   The Vector2 to scale.
 * \param scale
 *   The scalar to multiply the vector's components by.
 * \return A Vector2 with its components scaled by the given scalar.
 */
Vector2 Vector2_Scale(Vector2 vector, float scale)
{
  vector.x *= scale;
  vector.y *= scale;
  return vector;
}

/*!
 * \brief
 *   Multiplies two vectors together via components, resulting in (x1 * x2, y1 * y2).
 * \param first
 *   The first Vector2 to multiply.
 * \param second
 *   The second Vector2 to multiply.
 * \return
 *   A Vector2 that is the component product of the two given vectors.
 */
Vector2 Vector2_ComponentProduct(Vector2 first, Vector2 second)
{
  first.x *= second.x;
  first.y *= second.y;
  return first;
}

/*!
 * \brief Calculates the dot product of two Vector2s.
 * \param first
 *   The first Vector2 to dot.
 * \param second
 *   The second Vector2 to dot.
 * \return The dot product of the first and second Vector2.
 */
float Vector2_Dot(Vector2 first, Vector2 second)
{
  return first.x * second.x + first.y * second.y;
}

/*!
 * \brief alculates the dot product of two Vector2s, with the second being passed in by components.
 * \param vector
 *   The first Vector2 to add.
 * \param x
 *   The X value of the second Vector2.
 * \param y
 *   The Y value of the second Vector2.
 * \return The dot product of the first and second Vector2.
 */
float Vector2_DotFloats(Vector2 vector, float x, float y)
{
  return vector.x * x + vector.y * y;
}

/*!
 * \brief Fakes crossing two Vector3s together, assuming the first only has a Z value and assuming the second only has X and Y values.
 * \param z
 *   The z value of the first Vector3 to cross.
 * \param vector
 *   The Vector2 that holds the X and Y values of the second Vector3 to cross.
 * \return A new Vector2 struct that has the X and Y values of the cross product of the two faked Vector3s.
 */
Vector2 Vector2_CrossFloatVector(float z, Vector2 vector)
{
  Vector2 result = { 0, 0 };
  result.x = -z * vector.y;
  result.y = z * vector.x;
  return result;
}

/*!
 * \brief Fakes crossing two Vector3s together, assuming both only have X and Y values.
 * \param first
 *   The Vector2 that holds the X and Y values of the first Vector3 to cross.
 * \param second
 *   The Vector2 that holds the X and Y values of the second Vector3 to cross.
 * \return The float that is the Z value of the cross product of the two faked Vector3s.
 */
float Vector2_CrossVectorVector(Vector2 first, Vector2 second)
{
  return first.x * second.y - first.y * second.x;
}

/*!
 * \brief Find the Vector2 orthogonal to a given vector, retaining its magnitude.
 * \param vector
 *   The vector to get the orthogonal vector to.
 * \return A Vector2 in the direction orthogonal to and with the same magnetude of the given vector.
 */
Vector2 Vector2_GetOrthogonal(Vector2 vector)
{
  Vector2 result;
  result.x = -vector.y;
  result.y = vector.x;
  return result;
}

/*!
 * \brief Calculates the average between two points.
 * \param first
 *   The first point to average.
 * \param second
 *   The second point to average.
 * \return A Vector2 that is the average between the two given points.
 */
Vector2 Vector2_Average(Vector2 first, Vector2 second)
{
  Vector2 result;
  result.x = (first.x + second.x) / 2;
  result.y = (first.y + second.y) / 2;
  return result;
}

/*!
 * \brief Calculates the distance between two points.
 * \param first
 *   The first point.
 * \param second
 *  The second point.
 * \return The distance between the two points.
 */
float Vector2_Distance(Vector2 first, Vector2 second)
{
  return sqrtf((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}

/*!
 * \brief Calculates the distance squared between two points.
 * \param first
 *   The first point.
 * \param second
 *  The second point.
 * \return The distance squared between the two points.
 */
float Vector2_DistanceSquared(Vector2 first, Vector2 second)
{
  return (first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y);
}

/*!
 * \brief
 *   Linearly interpolates between two Vector2s.
 * \param first
 *   The first Vector2 to use.
 * \param second
 *   The second Vector2 to use.
 * \param t
 *   The percentage [0-1] to go from the first Vector2 to the second.
 * \param clamp
 *   If true, the resulting Vector2 will be between the first and second Vector2. (0 <= t <= 1)
 *   If false, it can go past in either direction.
 * \return
 *   A Vector2 that is t% of the way from the first to the second Vector2.
 */
Vector2 Vector2_Lerp(Vector2 first, Vector2 second, float t, bool clamp)
{
  Vector2 result = first;
  if (clamp)
  {
    t = max(t, 0);
    t = min(t, 1);
  }
  result.x += (second.x - first.x) * t;
  result.y += (second.y - first.y) * t;
  return result;
}

/*!
 * \brief
 *   Gets the angle between two Vector2s.
 * \param first
 *   The first Vector2 to use.
 * \param second
 *   The second Vector2 to use.
 * \return
 *   The angle between the two Vector2s.
 */
float Vector2_GetAngle(Vector2 first, Vector2 second)
{
  Vector2 normFirst = Vector2_Normalize(first);
  Vector2 normSecond = Vector2_Normalize(second);
  float dot = max(min(Vector2_Dot(normFirst, normSecond), 1), -1); //Clamp to between 0 and 1
  float angle = acosf(dot);

  return angle;
}

/*!
 * \brief
 *   Gets the signed angle between two Vector2s, where counterclockwise is positive.
 * \param first
 *   The first Vector2 to use.
 * \param second
 *   The second Vector2 to use.
 * \return
 *   The signed angle between the two Vector2s.
 */
float Vector2_GetSignedAngle(Vector2 first, Vector2 second)
{
  Vector2 normFirst = Vector2_Normalize(first);
  Vector2 normSecond = Vector2_Normalize(second);
  Vector2 normNormalSecond = Vector2_GetOrthogonal(normSecond);
  if (normFirst.x == 0 && normFirst.y == 0)
    normFirst.x = 1;
  if (normSecond.x == 0 && normSecond.y == 0)
  {
    normSecond.x = 1;
    normNormalSecond.y = 1;
  }
  float dot = max(min(Vector2_Dot(normFirst, normSecond), 1), -1); //Clamp to between 0 and 1
  float angle = acosf(dot);

  if (Vector2_Dot(normNormalSecond, normFirst) > 0)
  {
    return -angle;
  }
  else
  {
    return angle;
  }
}

Vector2 Vector2_FromAngle(float angle)
{
  Vector2 result;
  result.x = cosf(angle);
  result.y = sinf(angle);
  return result;
}

float Vector2_ToAngle(Vector2 vector)
{
  return atan2f(vector.y, vector.x);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/
