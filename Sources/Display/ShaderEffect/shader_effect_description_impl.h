/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#pragma once

#include "API/Display/ShaderEffect/shader_effect_description.h"
#include <map>

namespace clan
{
	class ShaderEffectDescription_Impl
	{
	public:
		ShaderEffectDescription_Impl()
			: glsl_version(0), elements_type(), draw_count(0)
		{
		}

		int glsl_version;

		std::map<std::string, std::string> defines;

		std::string vertex_shader_code[num_shader_languages];
		std::string fragment_shader_code[num_shader_languages];
		std::string compute_shader_code[num_shader_languages];

		RasterizerStateDescription rasterizer_state;
		BlendStateDescription blend_state;
		DepthStencilStateDescription depth_stencil_state;

		struct FragDataDescription
		{
			FragDataDescription() : use_back_buffer(false) { }

			RenderBuffer buffer;
			Texture texture;
			bool use_back_buffer;
		};

		std::map<std::string, FragDataDescription> frag_data;

		RenderBuffer depth_buffer;
		Texture depth_texture;

		RenderBuffer stencil_buffer;
		Texture stencil_texture;

		std::map<std::string, Resource<UniformBuffer >> uniform_buffers;
		std::map<std::string, Resource<Texture> > textures;
		std::map<std::string, Resource<Texture> > images;
		std::map<std::string, Resource<StorageBuffer> > storage_buffers;

		ElementArrayBuffer elements;
		VertexAttributeDataType elements_type;

		enum VertexAttributeType
		{
			attribute_type_buffer,
			attribute_type_screen_quad,
			attribute_type_uv_quad
		};

		struct VertexAttributeDescription
		{
			VertexAttributeDescription() : attribute_type(attribute_type_buffer), size(0), type(), offset(0), stride(0), normalize(false) { }

			VertexAttributeType attribute_type;
			VertexArrayBuffer buffer;
			int size;
			VertexAttributeDataType type;
			size_t offset;
			int stride;
			bool normalize;
		};
		std::map<std::string, VertexAttributeDescription> attributes;

		int draw_count;
	};
}
