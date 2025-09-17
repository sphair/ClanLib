/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "Display/precomp.h"
#include "graphic_screen.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "graphic_context_impl.h"
#include "API/Display/Render/depth_stencil_state_description.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Render/rasterizer_state_description.h"

namespace clan
{
	GraphicScreen::GraphicScreen(GraphicContextProvider *provider) : max_attributes(0), provider(provider), current(nullptr)
	{
		SharedGCData::add_ref();

		default_rasterizer_state = RasterizerState(provider, RasterizerStateDescription());
		default_blend_state = BlendState(provider, BlendStateDescription());
		default_depth_stencil_state = DepthStencilState(provider, DepthStencilStateDescription());

		set_default_state();
		max_attributes = provider->get_max_attributes();
	}

	GraphicScreen::~GraphicScreen()
	{
		SharedGCData::release_ref();
		if (provider)
			delete provider;
	}

	void GraphicScreen::set_active(GraphicContext_State *state)
	{
		if (current != state)
		{
			current = state;

			provider->throw_if_disposed();
			set_active_frame_buffer(state);	// Set the framebuffer first, this is more friendly for the GL1 target, so only the pbuffer context is set (depending on the application code flow)

			active_state.rasterizer_state = state->rasterizer_state;
			active_state.blend_state = state->blend_state;
			active_state.blend_color = state->blend_color;
			active_state.sample_mask = state->sample_mask;
			active_state.depth_stencil_state = state->depth_stencil_state;
			active_state.stencil_ref = state->stencil_ref;
			provider->set_rasterizer_state(active_state.rasterizer_state.get_provider());
			provider->set_blend_state(active_state.blend_state.get_provider(), active_state.blend_color, active_state.sample_mask);
			provider->set_depth_stencil_state(active_state.depth_stencil_state.get_provider(), active_state.stencil_ref);

			set_active_buffer_control(state);
			set_active_depth_range(state);
			set_active_textures(state);
			set_active_image_textures(state);
			set_active_scissor(state);
			set_active_viewport(state);
			set_active_program(state);
		}
	}

	void GraphicScreen::state_destroyed(GraphicContext_State *state)
	{
		if (current == state)
		{
			if (provider && !provider->is_disposed())
			{
				// Reset objects on provider (so any destroyed, but still bound, objects are released now)
				GraphicContext_State default_state;
				default_state.rasterizer_state = default_rasterizer_state;
				default_state.blend_state = default_blend_state;
				default_state.depth_stencil_state = default_depth_stencil_state;
				set_active(&default_state);
			}
			current = nullptr;
		}
	}

