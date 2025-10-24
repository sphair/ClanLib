/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magic Software - www.magic-software.com
*/

//! clanCore="Math"
//! header=core.h

#ifndef header_vector2d
#define header_vector2d

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

//: A 2D Vector class
//- !group=Core/Math!
//- !header=core.h!
//- CL_Vector2 provides a 2D vector class.
//- This is different from {link CL_Vector CL_Vector.html}
class CL_API_CORE CL_Vector2
{
public:
//! Construction:
	//: Default constructor
	CL_Vector2();
	
	//: Constructor with info
	//param fX: The X coordinate.
	//param fY: The Y coordinate.
	CL_Vector2(float fX, float fY);

	//: FIXME!
	CL_Vector2(float afCoordinate[2]);

	//: Copy constructor
	//param rkVector: Vector to copy
	CL_Vector2(const CL_Vector2& rkVector);

//! Operators:
	// access vector V as V[0] = V.x, V[1] = V.y
	//
	// WARNING.  These member functions rely on
	// (1) CL_Vector2 not having virtual functions
	// (2) the data packed in a 2*sizeof(float) memory block
	
	//: Accessor
	//- <p>Use vector V[0] for V.x
	//- Use vector V[1] for V.y</p>
	//return: The requested parameter
	const float& operator[] (int i) const;
	float& operator[] (int i);

	//: Unknown
	operator float* ();

	//: assignment operator
	CL_Vector2& operator= (const CL_Vector2& rkVector);

// comparison (supports fuzzy arithmetic when FUZZ > 0)

	//: Equivalancy operator
	//- <p>Supports fuzzy arimatic when FUZZ > 0</p>
	bool operator== (const CL_Vector2& rkVector) const;
	
	//: Not-equals operator
	//- <p>Supports fuzzy arimatic when FUZZ > 0</p>
	bool operator!= (const CL_Vector2& rkVector) const;
	
	//: Less than operator
	//- <p>Supports fuzzy arimatic when FUZZ > 0</p>
	bool operator<  (const CL_Vector2& rkVector) const;
	
	//: Less than or equal operator
	//- <p>Supports fuzzy arimatic when FUZZ > 0</p>
	bool operator<= (const CL_Vector2& rkVector) const;
	
	//: Greater than operator
	//- <p>Supports fuzzy arimatic when FUZZ > 0</p>
	bool operator>  (const CL_Vector2& rkVector) const;
	
	//: Greater than or equal operator		
	//- <p>Supports fuzzy arimatic when FUZZ > 0</p>
	bool operator>= (const CL_Vector2& rkVector) const;

// arithmetic operations

	//: Addition operator
	CL_Vector2 operator+ (const CL_Vector2& rkVector) const;
	
	//: Subtraction operator
	CL_Vector2 operator- (const CL_Vector2& rkVector) const;
	
	//: Multiplication operator
	CL_Vector2 operator* (float fScalar) const;
	
	//: Division operator
	CL_Vector2 operator/ (float fScalar) const;
	
	//: Negation operator
	//return: The negative of the Vector
	CL_Vector2 operator- () const;

//	MAGICFM friend CL_Vector2 operator* (float fScalar, const CL_Vector2& rkVector);

// arithmetic updates

	//: Addition assignment operator
	CL_Vector2& operator+= (const CL_Vector2& rkVector);
	
	//: Subtraction assignment operator
	CL_Vector2& operator-= (const CL_Vector2& rkVector);
	
	//: Multiplication assignment operator
	CL_Vector2& operator*= (float fScalar);
	
	//: Division assignment operator
	CL_Vector2& operator/= (float fScalar);

//! Attributes:

	//: Enable Fuzzy arithmatic
	void set_fuzz() { FUZZ = 1.0; }

	//: Disable Fuzzy arithmatic
	void disable_fuzz() { FUZZ = 0.0; }

	//: Using fuzzy arimatic
	//return: True if using fuzzy arithmatic
	bool get_fuzz() { return (FUZZ > 0.0)? true : false ;}

// vector operations
	//: The length of the vector
	//return: sqrt(x^2+y^2)
	float length() const;

	//: The squared length of the vector
	//return: x^2+y^2
	float squared_length() const;

	//: FIXME!!!
	float dot(const CL_Vector2& rkVector) const;

	//: FIXME!!!
	float unitize(float fTolerance = 1e-06f);

	//: Cross
	//- FIXME
	//return: Return y,-x
	CL_Vector2 cross() const;

	//: Unit Cross
	//return: (y,-x)/sqrt(x^2+y^2)
	CL_Vector2 unit_cross() const;

	// Gram-Schmidt orthonormalization.
//	static void Orthonormalize (CL_Vector2 akVector[/*2*/]);

//! Variables:
	// coordinates
	float x, y;

	// special points
	static const CL_Vector2 ZERO;
	static const CL_Vector2 UNIT_X;
	static const CL_Vector2 UNIT_Y;

	// fuzzy arithmetic (set FUZZ > 0 to enable)
	static float FUZZ;
};

#include "vector2.inl"

#endif
