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


#ifndef L_VECTOR
#define L_VECTOR


#include "L_Defination.h"


//! 2D Vector class
class L_Vector
{

public:
	L_REAL x;
	L_REAL y;

	L_Vector();
	L_Vector(L_REAL x_in, L_REAL y_in);
	L_Vector(const L_Vector& vec);
	L_Vector(const clan::Vec2f& vec);

	void set(L_REAL x_in, L_REAL y_in);
	void set(const L_Vector& vec);
	void set(const clan::Vec2f& vec);
	void set2(L_REAL magnitude, L_REAL radian);

	void set_magnitude(L_REAL magnitude);
	void set_angle(L_REAL radian);
	void normalize(void);
	void scale(L_REAL amount);
	void negate(void);
	L_Vector operator + ( const L_Vector& vector_in ) const;
	L_Vector operator - ( const L_Vector& vector_in ) const;
	L_Vector operator = ( const L_Vector& vector_in );
	void operator += ( const L_Vector& vector_in );
	void operator -= ( const L_Vector& vector_in );
	L_REAL dot_product(const L_Vector& vector_in ) const;

	L_REAL get_magnitude(void) const;
	L_REAL get_sqr_magnitude(void) const;
	L_REAL get_angle(void) const; // get angle in radian unit
	L_Vector get_normalized_vector(void) const;
	L_Vector get_scaled_vector(L_REAL amount) const;

};

#endif
