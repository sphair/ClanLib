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
#include "Scene3D/Model/model_mesh_visitor.h"

namespace clan
{

class Scene_Impl;

class GBufferPass : ModelMeshVisitor
{
public:
	GBufferPass(ResourceContainer &inout);
	void run(GraphicContext &gc, Scene_Impl *scene);

private:
	void setup_gbuffer(GraphicContext &gc);
	void render(GraphicContext &gc, ModelLOD *model_lod, int num_instances);

	// In:
	Resource<Rect> viewport;
	Resource<float> field_of_view;
	Resource<Mat4f> world_to_eye;

	// Out:
	Resource<Texture2D> diffuse_color_gbuffer;
	Resource<Texture2D> specular_color_gbuffer;
	Resource<Texture2D> specular_level_gbuffer;
	Resource<Texture2D> self_illumination_gbuffer;
	Resource<Texture2D> normal_z_gbuffer;
	Resource<Texture2D> zbuffer;

	FrameBuffer fb_gbuffer;
	GraphicContext gc;
	BlendState blend_state;
	BlendState early_z_blend_state;
	DepthStencilState depth_stencil_state;

	struct RenderEntry
	{
		RenderEntry(ModelLOD *model_lod, int num_instances) : model_lod(model_lod), num_instances(num_instances) { }

		ModelLOD *model_lod;
		int num_instances;
	};
	std::vector<RenderEntry> render_list;
};

}

