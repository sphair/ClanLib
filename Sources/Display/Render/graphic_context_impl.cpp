/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "graphic_context_impl.h"
#include "primitives_array_impl.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{
	GraphicContext_Impl::GraphicContext_Impl(GraphicContextProvider *provider)
		: graphic_screen(new GraphicScreen(provider))
	{
		resize_slot = graphic_screen->get_provider()->sig_window_resized().connect(bind_member(this, &GraphicContext_Impl::on_window_resized));
		set_viewport(-1, graphic_screen->get_provider()->get_display_window_size());
	}

	GraphicContext_Impl::GraphicContext_Impl(const GraphicContext_Impl *from_gc, bool clone)
		: graphic_screen(from_gc->graphic_screen)
	{
		default_rasterizer_state = from_gc->default_rasterizer_state;
		default_blend_state = from_gc->default_blend_state;
		default_depth_stencil_state = from_gc->default_depth_stencil_state;

		resize_slot = graphic_screen->get_provider()->sig_window_resized().connect(bind_member(this, &GraphicContext_Impl::on_window_resized));
		set_viewport(-1, from_gc->viewport[0]);

		if (clone)
		{
			copy_state(from_gc);
		}
		else
		{
			set_rasterizer_state(default_rasterizer_state);
			set_blend_state(default_blend_state, Colorf::white, 0xffffffff);
			set_depth_stencil_state(default_depth_stencil_state, 0);
		}
	}

	GraphicContext_Impl::~GraphicContext_Impl()
	{
		graphic_screen->state_destroyed(this);
	}

	void GraphicContext_Impl::on_window_resized(const Size &size)
	{
		display_window_size = size;
		if (write_frame_buffer.is_null())
			set_viewport(-1, get_size());
	}

	Size GraphicContext_Impl::get_size()
	{
		if (write_frame_buffer.is_null())
		{
			if ((display_window_size.width == 0) || (display_window_size.height == 0))
				display_window_size = graphic_screen->get_provider()->get_display_window_size();
			return display_window_size;
		}
		else
		{
			return write_frame_buffer.get_size();
		}
	}

	void GraphicContext_Impl::set_active()
	{
		graphic_screen->set_active(this);
	}

	void GraphicContext_Impl::set_texture(int unit_index, const Texture &texture)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((unit_index < 0) || (unit_index > 255))
			throw Exception("Invalid texture unit index");

		// Extend the selected textures array if required
		if (unit_index >= textures.size())
			textures.resize(unit_index + 1);

		textures[unit_index] = texture;
		graphic_screen->on_texture_changed(this, unit_index);
	}

	void GraphicContext_Impl::set_textures(std::vector<Texture> &new_textures)
	{
		textures = new_textures;
		graphic_screen->on_textures_changed(this);
	}

	void GraphicContext_Impl::set_image_texture(int unit_index, const Texture &texture)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((unit_index < 0) || (unit_index > 255))
			throw Exception("Invalid texture unit index");

		// Extend the selected textures array if required
		if (unit_index >= image_textures.size())
			image_textures.resize(unit_index + 1);

		image_textures[unit_index] = texture;
		graphic_screen->on_image_texture_changed(this, unit_index);
	}

	void GraphicContext_Impl::set_image_textures(std::vector<Texture> &new_textures)
	{
		image_textures = new_textures;
		graphic_screen->on_image_textures_changed(this);
	}

	void GraphicContext_Impl::set_uniform_buffer(int index, const UniformBuffer &buffer)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((index < 0) || (index > 255))
			throw Exception("Invalid uniform buffer index");

		// Extend the selected uniform_buffers array if required
		if (index >= uniform_buffers.size())
			uniform_buffers.resize(index + 1);

		uniform_buffers[index] = buffer;
		graphic_screen->on_uniform_buffer_changed(this, index);
	}

	void GraphicContext_Impl::set_storage_buffer(int index, const StorageBuffer &buffer)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((index < 0) || (index > 255))
			throw Exception("Invalid storage buffer index");

		// Extend the selected storage_buffers array if required
		if (index >= storage_buffers.size())
			storage_buffers.resize(index + 1);

		storage_buffers[index] = buffer;
		graphic_screen->on_storage_buffer_changed(this, index);
	}

	void GraphicContext_Impl::set_scissor(const Rect &rect)
	{
		scissor_set = true;
		scissor = rect;
		graphic_screen->on_scissor_changed(this);
	}

	void GraphicContext_Impl::reset_scissor()
	{
		scissor_set = false;
		graphic_screen->on_scissor_changed(this);
	}

	void GraphicContext_Impl::set_viewport(int index, const Rectf &viewport_box)
	{
		if (index < 0)	// Special case, clear all viewports and only have a single one
		{
			if (viewport.size() != 1)
				viewport.resize(1);
			index = 0;
		}

		if (viewport.size() <= index)	// Grow vector if not large enough
			viewport.resize(index + 1);

		viewport[index] = viewport_box;

		graphic_screen->on_viewport_changed(this);
	}

	void GraphicContext_Impl::set_depth_range(int viewport, float n, float f)
	{
		if (viewport < 0)	// Special case, clear all depth ranges and only have a single one
		{
			if (depth_range.size() != 1)
				depth_range.resize(1);
			viewport = 0;
		}

		if (depth_range.size() <= viewport)	// Grow vector if not large enough
			depth_range.resize(viewport + 1);

		depth_range[viewport] = Sizef(n, f);
		graphic_screen->on_depth_range_changed(this, viewport);
	}

	void GraphicContext_Impl::set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer)
	{
		write_frame_buffer = write_buffer;
		read_frame_buffer = read_buffer;
		graphic_screen->on_framebuffer_changed(this);
	}

	void GraphicContext_Impl::set_program_object(StandardProgram standard_program)
	{
		program = graphic_screen->get_provider()->get_program_object(standard_program);
		program_standard_set = true;
		program_standard = standard_program;
		graphic_screen->on_program_changed(this);
	}

	ProgramObject GraphicContext_Impl::get_program_object() const
	{
		return program;
	}

	void GraphicContext_Impl::set_program_object(const ProgramObject &new_program)
	{
		program = new_program;
		program_standard_set = false;
		graphic_screen->on_program_changed(this);
	}

	void GraphicContext_Impl::reset_program_object()
	{
		program = ProgramObject();
		program_standard_set = false;
		graphic_screen->on_program_changed(this);
	}

	void GraphicContext_Impl::set_rasterizer_state(const RasterizerState &state)
	{
		rasterizer_state = state;
		graphic_screen->on_rasterizer_state_changed(this);
	}

	void GraphicContext_Impl::set_blend_state(const BlendState &state, const Colorf &new_blend_color, unsigned int new_sample_mask)
	{
		blend_state = state;
		blend_color = new_blend_color;
		sample_mask = new_sample_mask;
		graphic_screen->on_blend_state_changed(this);
	}

	void GraphicContext_Impl::set_depth_stencil_state(const DepthStencilState &state, int new_stencil_ref)
	{
		depth_stencil_state = state;
		stencil_ref = new_stencil_ref;
		graphic_screen->on_depth_stencil_state_changed(this);
	}

	void GraphicContext_Impl::set_draw_buffer(DrawBuffer buffer)
	{
		draw_buffer = buffer;
		graphic_screen->on_draw_buffer_changed(this);
	}

	void GraphicContext_Impl::flush()
	{
		graphic_screen->get_provider()->flush();
	}
}
