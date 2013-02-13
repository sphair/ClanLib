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
#include "Scene3D/scene_light.h"
namespace clan
{

class GPUTimer;

class LightsourcePass : SceneLightVisitor
{
public:
	LightsourcePass(clan::GraphicContext &gc);
	~LightsourcePass();

	void run(clan::GraphicContext &gc, Scene &scene);

	InData<clan::Rect> viewport;
	InData<float> field_of_view;
	InData<clan::Mat4f> world_to_eye;
	InData<clan::Texture2D> diffuse_color_gbuffer;
	InData<clan::Texture2D> specular_color_gbuffer;
	InData<clan::Texture2D> specular_level_gbuffer;
	InData<clan::Texture2D> self_illumination_gbuffer;
	InData<clan::Texture2D> normal_z_gbuffer;
	InData<clan::Texture2DArray> shadow_maps;

	OutData<clan::Texture2D> final_color;

private:
	void find_lights(clan::GraphicContext &gc, Scene &scene);
	void upload(clan::GraphicContext &gc);
	void render(clan::GraphicContext &gc, GPUTimer &timer);
	void update_buffers(clan::GraphicContext &gc);
	clan::ProgramObject compile_and_link(clan::GraphicContext &gc, const std::string &compute_filename, const std::string &defines = std::string());

	// SceneLightVisitor
	void light(clan::GraphicContext &gc, const clan::Mat4f &world_to_eye, const clan::Mat4f &eye_to_projection, SceneLight_Impl *light);

	static const int max_lights = 1023;
	static const int light_slots_per_tile = 128;

	struct GPULight
	{
		clan::Vec4f position;
		clan::Vec4f color;
		clan::Vec4f range; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), hotspot
		clan::Vec4f spot_x;
		clan::Vec4f spot_y;
		clan::Vec4f spot_z;
	};

	struct Uniforms
	{
		float rcp_f;
		float rcp_f_div_aspect;
		clan::Vec2f two_rcp_viewport_size;
		unsigned int num_lights;
		unsigned int num_tiles_x;
		unsigned int num_tiles_y;
		unsigned int padding;
	};

	clan::UniformVector<Uniforms> compute_uniforms;
	clan::StorageVector<GPULight> compute_lights;
	clan::TransferVector<GPULight> transfer_lights;
	clan::StorageVector<unsigned int> compute_visible_lights;

	clan::ProgramObject cull_tiles_program;
	clan::ProgramObject render_tiles_program;

	std::vector<SceneLight_Impl *> lights;

	int tile_size;
	int num_tiles_x;
	int num_tiles_y;

	ZMinMax zminmax;
	InData<clan::Texture2D> zminmax_result;
};

}

