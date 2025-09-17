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

#pragma once

#include "graphic_context_state.h"

namespace clan
{
	class GraphicContextProvider;

	class GraphicScreen
	{
	public:
		GraphicScreen(GraphicContextProvider *provider);
		~GraphicScreen();
		GraphicContextProvider *get_provider() { return provider; }
		int get_max_attributes() const { return max_attributes; }

		void set_active(GraphicContext_State *state);
		void state_destroyed(GraphicContext_State *state);

		void on_rasterizer_state_changed(GraphicContext_State *state);
		void on_blend_state_changed(GraphicContext_State *state);
		void on_depth_stencil_state_changed(GraphicContext_State *state);

		void on_texture_changed(GraphicContext_State *state, int unit_index);
		void on_textures_changed(GraphicContext_State *state);
		void on_image_texture_changed(GraphicContext_State *state, int unit_index);
		void on_image_textures_changed(GraphicContext_State *state);
		void on_uniform_buffer_changed(GraphicContext_State *state, int index);
		void on_storage_buffer_changed(GraphicContext_State *state, int index);
		void on_scissor_changed(GraphicContext_State *state);
		void on_viewport_changed(GraphicContext_State *state);
		void on_depth_range_changed(GraphicContext_State *state, int viewport);
		void on_framebuffer_changed(GraphicContext_State *state);
		void on_program_changed(GraphicContext_State *state);
		void on_draw_buffer_changed(GraphicContext_State *state);

		RasterizerState default_rasterizer_state;
		BlendState default_blend_state;
		DepthStencilState default_depth_stencil_state;

	private:
		void set_default_state();
		void set_active_buffer_control(GraphicContext_State *state);
		void set_active_frame_buffer(GraphicContext_State *state);
		void set_active_textures(GraphicContext_State *state);
		void set_active_image_textures(GraphicContext_State *state);
		void set_active_uniform_buffers(GraphicContext_State *state);
		void set_active_storage_buffers(GraphicContext_State *state);
		void set_active_scissor(GraphicContext_State *state);
		void set_active_viewport(GraphicContext_State *state);
		void set_active_program(GraphicContext_State *state);
		void set_active_depth_range(GraphicContext_State *state);

		int max_attributes;
		GraphicContextProvider *provider;
		GraphicContext_State *current;
		GraphicContext_State active_state;
	};
}
