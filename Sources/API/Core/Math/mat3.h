/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
**    Harry Storbacka
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "mat2.h"
#include "mat4.h"
#include "../IOData/datatypes.h"

template<typename Type>
class CL_Mat2;

template<typename Type>
class CL_Mat3;

template<typename Type>
class CL_Mat4;

class CL_Angle;

/// \brief 3D matrix
///
/// These matrix templates are defined for: int (CL_Mat3i), float (CL_Mat3f), double (CL_Mat3d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Mat3
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a 3x3 matrix (uninitialised)
	CL_Mat3() { }

	/// \brief Constructs a 3x3 matrix (copied)
	CL_Mat3(const CL_Mat3<Type> &copy)
	{
		for (int i=0; i<9; i++)
			matrix[i] = copy.matrix[i];
	}

	/// \brief Constructs a 3x3 matrix (copied from a 2d matrix)
	CL_Mat3(const CL_Mat2<Type> &copy);

	/// \brief Constructs a 3x3 matrix (copied from a 4d matrix)
	CL_Mat3(const CL_Mat4<Type> &copy);

	/// \brief Constructs a 3x3 matrix (copied from 9 floats)
	CL_Mat3(const float *init_matrix)
	{
		for (int i=0; i<9; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 3x3 matrix (copied from specified values)
	CL_Mat3(Type m00, Type m01, Type m02, Type m10, Type m11, Type m12, Type m20, Type m21, Type m22)
	{
		matrix[0 * 3 + 0] = m00; matrix[0 * 3 + 1] = m01; matrix[0 * 3 + 2] = m02;
		matrix[1 * 3 + 0] = m10; matrix[1 * 3 + 1] = m11; matrix[1 * 3 + 2] = m12;
		matrix[2 * 3 + 0] = m20; matrix[2 * 3 + 1] = m21; matrix[2 * 3 + 2] = m22;
	}

	/// \brief Constructs a 3x3 matrix (copied from 9 doubles)
	CL_Mat3(const double *init_matrix)
	{
		for (int i=0; i<9; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 3x3 matrix (copied from 9, 64 bit integers)
	CL_Mat3(const cl_int64 *init_matrix)
	{
		for (int i=0; i<9; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 3x3 matrix (copied from 9, 32 bit integers)
	CL_Mat3(const cl_int32 *init_matrix)
	{
		for (int i=0; i<9; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 3x3 matrix (copied from 9, 16 bit integers)
	CL_Mat3(const cl_int16 *init_matrix)
	{
		for (int i=0; i<9; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 3x3 matrix (copied from 9, 8 bit integers)
	CL_Mat3(const cl_int8 *init_matrix)
	{
		for (int i=0; i<9; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	static CL_Mat3<Type> null();

	static CL_Mat3<Type> identity();

	/// \brief Create a rotation matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \param angle = Angle to rotate by
	/// \param x = Amount to rotate in the X axis
	/// \param y = Amount to rotate in the Y axis
	/// \param z = Amount to rotate in the Z axis
	/// \param normalize = true = Normalize x,y,z before creating rotation matrix
	/// \return The matrix (in column-major format)
	static CL_Mat3<Type> rotate(const CL_Angle &angle, Type x, Type y, Type z, bool normalize = true);

	/// \brief Multiply 2 matrices
	///
	/// This multiplies the matrix as follows: result = matrix1 * matrix2
	///
	/// \param matrix_1 = First Matrix to multiply
	/// \param matrix_2 = Second Matrix to multiply
	/// \return The matrix
	static CL_Mat3<Type> multiply(const CL_Mat3<Type> &matrix_1, const CL_Mat3<Type> &matrix_2);

	/// \brief Add 2 matrices
	///
	/// This adds the matrix as follows: result = matrix1 + matrix2
	///
	/// \param matrix_1 = First Matrix to add
	/// \param matrix_2 = Second Matrix to add
	/// \return The matrix
	static CL_Mat3<Type> add(const CL_Mat3<Type> &matrix_1, const CL_Mat3<Type> &matrix_2);

	/// \brief Subtract 2 matrices
	///
	/// This subtracts the matrix as follows: result = matrix1 - matrix2
	///
	/// \param matrix_1 = First Matrix to subtract
	/// \param matrix_2 = Second Matrix to subtract
	/// \return The matrix
	static CL_Mat3<Type> subtract(const CL_Mat3<Type> &matrix_1, const CL_Mat3<Type> &matrix_2);

	/// \brief Calculate the adjoint (or known as Adjugate or Conjugate Transpose) of a matrix
	///
	/// \param matrix = The matrix to use
	/// \return The adjoint matrix
	static CL_Mat3<Type> adjoint(const CL_Mat3<Type> &matrix);

	/// \brief Calculate the matrix inverse of a matrix
	///
	/// Creates a zero matrix if the determinent == 0
	/// \param matrix = The matrix to use
	/// \return The inversed matrix
	static CL_Mat3<Type> inverse(const CL_Mat3<Type> &matrix);

	/// \brief Calculate the transpose of a matrix 
	///
	/// \param matrix = The matrix to use
	/// \return The transposed matrix
	static CL_Mat3<Type> transpose(const CL_Mat3<Type> &matrix);

/// \}
/// \name Attributes
/// \{

public:
	Type matrix[9];

/// \}
/// \name Operations
/// \{

public:
	/// \brief Multiply this matrix and a specified matrix.
	///
	/// This multiplies the matrix as follows: this = mult * this
	///
	/// \param mult = Matrix to multiply
	///
	/// \return reference to this object
	CL_Mat3<Type> &multiply(const CL_Mat3<Type> &mult);

	/// \brief Add this matrix and a specified matrix.
	///
	/// This adds the matrix as follows: this = add_matrix + this
	///
	/// \param add_matrix = Matrix to add
	///
	/// \return reference to this object
	CL_Mat3<Type> &add(const CL_Mat3<Type> &add_matrix);

	/// \brief Subtract this matrix and a specified matrix.
	///
	/// This subtracts the matrix as follows: this = sub_matrix - this
	///
	/// \param sub_matrix = Matrix to subtract
	///
	/// \return reference to this object
	CL_Mat3<Type> &subtract(const CL_Mat3<Type> &sub_matrix);

	/// \brief Calculate the matrix determinant
	double det() const;

	/// \brief Creates the adjoint (or known as adjugate) of the matrix
	///
	/// \return reference to this object
	CL_Mat3<Type> &adjoint();

	/// \brief Create the matrix inverse. (Returns a zero matrix if the determinent = 0)
	///
	/// \return reference to this object
	CL_Mat3<Type> &inverse();

	/// \brief Calculate the transpose of this matrix 
	///
	/// \return reference to this object
	CL_Mat3<Type> &transpose();

/// \}
/// \name Operators
/// \{

public:
	/// \brief Operator that returns the matrix as a array.
	operator Type const*() const { return matrix; }

	/// \brief Operator that returns the matrix as a array.
	operator Type *() { return matrix; }

	/// \brief Operator that returns the matrix cell at the given index.
	Type &operator[](int i) { return matrix[i]; }

	/// \brief Operator that returns the matrix cell at the given index.
	const Type &operator[](int i) const { return matrix[i]; }

	/// \brief Operator that returns the matrix cell at the given index.
	Type &operator[](unsigned int i) { return matrix[i]; }

	/// \brief Operator that returns the matrix cell at the given index.
	const Type &operator[](unsigned int i) const { return matrix[i]; }

	/// \brief Copy assignment operator.
	CL_Mat3<Type> &operator =(const CL_Mat3<Type> &copy) {memcpy(matrix, copy.matrix, sizeof(matrix)); return *this; }

	/// \brief Copy assignment operator.
	CL_Mat3<Type> &operator =(const CL_Mat4<Type> &copy);

	/// \brief Copy assignment operator.
	CL_Mat3<Type> &operator =(const CL_Mat2<Type> &copy);

	/// \brief Multiplication operator.
	CL_Mat3<Type> operator *(const CL_Mat4<Type> &mult) const { CL_Mat3<Type> result = mult; result.multiply(*this); return result; }

	/// \brief Addition operator.
	CL_Mat3<Type> operator +(const CL_Mat4<Type> &add_matrix) const { CL_Mat3<Type> result = add_matrix; result.add(*this); return result; }

	/// \brief Subtraction operator.
	CL_Mat3<Type> operator -(const CL_Mat4<Type> &sub_matrix) const { CL_Mat3<Type> result = sub_matrix; result.subtract(*this); return result; }

	/// \brief Equality operator.
	bool operator==(const CL_Mat3<Type> &other)
	{
		for (int i=0; i<9; i++)
			if (matrix[i] != other.matrix[i]) return false;
		return true;
	}

	/// \brief Not-equal operator.
	bool operator!=(const CL_Mat3<Type> &other) { return !((*this) == other); }

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

typedef CL_Mat3<int> CL_Mat3i;
typedef CL_Mat3<float> CL_Mat3f;
typedef CL_Mat3<double> CL_Mat3d;

/// \}
