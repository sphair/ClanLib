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
**    Kenneth Gangstoe
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"

template<typename Type>
class CL_Vec2;

template<typename Type>
class CL_Vec3;

template<typename Type>
class CL_Vec4;

/// \brief 2D (width,height) size structure.
///
/// These line templates are defined for: int (CL_Size), float (CL_Sizef), double (CL_Sized)
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_API_CORE CL_Sizex
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a size structure.
	CL_Sizex() : width(0), height(0) { return; }

	/// \brief Constructs a size structure.
	///
	/// \param width Initial width of size structure.
	/// \param height Initial height of size structure.
	CL_Sizex(Type width, Type height)
	: width(width), height(height) { }

	/// \brief Constructs a size structure.
	///
	/// \param s = Size structure to construct this one from.
	CL_Sizex(const CL_Sizex<Type> &s)
	{ width = s.width; height = s.height; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Size width.
	Type width;

	/// \brief Size height.
	Type height;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Size += Size operator.
	CL_Sizex<Type> &operator+=(const CL_Sizex<Type> &s)
	{ width += s.width; height += s.height; return *this; }

	/// \brief Size -= Size operator.
	CL_Sizex<Type> &operator-=(const CL_Sizex<Type> &s)
	{ width -= s.width; height -= s.height; return *this; }

	/// \brief Size + Size operator.
	CL_Sizex<Type> operator+(const CL_Sizex<Type> &s) const
	{ return CL_Sizex<Type>(width + s.width, height + s.height); }

	/// \brief Size - Size operator.
	CL_Sizex<Type> operator-(const CL_Sizex<Type> &s) const
	{ return CL_Sizex<Type>(width - s.width, height - s.height); }

	/// \brief Size += operator.
	CL_Sizex<Type> &operator+=(const Type &s)
	{ width += s; height += s; return *this; }

	/// \brief Size -= operator.
	CL_Sizex<Type> &operator-=(const Type &s)
	{ width -= s; height -= s; return *this; }

	/// \brief Size *= operator.
	CL_Sizex<Type> &operator*=(const Type &s)
	{ width *= s; height *= s; return *this; }

	/// \brief Size /= operator.
	CL_Sizex<Type> &operator/=(const Type &s)
	{ width /= s; height /= s; return *this; }

	/// \brief Size + operator.
	CL_Sizex<Type> operator+(const Type &s) const
	{ return CL_Sizex<Type>(width + s, height + s); }

	/// \brief Size - operator.
	CL_Sizex<Type> operator-(const Type &s) const
	{ return CL_Sizex<Type>(width - s, height - s); }

	/// \brief Size * operator.
	CL_Sizex<Type> operator*(const Type &s) const
	{ return CL_Sizex<Type>(width * s, height * s); }

	/// \brief Size / operator.
	CL_Sizex<Type> operator/(const Type &s) const
	{ return CL_Sizex<Type>(width / s, height / s); }

	/// \brief Size == Size operator (deep compare).
	bool operator==(const CL_Sizex<Type> &s) const
	{ return (width == s.width) && (height == s.height); }

	/// \brief Size != Size operator (deep compare).
	bool operator!=(const CL_Sizex<Type> &s) const
	{ return (width != s.width) || (height != s.height); }
/// \}
};

/// \brief 2D (width,height) size structure - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Size : public CL_Sizex<int>
{
public:
	CL_Size() : CL_Sizex<int>() {}
	CL_Size(int width, int height) : CL_Sizex<int>(width, height) {}
	CL_Size(const CL_Sizex<int> &s) : CL_Sizex<int>(s) {}

	explicit CL_Size(const CL_Sizex<float> &copy) { width = (int)(copy.width+0.5f); height = (int)(copy.height+0.5f); }
	explicit CL_Size(const CL_Sizex<double> &copy) { width = (int)(copy.width+0.5); height = (int)(copy.height+0.5); }
};

/// \brief 2D (width,height) size structure - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Sizef : public CL_Sizex<float>
{
public:
	CL_Sizef() : CL_Sizex<float>() {}
	CL_Sizef(float width, float height) : CL_Sizex<float>(width, height) {}
	CL_Sizef(const CL_Sizex<float> &s) : CL_Sizex<float>(s) {}

	CL_Sizef(const CL_Sizex<int> &copy) { width = (float)copy.width; height = (float)copy.height; }
	explicit CL_Sizef(const CL_Sizex<double> &copy) { width = (float)copy.width; height = (float)copy.height; }
};

/// \brief 2D (width,height) size structure - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Sized : public CL_Sizex<double>
{
public:
	CL_Sized() : CL_Sizex<double>() {}
	CL_Sized(double width, double height) : CL_Sizex<double>(width, height) {}
	CL_Sized(const CL_Sizex<double> &s) : CL_Sizex<double>(s) {}

	CL_Sized(const CL_Sizex<int> &copy) { width = (double)copy.width; height = (double)copy.height; }
	CL_Sized(const CL_Sizex<float> &copy) { width = (double)copy.width; height = (double)copy.height; }
};

/// \}

