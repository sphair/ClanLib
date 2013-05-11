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

#include "API/Scene3D/scene_particle_emitter.h"
#include "API/Scene3D/scene_inout_data.h"
#include "particle_emitter_pass_data.h"
#include "Scene3D/scene_particle_emitter_impl.h"

namespace clan
{

class Scene_Impl;
class MaterialCache;

class ParticleEmitterPass : SceneParticleEmitterVisitor
{
public:
	ParticleEmitterPass(MaterialCache &texture_cache, const std::string &shader_path, SceneInOutDataContainer &inout);
	void run(GraphicContext &gc, Scene_Impl *scene);
	void update(GraphicContext &gc, float time_elapsed);

private:
	void setup(GraphicContext &gc);
	void emitter(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneParticleEmitter_Impl *emitter);

	// In:
	SceneInOutData<Rect> viewport;
	SceneInOutData<float> field_of_view;
	SceneInOutData<Mat4f> world_to_eye;
	SceneInOutData<Texture2D> zbuffer;
	SceneInOutData<Texture2D> normal_z_gbuffer;

	// InOut:
	SceneInOutData<Texture2D> final_color;

	std::string shader_path;

	MaterialCache &texture_cache;
	FrameBuffer fb;
	BlendState blend_state;
	DepthStencilState depth_stencil_state;
	RasterizerState rasterizer_state;
	ProgramObject program;

	PrimitivesArray prim_array;
	VertexArrayVector<Vec3f> billboard_positions;
	static Vec3f cpu_billboard_positions[6];

	Texture2D instance_texture;
	TransferTexture instance_transfer;

	std::vector< std::shared_ptr<ParticleEmitterPassData> > active_emitters;

	struct ParticleOrderIndex
	{
		ParticleOrderIndex(int index, float sqr_distance) : index(index), sqr_distance(sqr_distance) { }

		bool operator<(const ParticleOrderIndex &that) const
		{
			return sqr_distance > that.sqr_distance;
		}

		int index;
		float sqr_distance;
	};
};

}

