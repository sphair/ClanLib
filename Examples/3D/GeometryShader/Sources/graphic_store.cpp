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

#include "precomp.h"
#include "graphic_store.h"

GraphicStore::GraphicStore(GraphicContext &gc) : shader_color_geometry(gc)
{
	// Create a depth buffer
	framebuffer_depth = FrameBuffer(gc);
	texture_depth = Texture2D(gc, gc.get_size(), TextureFormat::depth_component32);
	texture_depth.set_wrap_mode(TextureWrapMode::clamp_to_edge, TextureWrapMode::clamp_to_edge);
	framebuffer_depth.attach_depth(texture_depth);

	// Load graphics
	texture_alpha = Texture2D(gc, "Resources/alpha_ball.png");
	texture_solid = Texture2D(gc, "Resources/alpha_ball2.png");
	texture_alpha.set_wrap_mode(TextureWrapMode::clamp_to_edge, TextureWrapMode::clamp_to_edge);
	texture_solid.set_wrap_mode(TextureWrapMode::clamp_to_edge, TextureWrapMode::clamp_to_edge);

}

GraphicStore::~GraphicStore()
{
}

