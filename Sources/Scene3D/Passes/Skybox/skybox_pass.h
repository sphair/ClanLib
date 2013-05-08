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

#include "API/Scene3D/scene_inout_data.h"

namespace clan
{

class Scene_Impl;

class SkyboxPass
{
public:
	SkyboxPass(const std::string &shader_path, SceneInOutDataContainer &inout);
	void run(GraphicContext &gc, Scene_Impl *scene);

private:
	void setup(GraphicContext &gc);
	void create_clouds(GraphicContext &gc);
	void create_cloud_texture(GraphicContext &gc);
	void create_stars(GraphicContext &gc);
	void create_star_texture(GraphicContext &gc);
	void create_programs(GraphicContext &gc);
	void create_billboard_program(GraphicContext &gc);
	void create_cube_program(GraphicContext &gc);
	static float random(float min_value, float max_value);

	// In:
	SceneInOutData<Rect> viewport;
	SceneInOutData<float> field_of_view;
	SceneInOutData<Mat4f> world_to_eye;

	// InOut:
	SceneInOutData<Texture2D> diffuse_color_gbuffer;
	SceneInOutData<Texture2D> specular_color_gbuffer;
	SceneInOutData<Texture2D> specular_level_gbuffer;
	SceneInOutData<Texture2D> self_illumination_gbuffer;
	SceneInOutData<Texture2D> normal_z_gbuffer;
	SceneInOutData<Texture2D> zbuffer;

	std::string shader_path;

	struct Uniforms
	{
		Mat4f object_to_eye;
		Mat4f eye_to_projection;
	};

	FrameBuffer fb;
	BlendState blend_state;
	DepthStencilState depth_stencil_state;
	RasterizerState rasterizer_state;

	UniformVector<Uniforms> uniforms;

	ProgramObject billboard_program;
	VertexArrayVector<Vec3f> billboard_positions;
	PrimitivesArray billboard_prim_array;

	ProgramObject cube_program;
	VertexArrayVector<Vec3f> cube_positions;
	PrimitivesArray cube_prim_array;

	static Vec3f cpu_billboard_positions[6];
	static Vec3f cpu_cube_positions[6 * 6];

	Texture2D cloud_texture;

	Texture2D star_texture;
	Texture2D star_instance_texture;
	static const int num_star_instances = 1024;
};

}

