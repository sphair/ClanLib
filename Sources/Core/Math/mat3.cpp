/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#include "Core/precomp.h"
#include "API/Core/Math/mat3.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Mat3 construction:

template<typename Type>
Mat3<Type>::Mat3(const Mat4<Type> &copy)
{
	matrix[0*3 + 0] = copy.matrix[0*4 + 0];
	matrix[0*3 + 1] = copy.matrix[0*4 + 1];
	matrix[0*3 + 2] = copy.matrix[0*4 + 2];
	matrix[1*3 + 0] = copy.matrix[1*4 + 0];
	matrix[1*3 + 1] = copy.matrix[1*4 + 1];
	matrix[1*3 + 2] = copy.matrix[1*4 + 2];
	matrix[2*3 + 0] = copy.matrix[2*4 + 0];
	matrix[2*3 + 1] = copy.matrix[2*4 + 1];
	matrix[2*3 + 2] = copy.matrix[2*4 + 2];
}

template<typename Type>
Mat3<Type>::Mat3(const Mat2<Type> &copy)
{
	matrix[0*3 + 0] = copy.matrix[0*2 + 0];
	matrix[0*3 + 1] = copy.matrix[0*2 + 1];
	matrix[0*3 + 2] = 0;
	matrix[1*3 + 0] = copy.matrix[1*2 + 0];
	matrix[1*3 + 1] = copy.matrix[1*2 + 1];
	matrix[1*3 + 2] = 0;
	matrix[2*3 + 0] = 0;
	matrix[2*3 + 1] = 0;
	matrix[2*3 + 2] = 1;
}

template<typename Type>
Mat3<Type> Mat3<Type>::null()
{
	Mat3<Type> m;
	memset(m.matrix, 0, sizeof(m.matrix));
	return m;
}

template<typename Type>
Mat3<Type> Mat3<Type>::identity()
{
	Mat3<Type> m = null();
	m.matrix[0] = 1;
	m.matrix[4] = 1;
	m.matrix[8] = 1;
	return m;
}

template<typename Type>
Mat3<Type> Mat3<Type>::rotate(const Angle &angle, Type x, Type y, Type z, bool normalize)
{
	if (normalize)
	{
		Type len2 = x*x+y*y+z*z;
		if (len2 != (Type)1)
		{	
			Type length = sqrt(len2);
			if (length > (Type) 0)
			{
				x /= length;
				y /= length;
				z /= length;
			}
			else
			{
				x = (Type) 0;
				y = (Type) 0;
				z = (Type) 0;
			}
		}
	}

	Mat3<Type> rotate_matrix;
	Type c = cos(angle.to_radians());
	Type s = sin(angle.to_radians());
	rotate_matrix.matrix[0+0*3] = (Type) (x*x*(1.0f - c) + c);
	rotate_matrix.matrix[0+1*3] = (Type) (x*y*(1.0f - c) - z*s);
	rotate_matrix.matrix[0+2*3] = (Type) (x*z*(1.0f - c) + y*s);
	rotate_matrix.matrix[1+0*3] = (Type) (y*x*(1.0f - c) + z*s);
	rotate_matrix.matrix[1+1*3] = (Type) (y*y*(1.0f - c) + c);
	rotate_matrix.matrix[1+2*3] = (Type) (y*z*(1.0f - c) - x*s);
	rotate_matrix.matrix[2+0*3] = (Type) (x*z*(1.0f - c) - y*s);
	rotate_matrix.matrix[2+1*3] = (Type) (y*z*(1.0f - c) + x*s);
	rotate_matrix.matrix[2+2*3] = (Type) (z*z*(1.0f - c) + c);
	return rotate_matrix;
}

// For ints
template<>
Mat3<int> Mat3<int>::rotate(const Angle &angle, int x, int y, int z, bool normalize)
{
	if (normalize)
	{
		int len2 = x*x+y*y+z*z;
		if (len2 != (int)1)
		{	
			int length = sqrt( (float) len2);
			if (length > 0)
			{
				x /= length;
				y /= length;
				z /= length;
			}
			else
			{
				x = 0;
				y = 0;
				z = 0;
			}
		}
	}

	Mat3<int> rotate_matrix;
	float c = cos(angle.to_radians());
	float s = sin(angle.to_radians());
	rotate_matrix.matrix[0+0*3] = (int) floor((x*x*(1.0f - c) + c)+0.5f);
	rotate_matrix.matrix[0+1*3] = (int) floor((x*y*(1.0f - c) - z*s)+0.5f);
	rotate_matrix.matrix[0+2*3] = (int) floor((x*z*(1.0f - c) + y*s)+0.5f);
	rotate_matrix.matrix[1+0*3] = (int) floor((y*x*(1.0f - c) + z*s)+0.5f);
	rotate_matrix.matrix[1+1*3] = (int) floor((y*y*(1.0f - c) + c)+0.5f);
	rotate_matrix.matrix[1+2*3] = (int) floor((y*z*(1.0f - c) - x*s)+0.5f);
	rotate_matrix.matrix[2+0*3] = (int) floor((x*z*(1.0f - c) - y*s)+0.5f);
	rotate_matrix.matrix[2+1*3] = (int) floor((y*z*(1.0f - c) + x*s)+0.5f);
	rotate_matrix.matrix[2+2*3] = (int) floor((z*z*(1.0f - c) + c)+0.5f);
	return rotate_matrix;
}


