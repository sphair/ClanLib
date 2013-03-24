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

SoftwareProgram_Standard::SoftwareProgram_Standard()
{
}

int SoftwareProgram_Standard::get_attribute_count() const
{
	return 4;
}

int SoftwareProgram_Standard::get_attribute_index(const std::string &name) const
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

Vec4f SoftwareProgram_Standard::get_attribute_default(int index)
{
	switch (index)
	{
	case 0:
		return Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
	case 1:
		return Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	case 2:
		return Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
	case 3:
	default:
		return Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

void SoftwareProgram_Standard::set_uniform(const std::string &name, const Vec4f &vec)
{
}

void SoftwareProgram_Standard::set_uniform_matrix(const std::string &name, const Mat4f &mat)
{
}

PixelCommand *SoftwareProgram_Standard::draw_triangle(PixelPipeline *pipeline, const std::vector<Vec4f> &attribute_values)
{
	Vec2f init_points[3] = { transform(attribute_values[0]), transform(attribute_values[1]), transform(attribute_values[2]) };
	Vec4f init_primcolor[3] = { attribute_values[3], attribute_values[4], attribute_values[5] };
	Vec2f init_texcoords[3] = { Vec2f(attribute_values[6]), Vec2f(attribute_values[7]), Vec2f(attribute_values[8]) };
	int init_sampler = (int)attribute_values[9].x;
	return new(pipeline) PixelCommandTriangle(init_points, init_primcolor, init_texcoords, init_sampler);
}

PixelCommand *SoftwareProgram_Standard::draw_sprite(PixelPipeline *pipeline, const std::vector<Vec4f> &attribute_values)
{
	Vec2f init_points[3] = { transform(attribute_values[0]), transform(attribute_values[1]), transform(attribute_values[2]) };
	Vec4f init_primcolor[3] = { attribute_values[3], attribute_values[4], attribute_values[5] };
	Vec2f init_texcoords[3] = { Vec2f(attribute_values[6]), Vec2f(attribute_values[7]), Vec2f(attribute_values[8]) };
	int init_sampler = (int)attribute_values[9].x;
	return new(pipeline) PixelCommandSprite(init_points, init_primcolor[0], init_texcoords, init_sampler);
}

PixelCommand *SoftwareProgram_Standard::draw_line(PixelPipeline *pipeline, const std::vector<Vec4f> &attribute_values)
{
	Vec2f init_points[2] = { transform(attribute_values[0]), transform(attribute_values[1]) };
	Vec4f init_primcolor[2] = { attribute_values[2], attribute_values[3] };
	Vec2f init_texcoords[2] = { Vec2f(attribute_values[4]), Vec2f(attribute_values[5]) };
	int init_sampler = (int)attribute_values[6].x;
	return new(pipeline) PixelCommandLine(init_points, init_primcolor, init_texcoords, init_sampler);
}

Vec2f SoftwareProgram_Standard::transform(const Vec4f &vertex) const
{
	return Vec2f(vertex.x, vertex.y);
}
}