	void GraphicScreen::on_rasterizer_state_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			active_state.rasterizer_state = state->rasterizer_state;
			provider->set_rasterizer_state(active_state.rasterizer_state.get_provider());
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_blend_state_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			active_state.blend_state = state->blend_state;
			active_state.blend_color = state->blend_color;
			active_state.sample_mask = state->sample_mask;
			provider->set_blend_state(active_state.blend_state.get_provider(), active_state.blend_color, active_state.sample_mask);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_depth_stencil_state_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			active_state.depth_stencil_state = state->depth_stencil_state;
			active_state.stencil_ref = state->stencil_ref;
			provider->set_depth_stencil_state(active_state.depth_stencil_state.get_provider(), active_state.stencil_ref);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_texture_changed(GraphicContext_State *state, int unit_index)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			if (active_state.textures.size() < unit_index + 1)
				active_state.textures.resize(unit_index + 1);
			active_state.textures[unit_index] = state->textures[unit_index];	// Copy to active state
			if (state->textures[unit_index].is_null())
			{
				provider->reset_texture(unit_index);
			}
			else
			{
				provider->set_texture(unit_index, state->textures[unit_index]);
			}
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_textures_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_textures(state);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_image_texture_changed(GraphicContext_State *state, int unit_index)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			if (active_state.image_textures.size() < unit_index + 1)
				active_state.image_textures.resize(unit_index + 1);
			active_state.image_textures[unit_index] = state->image_textures[unit_index];	// Copy to active state
			if (state->image_textures[unit_index].is_null())
			{
				provider->reset_image_texture(unit_index);
			}
			else
			{
				provider->set_image_texture(unit_index, state->image_textures[unit_index]);
			}
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_image_textures_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_image_textures(state);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_uniform_buffer_changed(GraphicContext_State *state, int unit_index)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			if (active_state.uniform_buffers.size() < unit_index + 1)
				active_state.uniform_buffers.resize(unit_index + 1);
			active_state.uniform_buffers[unit_index] = state->uniform_buffers[unit_index];	// Copy to active state
			if (state->uniform_buffers[unit_index].is_null())
			{
				provider->reset_uniform_buffer(unit_index);
			}
			else
			{
				provider->set_uniform_buffer(unit_index, state->uniform_buffers[unit_index]);
			}
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_storage_buffer_changed(GraphicContext_State *state, int unit_index)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			if (active_state.storage_buffers.size() < unit_index + 1)
				active_state.storage_buffers.resize(unit_index + 1);
			active_state.storage_buffers[unit_index] = state->storage_buffers[unit_index];	// Copy to active state
			if (state->storage_buffers[unit_index].is_null())
			{
				provider->reset_storage_buffer(unit_index);
			}
			else
			{
				provider->set_storage_buffer(unit_index, state->storage_buffers[unit_index]);
			}
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_scissor_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_scissor(state);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_viewport_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_viewport(state);
			set_active_program(state);	//<-- Is this correct to have this here?
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_depth_range_changed(GraphicContext_State *state, int viewport)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_depth_range(state);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_framebuffer_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_frame_buffer(state);
			set_active_program(state);	//<-- Is this correct to have this here?
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_program_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			set_active_program(state);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::on_draw_buffer_changed(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (state == current)
		{
			active_state.draw_buffer = state->draw_buffer;
			provider->set_draw_buffer(active_state.draw_buffer);
		}
		else
		{
			set_active(state);
		}
	}

	void GraphicScreen::set_active_buffer_control(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (active_state.draw_buffer != state->draw_buffer)
		{
			active_state.draw_buffer = state->draw_buffer;
			provider->set_draw_buffer(active_state.draw_buffer);
		}
	}

	void GraphicScreen::set_active_depth_range(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (active_state.depth_range != state->depth_range)
		{
			active_state.depth_range = state->depth_range;
			unsigned int depth_range_size = state->depth_range.size();
			if (depth_range_size == 1)
			{
				provider->set_depth_range(active_state.depth_range[0].width, active_state.depth_range[0].height);
			}
			else
			{
				for (unsigned int cnt = 0; cnt < depth_range_size; cnt++)
				{
					provider->set_depth_range(cnt, active_state.depth_range[0].width, active_state.depth_range[0].height);
				}
			}
		}
	}

	void GraphicScreen::set_active_frame_buffer(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		active_state.write_frame_buffer = state->write_frame_buffer;
		active_state.read_frame_buffer = state->read_frame_buffer;

		if (active_state.write_frame_buffer.is_null())
		{
			provider->reset_frame_buffer();
		}
		else
		{
			provider->set_frame_buffer(active_state.write_frame_buffer, active_state.read_frame_buffer);
		}
	}

	void GraphicScreen::set_active_textures(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		int old_max_textures = active_state.textures.size();
		active_state.textures = state->textures;
		unsigned int max_textures = active_state.textures.size();
		for (unsigned int cnt = 0; cnt < max_textures; cnt++)
		{
			Texture texture = active_state.textures[cnt];
			if (texture.is_null())
			{
				provider->reset_texture(cnt);
			}
			else
			{
				provider->set_texture(cnt, texture);
			}
		}
		for (unsigned int cnt = max_textures; cnt < old_max_textures; cnt++)
		{
			provider->reset_texture(cnt);
		}
	}