template<typename Type>
Mat3<Type> Mat3<Type>::multiply(const Mat3<Type> &matrix_1, const Mat3<Type> &matrix_2)
{
	return matrix_1 * matrix_2;
}

template<typename Type>
Mat3<Type> Mat3<Type>::add(const Mat3<Type> &matrix_1, const Mat3<Type> &matrix_2)
{
	return matrix_1 + matrix_2;
}

template<typename Type>
Mat3<Type> Mat3<Type>::subtract(const Mat3<Type> &matrix_1, const Mat3<Type> &matrix_2)
{
	return matrix_1 - matrix_2;
}

template<typename Type>
Mat3<Type> Mat3<Type>::adjoint(const Mat3<Type> &matrix)
{
	Mat3<Type> dest(matrix);
	dest.adjoint();
	return dest;
}

template<typename Type>
Mat3<Type> Mat3<Type>::inverse(const Mat3<Type> &matrix)
{
	Mat3<Type> dest(matrix);
	dest.inverse();
	return dest;
}

template<typename Type>
Mat3<Type> Mat3<Type>::transpose(const Mat3<Type> &matrix)
{
	Mat3<Type> dest(matrix);
	dest.transpose();
	return dest;
}

/////////////////////////////////////////////////////////////////////////////
// Mat3 attributes:

/////////////////////////////////////////////////////////////////////////////
// Mat3 operators:

template<typename Type>
Mat3<Type> &Mat3<Type>::operator =(const Mat4<Type> &copy)
{
	matrix[0*3 + 0] = copy.matrix[0*4 + 0];
	matrix[0*3 + 1] = copy.matrix[0*4 + 1];
	matrix[0*3 + 2] = copy.matrix[0*4 + 2];
	matrix[1*3 + 0] = copy.matrix[1*4 + 0];
	matrix[1*3 + 1] = copy.matrix[1*4 + 1];
	matrix[1*3 + 2] = copy.matrix[1*4 + 2];
	matrix[2*3 + 0] = copy.matrix[2*4 + 0];
	matrix[2*3 + 1] = copy.matrix[2*4 + 1];
	matrix[2*3 + 2] = copy.matrix[2*4 + 2];
	return *this;
}

