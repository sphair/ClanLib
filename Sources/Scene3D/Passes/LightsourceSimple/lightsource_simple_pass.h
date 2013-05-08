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

#include "API/Scene3D/scene_light.h"
#include "API/Scene3D/scene_inout_data.h"
#include "Scene3D/scene_light_impl.h"
#include "icosahedron.h"

namespace clan
{

class GPUTimer;
class Scene_Impl;

class LightsourceSimplePass : SceneLightVisitor
{
public:
	LightsourceSimplePass(GraphicContext &gc, const std::string &shader_path, SceneInOutDataContainer &inout);
	~LightsourceSimplePass();

	void run(GraphicContext &gc, Scene_Impl *scene);

private:
	ProgramObject compile_and_link(GraphicContext &gc, const std::string &shader_path, const std::string &type);

	void setup(GraphicContext &gc);
	void find_lights(GraphicContext &gc, Scene_Impl *scene);
	void upload(GraphicContext &gc, Scene_Impl *scene);
	void render(GraphicContext &gc, GPUTimer &timer);

	// SceneLightVisitor
	void light(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneLight_Impl *light);

	// In:
	SceneInOutData<Rect> viewport;
	SceneInOutData<float> field_of_view;
	SceneInOutData<Mat4f> world_to_eye;
	SceneInOutData<Texture2D> diffuse_color_gbuffer;
	SceneInOutData<Texture2D> specular_color_gbuffer;
	SceneInOutData<Texture2D> specular_level_gbuffer;
	SceneInOutData<Texture2D> self_illumination_gbuffer;
	SceneInOutData<Texture2D> normal_z_gbuffer;
	SceneInOutData<Texture2DArray> shadow_maps;
	SceneInOutData<Texture2D> zbuffer;

	// Out:
	SceneInOutData<Texture2D> final_color;

	static const int max_lights = 1023;
	static const int vectors_per_light = 6;

	struct Uniforms
	{
		Mat4f object_to_eye;
		Mat4f eye_to_projection;
		float rcp_f;
		float rcp_f_div_aspect;
		Vec2f two_rcp_viewport_size;
	};

	FrameBuffer fb;
	BlendState blend_state;

	DepthStencilState icosahedron_depth_stencil_state;
	RasterizerState icosahedron_rasterizer_state;

	DepthStencilState rect_depth_stencil_state;
	RasterizerState rect_rasterizer_state;

	PrimitivesArray icosahedron_prim_array;
	PrimitivesArray rect_prim_array;

	UniformVector<Uniforms> uniforms;

	std::unique_ptr<Icosahedron> icosahedron;
	VertexArrayVector<Vec4f> rect_positions;

	PixelBuffer light_instance_transfer;
	Texture1D light_instance_texture;

	ProgramObject icosahedron_light_program;
	ProgramObject rect_light_program;

	std::vector<SceneLight_Impl *> lights;
};

}
