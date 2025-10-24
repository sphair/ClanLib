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
**    Harry Storbacka
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_Angle_Impl;

/// \brief Angle Unit
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
enum CL_AngleUnit
{
	cl_degrees,
	cl_radians
};

/// \brief Euler angle rotation order
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
enum CL_EulerOrder
{
	cl_XYZ,
	cl_XZY,
	cl_YZX,
	cl_YXZ,
	cl_ZXY,
	cl_ZYX
};


/// \brief Angle class.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_API_CORE CL_Angle
{
/// \name Construction
/// \{
public:
	/// \brief Constructs an NULL Angle object.
	CL_Angle();

	/// \brief Constructs an Angle object.
	CL_Angle(float value, CL_AngleUnit unit);

	/// \brief From radians
	///
	/// \param value = value
	///
	/// \return Angle
	static CL_Angle from_radians(float value);

	/// \brief From degrees
	///
	/// \param value = value
	///
	/// \return Angle
	static CL_Angle from_degrees(float value);

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the angle as degrees.
	float to_degrees() const;

	/// \brief Returns the angle as radians.
	float to_radians() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set the angle value in degrees.
	void set_degrees(float degrees);

	/// \brief Set the angle value in radians.
	void set_radians(float radians);

	/// \brief Converts angle to range [0,360] degrees.
	///
	/// \return reference to this object
	CL_Angle &normalize();

	/// \brief Converts angle to range [-180,180] degrees.
	///
	/// \return reference to this object
	CL_Angle &normalize_180();

/// \}
/// \name Operators
/// \{
public:
	/// \brief += operator.
	void operator += (const CL_Angle &angle);

	/// \brief -= operator.
	void operator -= (const CL_Angle &angle);

	/// \brief *= operator.
	void operator *= (const CL_Angle &angle);

	/// \brief /= operator.
	void operator /= (const CL_Angle &angle);

	/// \brief + operator.
	CL_Angle operator + (const CL_Angle &angle) const;

	/// \brief - operator.
	CL_Angle operator - (const CL_Angle &angle) const;

	/// \brief * operator.
	CL_Angle operator * (const CL_Angle &angle) const;

	/// \brief * operator.
	CL_Angle operator * (float value) const;

	/// \brief / operator.
	CL_Angle operator / (const CL_Angle &angle) const;

	/// \brief / operator.
	CL_Angle operator / (float value) const;

	/// \brief < operator.
	bool operator < (const CL_Angle &angle) const;

	/// \brief < operator.
	bool operator <= (const CL_Angle &angle) const;

	/// \brief > operator.
	bool operator > (const CL_Angle &angle) const;

	/// \brief > operator.
	bool operator >= (const CL_Angle &angle) const;

	/// \brief == operator.
	bool operator== (const CL_Angle &angle) const;

	/// \brief != operator.
	bool operator!= (const CL_Angle &angle) const;

/// \}
/// \name Implementation
/// \{
private:
	float value_rad;

/// \}
};

/// \}
