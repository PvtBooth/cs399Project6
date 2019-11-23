/******************************************************************************/
/*!
	\file   Matrix3x3.c
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Matrix3x3.h"
#include "Unreferenced_Parameter.h"
#include <math.h>

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

static void MakeScaleMatrix(Matrix3x3 *matrix, Vector2 scale);

static void MakeRotationMatrix(Matrix3x3 *matrix, float rotation);

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief Creates an identity matrix and returns it.
 * \return An identity matrix.
 */
Matrix3x3 Matrix3x3_Identity()
{
  return (Matrix3x3){ 1, 0, 0, 0, 1, 0, 0, 0, 1 };
}

/*!
 * \brief Gets a given row from a matrix and returns it.
 * \param matrix
 *   The matrix to get the row from.
 * \param row
 *   The row to get from the matrix.
 * \return The Vector2 repesented by the given row of the given matrix.
 */
Vector2 Matrix3x3_GetRow(Matrix3x3 *matrix, int row)
{
  Vector2 result = { 0, 0 };
  if (row >= 0 && row <= 2)
  {
    result.x = matrix->data[row][0];
    result.y = matrix->data[row][1];
  }
  return result;
}

/*!
 * \brief Gets a given column from a matrix and returns it.
 * \param matrix
 *   The matrix to get the column from.
 * \param column
 *   The row to get from the matrix.
 * \return The Vector2 repesented by the given column of the given matrix.
 */
Vector2 Matrix3x3_GetColumn(Matrix3x3 *matrix, int column)
{
  Vector2 result = { 0, 0 };
  if (column >= 0 && column <= 2)
  {
    result.x = matrix->data[0][column];
    result.y = matrix->data[1][column];
  }
  return result;
}

/*!
 * \brief Sets a given row from a matrix.
 * \param matrix
 *   The matrix to set the row of.
 * \param vector
 *   The values to set the given row to in the matrix.
 * \param row
 *   The row to set from the given matrix.
 * \return Void.
 */
void Matrix3x3_SetRow(Matrix3x3 *matrix, Vector2 vector, int row)
{
  if (row >= 0 && row <= 2)
  {
    matrix->data[row][0] = vector.x;
    matrix->data[row][1] = vector.y;
  }
}

/*!
 * \brief Sets a given column from a matrix.
 * \param matrix
 *   The matrix to set the column of.
 * \param vector
 *   The values to set the given column to in the matrix.
 * \param column
 *   The column to set from the given matrix.
 * \return Void.
 */
void Matrix3x3_SetColumn(Matrix3x3 *matrix, Vector2 vector, int column)
{
  if (column >= 0 && column <= 2)
  {
    matrix->data[0][column] = vector.x;
    matrix->data[1][column] = vector.y;
  }
}

/*!
 * \brief Takes a matrix and scales the top two rows it by a given value.
 * \param matrix
 *   The matrix to scale.
 * \param scale
 *   The value to scale the matrix by.
 * \return A matrix where the top two rows are scaled by the given value.
 */
Matrix3x3 Matrix3x3_Scale(Matrix3x3 *matrix, float scale)
{
  Matrix3x3 result = *matrix;
  int m, n;
  for (m = 0; m < 1; m++)
  {
    for (n = 0; n < 2; n++)
    {
      result.data[m][n] *= scale;
    }
  }
  return result;
}

/*!
 * \brief Multiplies two matrices together, and returns the result.
 * \param first
 *   The first matrix to multiply.
 * \param second
 *   The second matrix to multiply
 * \return The product of multiplying the two given matrices together.
 */
Matrix3x3 Matrix3x3_Multiply(Matrix3x3 *first, Matrix3x3 *second)
{
  Matrix3x3 result = {0};
  int m, n, o;
  for (m = 0; m < 3; m++)
  {
    for (n = 0; n < 3; n++)
    {
      for (o = 0; o < 3; o++)
      {
        result.data[m][n] += first->data[m][o] * second->data[o][n];
      }
    }
  }
  return result;
}

/*!
 * \brief Calculates and returns the determinant of the matrix.
 * \param matrix
 *   The matrix to calculate the determinant of.
 * \return The determinant of the given matrix.
 */
float Matrix3x3_Determinant(Matrix3x3 *matrix)
{
  float result;

  result =  matrix->data[0][0] * (matrix->data[1][1] * matrix->data[2][2] - matrix->data[2][1] * matrix->data[1][2]);
  result -= matrix->data[0][1] * (matrix->data[1][0] * matrix->data[2][2] - matrix->data[2][0] * matrix->data[1][2]);
  result += matrix->data[0][2] * (matrix->data[1][0] * matrix->data[2][1] - matrix->data[2][0] * matrix->data[1][1]);

  return result;
}

/*!
 * \brief Calculates the inverse matrix of a matrix and returns the result.
 * \param matrix
 *   The matrix to calculate the inverse of.
 * \return The inverse matrix of the given matrix.
 */
