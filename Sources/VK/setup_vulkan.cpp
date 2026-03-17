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
#include "API/VK/vulkan_target.h"
#include "API/VK/setup_vulkan.h"
#include "API/VK/vulkan_context_description.h"
#ifdef _WIN32
#  include "VK/Platform/Win32/vulkan_window_provider.h"
   using VulkanWindowProviderPlatform = clan::VulkanWindowProvider;
#else
#  include "VK/Platform/X11/vulkan_window_provider_x11.h"
   using VulkanWindowProviderPlatform = clan::VulkanWindowProvider_X11;
#endif
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/TargetProviders/cursor_provider.h"

namespace clan
{
	class VulkanTargetProvider : public DisplayTargetProvider
	{
	public:
		explicit VulkanTargetProvider(VulkanContextDescription &desc)
			: vk_desc(desc) {}

		~VulkanTargetProvider() override {}

		std::unique_ptr<DisplayWindowProvider> alloc_display_window()
		{
			return std::make_unique<VulkanWindowProviderPlatform>(vk_desc);
		}

	private:
		VulkanContextDescription vk_desc;
	};

	void VulkanTarget::enable()
	{
		VulkanContextDescription default_desc;
		set_description(default_desc);
	}

	void VulkanTarget::enable(VulkanContextDescription &desc)
	{
		set_description(desc);
	}

	void VulkanTarget::set_description(VulkanContextDescription &desc)
	{
		// Keep a static copy of the description so the provider's stored value
		// outlives this call (DisplayTarget holds a shared_ptr to the provider).
		static VulkanContextDescription stored_desc;
		stored_desc = desc;

		// Construct a fresh provider each time so any change in desc is reflected
		// in windows created after this call.  Using a non-static shared_ptr means
		// the old provider is replaced in DisplayTarget immediately.
		auto provider = std::make_shared<VulkanTargetProvider>(stored_desc);
		DisplayTarget::set_current_target(provider);
	}

	class SetupVulkan_Impl
	{
	public:
		SetupVulkan_Impl()
		{
			VulkanTarget::enable();
		}

		~SetupVulkan_Impl()
		{
			DisplayTarget::set_current_target(nullptr);
		}
	};

	SetupVulkan::SetupVulkan()
		: impl(std::make_unique<SetupVulkan_Impl>())
	{}

	SetupVulkan::~SetupVulkan() {}
}
