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
#include "API/VK/vulkan_context_description.h"
#include "VK/vulkan_context_description_impl.h"

namespace clan
{
	VulkanContextDescription::VulkanContextDescription()
		: impl(std::make_shared<VulkanContextDescription_Impl>())
	{}

	VulkanContextDescription::~VulkanContextDescription() {}

	bool VulkanContextDescription::get_debug() const			{ return impl->debug_flag; }
	const std::vector<std::string> &VulkanContextDescription::get_instance_extensions() const { return impl->instance_extensions; }
	const std::vector<std::string> &VulkanContextDescription::get_device_extensions()  const { return impl->device_extensions; }
	bool VulkanContextDescription::get_sampler_anisotropy()  const { return impl->sampler_anisotropy; }
	bool VulkanContextDescription::get_fill_mode_non_solid() const { return impl->fill_mode_non_solid; }

	void VulkanContextDescription::set_debug(bool enable)		{ impl->debug_flag = enable; }
	void VulkanContextDescription::add_instance_extension(const std::string &name) { impl->instance_extensions.push_back(name); }
	void VulkanContextDescription::add_device_extension(const std::string &name)   { impl->device_extensions.push_back(name); }
	void VulkanContextDescription::set_sampler_anisotropy(bool enable)  { impl->sampler_anisotropy = enable; }
	void VulkanContextDescription::set_fill_mode_non_solid(bool enable) { impl->fill_mode_non_solid = enable; }
}
