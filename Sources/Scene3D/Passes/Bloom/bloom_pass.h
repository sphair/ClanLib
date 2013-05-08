/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#pragma once

#include "Scene3D/Passes/GaussianBlur/gaussian_blur.h"
#include "API/Scene3D/scene_inout_data.h"

namespace clan
{

class BloomPass
{
public:
	BloomPass(GraphicContext &gc, const std::string &shader_path, SceneInOutDataContainer &inout);
	void run(GraphicContext &gc);

private:
	void setup_bloom_extract(GraphicContext &gc);

	// In:
	SceneInOutData<Rect> viewport;
	SceneInOutData<Texture2D> final_color;

	// Out:
	SceneInOutData<Texture2D> bloom_contribution;

	GaussianBlur bloom_blur;
	VertexArrayVector<Vec4f> rect_positions;
	PrimitivesArray rect_primarray;
	ProgramObject bloom_shader;
	FrameBuffer fb_bloom_extract;
	BlendState blend_state;
};

}

