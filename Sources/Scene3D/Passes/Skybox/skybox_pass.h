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

class Scene;

class SkyboxPass
{
public:
	SkyboxPass();
	void run(clan::GraphicContext &gc, Scene &scene);

	InData<clan::Rect> viewport;
	InData<float> field_of_view;
	InData<clan::Mat4f> world_to_eye;

	InOutData<clan::Texture2D> diffuse_color_gbuffer;
	InOutData<clan::Texture2D> specular_color_gbuffer;
	InOutData<clan::Texture2D> specular_level_gbuffer;
	InOutData<clan::Texture2D> self_illumination_gbuffer;
	InOutData<clan::Texture2D> normal_z_gbuffer;
	InOutData<clan::Texture2D> zbuffer;

private:
	void setup(clan::GraphicContext &gc);
	void create_clouds(clan::GraphicContext &gc);
	void create_cloud_texture(clan::GraphicContext &gc);
	void create_stars(clan::GraphicContext &gc);
	void create_star_texture(clan::GraphicContext &gc);
	void create_programs(clan::GraphicContext &gc);
	void create_billboard_program(clan::GraphicContext &gc);
	void create_cube_program(clan::GraphicContext &gc);
	static float random(float min_value, float max_value);

	struct Uniforms
	{
		clan::Mat4f object_to_eye;
		clan::Mat4f eye_to_projection;
	};

	clan::FrameBuffer fb;
	clan::BlendState blend_state;
	clan::DepthStencilState depth_stencil_state;
	clan::RasterizerState rasterizer_state;

	clan::UniformVector<Uniforms> uniforms;

	clan::ProgramObject billboard_program;
	clan::VertexArrayVector<clan::Vec3f> billboard_positions;
	clan::PrimitivesArray billboard_prim_array;

	clan::ProgramObject cube_program;
	clan::VertexArrayVector<clan::Vec3f> cube_positions;
	clan::PrimitivesArray cube_prim_array;

	static clan::Vec3f cpu_billboard_positions[6];
	static clan::Vec3f cpu_cube_positions[6 * 6];

	clan::Texture2D cloud_texture;

	clan::Texture2D star_texture;
	clan::Texture2D star_instance_texture;
	static const int num_star_instances = 1024;
};

}

