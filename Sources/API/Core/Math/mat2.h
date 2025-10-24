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
#include "../IOData/datatypes.h"
#include "mat3.h"
#include "mat4.h"

template<typename Type>
class CL_Mat2;

template<typename Type>
class CL_Mat3;

template<typename Type>
class CL_Mat4;

class CL_Angle;

/// \brief 2D matrix
///
/// These matrix templates are defined for: int (CL_Mat2i), float (CL_Mat2f), double (CL_Mat2d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Mat2
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a 2x2 matrix (uninitialised)
	CL_Mat2() { }

	/// \brief Constructs a 2x2 matrix (copied)
	CL_Mat2(const CL_Mat2<Type> &copy)
	{
		for (int i=0; i<4; i++)
			matrix[i] = copy.matrix[i];
	}

	/// \brief Constructs a 2x2 matrix (copied from a 3d matrix)
	CL_Mat2(const CL_Mat3<Type> &copy);

	/// \brief Constructs a 2x2 matrix (copied from a 4d matrix)
	CL_Mat2(const CL_Mat4<Type> &copy);

	/// \brief Constructs a 2x2 matrix (copied from 4 floats)
	CL_Mat2(const float *init_matrix)
	{
		for (int i=0; i<4; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 2x2 matrix (copied from specified values)
	CL_Mat2(Type m00, Type m01, Type m10, Type m11)
	{
		matrix[0 * 2 + 0] = m00; matrix[0 * 2 + 1] = m01;
		matrix[1 * 2 + 0] = m10; matrix[1 * 2 + 1] = m11;
	}

	/// \brief Constructs a 2x2 matrix (copied from 4 doubles)
	CL_Mat2(const double *init_matrix)
	{
		for (int i=0; i<4; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 2x2 matrix (copied from 4, 64 bit integers)
	CL_Mat2(const cl_int64 *init_matrix)
	{
		for (int i=0; i<4; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 2x2 matrix (copied from 4, 32 bit integers)
	CL_Mat2(const cl_int32 *init_matrix)
	{
		for (int i=0; i<4; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 2x2 matrix (copied from 4, 16 bit integers)
	CL_Mat2(const cl_int16 *init_matrix)
	{
		for (int i=0; i<4; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 2x2 matrix (copied from 4, 8 bit integers)
	CL_Mat2(const cl_int8 *init_matrix)
	{
		for (int i=0; i<4; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	static CL_Mat2<Type> null();

	static CL_Mat2<Type> identity();

	/// \brief Multiply 2 matrices
	///
	/// This multiplies the matrix as follows: result = matrix1 * matrix2
	///
	/// \param matrix_1 = First Matrix to multiply
	/// \param matrix_2 = Second Matrix to multiply
	/// \return The matrix
	static CL_Mat2<Type> multiply(const CL_Mat2<Type> &matrix_1, const CL_Mat2<Type> &matrix_2);

	/// \brief Add 2 matrices
	///
	/// This adds the matrix as follows: result = matrix1 + matrix2
	///
	/// \param matrix_1 = First Matrix to add
	/// \param matrix_2 = Second Matrix to add
	/// \return The matrix
	static CL_Mat2<Type> add(const CL_Mat2<Type> &matrix_1, const CL_Mat2<Type> &matrix_2);

	/// \brief Subtract 2 matrices
	///
	/// This subtract the matrix as follows: result = matrix1 - matrix2
	///
	/// \param matrix_1 = First Matrix to subtract
	/// \param matrix_2 = Second Matrix to subtract
	/// \return The matrix
	static CL_Mat2<Type> subtract(const CL_Mat2<Type> &matrix_1, const CL_Mat2<Type> &matrix_2);

/// \}
/// \name Attributes
/// \{

public:
	Type matrix[4];

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
	CL_Mat2<Type> &multiply(const CL_Mat2<Type> &mult);

	/// \brief Add this matrix and a specified matrix.
	///
	/// This adds the matrix as follows: this = add_matrix - this
	///
	/// \param add_matrix = Matrix to add
	///
	/// \return reference to this object
	CL_Mat2<Type> &add(const CL_Mat2<Type> &add_matrix);

	/// \brief Subtract this matrix and a specified matrix.
	///
	/// This subtracts the matrix as follows: this = subtract_matrix - this
	///
	/// \param subtract_matrix = Matrix to subtract
	///
	/// \return reference to this object
	CL_Mat2<Type> &subtract(const CL_Mat2<Type> &subtract_matrix);

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
	CL_Mat2<Type> &operator =(const CL_Mat2<Type> &copy) {memcpy(matrix, copy.matrix, sizeof(matrix)); return *this; }

	/// \brief Copy assignment operator.
	CL_Mat2<Type> &operator =(const CL_Mat4<Type> &copy);

	/// \brief Copy assignment operator.
	CL_Mat2<Type> &operator =(const CL_Mat3<Type> &copy);

	/// \brief Multiplication operator.
	CL_Mat2<Type> operator *(const CL_Mat2<Type> &mult) const { CL_Mat2<Type> result = mult; result.multiply(*this); return result; }

	/// \brief Addition operator.
	CL_Mat2<Type> operator +(const CL_Mat2<Type> &add_matrix) const { CL_Mat2<Type> result = add_matrix; result.add(*this); return result; }

	/// \brief Subtract operator.
	CL_Mat2<Type> operator -(const CL_Mat2<Type> &subtract_matrix) const { CL_Mat2<Type> result = subtract_matrix; result.subtract(*this); return result; }

	/// \brief Equality operator.
	bool operator==(const CL_Mat2<Type> &other)
	{
		for (int i=0; i<4; i++)
			if (matrix[i] != other.matrix[i]) return false;
		return true;
	}

	/// \brief Not-equal operator.
	bool operator!=(const CL_Mat2<Type> &other) { return !((*this) == other); }

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

typedef CL_Mat2<int> CL_Mat2i;
typedef CL_Mat2<float> CL_Mat2f;
typedef CL_Mat2<double> CL_Mat2d;

/// \}
