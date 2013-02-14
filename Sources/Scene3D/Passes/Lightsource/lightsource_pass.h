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

#include "Scene3D/Passes/ZMinMax/z_minmax.h"
#include "Scene3D/Framework/inout_data.h"
#include "API/Scene3D/scene_light.h"
#include "Scene3D/scene_light_impl.h"
namespace clan
{

class GPUTimer;
class Scene_Impl;

class LightsourcePass : SceneLightVisitor
{
public:
	LightsourcePass(GraphicContext &gc);
	~LightsourcePass();

	void run(GraphicContext &gc, Scene_Impl *scene);

	InData<Rect> viewport;
	InData<float> field_of_view;
	InData<Mat4f> world_to_eye;
	InData<Texture2D> diffuse_color_gbuffer;
	InData<Texture2D> specular_color_gbuffer;
	InData<Texture2D> specular_level_gbuffer;
	InData<Texture2D> self_illumination_gbuffer;
	InData<Texture2D> normal_z_gbuffer;
	InData<Texture2DArray> shadow_maps;

	OutData<Texture2D> final_color;

private:
	void find_lights(GraphicContext &gc, Scene_Impl *scene);
	void upload(GraphicContext &gc);
	void render(GraphicContext &gc, GPUTimer &timer);
	void update_buffers(GraphicContext &gc);
	ProgramObject compile_and_link(GraphicContext &gc, const std::string &compute_filename, const std::string &defines = std::string());

	// SceneLightVisitor
	void light(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneLight_Impl *light);

	static const int max_lights = 1023;
	static const int light_slots_per_tile = 128;

	struct GPULight
	{
		Vec4f position;
		Vec4f color;
		Vec4f range; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), hotspot
		Vec4f spot_x;
		Vec4f spot_y;
		Vec4f spot_z;
	};

	struct Uniforms
	{
		float rcp_f;
		float rcp_f_div_aspect;
		Vec2f two_rcp_viewport_size;
		unsigned int num_lights;
		unsigned int num_tiles_x;
		unsigned int num_tiles_y;
		unsigned int padding;
	};

	UniformVector<Uniforms> compute_uniforms;
	StorageVector<GPULight> compute_lights;
	TransferVector<GPULight> transfer_lights;
	StorageVector<unsigned int> compute_visible_lights;

	ProgramObject cull_tiles_program;
	ProgramObject render_tiles_program;

	std::vector<SceneLight_Impl *> lights;

	int tile_size;
	int num_tiles_x;
	int num_tiles_y;

	ZMinMax zminmax;
	InData<Texture2D> zminmax_result;
};

}

