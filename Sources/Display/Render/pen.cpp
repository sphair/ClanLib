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
#include "API/Display/Render/pen.h"
 
/////////////////////////////////////////////////////////////////////////////
// CL_Pen_Impl Class:
 
class CL_Pen_Impl
{
public:
	CL_Pen_Impl() :
		point_size(1.0),
		point_fade_treshold_size(1.0),
		line_width(1.0),
		line_antialiased(false),
		point_sprites(false),
		vertex_shader_point_sizes(false),
		point_sprite_origin(cl_point_sprite_origin_upper_left)
	{
		return;
	}

	float point_size;
	float point_fade_treshold_size;
	float line_width;
	bool line_antialiased;
	bool point_sprites;
	bool vertex_shader_point_sizes;
	CL_PointSpriteOrigin point_sprite_origin;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Pen Construction:

CL_Pen::CL_Pen()
: impl(new CL_Pen_Impl)
{
}

CL_Pen::~CL_Pen()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Pen Attributes:

float CL_Pen::get_point_size() const
{
	return impl->point_size;
}

float CL_Pen::get_point_fade_treshold_size() const
{
	return impl->point_fade_treshold_size;
}

float CL_Pen::get_line_width() const
{
	return impl->line_width;
}

bool CL_Pen::is_line_antialiased() const
{
	return impl->line_antialiased;
}

bool CL_Pen::is_using_vertex_program_point_sizes() const
{
	return impl->vertex_shader_point_sizes;
}

CL_PointSpriteOrigin CL_Pen::get_point_sprite_origin() const
{
	return impl->point_sprite_origin;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Pen Operations:

void CL_Pen::set_point_size(float value)
{
	impl->point_size = value;
}

void CL_Pen::set_point_fade_treshold_size(float value)
{
	impl->point_fade_treshold_size = value;
}

void CL_Pen::set_line_width(float value)
{
	impl->line_width = value;
}

void CL_Pen::enable_line_antialiasing(bool value)
{
	impl->line_antialiased = value;
}

void CL_Pen::enable_point_sprite(bool enable)
{
	impl->point_sprites = enable;
}

void CL_Pen::enable_vertex_program_point_size(bool enable)
{
	impl->vertex_shader_point_sizes = enable;
}

void CL_Pen::set_point_sprite_origin(CL_PointSpriteOrigin origin)
{
	impl->point_sprite_origin = origin;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Pen Implementation:
