/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
		: impl(std::make_shared<ShaderEffectDescription_Impl>())
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
		impl->vertex_shader_code[static_cast<int>(language)] = code;
	}

	void ShaderEffectDescription::set_fragment_shader(std::string code, ShaderLanguage language)
	{
		impl->fragment_shader_code[static_cast<int>(language)] = code;
	}

	void ShaderEffectDescription::set_compute_shader(std::string code, ShaderLanguage language)
	{
		impl->compute_shader_code[static_cast<int>(language)] = code;
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

	void ShaderEffectDescription::set_frag_data_to_back_buffer(std::string name)
	{
		ShaderEffectDescription_Impl::FragDataDescription desc;
		desc.use_back_buffer = true;
		impl->frag_data[name] = desc;
	}

	void ShaderEffectDescription::set_frag_data(std::string name, RenderBuffer buffer)
	{
		ShaderEffectDescription_Impl::FragDataDescription desc;
		desc.buffer = buffer;
		impl->frag_data[name] = desc;
	}

	void ShaderEffectDescription::set_frag_data(std::string name, Texture texture)
	{
		ShaderEffectDescription_Impl::FragDataDescription desc;
		desc.texture = texture;
		impl->frag_data[name] = desc;
	}

	void ShaderEffectDescription::set_depth_data(RenderBuffer buffer)
	{
		impl->depth_buffer = buffer;
		impl->depth_texture = Texture();
	}

	void ShaderEffectDescription::set_depth_data(Texture texture)
	{
		impl->depth_buffer = RenderBuffer();
		impl->depth_texture = texture;
	}

	void ShaderEffectDescription::set_stencil_data(RenderBuffer buffer)
	{
		impl->stencil_buffer = buffer;
		impl->stencil_texture = Texture();
	}

	void ShaderEffectDescription::set_stencil_data(Texture texture)
	{
		impl->stencil_buffer = RenderBuffer();
		impl->stencil_texture = texture;
	}

	void ShaderEffectDescription::set_texture(std::string name, Resource<Texture> texture)
	{
		impl->textures[name] = texture;
	}

	void ShaderEffectDescription::set_image(std::string name, Resource<Texture> texture)
	{
		impl->images[name] = texture;
	}

	void ShaderEffectDescription::set_storage(std::string name, Resource<StorageBuffer> values)
	{
		impl->storage_buffers[name] = values;
	}

	void ShaderEffectDescription::set_uniform_block(std::string name, Resource<UniformBuffer> values)
	{
		impl->uniform_buffers[name] = values;
	}

	void ShaderEffectDescription::set_attribute_screen_quad(std::string name)
	{
		ShaderEffectDescription_Impl::VertexAttributeDescription desc;
		desc.attribute_type = ShaderEffectDescription_Impl::attribute_type_screen_quad;
		impl->attributes[name] = desc;
	}

	void ShaderEffectDescription::set_attribute_uv_quad(std::string name)
	{
		ShaderEffectDescription_Impl::VertexAttributeDescription desc;
		desc.attribute_type = ShaderEffectDescription_Impl::attribute_type_uv_quad;
		impl->attributes[name] = desc;
	}

	void ShaderEffectDescription::set_attribute(std::string name, VertexArrayBuffer &buffer, int size, VertexAttributeDataType type, size_t offset, int stride, bool normalize)
	{
		ShaderEffectDescription_Impl::VertexAttributeDescription desc;
		desc.buffer = buffer;
		desc.size = size;
		desc.type = type;
		desc.offset = offset;
		desc.stride = stride;
		desc.normalize = normalize;
		impl->attributes[name] = desc;
	}

	void ShaderEffectDescription::set_elements(ElementArrayBuffer &element_array, VertexAttributeDataType indices_type)
	{
		impl->elements = element_array;
		impl->elements_type = indices_type;
	}

	void ShaderEffectDescription::set_draw_count(int count)
	{
		impl->draw_count = count;
	}
}
