/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

#include "SWRender/precomp.h"
#include "software_program_standard.h"
#include "Canvas/pixel_canvas.h"
#include "Canvas/Commands/pixel_command_sprite.h"
#include "Canvas/Commands/pixel_command_triangle.h"
#include "Canvas/Commands/pixel_command_line.h"

namespace clan
{

CL_SoftwareProgram_Standard::CL_SoftwareProgram_Standard()
: modelview(CL_Mat4f::identity())
{
}

int CL_SoftwareProgram_Standard::get_attribute_count() const
{
	return 4;
}

int CL_SoftwareProgram_Standard::get_attribute_index(const std::string &name) const
{
	if (name == "Position")
		return 0;
	else if (name == "Color0")
		return 1;
	else if (name == "TexCoord0")
		return 2;
	else if (name == "TexIndex0")
		return 3;
	else
		return -1;
}

CL_Vec4f CL_SoftwareProgram_Standard::get_attribute_default(int index)
{
	switch (index)
	{
	case 0:
		return CL_Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
	case 1:
		return CL_Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	case 2:
		return CL_Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
	case 3:
	default:
		return CL_Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

void CL_SoftwareProgram_Standard::set_uniform(const std::string &name, const CL_Vec4f &vec)
{
}

void CL_SoftwareProgram_Standard::set_uniform_matrix(const std::string &name, const CL_Mat4f &mat)
{
	if (name == "cl_ModelView")
		set_modelview(mat);
}

CL_PixelCommand *CL_SoftwareProgram_Standard::draw_triangle(CL_PixelPipeline *pipeline, const std::vector<CL_Vec4f> &attribute_values)
{
	CL_Vec2f init_points[3] = { transform(attribute_values[0]), transform(attribute_values[1]), transform(attribute_values[2]) };
	CL_Vec4f init_primcolor[3] = { attribute_values[3], attribute_values[4], attribute_values[5] };
	CL_Vec2f init_texcoords[3] = { CL_Vec2f(attribute_values[6]), CL_Vec2f(attribute_values[7]), CL_Vec2f(attribute_values[8]) };
	int init_sampler = (int)attribute_values[9].x;
	return new(pipeline) CL_PixelCommandTriangle(init_points, init_primcolor, init_texcoords, init_sampler);
}

CL_PixelCommand *CL_SoftwareProgram_Standard::draw_sprite(CL_PixelPipeline *pipeline, const std::vector<CL_Vec4f> &attribute_values)
{
	CL_Vec2f init_points[3] = { transform(attribute_values[0]), transform(attribute_values[1]), transform(attribute_values[2]) };
	CL_Vec4f init_primcolor[3] = { attribute_values[3], attribute_values[4], attribute_values[5] };
	CL_Vec2f init_texcoords[3] = { CL_Vec2f(attribute_values[6]), CL_Vec2f(attribute_values[7]), CL_Vec2f(attribute_values[8]) };
	int init_sampler = (int)attribute_values[9].x;
	return new(pipeline) CL_PixelCommandSprite(init_points, init_primcolor[0], init_texcoords, init_sampler);
}

CL_PixelCommand *CL_SoftwareProgram_Standard::draw_line(CL_PixelPipeline *pipeline, const std::vector<CL_Vec4f> &attribute_values)
{
	CL_Vec2f init_points[2] = { transform(attribute_values[0]), transform(attribute_values[1]) };
	CL_Vec4f init_primcolor[2] = { attribute_values[2], attribute_values[3] };
	CL_Vec2f init_texcoords[2] = { CL_Vec2f(attribute_values[4]), CL_Vec2f(attribute_values[5]) };
	int init_sampler = (int)attribute_values[6].x;
	return new(pipeline) CL_PixelCommandLine(init_points, init_primcolor, init_texcoords, init_sampler);
}

void CL_SoftwareProgram_Standard::set_modelview(const CL_Mat4f &new_modelview)
{
	modelview = new_modelview;
}

CL_Vec2f CL_SoftwareProgram_Standard::transform(const CL_Vec4f &vertex) const
{
	CL_Vec4f v = modelview * vertex;
	return CL_Vec2f(v.x, v.y);
}
}
