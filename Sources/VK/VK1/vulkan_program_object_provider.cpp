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
#include "VK/VK1/vulkan_program_object_provider.h"
#include "VK/VK1/vulkan_shader_object_provider.h"
#include "VK/VK1/vulkan_shader_object_provider.h"
#include "VK/vulkan_device.h"
#include <cstring>

namespace clan
{
	VulkanProgramObjectProvider::VulkanProgramObjectProvider(VulkanDevice *device)
		: vk_device(device)
	{
		// push_constants is left empty. It is grown on demand by write_push_constant()
		// when set_uniform* is called. has_push_constants() returns false for programs
		// (color_only, single_texture, sprite) whose shaders have no push_constant block,
		// suppressing the unnecessary vkCmdPushConstants call.
	}

	VulkanProgramObjectProvider::~VulkanProgramObjectProvider()
	{
		for (auto *s : raw_shaders)
			delete s;
	}

	void VulkanProgramObjectProvider::attach(const ShaderObject &shader)
	{
		attached_shaders.push_back(shader);
		link_status = false;
	}

	void VulkanProgramObjectProvider::attach_shader(VulkanShaderObjectProvider *provider)
	{
		raw_shaders.push_back(provider);
		link_status = false;
	}

	void VulkanProgramObjectProvider::detach(const ShaderObject &shader)
	{
		attached_shaders.erase(
			std::remove_if(attached_shaders.begin(), attached_shaders.end(),
						[&](const ShaderObject &s) { return s == shader; }),
			attached_shaders.end());
		link_status = false;
	}

	void VulkanProgramObjectProvider::link()
	{
		pipeline_stages.clear();
		entry_point_names.clear();
		info_log.clear();

		// Helper lambda: adds one stage info using a stable pName pointer.
		// The entry point string is pushed into entry_point_names (a deque, so
		// previously pushed elements have stable addresses on further push_back).
		// Only after the string is stored do we take c_str() for pName, so the
		// pointer is always valid for the lifetime of this provider.
		auto add_stage = [&](VkShaderStageFlagBits stage,
							VkShaderModule		mod,
							const char *		entry_point_cstr) -> bool
		{
			if (mod == VK_NULL_HANDLE) return false;
			entry_point_names.emplace_back(entry_point_cstr);
			VkPipelineShaderStageCreateInfo si{};
			si.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			si.stage  = stage;
			si.module = mod;
			si.pName  = entry_point_names.back().c_str();
			pipeline_stages.push_back(si);
			return true;
		};

		for (const auto &shader_obj : attached_shaders)
		{
			auto *provider = static_cast<VulkanShaderObjectProvider *>(
				shader_obj.get_provider());

			if (!provider->get_compile_status())
			{
				info_log	= "Shader not compiled: " + provider->get_info_log();
				link_status = false;
				pipeline_stages.clear();
				entry_point_names.clear();
				return;
			}

			if (!add_stage(provider->get_stage(),
						provider->get_shader_module(),
						provider->get_entry_point()))
			{
				info_log	= "Shader has null VkShaderModule — was compile() called?";
				link_status = false;
				pipeline_stages.clear();
				entry_point_names.clear();
				return;
			}
		}

		for (auto *provider : raw_shaders)
		{
			if (!provider->get_compile_status())
			{
				info_log	= "Shader not compiled: " + provider->get_info_log();
				link_status = false;
				pipeline_stages.clear();
				entry_point_names.clear();
				return;
			}

			if (!add_stage(provider->get_stage(),
						provider->get_shader_module(),
						provider->get_entry_point()))
			{
				info_log	= "Shader has null VkShaderModule — was compile() called?";
				link_status = false;
				pipeline_stages.clear();
				entry_point_names.clear();
				return;
			}
		}

		link_status = true;
	}

	// ---- validate() — no meaningful Vulkan equivalent, always succeeds ----
	void VulkanProgramObjectProvider::validate()
	{
		validate_status = link_status;
	}

	// ---- Attribute / uniform location queries ----

	void VulkanProgramObjectProvider::bind_attribute_location(int index, const std::string &name)
	{
		attribute_location_map[name] = index;
	}

	void VulkanProgramObjectProvider::bind_frag_data_location(int /*color_number*/,
															const std::string & /*name*/)
	{
		// Vulkan determines output locations from the SPIR-V layout(location=N).
		// No runtime binding needed.
	}

