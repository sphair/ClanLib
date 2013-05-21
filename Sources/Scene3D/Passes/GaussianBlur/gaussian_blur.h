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

namespace clan
{

class GaussianBlur
{
public:
	GaussianBlur();
	void blur(GraphicContext &gc, TextureFormat format, float blur_amount, int sample_count = 15);

	Resource<Texture2D> input;
	Resource<FrameBuffer> output;

private:
	void setup(GraphicContext &gc, Size new_size, TextureFormat new_format, float blur_amount, int sample_count);

	void get_shader_glsl(float blur_amount, int sample_count, std::string &out_vertex_shader, std::string &out_vertical_fragment_shader, std::string &out_horizontal_fragment_shader);
	void get_shader_hlsl(float blur_amount, int sample_count, std::string &out_vertex_shader, std::string &out_vertical_fragment_shader, std::string &out_horizontal_fragment_shader);
	float compute_gaussian(float n, float theta);
	void compute_blur_samples(int sample_count, float blur_amount, int dx, int dy, std::vector<float> &sample_weights, std::vector<Vec2i> &sample_offsets);

	struct BlurSetup
	{
		BlurSetup(float blur_amount, int sample_count) : blur_amount(blur_amount), sample_count(sample_count) { }

		float blur_amount;
		int sample_count;
		ProgramObject vertical_blur_program;
		ProgramObject horizontal_blur_program;
	};

	PrimitivesArray prim_array;
	VertexArrayVector<Vec4f> gpu_positions;
	FrameBuffer fb0;
	Texture2D pass0_texture;
	Size size;
	TextureFormat format;
	std::vector<BlurSetup> blur_setups;
	std::vector<BlurSetup>::iterator current_blur_setup;
	BlendState blend_state;
};

}

