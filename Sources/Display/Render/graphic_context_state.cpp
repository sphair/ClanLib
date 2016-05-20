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
**    Mark Page
*/

#include "Display/precomp.h"
#include "graphic_context_state.h"

namespace clan
{
	GraphicContext_State::GraphicContext_State()
	{
		textures.reserve(8);	// Reserse space for 8 unit indexes
		viewport.resize(1);		// Always have a minimum of 1 viewport
		depth_range.push_back(Sizef(0.0f, 1.0f));		// Always have a minimum of 1 viewport
		scissor_set = false;
		program_standard_set = false;
		blend_color = StandardColorf::white();
		sample_mask = 0xffffffff;
		stencil_ref = 0;
		draw_buffer = buffer_back;
	}

	void GraphicContext_State::copy_state(const GraphicContext_State *other)
	{
		*this = *other;
	}
}