template<typename Type>
Mat3<Type> &Mat3<Type>::operator =(const Mat2<Type> &copy)
{
	matrix[0*3 + 0] = copy.matrix[0*2 + 0];
	matrix[0*3 + 1] = copy.matrix[0*2 + 1];
	matrix[0*3 + 2] = 0;
	matrix[1*3 + 0] = copy.matrix[1*2 + 0];
	matrix[1*3 + 1] = copy.matrix[1*2 + 1];
	matrix[1*3 + 2] = 0;
	matrix[2*3 + 0] = 0;
	matrix[2*3 + 1] = 0;
	matrix[2*3 + 2] = 1;
	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// Mat3 operations:

template<typename Type>
Mat3<Type> Mat3<Type>::operator *(const Mat3<Type> &mult) const
{
	Mat3<Type> result;
	for (int x=0; x<3; x++)
	{
		for (int y=0; y<3; y++)
		{
			result.matrix[x+y*3] =
				matrix[0*3 + x]*mult.matrix[y*3 + 0] +
				matrix[1*3 + x]*mult.matrix[y*3 + 1] +
				matrix[2*3 + x]*mult.matrix[y*3 + 2];

		}
	}
	return result;
}

template<typename Type>
Mat3<Type> Mat3<Type>::operator +(const Mat3<Type> &add_matrix) const
{
	Mat3<Type> result;
	result.matrix[(0*3) + 0] = matrix[(0*3) + 0] + add_matrix.matrix[(0*3) + 0];
	result.matrix[(0*3) + 1] = matrix[(0*3) + 1] + add_matrix.matrix[(0*3) + 1];
	result.matrix[(0*3) + 2] = matrix[(0*3) + 2] + add_matrix.matrix[(0*3) + 2];
	result.matrix[(1*3) + 0] = matrix[(1*3) + 0] + add_matrix.matrix[(1*3) + 0];
	result.matrix[(1*3) + 1] = matrix[(1*3) + 1] + add_matrix.matrix[(1*3) + 1];
	result.matrix[(1*3) + 2] = matrix[(1*3) + 2] + add_matrix.matrix[(1*3) + 2];
	result.matrix[(2*3) + 0] = matrix[(2*3) + 0] + add_matrix.matrix[(2*3) + 0];
	result.matrix[(2*3) + 1] = matrix[(2*3) + 1] + add_matrix.matrix[(2*3) + 1];
	result.matrix[(2*3) + 2] = matrix[(2*3) + 2] + add_matrix.matrix[(2*3) + 2];
	return result;
}

template<typename Type>
Mat3<Type> Mat3<Type>::operator -(const Mat3<Type> &sub_matrix) const
{
	Mat3<Type> result;
	result.matrix[(0*3) + 0] = matrix[(0*3) + 0] - sub_matrix.matrix[(0*3) + 0];
	result.matrix[(0*3) + 1] = matrix[(0*3) + 1] - sub_matrix.matrix[(0*3) + 1];
	result.matrix[(0*3) + 2] = matrix[(0*3) + 2] - sub_matrix.matrix[(0*3) + 2];
	result.matrix[(1*3) + 0] = matrix[(1*3) + 0] - sub_matrix.matrix[(1*3) + 0];
	result.matrix[(1*3) + 1] = matrix[(1*3) + 1] - sub_matrix.matrix[(1*3) + 1];
	result.matrix[(1*3) + 2] = matrix[(1*3) + 2] - sub_matrix.matrix[(1*3) + 2];
	result.matrix[(2*3) + 0] = matrix[(2*3) + 0] - sub_matrix.matrix[(2*3) + 0];
	result.matrix[(2*3) + 1] = matrix[(2*3) + 1] - sub_matrix.matrix[(2*3) + 1];
	result.matrix[(2*3) + 2] = matrix[(2*3) + 2] - sub_matrix.matrix[(2*3) + 2];
	return result;
}

template<typename Type>
double Mat3<Type>::det() const
{
	double value;

	value  = matrix[0*3 + 0] * ( (matrix[1*3 + 1] * matrix[2*3 + 2]) - (matrix[2*3 + 1] * matrix[1*3 + 2]) );
	value -= matrix[0*3 + 1] * ( (matrix[1*3 + 0] * matrix[2*3 + 2]) - (matrix[2*3 + 0] * matrix[1*3 + 2]) );
	value += matrix[0*3 + 2] * ( (matrix[1*3 + 0] * matrix[2*3 + 1]) - (matrix[2*3 + 0] * matrix[1*3 + 1]) );

	return value;
}

template<typename Type>
Mat3<Type> &Mat3<Type>::adjoint()
{
	Mat3<Type> result;
	result.matrix[ 0*3 + 0 ] =  ( ( matrix[1*3 + 1] * matrix[2*3 + 2] ) - ( matrix[1*3 + 2] * matrix[2*3 + 1] ) );
	result.matrix[ 1*3 + 0 ] = -( ( matrix[1*3 + 0] * matrix[2*3 + 2] ) - ( matrix[1*3 + 2] * matrix[2*3 + 0] ) );
	result.matrix[ 2*3 + 0 ] =  ( ( matrix[1*3 + 0] * matrix[2*3 + 1] ) - ( matrix[1*3 + 1] * matrix[2*3 + 0] ) );
	result.matrix[ 0*3 + 1 ] = -( ( matrix[0*3 + 1] * matrix[2*3 + 2] ) - ( matrix[0*3 + 2] * matrix[2*3 + 1] ) );
	result.matrix[ 1*3 + 1 ] =  ( ( matrix[0*3 + 0] * matrix[2*3 + 2] ) - ( matrix[0*3 + 2] * matrix[2*3 + 0] ) );
	result.matrix[ 2*3 + 1 ] = -( ( matrix[0*3 + 0] * matrix[2*3 + 1] ) - ( matrix[0*3 + 1] * matrix[2*3 + 0] ) );
	result.matrix[ 0*3 + 2 ] =  ( ( matrix[0*3 + 1] * matrix[1*3 + 2] ) - ( matrix[0*3 + 2] * matrix[1*3 + 1] ) );
	result.matrix[ 1*3 + 2 ] = -( ( matrix[0*3 + 0] * matrix[1*3 + 2] ) - ( matrix[0*3 + 2] * matrix[1*3 + 0] ) );
	result.matrix[ 2*3 + 2 ] =  ( ( matrix[0*3 + 0] * matrix[1*3 + 1] ) - ( matrix[0*3 + 1] * matrix[1*3 + 0] ) );
	*this = result;
	return *this;
}

template<typename Type>
Mat3<Type> &Mat3<Type>::inverse()
{
	double d;

	d = det();

	// Inverse unknown when determinant is close to zero
	if (fabs(d) < 1e-15)
	{
		*this = null();
	}
	else
	{
		Mat3<Type> result = *this;
		result.adjoint();

		d=1.0/d;	// Inverse the determinant
		for (int i=0; i<9; i++)
		{
			result.matrix[i] = (Type) (result.matrix[i] * d);
		}

		*this = result;
	}
	return *this;
}

template<typename Type>
Mat3<Type> &Mat3<Type>::transpose()
{
	Type original[9];
	for (int cnt=0; cnt<9; cnt++)
		original[cnt] = matrix[cnt];

	matrix[0] = original[0];
	matrix[1] = original[3];
	matrix[2] = original[6];
	matrix[3] = original[1];
	matrix[4] = original[4];
	matrix[5] = original[7];
	matrix[6] = original[2];
	matrix[7] = original[5];
	matrix[8] = original[8];

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Mat3 implementation:

// Explicit instantiate the versions we use:
template class Mat3<int>;
template class Mat3<float>;
template class Mat3<double>;

}
