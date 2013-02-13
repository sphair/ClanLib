/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "API/Scene3D/scene.h"
#include "Scene3D/Framework/inout_data.h"
namespace clan
{

class DiffuseGIPassCS
{
public:
	DiffuseGIPassCS(clan::GraphicContext &gc);
	~DiffuseGIPassCS();
	void run(clan::GraphicContext &gc, Scene &scene);

	InData<clan::Rect> viewport;
	InData<clan::Texture2D> diffuse_color_gbuffer;
	InData<clan::Texture2D> normal_z_gbuffer;
	InData<clan::Texture2DArray> shadow_maps;

	OutData<clan::Texture2D> final_color;

private:
	void update_buffers(clan::GraphicContext &gc);
	clan::ProgramObject compile_and_link(clan::GraphicContext &gc, const std::string &compute_filename);

	clan::ProgramObject init_lpv_program;
	clan::ProgramObject init_gv_program;
	clan::ProgramObject propagate_lpv_program;
	clan::ProgramObject accumulate_lpv_program;
	clan::ProgramObject render_result_program;
};

}