	void GraphicScreen::set_active_image_textures(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		int old_max_textures = active_state.image_textures.size();
		active_state.image_textures = state->image_textures;
		unsigned int max_textures = active_state.image_textures.size();
		for (unsigned int cnt = 0; cnt < max_textures; cnt++)
		{
			Texture texture = active_state.image_textures[cnt];
			if (texture.is_null())
			{
				provider->reset_image_texture(cnt);
			}
			else
			{
				provider->set_image_texture(cnt, texture);
			}
		}
		for (unsigned int cnt = max_textures; cnt < old_max_textures; cnt++)
		{
			provider->reset_image_texture(cnt);
		}
	}

	void GraphicScreen::set_active_uniform_buffers(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		int old_max_uniform_buffers = active_state.uniform_buffers.size();
		active_state.uniform_buffers = state->uniform_buffers;
		unsigned int max_uniform_buffers = active_state.uniform_buffers.size();
		for (unsigned int cnt = 0; cnt < max_uniform_buffers; cnt++)
		{
			UniformBuffer buffer = active_state.uniform_buffers[cnt];
			if (buffer.is_null())
			{
				provider->reset_uniform_buffer(cnt);
			}
			else
			{
				provider->set_uniform_buffer(cnt, buffer);
			}
		}
		for (unsigned int cnt = max_uniform_buffers; cnt < old_max_uniform_buffers; cnt++)
		{
			provider->reset_uniform_buffer(cnt);
		}
	}

	void GraphicScreen::set_active_storage_buffers(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		int old_max_storage_buffers = active_state.storage_buffers.size();
		active_state.storage_buffers = state->storage_buffers;
		unsigned int max_storage_buffers = active_state.storage_buffers.size();
		for (unsigned int cnt = 0; cnt < max_storage_buffers; cnt++)
		{
			StorageBuffer buffer = active_state.storage_buffers[cnt];
			if (buffer.is_null())
			{
				provider->reset_storage_buffer(cnt);
			}
			else
			{
				provider->set_storage_buffer(cnt, buffer);
			}
		}
		for (unsigned int cnt = max_storage_buffers; cnt < old_max_storage_buffers; cnt++)
		{
			provider->reset_storage_buffer(cnt);
		}
	}

	void GraphicScreen::set_active_scissor(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		active_state.scissor_set = state->scissor_set;
		if (active_state.scissor_set)
		{
			active_state.scissor = state->scissor;
			provider->set_scissor(active_state.scissor);
		}
		else
		{
			provider->reset_scissor();
		}
	}

	void GraphicScreen::set_active_viewport(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		if (active_state.viewport != state->viewport)
		{
			active_state.viewport = state->viewport;

			unsigned int viewport_size = state->viewport.size();
			if (viewport_size == 1)
			{
				provider->set_viewport(active_state.viewport[0]);
			}
			else
			{
				for (unsigned int cnt = 0; cnt < viewport_size; cnt++)
				{
					provider->set_viewport(cnt, active_state.viewport[cnt]);
				}
			}
		}
	}

	void GraphicScreen::set_active_program(GraphicContext_State *state)
	{
		provider->throw_if_disposed();
		active_state.program_standard = state->program_standard;
		active_state.program_standard_set = state->program_standard_set;
		active_state.program = state->program;

		if (active_state.program.is_null())
		{
			provider->reset_program_object();
		}
		else
		{
			if (active_state.program_standard_set)
			{
				provider->set_program_object(active_state.program_standard);
			}
			else
			{
				provider->set_program_object(active_state.program);
			}
		}
	}

	// This is used to initialise OpenGL to the default GraphicContext_State
	void GraphicScreen::set_default_state()
	{
		provider->set_draw_buffer(active_state.draw_buffer);

		provider->reset_scissor();
		provider->set_viewport(active_state.viewport[0]);
		provider->set_depth_range(active_state.depth_range[0].width, active_state.depth_range[0].height);

		// Frame buffer should be already reset
		// Textures should be already reset
		// Uniform buffers should be already reset
		// Program objects should be already reset
	}
}
