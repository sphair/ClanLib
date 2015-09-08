/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/Math/mat2.h"

#include <cstring>

namespace clan
{
	template<typename Type>
	Mat2<Type>::Mat2(const Mat4<Type> &copy)
	{
		matrix[0 * 2 + 0] = copy.matrix[0 * 4 + 0];
		matrix[0 * 2 + 1] = copy.matrix[0 * 4 + 1];
		matrix[1 * 2 + 0] = copy.matrix[1 * 4 + 0];
		matrix[1 * 2 + 1] = copy.matrix[1 * 4 + 1];
	}

	template<typename Type>
	Mat2<Type>::Mat2(const Mat3<Type> &copy)
	{
		matrix[0 * 2 + 0] = copy.matrix[0 * 3 + 0];
		matrix[0 * 2 + 1] = copy.matrix[0 * 3 + 1];
		matrix[1 * 2 + 0] = copy.matrix[1 * 3 + 0];
		matrix[1 * 2 + 1] = copy.matrix[1 * 3 + 1];
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::null()
	{
		Mat2<Type> m;
		memset(m.matrix, 0, sizeof(m.matrix));
		return m;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::identity()
	{
		Mat2<Type> m = null();
		m.matrix[0] = 1;
		m.matrix[3] = 1;
		return m;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::multiply(const Mat2<Type> &matrix_1, const Mat2<Type> &matrix_2)
	{
		return matrix_1 * matrix_2;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::add(const Mat2<Type> &matrix_1, const Mat2<Type> &matrix_2)
	{
		return matrix_1 + matrix_2;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::subtract(const Mat2<Type> &matrix_1, const Mat2<Type> &matrix_2)
	{
		return matrix_1 - matrix_2;
	}

	template<typename Type>
	Mat2<Type> &Mat2<Type>::operator =(const Mat4<Type> &copy)
	{
		matrix[0 * 2 + 0] = copy.matrix[0 * 4 + 0];
		matrix[0 * 2 + 1] = copy.matrix[0 * 4 + 1];
		matrix[1 * 2 + 0] = copy.matrix[1 * 4 + 0];
		matrix[1 * 2 + 1] = copy.matrix[1 * 4 + 1];
		return *this;
	}

	template<typename Type>
	Mat2<Type> &Mat2<Type>::operator =(const Mat3<Type> &copy)
	{
		matrix[0 * 2 + 0] = copy.matrix[0 * 3 + 0];
		matrix[0 * 2 + 1] = copy.matrix[0 * 3 + 1];
		matrix[1 * 2 + 0] = copy.matrix[1 * 3 + 0];
		matrix[1 * 2 + 1] = copy.matrix[1 * 3 + 1];
		return *this;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::operator *(const Mat2<Type> &mult) const
	{
		Mat2<Type> result;
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				result.matrix[x + y * 2] =
					matrix[0 * 2 + x] * mult.matrix[y * 2 + 0] +
					matrix[1 * 2 + x] * mult.matrix[y * 2 + 1];
			}
		}
		return result;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::operator +(const Mat2<Type> &add_matrix) const
	{
		Mat2<Type> result;
		result.matrix[(0 * 2) + 0] = matrix[(0 * 2) + 0] + add_matrix.matrix[(0 * 2) + 0];
		result.matrix[(0 * 2) + 1] = matrix[(0 * 2) + 1] + add_matrix.matrix[(0 * 2) + 1];
		result.matrix[(1 * 2) + 0] = matrix[(1 * 2) + 0] + add_matrix.matrix[(1 * 2) + 0];
		result.matrix[(1 * 2) + 1] = matrix[(1 * 2) + 1] + add_matrix.matrix[(1 * 2) + 1];
		return result;
	}

	template<typename Type>
	Mat2<Type> Mat2<Type>::operator -(const Mat2<Type> &subtract_matrix) const
	{
		Mat2<Type> result;
		result.matrix[(0 * 2) + 0] = matrix[(0 * 2) + 0] - subtract_matrix.matrix[(0 * 2) + 0];
		result.matrix[(0 * 2) + 1] = matrix[(0 * 2) + 1] - subtract_matrix.matrix[(0 * 2) + 1];
		result.matrix[(1 * 2) + 0] = matrix[(1 * 2) + 0] - subtract_matrix.matrix[(1 * 2) + 0];
		result.matrix[(1 * 2) + 1] = matrix[(1 * 2) + 1] - subtract_matrix.matrix[(1 * 2) + 1];
		return result;
	}

	// Explicit instantiate the versions we use:
	template class Mat2<int>;
	template class Mat2<float>;
	template class Mat2<double>;
}