Matrix3x3 Matrix3x3_Inverse(Matrix3x3 *matrix)
{
  Matrix3x3 result = *matrix;
  float determinant = Matrix3x3_Determinant(matrix);
  if (determinant != 0)
  {
    int m, n;
    for (m = 0; m < 3; m++)
    {
      for (n = 0; n < 3; n++)
      {
        result.data[m][n] /= determinant * (1 - (2 * ((m + n) % 2)));
      }
    }
  }
  return result;
}

/*!
 * \brief Transposes the values of a matrix and returns the result.
 * \param matrix
 *   The matrix to transpose.
 * \return The transpose of the given matrix.
 */
Matrix3x3 Matrix3x3_Transpose(Matrix3x3 *matrix)
{
  Matrix3x3 result = *matrix;

  result.data[0][1] = matrix->data[1][0];
  result.data[0][2] = matrix->data[2][0];
  result.data[1][0] = matrix->data[0][1];
  result.data[1][2] = matrix->data[2][1];
  result.data[2][0] = matrix->data[0][2];
  result.data[2][1] = matrix->data[1][2];

  return result;
}

/*!
 * \brief Calculates the inverse transpose of a matrix and returns the result.
 * \param matrix
 *   The matrix to calculate the inverse transpose of.
 * \return The inverse transpose of the given matrix.
 */
Matrix3x3 Matrix3x3_InverseTranspose(Matrix3x3 *matrix)
{
  Matrix3x3 result = Matrix3x3_Inverse(matrix);
  result = Matrix3x3_Transpose(&result);
  return result;
}

/*!
 * \brief Transforms a vector by the given matrix and returns the result.
 * \param matrix
 *   The matrix to transform the vector with.
 * \param vector
 *   The vector to be transformed.
 * \return The vector transformed by the given matrix.
 */
Vector2 Matrix3x3_TransformVector(Matrix3x3 *matrix, Vector2 vector)
{
  Vector2 result;

  result.x = matrix->data[0][0] * vector.x + matrix->data[0][1] * vector.y;

  result.y = matrix->data[1][0] * vector.x + matrix->data[1][1] * vector.y;

  //We don't multiply by the third part of data on both the x and y because we assume z = 0 on the vector.

  return result;
}

/*!
 * \brief Transforms a vector by the given matrix and returns the result.
 * \param matrix
 *   The matrix to transform the vector with.
 * \param point
 *   A Vector2 representing the point to be transformed.
 * \return The point transformed by the given matrix.
 */
Vector2 Matrix3x3_TransformPoint(Matrix3x3 *matrix, Vector2 point)
{
  Vector2 result;

  result.x = matrix->data[0][0] * point.x + matrix->data[0][1] * point.y + matrix->data[0][2];

  result.y = matrix->data[1][0] * point.x + matrix->data[1][1] * point.y + matrix->data[1][2];

  //We add the affine component of the matrix because we assume z = 1 on the vector, because it's a point.
  //                                                                                                       That's the point.
  
  return result;
}

/*!
 * \brief Generates a transform matrix based off of the values passed in.
 * \param translation
 *   The Vector2 representing the translation of the object.
 * \param rotation
 *   The float representing the rotaiton of the object.
 * \param scale
 *   The Vector2 representing the scale of the object.
 * \return The Matrix3x3 that is the transform matrix generated from the given values.
 */
Matrix3x3 Matrix3x3_BuildTransform(Vector2 translation, float rotation, Vector2 scale)
{
  Matrix3x3 scaleMatrix = {0};
  Matrix3x3 rotationMatrix = {0};
  //Matrix3x3 translationMatrix = {0};
  Matrix3x3 result = {0};
  result.data[0][0] = 1;
  result.data[1][1] = 1;

  MakeScaleMatrix(&scaleMatrix, scale);
  MakeRotationMatrix(&rotationMatrix, rotation);
  //MakeTranslationMatrix(&translationMatrix, translation);

  result = Matrix3x3_Multiply(&scaleMatrix, &result);
  result = Matrix3x3_Multiply(&rotationMatrix, &result);
  result.data[0][2] = translation.x;
  result.data[1][2] = translation.y;
  result.data[2][2] = 1;

  return result;
}

/*!
 * \brief
 *   Builds a camera matrix from a translation [NOT USED] and the inverse size.
 * \param translation
 *   [NOT USED] The offset of the camera.
 * \param inverseSize
 *   The inverse size of the camera.
 * \return
 *   The resulting camera matrix.
 */
Matrix3x3 Matrix3x3_BuildCamera(Vector2 translation, float inverseSize)
{
  UNREFERENCED_PARAMETER(translation);
  Matrix3x3 result = { 0 };
  result.data[0][0] = inverseSize;
  result.data[1][1] = inverseSize;
  result.data[2][2] = inverseSize;
  //result.data[0][2] = translation->x;
  //result.data[1][2] = translation->y;
  return result;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void MakeScaleMatrix(Matrix3x3 *matrix, Vector2 scale)
{
  matrix->data[0][0] = scale.x;
  matrix->data[1][1] = scale.y;
}

static void MakeRotationMatrix(Matrix3x3 *matrix, float rotation)
{
  matrix->data[0][0] = cosf(rotation);
  matrix->data[0][1] = -sinf(rotation);
  matrix->data[1][0] = sinf(rotation);
  matrix->data[1][1] = cosf(rotation);
}
