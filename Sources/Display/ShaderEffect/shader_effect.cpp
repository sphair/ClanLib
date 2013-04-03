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
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Display/ShaderEffect/shader_effect.h"
#include "API/Display/ShaderEffect/shader_effect_description.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/rasterizer_state.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/depth_stencil_state.h"
#include "API/Display/Render/primitives_array.h"
#include <map>

namespace clan
{

class ShaderEffect_Impl
{
public:
	ShaderEffect_Impl() : num_vertices(0) { }

	FrameBuffer fb;

	RasterizerState rasterizer_state;
	BlendState blend_state;
	DepthStencilState depth_stencil_state;

	std::vector<VertexArrayBuffer> attributes;
	ElementArrayBuffer elements;

	PrimitivesArray prim_array;

	std::map<int, UniformBuffer> uniform_bindings;

	std::map<int, StorageBuffer> storage_bindings;
	std::map<int, Texture> image_bindings;

	std::map<int, Texture> texture_bindings;
	std::map<int, Texture> sampler_bindings;

	int num_vertices;
};

ShaderEffect::ShaderEffect()
{
}

ShaderEffect::ShaderEffect(GraphicContext &gc, const ShaderEffectDescription &description)
: impl(new ShaderEffect_Impl)
{
}

void ShaderEffect::dispatch(GraphicContext &gc, int x, int y, int z)
{
}
	
void ShaderEffect::draw(GraphicContext &gc)
{
}

}
