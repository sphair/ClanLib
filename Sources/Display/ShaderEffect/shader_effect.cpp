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
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/rasterizer_state.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/depth_stencil_state.h"
#include "API/Display/Render/vertex_array_buffer.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Render/element_array_buffer.h"
#include "API/Display/Render/uniform_buffer.h"
#include "API/Display/Render/storage_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Text/string_format.h"
#include "shader_effect_description_impl.h"
#include <map>

namespace clan
{

class ShaderEffect_Impl
{
public:
	ShaderEffect_Impl() : elements_type(), num_vertices(0) { }

	static std::string defines_prefix(GraphicContext &gc, std::vector<const std::string> &defines, int glsl_shader_version = 330);

	ProgramObject program;

	FrameBuffer fb;

	RasterizerState rasterizer_state;
	BlendState blend_state;
	DepthStencilState depth_stencil_state;

	std::vector<VertexArrayBuffer> attributes;
	PrimitivesArray prim_array;

	ElementArrayBuffer elements;
	VertexAttributeDataType elements_type;

	std::map<int, UniformBuffer> uniform_bindings;

	std::map<int, StorageBuffer> storage_bindings;
	std::map<int, Texture> image_bindings;

	std::map<int, Texture> texture_bindings;

	int num_vertices;
};

ShaderEffect::ShaderEffect()
{
}

ShaderEffect::ShaderEffect(GraphicContext &gc, const ShaderEffectDescription &description)
: impl(new ShaderEffect_Impl)
{
}

bool ShaderEffect::is_null() const
{
	return !impl;
}

void ShaderEffect::dispatch(GraphicContext &gc, int x, int y, int z)
{
	gc.set_program_object(impl->program);

	for (auto it = impl->uniform_bindings.begin(); it != impl->uniform_bindings.end(); ++it)
	{
		gc.set_uniform_buffer(it->first, it->second);
	}

	for (auto it = impl->storage_bindings.begin(); it != impl->storage_bindings.end(); ++it)
	{
		gc.set_storage_buffer(it->first, it->second);
	}

	for (auto it = impl->image_bindings.begin(); it != impl->image_bindings.end(); ++it)
	{
		gc.set_image_texture(it->first, it->second);
	}

	for (auto it = impl->texture_bindings.begin(); it != impl->texture_bindings.end(); ++it)
	{
		gc.set_texture(it->first, it->second);
	}

	gc.dispatch(x, y, z);

	for (auto it = impl->uniform_bindings.begin(); it != impl->uniform_bindings.end(); ++it)
	{
		gc.reset_uniform_buffer(it->first);
	}

	for (auto it = impl->storage_bindings.begin(); it != impl->storage_bindings.end(); ++it)
	{
		gc.reset_storage_buffer(it->first);
	}

	for (auto it = impl->image_bindings.begin(); it != impl->image_bindings.end(); ++it)
	{
		gc.reset_image_texture(it->first);
	}

	for (auto it = impl->texture_bindings.begin(); it != impl->texture_bindings.end(); ++it)
	{
		gc.reset_texture(it->first);
	}

	gc.reset_program_object();
}

void ShaderEffect::draw(GraphicContext &gc)
{
	gc.set_program_object(impl->program);

	if (!impl->fb.is_null())
		gc.set_frame_buffer(impl->fb);

	if (!impl->rasterizer_state.is_null())
		gc.set_rasterizer_state(impl->rasterizer_state);

	if (!impl->blend_state.is_null())
		gc.set_blend_state(impl->blend_state);

	if (!impl->depth_stencil_state.is_null())
		gc.set_depth_stencil_state(impl->depth_stencil_state);

	for (auto it = impl->uniform_bindings.begin(); it != impl->uniform_bindings.end(); ++it)
	{
		gc.set_uniform_buffer(it->first, it->second);
	}

	for (auto it = impl->storage_bindings.begin(); it != impl->storage_bindings.end(); ++it)
	{
		gc.set_storage_buffer(it->first, it->second);
	}

	for (auto it = impl->image_bindings.begin(); it != impl->image_bindings.end(); ++it)
	{
		gc.set_image_texture(it->first, it->second);
	}

	for (auto it = impl->texture_bindings.begin(); it != impl->texture_bindings.end(); ++it)
	{
		gc.set_texture(it->first, it->second);
	}

	if (!impl->elements.is_null())
	{
		gc.set_primitives_elements(impl->elements);
		gc.set_primitives_array(impl->prim_array);
		gc.draw_primitives_elements(type_triangles, impl->num_vertices, impl->elements_type);
		gc.reset_primitives_array();
		gc.reset_primitives_elements();
	}
	else
	{
		gc.set_primitives_array(impl->prim_array);
		gc.draw_primitives_array(type_triangles, 0, impl->num_vertices);
		gc.reset_primitives_array();
	}

	for (auto it = impl->uniform_bindings.begin(); it != impl->uniform_bindings.end(); ++it)
	{
		gc.reset_uniform_buffer(it->first);
	}

	for (auto it = impl->storage_bindings.begin(); it != impl->storage_bindings.end(); ++it)
	{
		gc.reset_storage_buffer(it->first);
	}

	for (auto it = impl->image_bindings.begin(); it != impl->image_bindings.end(); ++it)
	{
		gc.reset_image_texture(it->first);
	}

	for (auto it = impl->texture_bindings.begin(); it != impl->texture_bindings.end(); ++it)
	{
		gc.reset_texture(it->first);
	}

	if (!impl->rasterizer_state.is_null())
		gc.reset_rasterizer_state();

	if (!impl->blend_state.is_null())
		gc.reset_blend_state();

	if (!impl->depth_stencil_state.is_null())
		gc.reset_depth_stencil_state();

	if (!impl->fb.is_null())
		gc.reset_frame_buffer();

	gc.reset_program_object();
}

/////////////////////////////////////////////////////////////////////////////

std::string ShaderEffect_Impl::defines_prefix(GraphicContext &gc, std::vector<const std::string> &defines, int glsl_shader_version)
{
	std::string prefix;
	if (gc.get_shader_language() == shader_glsl)
		prefix += string_format("#version %1\r\n", glsl_shader_version);
	for (size_t i = 0; i < defines.size(); i++)
		prefix += string_format("#define %1\r\n", defines[i]);
	prefix += "#line 0\r\n";
	return prefix;
}

}
