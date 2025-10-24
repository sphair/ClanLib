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
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/bezier_curve.h"
#include "bezier_curve_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve Construction:

CL_BezierCurve::CL_BezierCurve()
: impl(new CL_BezierCurve_Impl)
{
}

CL_BezierCurve::~CL_BezierCurve()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve Attributes:

std::vector<CL_Pointf> CL_BezierCurve::get_control_points() const
{
	return impl->control_points;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve Operations:

void CL_BezierCurve::add_control_point(float x, float y)
{
	impl->control_points.push_back(CL_Pointf(x, y));
}

void CL_BezierCurve::add_control_point(const CL_Pointf &value)
{
	impl->control_points.push_back(value);
}

std::vector<CL_Pointf> CL_BezierCurve::generate_curve_points(const CL_Angle &split_angle)
{
	return impl->generate_curve_points(split_angle);
}

CL_Pointf CL_BezierCurve::get_point_relative(float pos_0_to_1) const
{
	return impl->get_point_relative(pos_0_to_1);
}

/////////////////////////////////////////////////////////////////////////////
// CL_BezierCurve Implementation:
