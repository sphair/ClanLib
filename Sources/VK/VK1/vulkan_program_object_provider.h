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

#pragma once

#include "API/VK/volk.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include "API/Display/TargetProviders/program_object_provider.h"

namespace clan { class VulkanShaderObjectProvider; }

namespace clan
{
	class VulkanDevice;
	class VulkanShaderObjectProvider;

	/// \brief Holds shader stages and descriptor layout for one "program".
	///
	/// Replaces GL3ProgramObjectProvider.  The actual VkPipeline is NOT created
	/// here – it is created lazily in VulkanGraphicContextProvider when draw()
	/// is called, because Vulkan pipelines encode render-pass, vertex layout,
	/// and rasteriser state that is not known until draw time.
	class VulkanProgramObjectProvider : public ProgramObjectProvider
	{
	public:
		explicit VulkanProgramObjectProvider(VulkanDevice *device);
		~VulkanProgramObjectProvider() override;

		// ---- ProgramObjectProvider interface ----
		void attach(const ShaderObject &shader) override;
		void detach(const ShaderObject &shader) override;

		/// Vulkan-internal helper: attach a shader provider directly without
		/// going through a ShaderObject wrapper.  Used by VulkanStandardPrograms
		/// where no GraphicContext is available to construct a ShaderObject.
		void attach_shader(VulkanShaderObjectProvider *provider);
		void link() override;
		void validate() override;

		unsigned int get_handle()		const override { return 0; } // no GL handle in Vulkan
		bool		get_link_status()	const override { return link_status; }
		bool		get_validate_status() const override { return validate_status; }
		std::string  get_info_log()		const override { return info_log; }

		std::vector<ShaderObject> get_shaders() const override { return attached_shaders; }

		void bind_attribute_location(int index, const std::string &name) override;
		void bind_frag_data_location(int color_number, const std::string &name) override;

		int  get_attribute_location(const std::string &name) const override;
		int  get_uniform_location(const std::string &name)   const override;
		int  get_uniform_buffer_size(int block_index)		const override;
		int  get_uniform_buffer_index(const std::string &name) const override;
		int  get_storage_buffer_index(const std::string &name) const override;

		// set_uniform* use int location, matching ProgramObjectProvider base class.
		// The location value is treated as a byte offset into the push-constant buffer,
		// set via get_uniform_location() which returns the offset from uniform_binding_map.
		void set_uniform1i(int location, int value) override;
		void set_uniform2i(int location, int x, int y) override;
		void set_uniform3i(int location, int x, int y, int z) override;
		void set_uniform4i(int location, int x, int y, int z, int w) override;
		void set_uniform1f(int location, float v) override;
		void set_uniform2f(int location, float x, float y) override;
		void set_uniform3f(int location, float x, float y, float z) override;
		void set_uniform4f(int location, float x, float y, float z, float w) override;
		void set_uniformfv(int location, int size, int count, const float *data) override;
		void set_uniformiv(int location, int size, int count, const int *data) override;
		void set_uniform_matrix(int location, int size, int count,
								bool transpose, const float *data) override;

		void set_uniform_buffer_index(int block_index, int bind_index) override;
		void set_storage_buffer_index(int buffer_index, int bind_unit_index) override;

		// ---- Vulkan-specific ----

	/// The shader stages to pass into VkGraphicsPipelineCreateInfo.
		/// Returns a copy of the internal stage vector.
		/// Callers MUST use the returned vector by value (not by const ref) when
		/// passing pStages to vkCreateGraphicsPipelines, because pName points
		/// into entry_point_names which is a member of this provider — the copy
		/// ensures the pName pointers remain valid for the duration of the call
		/// even if the program is relinked or moved while the pipeline is built.
		std::vector<VkPipelineShaderStageCreateInfo> get_stages() const
		{
			return pipeline_stages;
		}

		/// Map a GLSL uniform name to a descriptor binding slot.
		/// Call before link() if not using SPIR-V reflection.
		void set_uniform_binding(const std::string &name, int binding)
		{
			uniform_binding_map[name] = binding;
		}

		/// Returns the push-constant blob to be uploaded via vkCmdPushConstants.
		const std::vector<uint8_t> &get_push_constants() const { return push_constants; }
		bool has_push_constants() const { return !push_constants.empty(); }

	private:
		VulkanDevice *vk_device = nullptr;

		std::vector<ShaderObject>					attached_shaders;
		std::vector<VulkanShaderObjectProvider *>	raw_shaders;   // owned by us when attached via attach_shader()
		std::vector<VkPipelineShaderStageCreateInfo> pipeline_stages;

		// Stores the entry point name strings whose c_str() is referenced by
		// pipeline_stages[i].pName.  Must outlive pipeline_stages and must not
		// be resized or modified after link() populates it, because resizing
		// std::vector<std::string> would invalidate c_str() pointers.
		// A std::deque provides stable pointers on push_back.
		std::deque<std::string>					entry_point_names;

		std::unordered_map<std::string, int> attribute_location_map;
		std::unordered_map<std::string, int> uniform_binding_map;

		// Simple push-constant buffer (128 bytes – guaranteed min in Vulkan spec)
		// Used for small per-draw uniforms (scalars, vec4, mat4).
		std::vector<uint8_t> push_constants;
		static constexpr int PUSH_CONSTANT_SIZE = 128;

		std::string info_log;
		bool		link_status	= false;
		bool		validate_status = false;

		// UBO and SSBO binding assignments set via set_uniform_buffer_index /
		// set_storage_buffer_index.  Stored here for the GC provider to apply
		// when building the descriptor set before each draw.
		std::unordered_map<int, int> ubo_binding_map;   // block_index → bind_index
		std::unordered_map<int, int> ssbo_binding_map;  // buffer_index → bind_unit_index

		template<typename T>
		void write_push_constant(int offset, const T *data, size_t bytes);
	};
}
