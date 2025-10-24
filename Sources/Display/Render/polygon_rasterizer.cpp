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

#include "Display/precomp.h"
#include "API/Display/Render/polygon_rasterizer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Pen_Impl Class:

class CL_PolygonRasterizer_Impl
{
public:
	CL_PolygonRasterizer_Impl() :
		is_stippled(false),
		is_antialiased(false),
		is_culled(false),
		is_point_offset(false),
		is_line_offset(false),
		is_polygon_offset(false),
		face_cull_mode(cl_cull_back),
		face_fill_mode_front(cl_fill_polygon),
		face_fill_mode_back(cl_fill_polygon),
		front_face_side(cl_face_side_counter_clockwise),
		offset_factor(0.0f),
		offset_units(0.0f)
	{
	}

	bool is_stippled;
	bool is_antialiased;
	bool is_culled;
	bool is_point_offset;
	bool is_line_offset;
	bool is_polygon_offset;

	CL_CullMode face_cull_mode;

	CL_FillMode face_fill_mode_front;
	CL_FillMode face_fill_mode_back;
	CL_FaceSide front_face_side;

	float offset_factor;
	float offset_units;
};

/////////////////////////////////////////////////////////////////////////////
// CL_PolygonRasterizer Construction:

CL_PolygonRasterizer::CL_PolygonRasterizer()
: impl(new CL_PolygonRasterizer_Impl)
{

}

CL_PolygonRasterizer::~CL_PolygonRasterizer()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_PolygonRasterizer Attributes:


bool CL_PolygonRasterizer::is_antialiased() const
{
	return impl->is_antialiased;
}

bool CL_PolygonRasterizer::is_culled() const
{
	return impl->is_culled;
}

bool CL_PolygonRasterizer::is_point_offset() const
{
	return impl->is_point_offset;
}

bool CL_PolygonRasterizer::is_line_offset() const
{
	return impl->is_line_offset;
}

bool CL_PolygonRasterizer::is_polygon_offset() const
{
	return impl->is_polygon_offset;
}

CL_CullMode CL_PolygonRasterizer::get_face_cull_mode() const
{
	return impl->face_cull_mode;
}

CL_FillMode CL_PolygonRasterizer::get_face_fill_mode_front() const
{
	return impl->face_fill_mode_front;
}

CL_FillMode CL_PolygonRasterizer::get_face_fill_mode_back() const
{
	return impl->face_fill_mode_back;
}

float CL_PolygonRasterizer::get_offset_factor() const
{
	return impl->offset_factor;
}

float CL_PolygonRasterizer::get_offset_units() const
{
	return impl->offset_units;
}

CL_FaceSide CL_PolygonRasterizer::get_front_face() const
{
	return impl->front_face_side;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PolygonRasterizer Operations:

void CL_PolygonRasterizer::set_antialiased(bool value)
{
	impl->is_antialiased = value;
}

void CL_PolygonRasterizer::set_culled(bool value)
{
	impl->is_culled = value;
}

void CL_PolygonRasterizer::set_point_offset(bool value)
{
	impl->is_point_offset = value;
}

void CL_PolygonRasterizer::set_line_offset(bool value)
{
	impl->is_line_offset = value;
}

void CL_PolygonRasterizer::set_polygon_offset(bool value)
{
	impl->is_polygon_offset = value;
}

void CL_PolygonRasterizer::set_face_cull_mode(CL_CullMode value)
{
	impl->face_cull_mode = value;
}

void CL_PolygonRasterizer::set_face_fill_mode_front(CL_FillMode value)
{
	impl->face_fill_mode_front = value;
}

void CL_PolygonRasterizer::set_face_fill_mode_back(CL_FillMode value)
{
	impl->face_fill_mode_back = value;
}

void CL_PolygonRasterizer::set_front_face(CL_FaceSide value)
{
	impl->front_face_side = value;
}

void CL_PolygonRasterizer::set_offset_factor(float value)
{
	impl->offset_factor = value;
}

void CL_PolygonRasterizer::set_offset_units(float value)
{
	impl->offset_units = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PolygonRasterizer Implementation:
