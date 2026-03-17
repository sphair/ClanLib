/*
**  ClanLib SDK
**  Copyright (c) 1997-2026 The ClanLib Team
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
**	claim that you wrote the original software. If you use this software
**	in a product, an acknowledgment in the product documentation would be
**	appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**	misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**	Magnus Norddahl
**	Mark Page
*/

#include "VK/precomp.h"
#include "VK/VK1/vulkan_standard_programs.h"
#include "VK/VK1/vulkan_program_object_provider.h"
#include "VK/VK1/vulkan_shader_object_provider.h"
#include "VK/vulkan_device.h"

namespace clan
{

// ===========================================================================
// color_only  – Position + Color passthrough
// ===========================================================================

const std::string VulkanStandardPrograms::color_only_vert = R"glsl(
#version 450

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color0;

layout(location = 0) out vec4 vColor;

void main()
{
	gl_Position = Position;
	vColor	= Color0;
}
)glsl";

const std::string VulkanStandardPrograms::color_only_frag = R"glsl(
#version 450

layout(location = 0) in  vec4 vColor;
layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = vColor;
}
)glsl";

// ===========================================================================
// single_texture  – Position + Color + TexCoord0 + 1 sampler
// ===========================================================================

const std::string VulkanStandardPrograms::single_texture_vert = R"glsl(
#version 450

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color0;
layout(location = 2) in vec2 TexCoord0;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;

void main()
{
	gl_Position = Position;
	vColor	= Color0;
	vTexCoord   = TexCoord0;
}
)glsl";

const std::string VulkanStandardPrograms::single_texture_frag = R"glsl(
#version 450

layout(location = 0) in  vec4 vColor;
layout(location = 1) in  vec2 vTexCoord;
layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform sampler2D Texture0;

void main()
{
	fragColor = vColor * texture(Texture0, vTexCoord);
}
)glsl";

// ===========================================================================
// sprite  – Position + Color + TexCoord0 + TexIndex0 + 16 samplers
//		(switch-based multi-texture, matching original gl3 program)
// ===========================================================================

const std::string VulkanStandardPrograms::sprite_vert = R"glsl(
#version 450

layout(location = 0) in vec4  Position;
layout(location = 1) in vec4  Color0;
layout(location = 2) in vec2  TexCoord0;
layout(location = 3) in int   TexIndex0;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;
layout(location = 2) flat out int vTexIndex;

void main()
{
	gl_Position = Position;
	vColor	= Color0;
	vTexCoord   = TexCoord0;
	vTexIndex   = TexIndex0;
}
)glsl";

const std::string VulkanStandardPrograms::sprite_frag = R"glsl(
#version 450

layout(location = 0) in  vec4 vColor;
layout(location = 1) in  vec2 vTexCoord;
layout(location = 2) flat in int vTexIndex;
layout(location = 0) out vec4 fragColor;

// One sampler per binding slot, matching the descriptor layout which creates
// MAX_TEXTURES individual COMBINED_IMAGE_SAMPLER bindings at binding 0..31.
// A Textures[16] array at binding=0 would require descriptorCount=16 in the
// layout and is incompatible with individual per-slot bindings, causing
// VK_ERROR_DEVICE_LOST on submit.
layout(set = 0, binding =  0) uniform sampler2D Texture0;
layout(set = 0, binding =  1) uniform sampler2D Texture1;
layout(set = 0, binding =  2) uniform sampler2D Texture2;
layout(set = 0, binding =  3) uniform sampler2D Texture3;
layout(set = 0, binding =  4) uniform sampler2D Texture4;
layout(set = 0, binding =  5) uniform sampler2D Texture5;
layout(set = 0, binding =  6) uniform sampler2D Texture6;
layout(set = 0, binding =  7) uniform sampler2D Texture7;
layout(set = 0, binding =  8) uniform sampler2D Texture8;
layout(set = 0, binding =  9) uniform sampler2D Texture9;
layout(set = 0, binding = 10) uniform sampler2D Texture10;
layout(set = 0, binding = 11) uniform sampler2D Texture11;
layout(set = 0, binding = 12) uniform sampler2D Texture12;
layout(set = 0, binding = 13) uniform sampler2D Texture13;
layout(set = 0, binding = 14) uniform sampler2D Texture14;
layout(set = 0, binding = 15) uniform sampler2D Texture15;

void main()
{
	vec4 texel;
	switch (vTexIndex)
	{
	case  0: texel = texture(Texture0,  vTexCoord); break;
	case  1: texel = texture(Texture1,  vTexCoord); break;
	case  2: texel = texture(Texture2,  vTexCoord); break;
	case  3: texel = texture(Texture3,  vTexCoord); break;
	case  4: texel = texture(Texture4,  vTexCoord); break;
	case  5: texel = texture(Texture5,  vTexCoord); break;
	case  6: texel = texture(Texture6,  vTexCoord); break;
	case  7: texel = texture(Texture7,  vTexCoord); break;
	case  8: texel = texture(Texture8,  vTexCoord); break;
	case  9: texel = texture(Texture9,  vTexCoord); break;
	case 10: texel = texture(Texture10, vTexCoord); break;
	case 11: texel = texture(Texture11, vTexCoord); break;
	case 12: texel = texture(Texture12, vTexCoord); break;
	case 13: texel = texture(Texture13, vTexCoord); break;
	case 14: texel = texture(Texture14, vTexCoord); break;
	case 15: texel = texture(Texture15, vTexCoord); break;
	default: texel = vec4(1.0, 1.0, 1.0, 1.0); // white = no texture (matches GL3 behaviour, index >= 16 means colour-only)
	}
	fragColor = vColor * texel;
}
)glsl";

// ===========================================================================
// path  – ivec4 Vertex + complex brush/gradient/image fill via texelFetch
//		Matches the gl3 path program semantics.
//
// Push constants carry: mat4 modelview, vec4 transform_origin, vec2 pixel_size
// instance_data texture (binding=1) carries per-instance brush parameters.
// ===========================================================================

const std::string VulkanStandardPrograms::path_vert = R"glsl(
#version 450

layout(location = 0) in ivec4 Vertex;

layout(push_constant) uniform PushConstants
{
	mat4  modelview;
	vec4  transform_origin;
	vec2  pixel_size;
} pc;

layout(location = 0) out vec2 vPosition;
layout(location = 1) flat out int vInstanceIndex;

void main()
{
	int instance_index = Vertex.z;
	vInstanceIndex	= instance_index;

	vec2 pos = vec2(Vertex.xy) * pc.pixel_size + pc.transform_origin.xy;
	vPosition = pos;

	vec4 clip = pc.modelview * vec4(pos, 0.0, 1.0);
	clip.y	= -clip.y;   // Vulkan y-flip
	gl_Position = clip;
}
)glsl";

const std::string VulkanStandardPrograms::path_frag = R"glsl(
#version 450

layout(location = 0) in  vec2 vPosition;
layout(location = 1) flat in int vInstanceIndex;
layout(location = 0) out vec4 fragColor;

// instance_data: isampler2D carrying brush/fill parameters per instance
layout(set = 0, binding = 1) uniform isampler2D instance_data;

// Optional gradient / image textures
layout(set = 0, binding = 2) uniform sampler2D image_texture;

// Constants matching the GL3 path program defines
const int fill_solid	= 0;
const int fill_linear	= 1;
const int fill_radial	= 2;
const int fill_image	= 3;

void main()
{
	// Fetch per-instance data (same layout as GL3 path program)
	ivec4 data0 = texelFetch(instance_data, ivec2(0, vInstanceIndex), 0);
	ivec4 data1 = texelFetch(instance_data, ivec2(1, vInstanceIndex), 0);
	ivec4 data2 = texelFetch(instance_data, ivec2(2, vInstanceIndex), 0);

	int   fill_type  = data0.x;
	vec4  color	= vec4(data0) / 255.0;  // packed RGBA8 colour

	vec2  gradient_p0 = vec2(data1.xy) / 65536.0;
	vec2  gradient_p1 = vec2(data1.zw) / 65536.0;

	vec4 result = vec4(0.0);

	if (fill_type == fill_solid)
	{
		result = color;
	}
	else if (fill_type == fill_linear)
	{
		vec2  dir	= gradient_p1 - gradient_p0;
		float len_sq  = dot(dir, dir);
		float t	= (len_sq > 0.0) ? clamp(dot(vPosition - gradient_p0, dir) / len_sq, 0.0, 1.0) : 0.0;
		ivec4 col0	= texelFetch(instance_data, ivec2(2, vInstanceIndex), 0);
		ivec4 col1	= texelFetch(instance_data, ivec2(3, vInstanceIndex), 0);
		result = mix(vec4(col0) / 255.0, vec4(col1) / 255.0, t);
	}
	else if (fill_type == fill_radial)
	{
		float radius  = float(data1.z) / 65536.0;
		float d	= distance(vPosition, gradient_p0);
		float t	= clamp(d / max(radius, 0.0001), 0.0, 1.0);
		ivec4 col0	= texelFetch(instance_data, ivec2(2, vInstanceIndex), 0);
		ivec4 col1	= texelFetch(instance_data, ivec2(3, vInstanceIndex), 0);
		result = mix(vec4(col0) / 255.0, vec4(col1) / 255.0, t);
	}
	else if (fill_type == fill_image)
	{
		vec2 uv = vPosition;
		result  = texture(image_texture, uv) * color;
	}
	else
	{
		result = vec4(1.0, 0.0, 1.0, 1.0); // magenta for unknown fill type
	}

	fragColor = result;
}
)glsl";

// ===========================================================================
// Factory
// ===========================================================================

void VulkanStandardPrograms::create(VulkanDevice *device,
									VulkanProgramObjectProvider *&out_color_only,
									VulkanProgramObjectProvider *&out_single_texture,
									VulkanProgramObjectProvider *&out_sprite,
									VulkanProgramObjectProvider *&out_path)
{
	auto make_program = [&](const std::string &vert_src,
							const std::string &frag_src) -> VulkanProgramObjectProvider *
	{
		auto *vert = new VulkanShaderObjectProvider(device);
		vert->create(ShaderType::vertex, vert_src);
		vert->compile();
		if (!vert->get_compile_status())
			throw Exception("VulkanStandardPrograms: vertex shader compile failed:\n" + vert->get_info_log());

		auto *frag = new VulkanShaderObjectProvider(device);
		frag->create(ShaderType::fragment, frag_src);
		frag->compile();
		if (!frag->get_compile_status())
			throw Exception("VulkanStandardPrograms: fragment shader compile failed:\n" + frag->get_info_log());

		auto *prog = new VulkanProgramObjectProvider(device);
		prog->attach_shader(vert);
		prog->attach_shader(frag);
		prog->link();
		if (!prog->get_link_status())
			throw Exception("VulkanStandardPrograms: program link failed:\n" + prog->get_info_log());
		return prog;
	};

	out_color_only	= make_program(color_only_vert,	color_only_frag);
	out_single_texture  = make_program(single_texture_vert,  single_texture_frag);
	out_sprite		= make_program(sprite_vert,		sprite_frag);
	out_path			= make_program(path_vert,			path_frag);
}

} // namespace clan
