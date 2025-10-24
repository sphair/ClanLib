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
#include "mat2.h"
#include "mat3.h"
#include "angle.h"

template<typename Type>
class CL_Mat2;

template<typename Type>
class CL_Mat3;

template<typename Type>
class CL_Mat4;

template<typename Type>
class CL_Vec3;

class CL_Angle;

/// \brief 4D matrix
///
/// These matrix templates are defined for: int (CL_Mat4i), float (CL_Mat4f), double (CL_Mat4d)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Mat4
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a 4x4 matrix (null)
	CL_Mat4()
	{
		for (int i=0; i<16; i++)
			matrix[i] = 0;
	}

	/// \brief Constructs a 4x4 matrix (copied)
	CL_Mat4(const CL_Mat4<Type> &copy)
	{
		for (int i=0; i<16; i++)
			matrix[i] = copy.matrix[i];
	}

	/// \brief Constructs a 4x4 matrix (copied from a 2d matrix)
	CL_Mat4(const CL_Mat2<Type> &copy);

	/// \brief Constructs a 4x4 matrix (copied from a 3d matrix)
	CL_Mat4(const CL_Mat3<Type> &copy);

	/// \brief Constructs a 4x4 matrix (copied from a array of floats)
	CL_Mat4(const float *init_matrix)
	{
		for (int i=0; i<16; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 4x4 matrix (copied from a array of doubles)
	CL_Mat4(const double *init_matrix)
	{
		for (int i=0; i<16; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 4x4 matrix (copied from a array of 64 bit integers)
	CL_Mat4(const cl_int64 *init_matrix)
	{
		for (int i=0; i<16; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 4x4 matrix (copied from a array of 32 bit integers)
	CL_Mat4(const cl_int32 *init_matrix)
	{
		for (int i=0; i<16; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 4x4 matrix (copied from a array of 16 bit integers)
	CL_Mat4(const cl_int16 *init_matrix)
	{
		for (int i=0; i<16; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Constructs a 4x4 matrix (copied from a array of 8 bit integers)
	CL_Mat4(const cl_int8 *init_matrix)
	{
		for (int i=0; i<16; i++)
			matrix[i] = (Type) init_matrix[i];
	}

	/// \brief Create a zero matrix
	///
	/// \return The matrix
	static CL_Mat4<Type> null();

	/// \brief Create the identity matrix
	/// \return The matrix
	static CL_Mat4<Type> identity();

	/// \brief Create a frustum matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> frustum(Type left, Type right, Type bottom, Type top, Type z_near, Type z_far);

	/// \brief Create a perspective matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> perspective(
		Type field_of_view_y_degrees,
		Type aspect,
		Type z_near,
		Type z_far);

	/// \brief Create a ortho matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> ortho(Type left, Type right, Type bottom, Type top, Type z_near, Type z_far);

	/// \brief Create a ortho_2d matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> ortho_2d(Type left, Type right, Type bottom, Type top);

	/// \brief Create a rotation matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \param angle = Angle to rotate by
	/// \param x = Amount to rotate in the X axis
	/// \param y = Amount to rotate in the Y axis
	/// \param z = Amount to rotate in the Z axis
	/// \param normalize = true = Normalize x,y,z before creating rotation matrix
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> rotate(const CL_Angle &angle, Type x, Type y, Type z, bool normalize = true);

	/// \brief Create a rotation matrix using euler angles
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	///
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> rotate(const CL_Angle &angle_x, const CL_Angle &angle_y, const CL_Angle &angle_z, CL_EulerOrder order);

	/// \brief Create a scale matrix
	///
	/// \param x = Scale X
	/// \param y = Scale Y
	/// \param z = Scale Z
	/// \return The matrix
	static CL_Mat4<Type> scale(Type x, Type y, Type z);

	/// \brief Create a translation matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \param x = Translate X
	/// \param y = Translate Y
	/// \param z = Translate Z
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> translate(Type x, Type y, Type z);

	/// \brief Create the "look at" matrix
	///
	/// Matrix is created in the Column-Major matrix format (opengl native)
	/// \param eye_x = Eye position X
	/// \param eye_y = Eye position Y
	/// \param eye_z = Eye position Z
	/// \param center_x = Center X
	/// \param center_y = Center Y
	/// \param center_z = Center Z
	/// \param up_x = Translation X
	/// \param up_y = Translation Y
	/// \param up_z = Translation Z
	/// \return The matrix (in column-major format)
	static CL_Mat4<Type> look_at(
		Type eye_x, Type eye_y, Type eye_z,
		Type center_x, Type center_y, Type center_z,
		Type up_x, Type up_y, Type up_z);

	/// \brief Multiply 2 matrices
	///
	/// This multiplies the matrix as follows: result = matrix1 * matrix2
	///
	/// \param matrix_1 = First Matrix to multiply
	/// \param matrix_2 = Second Matrix to multiply
	/// \return The matrix
	static CL_Mat4<Type> multiply(const CL_Mat4<Type> &matrix_1, const CL_Mat4<Type> &matrix_2);

	/// \brief Add 2 matrices
	///
	/// This adds the matrix as follows: result = matrix1 + matrix2
	///
	/// \param matrix_1 = First Matrix to add
	/// \param matrix_2 = Second Matrix to add
	/// \return The matrix
	static CL_Mat4<Type> add(const CL_Mat4<Type> &matrix_1, const CL_Mat4<Type> &matrix_2);

	/// \brief Subtract 2 matrices
	///
	/// This subtracts the matrix as follows: result = matrix1 - matrix2
	///
	/// \param matrix_1 = First Matrix to subtract
	/// \param matrix_2 = Second Matrix to subtract
	/// \return The matrix
	static CL_Mat4<Type> subtract(const CL_Mat4<Type> &matrix_1, const CL_Mat4<Type> &matrix_2);

	/// \brief Calculate the adjoint (or known as Adjugate or Conjugate Transpose) of a matrix
	///
	/// \param matrix = The matrix to use
	/// \return The adjoint matrix
	static CL_Mat4<Type> adjoint(const CL_Mat4<Type> &matrix);

	/// \brief Calculate the matrix inverse of a matrix
	///
	/// Creates a zero matrix if the determinent == 0
	/// \param matrix = The matrix to use
	/// \return The inversed matrix
	static CL_Mat4<Type> inverse(const CL_Mat4<Type> &matrix);

	/// \brief Calculate the transpose of a matrix 
	///
	/// \param matrix = The matrix to use
	/// \return The transposed matrix
	static CL_Mat4<Type> transpose(const CL_Mat4<Type> &matrix);

/// \}
/// \name Attributes
/// \{

public:
	/// The matrix (in column-major format)
	Type matrix[16];

	/// \brief Returns the x coordinate for the point (0,0,0) multiplied with this matrix.
	Type get_origin_x() const { return matrix[12]; }

	/// \brief Returns the y coordinate for the point (0,0,0) multiplied with this matrix.
	Type get_origin_y() const { return matrix[13]; }

	/// \brief Returns the z coordinate for the point (0,0,0) multiplied with this matrix.
	Type get_origin_z() const { return matrix[14]; }

	/// \brief Extract the euler angles (in radians) from a matrix (in column-major format)
	///
	/// \return The x,y,z angles (in radians)
	CL_Vec3<Type> get_euler(CL_EulerOrder order) const;

	/// \brief Get a transformed point from the matrix (in column-major format)
	///
	/// \return The transformed point
	CL_Vec3<Type> get_transformed_point(const CL_Vec3<Type> &vector) const;

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
	CL_Mat4<Type> &multiply(const CL_Mat4<Type> &mult);

	/// \brief Add this matrix and a specified matrix.
	///
	/// This adds the matrix as follows: this = mult + this
	///
	/// \param add_matrix = Matrix to add
	///
	/// \return reference to this object
	CL_Mat4<Type> &add(const CL_Mat4<Type> &add_matrix);

	/// \brief Subtract this matrix and a specified matrix.
	///
	/// This subtracts the matrix as follows: this = mult - this
	///
	/// \param sub_matrix = Matrix to subtract
	///
	/// \return reference to this object
	CL_Mat4<Type> &subtract(const CL_Mat4<Type> &sub_matrix);

	/// \brief Scale this matrix
	///
	/// This is faster than using: multiply(CL_Mat4<Type>::scale(x,y,z) )
	///
	/// \param x = Scale X
	/// \param y = Scale Y
	/// \param z = Scale Z
	///
	/// \return reference to this object
	CL_Mat4<Type> &scale_self(Type x, Type y, Type z);

	/// \brief Translate this matrix
	///
	/// Matrix is assumed to be in the Column-Major matrix format (opengl native)\n
	/// This is faster than using: multiply(CL_Mat4<Type>::translate(x,y,z) )
	///
	/// \param x = Translate X
	/// \param y = Translate Y
	/// \param z = Translate Z
	///
	/// \return reference to this object
	CL_Mat4<Type> &translate_self(Type x, Type y, Type z);

	/// \brief Calculate the matrix determinant of this matrix
	///
	/// \return The determinant
	double det() const;

	/// \brief Calculate the adjoint (or known as adjugate) of this matrix
	///
	/// \return reference to this object
	CL_Mat4<Type> &adjoint();

	/// \brief Calculate the matrix inverse of this matrix
	///
	/// Creates a zero matrix if the determinent == 0
	/// \return reference to this object
	CL_Mat4<Type> &inverse();

	/// \brief Calculate the transpose of this matrix 
	///
	/// \return reference to this object
	CL_Mat4<Type> &transpose();

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
	CL_Mat4<Type> &operator =(const CL_Mat4<Type> &copy) {memcpy(matrix, copy.matrix, sizeof(matrix)); return *this; }

	/// \brief Copy assignment operator.
	CL_Mat4<Type> &operator =(const CL_Mat3<Type> &copy);

	/// \brief Copy assignment operator.
	CL_Mat4<Type> &operator =(const CL_Mat2<Type> &copy);

	/// \brief Multiplication operator.
	CL_Mat4<Type> operator *(const CL_Mat4<Type> &mult) const { CL_Mat4<Type> result = mult; result.multiply(*this); return result; }

	/// \brief Addition operator.
	CL_Mat4<Type> operator +(const CL_Mat4<Type> &add_matrix) const { CL_Mat4<Type> result = add_matrix; result.add(*this); return result; }

	/// \brief Subtraction operator.
	CL_Mat4<Type> operator -(const CL_Mat4<Type> &sub_matrix) const { CL_Mat4<Type> result = sub_matrix; result.subtract(*this); return result; }

	/// \brief Equality operator.
	bool operator==(const CL_Mat4<Type> &other)
	{
		for (int i=0; i<16; i++)
			if (matrix[i] != other.matrix[i]) return false;
		return true;
	}

	/// \brief Not-equal operator.
	bool operator!=(const CL_Mat4<Type> &other) { return !((*this) == other); }

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

typedef CL_Mat4<int> CL_Mat4i;
typedef CL_Mat4<float> CL_Mat4f;
typedef CL_Mat4<double> CL_Mat4d;

/// \}
