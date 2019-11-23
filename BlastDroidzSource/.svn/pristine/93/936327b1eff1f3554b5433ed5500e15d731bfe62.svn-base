/******************************************************************************/
/*!
	\file   Matrix3x3.h
	\author Evan Kau
	\date   1/16/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#pragma once
#include "Vector2.h"

/*------------------------------------------------------------------------------
// Public Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Structures:
//----------------------------------------------------------------------------*/

typedef struct Matrix3x3
{
  float data[3][3];
} Matrix3x3;

/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 * \brief Creates an identity matrix and returns it.
 * \return An identity matrix.
 */
Matrix3x3 Matrix3x3_Identity();

/*!
 * \brief Gets a given row from a matrix and returns it.
 * \param matrix
 *   The matrix to get the row from.
 * \param row
 *   The row to get from the matrix.
 * \return The Vector2 repesented by the given row of the given matrix.
 */
Vector2 Matrix3x3_GetRow(Matrix3x3 *matrix, int row);

/*!
 * \brief Gets a given column from a matrix and returns it.
 * \param matrix
 *   The matrix to get the column from.
 * \param column
 *   The row to get from the matrix.
 * \return The Vector2 repesented by the given column of the given matrix.
 */
Vector2 Matrix3x3_GetColumn(Matrix3x3 *matrix, int column);

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
void Matrix3x3_SetRow(Matrix3x3 *matrix, Vector2 vector, int row);

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
void Matrix3x3_SetColumn(Matrix3x3 *matrix, Vector2 vector, int column);

/*!
 * \brief Takes a matrix and scales the top two rows it by a given value.
 * \param matrix
 *   The matrix to scale.
 * \param scale
 *   The value to scale the matrix by.
 * \return A matrix where the top two rows are scaled by the given value.
 */
Matrix3x3 Matrix3x3_Scale(Matrix3x3 *matrix, float scale);

/*!
 * \brief Multiplies [AKA concatenates] two matrices together, and returns the result.
 * \param first
 *   The first matrix to multiply.
 * \param second
 *   The second matrix to multiply
 * \return The product of multiplying the two given matrices together.
 */
Matrix3x3 Matrix3x3_Multiply(Matrix3x3 *first, Matrix3x3 *second);

/*!
 * \brief Calculates and returns the determinant of the matrix.
 * \param matrix
 *   The matrix to calculate the determinant of.
 * \return The determinant of the given matrix.
 */
float Matrix3x3_Determinant(Matrix3x3 *matrix);

/*!
 * \brief Calculates the inverse matrix of a matrix and returns the result.
 * \param matrix
 *   The matrix to calculate the inverse of.
 * \return The inverse matrix of the given matrix.
 */
Matrix3x3 Matrix3x3_Inverse(Matrix3x3 *matrix);

/*!
 * \brief Transposes the values of a matrix and returns the result.
 * \param matrix
 *   The matrix to transpose.
 * \return The transpose of the given matrix.
 */
Matrix3x3 Matrix3x3_Transpose(Matrix3x3 *matrix);

/*!
 * \brief Calculates the inverse transpose of a matrix and returns the result.
 * \param matrix
 *   The matrix to calculate the inverse transpose of.
 * \return The inverse transpose of the given matrix.
 */
Matrix3x3 Matrix3x3_InverseTranspose(Matrix3x3 *matrix);

/*!
 * \brief Transforms a vector by the given matrix and returns the result.
 * \param matrix
 *   The matrix to transform the vector with.
 * \param vector
 *   The vector to be transformed.
 * \return The vector transformed by the given matrix.
 */
Vector2 Matrix3x3_TransformVector(Matrix3x3 *matrix, Vector2 vector);

/*!
 * \brief Transforms a vector by the given matrix and returns the result.
 * \param matrix
 *   The matrix to transform the vector with.
 * \param point
 *   A Vector2 representing the point to be transformed.
 * \return The point transformed by the given matrix.
 */
Vector2 Matrix3x3_TransformPoint(Matrix3x3 *matrix, Vector2 point);

/*!
 * \brief Generates a transform matrix based off of the values passed in.
 * \param position
 *   The Vector2 representing the position of the object.
 * \param rotation
 *   The float representing the rotaiton of the object.
 * \param scale
 *   The Vector2 representing the scale of the object.
 * \return The Matrix3x3 that is the transform matrix generated from the given values.
 */
Matrix3x3 Matrix3x3_BuildTransform(Vector2 position, float rotation, Vector2 scale);

Matrix3x3 Matrix3x3_BuildCamera(Vector2 translation, float inverseSize);