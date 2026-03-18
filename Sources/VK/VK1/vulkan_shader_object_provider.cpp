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
#include "VK/VK1/vulkan_shader_object_provider.h"
#include "VK/vulkan_device.h"

// glslang public API
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>

namespace clan
{

// ---------------------------------------------------------------------------
// Process-lifetime glslang initialisation
// ---------------------------------------------------------------------------

namespace {

void glslang_finalise() { glslang::FinalizeProcess(); }

void ensure_glslang_initialised()
{
	static bool done = false;
	if (!done)
	{
		if (!glslang::InitializeProcess())
			throw Exception("glslang::InitializeProcess() failed");
		std::atexit(glslang_finalise);
		done = true;
	}
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

VulkanShaderObjectProvider::VulkanShaderObjectProvider(VulkanDevice *device)
	: vk_device(device)
{
	ensure_glslang_initialised();
}

VulkanShaderObjectProvider::~VulkanShaderObjectProvider()
{
	if (shader_module != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(vk_device->get_device(), shader_module, nullptr);
		shader_module = VK_NULL_HANDLE;
	}
}

// ---------------------------------------------------------------------------
// create() overloads  --  equivalent to glShaderSource
// ---------------------------------------------------------------------------

void VulkanShaderObjectProvider::create(ShaderType type, const std::string &source)
{
	shader_type	= type;
	stage_flags	= to_stage(type);
	glsl_source	= source;
	compile_status = false;
	info_log.clear();
}

void VulkanShaderObjectProvider::create(ShaderType type,
										const std::vector<std::string> &sources)
{
	std::string combined;
	for (const auto &s : sources) combined += s + "\n";
	create(type, combined);
}

// Binary/SPIR-V overload: treat as raw SPIR-V if size is a multiple of 4,
// otherwise treat as a GLSL string.
void VulkanShaderObjectProvider::create(ShaderType type,
										const void *source, int source_size)
{
	if (source_size > 0 && (source_size % 4) == 0)
	{
		load_spirv(type,
				reinterpret_cast<const uint32_t *>(source),
				static_cast<size_t>(source_size) / sizeof(uint32_t));
	}
	else
	{
		create(type, std::string(static_cast<const char *>(source),
								static_cast<size_t>(source_size)));
	}
}

// ---------------------------------------------------------------------------
// compile()  --  equivalent to glCompileShader
// ---------------------------------------------------------------------------

void VulkanShaderObjectProvider::compile()
{
	// Destroy any previously compiled module.
	if (shader_module != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(vk_device->get_device(), shader_module, nullptr);
		shader_module = VK_NULL_HANDLE;
	}

	// Compile GLSL -> SPIR-V using glslang.
	std::vector<uint32_t> spirv;
	try
	{
		spirv = compile_glsl_to_spirv(glsl_source, shader_type);
	}
	catch (const std::exception &e)
	{
		info_log	= e.what();
		compile_status = false;
		return;
	}

	// Hand SPIR-V to Vulkan.
	VkShaderModuleCreateInfo ci{};
	ci.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ci.codeSize = spirv.size() * sizeof(uint32_t);
	ci.pCode	= spirv.data();

	if (vkCreateShaderModule(vk_device->get_device(), &ci, nullptr, &shader_module)
			!= VK_SUCCESS)
	{
		info_log	= "vkCreateShaderModule failed";
		compile_status = false;
		return;
	}

	info_log.clear();
	compile_status = true;
}

// ---------------------------------------------------------------------------
// load_spirv()  --  bypass glslang for pre-compiled modules
// ---------------------------------------------------------------------------

void VulkanShaderObjectProvider::load_spirv(ShaderType type,
											const uint32_t *spirv,
											size_t word_count)
{
	shader_type = type;
	stage_flags = to_stage(type);

	if (shader_module != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(vk_device->get_device(), shader_module, nullptr);
		shader_module = VK_NULL_HANDLE;
	}

	VkShaderModuleCreateInfo ci{};
	ci.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ci.codeSize = word_count * sizeof(uint32_t);
	ci.pCode	= spirv;

	if (vkCreateShaderModule(vk_device->get_device(), &ci, nullptr, &shader_module)
			!= VK_SUCCESS)
		throw Exception("vkCreateShaderModule failed for pre-compiled SPIR-V");

	info_log.clear();
	compile_status = true;
}

// ---------------------------------------------------------------------------
// compile_glsl_to_spirv()  --  glslang GLSL -> SPIR-V
// ---------------------------------------------------------------------------

std::vector<uint32_t> VulkanShaderObjectProvider::compile_glsl_to_spirv(
	const std::string &source, ShaderType type) const
{
	const EShLanguage stage = static_cast<EShLanguage>(to_esh_language(type));

	glslang::TShader shader(stage);

	const char *src_ptr  = source.c_str();
	const int   src_len  = static_cast<int>(source.size());
	shader.setStringsWithLengths(&src_ptr, &src_len, 1);

	// Target Vulkan 1.2 / SPIR-V 1.5
	shader.setEnvInput(glslang::EShSourceGlsl, stage,
					glslang::EShClientVulkan, 120 /*clientVersion*/);
	shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
	shader.setEnvTarget(glslang::EShTargetSpv,	glslang::EShTargetSpv_1_5);

	// Use the built-in default resource limits provided by
	// glslang-default-resource-limits.lib (GetDefaultResources()).
	const TBuiltInResource &resources = *GetDefaultResources();

	constexpr EShMessages messages =
		static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

	if (!shader.parse(&resources, 450 /*defaultVersion*/, false, messages))
	{
		std::string log = shader.getInfoLog();
		if (const char *dbg = shader.getInfoDebugLog(); dbg && dbg[0])
			log += "\n[debug]\n" + std::string(dbg);
		throw Exception("glslang parse failed:\n" + log);
	}

	glslang::TProgram program;
	program.addShader(&shader);

	if (!program.link(messages))
	{
		std::string log = program.getInfoLog();
		if (const char *dbg = program.getInfoDebugLog(); dbg && dbg[0])
			log += "\n[debug]\n" + std::string(dbg);
		throw Exception("glslang link failed:\n" + log);
	}

	std::vector<uint32_t> spirv;
	spv::SpvBuildLogger   logger;
	glslang::SpvOptions   spv_opts;
	spv_opts.validate	= true;
	spv_opts.optimizeSize   = false;
	spv_opts.disassemble	= false;

	glslang::GlslangToSpv(*program.getIntermediate(stage),
						spirv, &logger, &spv_opts);

	// Surface any SPIR-V-level warnings/errors.
	const std::string spv_log = logger.getAllMessages();
	if (!spv_log.empty())
	{
		// Non-fatal: log but don't throw (warnings may appear for valid shaders).
		// If spirv is empty the caller will catch the resulting vkCreateShaderModule failure.
		log_event("vulkan-shader", spv_log);
	}

	if (spirv.empty())
		throw Exception("glslang produced empty SPIR-V");

	return spirv;
}

// ---------------------------------------------------------------------------
// Static helpers
// ---------------------------------------------------------------------------

VkShaderStageFlagBits VulkanShaderObjectProvider::to_stage(ShaderType type)
{
	switch (type)
	{
	case ShaderType::vertex:		return VK_SHADER_STAGE_VERTEX_BIT;
	case ShaderType::fragment:		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case ShaderType::geometry:		return VK_SHADER_STAGE_GEOMETRY_BIT;
	case ShaderType::tess_control:	return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case ShaderType::tess_evaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	case ShaderType::compute:		return VK_SHADER_STAGE_COMPUTE_BIT;
	default:
		throw Exception("Unknown ShaderType");
	}
}

int VulkanShaderObjectProvider::to_esh_language(ShaderType type)
{
	switch (type)
	{
	case ShaderType::vertex:		return EShLangVertex;
	case ShaderType::fragment:		return EShLangFragment;
	case ShaderType::geometry:		return EShLangGeometry;
	case ShaderType::tess_control:	return EShLangTessControl;
	case ShaderType::tess_evaluation: return EShLangTessEvaluation;
	case ShaderType::compute:		return EShLangCompute;
	default:
		throw Exception("Unknown ShaderType");
	}
}

} // namespace clan
