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
**    Mark Page
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "color_hsv.h"

// *** This file has to be separate to color_hsv.h because of gcc issues ***

/// \brief HSL Color - Integer
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_ColorHSLi : public CL_ColorHSLx<int>
{
public:
	CL_ColorHSLi() : CL_ColorHSLx<int>() {}
	CL_ColorHSLi(int h, int s, int l, int a) : CL_ColorHSLx<int>(h,s,l,a) {}
	CL_ColorHSLi(const CL_ColorHSLx<int> &copy) : CL_ColorHSLx<int>(copy) {}
	CL_ColorHSLi(const CL_Color &color) : CL_ColorHSLx<int>(color) {}
	CL_ColorHSLi(const CL_Colorf &color): CL_ColorHSLx<int>(color) {}
	CL_ColorHSLi(const CL_Colord &color) : CL_ColorHSLx<int>(color) {}

	operator CL_Color();

};

/// \brief HSL Color - Float
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_ColorHSLf : public CL_ColorHSLx<float>
{
public:
	CL_ColorHSLf() : CL_ColorHSLx<float>() {}
	CL_ColorHSLf(float h, float s, float l, float a) : CL_ColorHSLx<float>(h,s,l,a) {}
	CL_ColorHSLf(const CL_ColorHSLx<float> &copy) : CL_ColorHSLx<float>(copy) {}
	CL_ColorHSLf(const CL_Color &color) : CL_ColorHSLx<float>(color) {}
	CL_ColorHSLf(const CL_Colorf &color): CL_ColorHSLx<float>(color) {}
	CL_ColorHSLf(const CL_Colord &color) : CL_ColorHSLx<float>(color) {}

	operator CL_Colorf();

};

/// \brief HSL Color - Double
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_ColorHSLd : public CL_ColorHSLx<double>
{
public:
	CL_ColorHSLd() : CL_ColorHSLx<double>() {}
	CL_ColorHSLd(double h, double s, double l, double a) : CL_ColorHSLx<double>(h,s,l,a) {}
	CL_ColorHSLd(const CL_ColorHSLx<double> &copy) : CL_ColorHSLx<double>(copy) {}
	CL_ColorHSLd(const CL_Color &color) : CL_ColorHSLx<double>(color) {}
	CL_ColorHSLd(const CL_Colorf &color): CL_ColorHSLx<double>(color) {}
	CL_ColorHSLd(const CL_Colord &color) : CL_ColorHSLx<double>(color) {}

	operator CL_Colord();

};

/// \}

