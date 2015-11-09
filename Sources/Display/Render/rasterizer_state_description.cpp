/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/Render/rasterizer_state_description.h"

namespace clan
{
	class RasterizerStateDescription_Impl
	{
	public:
		RasterizerStateDescription_Impl() : culled(), line_antialiasing(), face_cull_mode(), face_fill_mode(), front_face(), enable_scissor(),
			is_antialiased(false),
			is_offset_point(false),
			is_offset_line(false),
			is_offset_fill(false),
			offset_factor(0.0f),
			offset_units(0.0f),
			pen_point_size(1.0f),
			pen_point_fade_treshold_size(1.0f),
			pen_vertex_shader_point_sizes(false),
			pen_point_sprite_origin(origin_upper_left)

		{
			line_antialiasing = false;
			culled = false;
			face_cull_mode = cull_back;
			face_fill_mode = fill_polygon;
			front_face = face_counter_clockwise;
			enable_scissor = true;
		}

		bool operator==(const RasterizerStateDescription_Impl &other) const
		{
			return
				culled == other.culled &&
				line_antialiasing == other.line_antialiasing &&
				face_cull_mode == other.face_cull_mode &&
				face_fill_mode == other.face_fill_mode &&
				front_face == other.front_face &&
				enable_scissor == other.enable_scissor &&
				is_antialiased == other.is_antialiased &&
				is_offset_point == other.is_offset_point &&
				is_offset_line == other.is_offset_line &&
				is_offset_fill == other.is_offset_fill &&
				offset_factor == other.offset_factor &&
				offset_units == other.offset_units &&
				pen_point_size == other.pen_point_size &&
				pen_point_fade_treshold_size == other.pen_point_fade_treshold_size &&
				pen_vertex_shader_point_sizes == other.pen_vertex_shader_point_sizes &&
				pen_point_sprite_origin == other.pen_point_sprite_origin;
		}

		bool operator<(const RasterizerStateDescription_Impl &other) const
		{
			if (culled != other.culled)
				return culled < other.culled;
			else if (line_antialiasing != other.line_antialiasing)
				return line_antialiasing < other.line_antialiasing;
			else if (face_cull_mode != other.face_cull_mode)
				return face_cull_mode < other.face_cull_mode;
			else if (face_fill_mode != other.face_fill_mode)
				return face_fill_mode < other.face_fill_mode;
			else if (front_face != other.front_face)
				return front_face < other.front_face;
			else if (enable_scissor != other.enable_scissor)
				return enable_scissor < other.enable_scissor;
			else if (is_antialiased != other.is_antialiased)
				return is_antialiased < other.is_antialiased;
			else if (is_offset_point != other.is_offset_point)
				return is_offset_point < other.is_offset_point;
			else if (is_offset_line != other.is_offset_line)
				return is_offset_line < other.is_offset_line;
			else if (is_offset_fill != other.is_offset_fill)
				return is_offset_fill < other.is_offset_fill;
			else if (offset_factor != other.offset_factor)
				return offset_factor < other.offset_factor;
			else if (offset_units != other.offset_units)
				return offset_units < other.offset_units;
			else if (pen_point_size != other.pen_point_size)
				return pen_point_size < other.pen_point_size;
			else if (pen_point_fade_treshold_size != other.pen_point_fade_treshold_size)
				return pen_point_fade_treshold_size < other.pen_point_fade_treshold_size;
			else if (pen_vertex_shader_point_sizes != other.pen_vertex_shader_point_sizes)
				return pen_vertex_shader_point_sizes < other.pen_vertex_shader_point_sizes;
			else
				return pen_point_sprite_origin < other.pen_point_sprite_origin;
		}

		bool culled;
		bool line_antialiasing;
		CullMode face_cull_mode;
		FillMode face_fill_mode;
		FaceSide front_face;
		bool enable_scissor;

		bool is_antialiased;
		bool is_offset_point;
		bool is_offset_line;
		bool is_offset_fill;
		float offset_factor;
		float offset_units;

		float pen_point_size;
		float pen_point_fade_treshold_size;
		bool pen_vertex_shader_point_sizes;
		PointSpriteOrigin pen_point_sprite_origin;

	};

	RasterizerStateDescription::RasterizerStateDescription()
		: impl(std::make_shared<RasterizerStateDescription_Impl>())
	{
	}

