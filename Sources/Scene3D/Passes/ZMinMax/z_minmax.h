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

#include "Scene3D/Framework/inout_data.h"
namespace clan
{

class ZMinMax
{
public:
	ZMinMax(int tile_size = 16);
	void minmax(clan::GraphicContext &gc);

	InData<clan::Rect> viewport;
	InData<clan::Texture2D> normal_z;
	OutData<clan::Texture2D> result;

private:
	void update_buffers(clan::GraphicContext &gc);
	void get_shader_glsl(std::string &out_vertex_shader, std::string &out_fragment_shader0, std::string &out_fragment_shader1);
	void get_shader_hlsl(std::string &out_vertex_shader, std::string &out_fragment_shader0, std::string &out_fragment_shader1);
	clan::Size find_texture_size(clan::Texture2D &normal_z);
	clan::ProgramObject compile_and_link(clan::GraphicContext &gc, const std::string &vertex_source, const std::string &fragment_source);
	static int find_iterations(int size);

	int tile_size;
	int iterations;
	clan::ProgramObject program0, program1;
	clan::PrimitivesArray prim_array;
	clan::VertexArrayVector<clan::Vec4f> vertices;
	clan::FrameBuffer fb0, fb1;
	clan::Texture2D texture0, texture1;
	clan::BlendState blend_state;
};

}

