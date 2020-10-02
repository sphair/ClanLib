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

#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/uniform_buffer.h"
#include "API/Display/Render/storage_buffer.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/depth_stencil_state.h"
#include "API/Display/Render/rasterizer_state.h"

namespace clan
{
	class GraphicContext_State
	{
	public:
		GraphicContext_State();
		void copy_state(const GraphicContext_State *other);

		FrameBuffer read_frame_buffer;
		FrameBuffer write_frame_buffer;

		std::vector<Texture> textures;
		std::vector<Texture> image_textures;
		std::vector<UniformBuffer> uniform_buffers;
		std::vector<StorageBuffer> storage_buffers;

		Rect scissor;
		bool scissor_set;

		std::vector<Rectf> viewport;
		std::vector<Sizef> depth_range;

		StandardProgram program_standard;
		bool program_standard_set;
		ProgramObject program;	//<-- Note this is valid when program_standard_set is true

		RasterizerState rasterizer_state;
		BlendState blend_state;
		Colorf blend_color;
		unsigned int sample_mask;
		DepthStencilState depth_stencil_state;
		int stencil_ref;

		DrawBuffer draw_buffer;
	};
}
