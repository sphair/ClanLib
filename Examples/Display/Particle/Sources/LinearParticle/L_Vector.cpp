//===============================================================================
//
// LinearParticle Copyright (c) 2006 Wong Chin Foo
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//===============================================================================


#include "precomp.h"
#include "L_Vector.h"


L_Vector::L_Vector()
{
	x = 0;
	y = 0;
}


L_Vector::L_Vector(L_REAL x_in, L_REAL y_in)
{
	x = x_in;
	y = y_in;
}


L_Vector::L_Vector(const L_Vector &vec)
{
	x = vec.x;
	y = vec.y;
}


L_Vector::L_Vector(const clan::Vec2f& vec)
{
	x = vec.x;
	y = vec.y;
}


void L_Vector::set(L_REAL x_in, L_REAL y_in)
{
	x = x_in;
	y = y_in;
}


void L_Vector::set(const L_Vector& vec)
{
	x = vec.x;
	y = vec.y;
}


void L_Vector::set(const clan::Vec2f& vec)
{
	x = vec.x;
	y = vec.y;
}


void L_Vector::set2(L_REAL magnitude, L_REAL radian)
{
	x = magnitude * L_COS(radian);
	y = magnitude * L_SIN(radian);
}


void L_Vector::set_magnitude(L_REAL magnitude)
{
	normalize();
	scale(magnitude);
}


void L_Vector::set_angle(L_REAL radian)
{
	L_REAL mag = get_magnitude();

	x = mag * L_COS(radian);
	y = mag * L_SIN(radian);
}


void L_Vector::normalize(void)
{
	L_REAL mag = get_magnitude();

	if( mag != 0 )
	{
		L_REAL div = 1.0f / mag;
		x *= div;
		y *= div;
	}
}


void L_Vector::scale(L_REAL amount)
{
	x *= amount;
	y *= amount;
}


void L_Vector::negate(void)
{
	x = -x;
	y = -y;
}


L_Vector L_Vector::operator + ( const L_Vector& vector_in ) const
{
	return L_Vector( x+vector_in.x, y+vector_in.y );
}


L_Vector L_Vector::operator - ( const L_Vector& vector_in ) const
{
	return L_Vector( x-vector_in.x, y-vector_in.y );
}


L_Vector L_Vector::operator = ( const L_Vector& vector_in )
{
	x = vector_in.x;
	y = vector_in.y;

	return *this;
}


void L_Vector::operator += ( const L_Vector& vector_in )
{
	x += vector_in.x;
	y += vector_in.y;
}


void L_Vector::operator -= ( const L_Vector& vector_in )
{
	x -= vector_in.x;
	y -= vector_in.y;
}


L_REAL L_Vector::dot_product(const L_Vector& vector_in ) const
{
	return ( x*vector_in.x + y*vector_in.y );
}


L_REAL L_Vector::get_magnitude(void) const
{
    return L_SQRT( x*x + y*y );
}


L_REAL L_Vector::get_sqr_magnitude(void) const
{
	return x*x + y*y;
}


L_REAL L_Vector::get_angle(void) const
{
	if( x != 0 && y != 0 )
		return L_ATAN2(y, x);

	return L_REAL_MIN;
}


L_Vector L_Vector::get_normalized_vector(void) const
{
	L_Vector vector;

	L_REAL mag = get_magnitude();

	if( mag != 0 )
	{
		L_REAL div = 1.0f / mag;
		vector.x = x*div;
		vector.y = y*div;
	}

	return vector;
}


L_Vector L_Vector::get_scaled_vector(L_REAL amount) const
{
	return L_Vector(x*amount, y*amount);
}
