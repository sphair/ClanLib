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

/// \brief HSV Color - Integer
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_ColorHSVi : public CL_ColorHSVx<int>
{
public:
	CL_ColorHSVi() : CL_ColorHSVx<int>() {}
	CL_ColorHSVi(int h, int s, int v, int a) : CL_ColorHSVx<int>(h,s,v,a) {}
	CL_ColorHSVi(const CL_ColorHSVx<int> &copy) : CL_ColorHSVx<int>(copy) {}
	CL_ColorHSVi(const CL_Color &color) : CL_ColorHSVx<int>(color) {}
	CL_ColorHSVi(const CL_Colorf &color): CL_ColorHSVx<int>(color) {}
	CL_ColorHSVi(const CL_Colord &color) : CL_ColorHSVx<int>(color) {}

	operator CL_Color();

};

/// \brief HSV Color - Float
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_ColorHSVf : public CL_ColorHSVx<float>
{
public:
	CL_ColorHSVf() : CL_ColorHSVx<float>() {}
	CL_ColorHSVf(float h, float s, float v, float a) : CL_ColorHSVx<float>(h,s,v,a) {}
	CL_ColorHSVf(const CL_ColorHSVx<float> &copy) : CL_ColorHSVx<float>(copy) {}
	CL_ColorHSVf(const CL_Color &color) : CL_ColorHSVx<float>(color) {}
	CL_ColorHSVf(const CL_Colorf &color): CL_ColorHSVx<float>(color) {}
	CL_ColorHSVf(const CL_Colord &color) : CL_ColorHSVx<float>(color) {}

	operator CL_Colorf();

};

/// \brief HSV Color - Double
///
/// A line segment has a start point and an end point\n
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_ColorHSVd : public CL_ColorHSVx<double>
{
public:
	CL_ColorHSVd() : CL_ColorHSVx<double>() {}
	CL_ColorHSVd(double h, double s, double v, double a) : CL_ColorHSVx<double>(h,s,v,a) {}
	CL_ColorHSVd(const CL_ColorHSVx<double> &copy) : CL_ColorHSVx<double>(copy) {}
	CL_ColorHSVd(const CL_Color &color) : CL_ColorHSVx<double>(color) {}
	CL_ColorHSVd(const CL_Colorf &color): CL_ColorHSVx<double>(color) {}
	CL_ColorHSVd(const CL_Colord &color) : CL_ColorHSVx<double>(color) {}

	operator CL_Colord();

};

/// \}
