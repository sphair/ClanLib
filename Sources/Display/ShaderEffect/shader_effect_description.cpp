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
#include "API/Display/ShaderEffect/shader_effect_description.h"
#include "shader_effect_description_impl.h"

namespace clan
{

ShaderEffectDescription::ShaderEffectDescription()
	: impl(new ShaderEffectDescription_Impl())
{
}

void ShaderEffectDescription::set_glsl_version(int version)
{
	impl->glsl_version = version;
}

void ShaderEffectDescription::set_define(std::string name, std::string value)
{
	impl->defines[name] = value;
}

void ShaderEffectDescription::set_vertex_shader(std::string code, ShaderLanguage language)
{
	impl->vertex_shader_code = code;
	impl->vertex_shader_language = language;
}

void ShaderEffectDescription::set_fragment_shader(std::string code, ShaderLanguage language)
{
	impl->fragment_shader_code = code;
	impl->fragment_shader_language = language;
}

void ShaderEffectDescription::set_compute_shader(std::string code, ShaderLanguage language)
{
	impl->compute_shader_code = code;
	impl->compute_shader_language = language;
}

RasterizerStateDescription &ShaderEffectDescription::rasterizer()
{
	return impl->rasterizer_state;
}

BlendStateDescription &ShaderEffectDescription::blend()
{
	return impl->blend_state;
}

DepthStencilStateDescription &ShaderEffectDescription::depth_stencil()
{
	return impl->depth_stencil_state;
}

void ShaderEffectDescription::set_frag_data_back_buffer(std::string name)
{
}

void ShaderEffectDescription::set_frag_data(std::string name, RenderBuffer buffer)
{
}

void ShaderEffectDescription::set_frag_data(std::string name, Texture texture)
{
}

void ShaderEffectDescription::set_depth_data(TextureFormat format)
{
}

void ShaderEffectDescription::set_depth_data(RenderBuffer buffer)
{
}

void ShaderEffectDescription::set_depth_data(Texture texture)
{
}

void ShaderEffectDescription::set_stencil_data(TextureFormat format)
{
}

void ShaderEffectDescription::set_stencil_data(RenderBuffer buffer)
{
}

void ShaderEffectDescription::set_stencil_data(Texture texture)
{
}

void ShaderEffectDescription::set_depth_stencil_data(TextureFormat format)
{
}

void ShaderEffectDescription::set_depth_stencil_data(RenderBuffer buffer)
{
}

void ShaderEffectDescription::set_depth_stencil_data(Texture texture)
{
}

void ShaderEffectDescription::set_texture(std::string name, Texture texture)
{
}

void ShaderEffectDescription::set_storage(std::string name, StorageBuffer values)
{
}

void ShaderEffectDescription::set_empty_storage(std::string name, int size)
{
}

void ShaderEffectDescription::set_uniform_block(std::string name, UniformBuffer values)
{
}

void ShaderEffectDescription::set_attribute_screen_quad(std::string name)
{
}

void ShaderEffectDescription::set_attribute_uv_quad(std::string name, UVQuadType type)
{
}

}