	RasterizerStateDescription RasterizerStateDescription::clone() const
	{
		RasterizerStateDescription copy;
		if (impl)
			copy.impl = std::shared_ptr<RasterizerStateDescription_Impl>(new RasterizerStateDescription_Impl(*impl.get()));
		return copy;
	}

	bool RasterizerStateDescription::get_culled() const
	{
		return impl->culled;
	}

	bool RasterizerStateDescription::get_enable_line_antialiasing() const
	{
		return impl->line_antialiasing;
	}

	CullMode RasterizerStateDescription::get_face_cull_mode() const
	{
		return impl->face_cull_mode;
	}

	FillMode RasterizerStateDescription::get_face_fill_mode() const
	{
		return impl->face_fill_mode;
	}

	FaceSide RasterizerStateDescription::get_front_face() const
	{
		return impl->front_face;
	}

	bool RasterizerStateDescription::get_enable_scissor() const
	{
		return impl->enable_scissor;
	}

	bool RasterizerStateDescription::get_antialiased() const
	{
		return impl->is_antialiased;
	}

	bool RasterizerStateDescription::get_offset_point() const
	{
		return impl->is_offset_point;
	}

	bool RasterizerStateDescription::get_offset_line() const
	{
		return impl->is_offset_line;
	}

	bool RasterizerStateDescription::get_offset_fill() const
	{
		return impl->is_offset_fill;
	}

	void RasterizerStateDescription::get_polygon_offset(float &out_factor, float &out_units) const
	{
		out_factor = impl->offset_factor;
		out_units = impl->offset_units;
	}

	float RasterizerStateDescription::get_point_size() const
	{
		return impl->pen_point_size;
	}

	float RasterizerStateDescription::get_point_fade_treshold_size() const
	{
		return impl->pen_point_fade_treshold_size;
	}

	bool RasterizerStateDescription::is_point_size() const
	{
		return impl->pen_vertex_shader_point_sizes;
	}

	PointSpriteOrigin RasterizerStateDescription::get_point_sprite_origin() const
	{
		return impl->pen_point_sprite_origin;
	}


	void RasterizerStateDescription::enable_antialiased(bool value)
	{
		impl->is_antialiased = value;
	}

	void RasterizerStateDescription::enable_offset_point(bool value)
	{
		impl->is_offset_point = value;
	}

	void RasterizerStateDescription::enable_offset_line(bool value)
	{
		impl->is_offset_line = value;
	}

	void RasterizerStateDescription::enable_offset_fill(bool value)
	{
		impl->is_offset_fill = value;
	}

	void RasterizerStateDescription::set_polygon_offset(float factor, float units)
	{
		impl->offset_factor = factor;
		impl->offset_units = units;
	}

	void RasterizerStateDescription::set_point_size(float value)
	{
		impl->pen_point_size = value;
	}

	void RasterizerStateDescription::set_point_fade_treshold_size(float value)
	{
		impl->pen_point_fade_treshold_size = value;
	}

	void RasterizerStateDescription::enable_point_size(bool enable)
	{
		impl->pen_vertex_shader_point_sizes = enable;
	}

	void RasterizerStateDescription::set_point_sprite_origin(PointSpriteOrigin origin)
	{
		impl->pen_point_sprite_origin = origin;
	}

	void RasterizerStateDescription::set_culled(bool value)
	{
		impl->culled = value;
	}

	void RasterizerStateDescription::enable_line_antialiasing(bool enabled)
	{
		impl->line_antialiasing = enabled;
	}

	void RasterizerStateDescription::set_face_cull_mode(CullMode value)
	{
		impl->face_cull_mode = value;
	}

	void RasterizerStateDescription::set_face_fill_mode(FillMode value)
	{
		impl->face_fill_mode = value;
	}

	void RasterizerStateDescription::set_front_face(FaceSide value)
	{
		impl->front_face = value;
	}

	void RasterizerStateDescription::enable_scissor(bool enabled)
	{
		impl->enable_scissor = enabled;
	}

	bool RasterizerStateDescription::operator==(const RasterizerStateDescription &other) const
	{
		return impl == other.impl || *impl.get() == *other.impl.get();
	}

	bool RasterizerStateDescription::operator<(const RasterizerStateDescription &other) const
	{
		return *impl.get() < *other.impl.get();
	}
}
