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

#include <memory>
#include <vector>
#include <string>

namespace clan
{
	class VulkanContextDescription_Impl;

	/// \brief Vulkan device/instance configuration
	///
	/// Pass to VulkanTarget::set_description() before creating display windows.
	class VulkanContextDescription
	{
	public:
		VulkanContextDescription();
		~VulkanContextDescription();

		// ---- Getters ----

		/// Returns true if the Vulkan validation layers should be enabled.
		bool get_debug() const;

		/// Returns the list of extra instance extensions to request.
		const std::vector<std::string> &get_instance_extensions() const;

		/// Returns the list of extra device extensions to request.
		const std::vector<std::string> &get_device_extensions() const;

		/// Returns true if anisotropic filtering should be enabled (VkPhysicalDeviceFeatures).
		bool get_sampler_anisotropy() const;

		/// Returns true if the fill mode non-solid feature should be enabled (wireframe support).
		bool get_fill_mode_non_solid() const;

		// ---- Setters ----

		/// Enable/disable Vulkan validation layers (VK_LAYER_KHRONOS_validation).
		void set_debug(bool enable);

		/// Append an extra instance extension (e.g. VK_EXT_debug_utils).
		void add_instance_extension(const std::string &name);

		/// Append an extra device extension.
		void add_device_extension(const std::string &name);

		/// Request the samplerAnisotropy device feature.
		void set_sampler_anisotropy(bool enable);

		/// Request the fillModeNonSolid device feature (needed for wireframe rendering).
		void set_fill_mode_non_solid(bool enable);

	private:
		std::shared_ptr<VulkanContextDescription_Impl> impl;
	};
}