	int VulkanProgramObjectProvider::get_attribute_location(const std::string &name) const
	{
		auto it = attribute_location_map.find(name);
		return (it != attribute_location_map.end()) ? it->second : -1;
	}

	int VulkanProgramObjectProvider::get_uniform_location(const std::string &name) const
	{
		auto it = uniform_binding_map.find(name);
		return (it != uniform_binding_map.end()) ? it->second : -1;
	}

	int VulkanProgramObjectProvider::get_uniform_buffer_size(int /*block_index*/) const
	{
		// No reflection in this implementation — return 0 as a safe default.
		// Callers using UBOs should size their buffers explicitly.
		return 0;
	}

	int VulkanProgramObjectProvider::get_uniform_buffer_index(const std::string &name) const
	{
		return get_uniform_location(name);
	}

	int VulkanProgramObjectProvider::get_storage_buffer_index(const std::string &name) const
	{
		return get_uniform_location(name);
	}

	void VulkanProgramObjectProvider::set_uniform_buffer_index(int block_index, int bind_index)
	{
		ubo_binding_map[block_index] = bind_index;
	}

	void VulkanProgramObjectProvider::set_storage_buffer_index(int buffer_index, int bind_unit_index)
	{
		ssbo_binding_map[buffer_index] = bind_unit_index;
	}

	// ---- Uniform setters — location is a byte offset into the push-constant buffer ----
	// The base class passes int location (from get_uniform_location()) directly.
	// Our get_uniform_location() returns the offset stored in uniform_binding_map,
	// so we write directly to that offset in the push-constant buffer.

	template<typename T>
	void VulkanProgramObjectProvider::write_push_constant(int offset, const T *data, size_t bytes)
	{
		if (offset < 0) return;
		const size_t required = static_cast<size_t>(offset) + bytes;
		if (required > PUSH_CONSTANT_SIZE) return;
		// Grow to the full push constant block on first write so vkCmdPushConstants
		// always uploads a complete, consistently-sized block.
		if (push_constants.size() < PUSH_CONSTANT_SIZE)
			push_constants.resize(PUSH_CONSTANT_SIZE, 0);
		std::memcpy(push_constants.data() + offset, data, bytes);
	}

	void VulkanProgramObjectProvider::set_uniform1i(int location, int v)
	{ write_push_constant(location, &v, sizeof(v)); }

	void VulkanProgramObjectProvider::set_uniform2i(int location, int x, int y)
	{ int d[2]{x,y}; write_push_constant(location, d, sizeof(d)); }

	void VulkanProgramObjectProvider::set_uniform3i(int location, int x, int y, int z)
	{ int d[3]{x,y,z}; write_push_constant(location, d, sizeof(d)); }

	void VulkanProgramObjectProvider::set_uniform4i(int location, int x, int y, int z, int w)
	{ int d[4]{x,y,z,w}; write_push_constant(location, d, sizeof(d)); }

	void VulkanProgramObjectProvider::set_uniform1f(int location, float v)
	{ write_push_constant(location, &v, sizeof(v)); }

	void VulkanProgramObjectProvider::set_uniform2f(int location, float x, float y)
	{ float d[2]{x,y}; write_push_constant(location, d, sizeof(d)); }

	void VulkanProgramObjectProvider::set_uniform3f(int location, float x, float y, float z)
	{ float d[3]{x,y,z}; write_push_constant(location, d, sizeof(d)); }

	void VulkanProgramObjectProvider::set_uniform4f(int location, float x, float y, float z, float w)
	{ float d[4]{x,y,z,w}; write_push_constant(location, d, sizeof(d)); }

	void VulkanProgramObjectProvider::set_uniformfv(int location, int size, int count, const float *data)
	{ write_push_constant(location, data, size * count * sizeof(float)); }

	void VulkanProgramObjectProvider::set_uniformiv(int location, int size, int count, const int *data)
	{ write_push_constant(location, data, size * count * sizeof(int)); }

	void VulkanProgramObjectProvider::set_uniform_matrix(int location, int size, int count,
														bool /*transpose*/, const float *data)
	{
		// Vulkan SPIR-V matrices are column-major by default (same as GLSL).
		// transpose flag is ignored — callers should pre-transpose if row-major.
		write_push_constant(location, data, size * size * count * sizeof(float));
	}
}
