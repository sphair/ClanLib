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
#include "VK/vulkan_device.h"
#include "API/VK/vulkan_context_description.h"
#include "VK/vulkan_context_description_impl.h"
#include "API/Core/Text/logger.h"

#include <set>
#include <algorithm>
#include <stdexcept>
#include <cstring>

namespace clan
{
	const std::vector<const char *> VulkanDevice::validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char *> VulkanDevice::required_device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// -------------------------------------------------------------------------
	VulkanDevice::VulkanDevice(const VulkanContextDescription &desc)
	{
		validation_enabled = desc.get_debug();
		create_instance(desc);
		if (validation_enabled)
			setup_debug_messenger();
		pick_physical_device();
		create_logical_device(desc);
		create_command_pool();
	}

	VulkanDevice::~VulkanDevice()
	{
		if (command_pool != VK_NULL_HANDLE)
			vkDestroyCommandPool(device, command_pool, nullptr);
		if (device != VK_NULL_HANDLE)
			vkDestroyDevice(device, nullptr);
		if (validation_enabled && debug_messenger != VK_NULL_HANDLE)
		{
			// volk loaded vkDestroyDebugUtilsMessengerEXT via volkLoadInstance().
			if (vkDestroyDebugUtilsMessengerEXT)
				vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
		}
		if (instance != VK_NULL_HANDLE)
			vkDestroyInstance(instance, nullptr);
	}

	// -------------------------------------------------------------------------
	void VulkanDevice::create_instance(const VulkanContextDescription &desc)
	{
		// Dynamically load the Vulkan loader library and resolve pre-instance
		// entry points.  This must happen before any vk* call is made.
		if (volkInitialize() != VK_SUCCESS)
			throw Exception("Failed to load Vulkan loader library (volkInitialize). "
							"Ensure the Vulkan runtime is installed.");

		if (validation_enabled && !check_validation_layer_support())
			throw Exception("Vulkan validation layers requested but not available");

		VkApplicationInfo app_info{};
		app_info.sType			= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName   = "ClanLib Application";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName		= "ClanLib";
		app_info.engineVersion	= VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion		= VK_API_VERSION_1_2;

		// Required instance extensions: base surface + platform surface
		std::vector<const char *> extensions = {
			VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#else
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif
		};
		if (validation_enabled)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		for (const auto &e : desc.get_instance_extensions())
			extensions.push_back(e.c_str());

		VkInstanceCreateInfo create_info{};
		create_info.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo		= &app_info;
		create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();

		if (validation_enabled)
		{
			create_info.enabledLayerCount   = static_cast<uint32_t>(validation_layers.size());
			create_info.ppEnabledLayerNames = validation_layers.data();
		}

		if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan instance");

		// Populate all instance-level function pointers via volk.
		// This must be called immediately after vkCreateInstance so that
		// every subsequent vk* call resolves through the loaded ICD rather
		// than a stale or null pointer.
		volkLoadInstance(instance);
	}

	// -------------------------------------------------------------------------
	void VulkanDevice::setup_debug_messenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT create_info{};
		create_info.sType		= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info.messageType	=
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info.pfnUserCallback = debug_callback;

		// volk loaded vkCreateDebugUtilsMessengerEXT via volkLoadInstance().
		// Call it directly; no manual vkGetInstanceProcAddr lookup needed.
		if (!vkCreateDebugUtilsMessengerEXT ||
			vkCreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
			throw Exception("Failed to set up Vulkan debug messenger");
	}

	// -------------------------------------------------------------------------
	void VulkanDevice::pick_physical_device()
	{
		uint32_t device_count = 0;
		if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) != VK_SUCCESS)
			throw Exception("Failed to enumerate Vulkan physical devices");
		if (device_count == 0)
			throw Exception("No Vulkan-capable GPU found");

		std::vector<VkPhysicalDevice> devices(device_count);
		if (vkEnumeratePhysicalDevices(instance, &device_count, devices.data()) != VK_SUCCESS)
			throw Exception("Failed to retrieve Vulkan physical devices");

		int best_score = -1;
		for (auto &dev : devices)
		{
			int score = rate_device(dev);
			if (score > best_score)
			{
				best_score	= score;
				physical_device = dev;
			}
		}
		if (physical_device == VK_NULL_HANDLE)
			throw Exception("Failed to find a suitable Vulkan GPU");

		// Find graphics queue family (present family is confirmed later when
		// we have a real surface – see init_present_queue).
		uint32_t qf_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &qf_count, nullptr);
		std::vector<VkQueueFamilyProperties> qf_props(qf_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &qf_count, qf_props.data());

		for (uint32_t i = 0; i < qf_count; i++)
		{
			if (qf_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphics_family_index = i;
				break;
			}
		}
		if (graphics_family_index == UINT32_MAX)
			throw Exception("No graphics queue family found on selected GPU");
	}

	// -------------------------------------------------------------------------
	void VulkanDevice::create_logical_device(const VulkanContextDescription &desc)
	{
		// Initially assume present == graphics; corrected by init_present_queue().
		present_family_index = graphics_family_index;

		std::set<uint32_t> unique_families = { graphics_family_index, present_family_index };
		float priority = 1.0f;
		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
		for (uint32_t family : unique_families)
		{
			VkDeviceQueueCreateInfo qi{};
			qi.sType			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			qi.queueFamilyIndex = family;
			qi.queueCount	= 1;
			qi.pQueuePriorities = &priority;
			queue_create_infos.push_back(qi);
		}

		VkPhysicalDeviceFeatures features{};
		features.samplerAnisotropy = desc.get_sampler_anisotropy() ? VK_TRUE : VK_FALSE;
		features.fillModeNonSolid  = desc.get_fill_mode_non_solid() ? VK_TRUE : VK_FALSE;

		// Enable descriptorBindingPartiallyBound (Vulkan 1.2 core).
		// This allows unwritten descriptor set bindings as long as the shader does not
		// access them.  Without this, submitting a command buffer that binds a descriptor
		// set with any unwritten slot causes VK_ERROR_DEVICE_LOST on many drivers,
		// even when the current shader ignores those bindings entirely.
		VkPhysicalDeviceDescriptorIndexingFeatures indexing_features{};
		indexing_features.sType							= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		indexing_features.descriptorBindingPartiallyBound  = VK_TRUE;

		std::vector<const char *> device_exts(
			required_device_extensions.begin(),
			required_device_extensions.end());
		for (const auto &e : desc.get_device_extensions())
			device_exts.push_back(e.c_str());

		VkDeviceCreateInfo create_info{};
		create_info.sType				= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.pNext				= &indexing_features;
		create_info.queueCreateInfoCount	= static_cast<uint32_t>(queue_create_infos.size());
		create_info.pQueueCreateInfos	= queue_create_infos.data();
		create_info.pEnabledFeatures		= &features;
		create_info.enabledExtensionCount   = static_cast<uint32_t>(device_exts.size());
		create_info.ppEnabledExtensionNames = device_exts.data();

		if (validation_enabled)
		{
			create_info.enabledLayerCount   = static_cast<uint32_t>(validation_layers.size());
			create_info.ppEnabledLayerNames = validation_layers.data();
		}

		if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan logical device");

		// Load all device-level function pointers directly from the driver.
		// This bypasses the loader dispatch table, eliminating one level of
		// indirection on every Vulkan call and enabling correct extension
		// entry points for the specific driver/ICD that was selected.
		volkLoadDevice(device);

		vkGetDeviceQueue(device, graphics_family_index, 0, &graphics_queue);
		vkGetDeviceQueue(device, present_family_index,  0, &present_queue);
	}

	// -------------------------------------------------------------------------
	void VulkanDevice::create_command_pool()
	{
		VkCommandPoolCreateInfo pool_info{};
		pool_info.sType			= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.queueFamilyIndex = graphics_family_index;
		pool_info.flags			= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan command pool");
	}

	// -------------------------------------------------------------------------
	void VulkanDevice::init_present_queue(VkSurfaceKHR surface)
	{
		uint32_t qf_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &qf_count, nullptr);

		for (uint32_t i = 0; i < qf_count; i++)
		{
			VkBool32 supports_present = VK_FALSE;
			if (vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &supports_present) != VK_SUCCESS)
				throw Exception("Failed to query Vulkan surface present support");
			if (supports_present)
			{
				present_family_index = i;
				vkGetDeviceQueue(device, present_family_index, 0, &present_queue);
				return;
			}
		}
		throw Exception("No present-capable queue family found");
	}

	// -------------------------------------------------------------------------
	VkCommandBuffer VulkanDevice::begin_single_time_commands()
	{
		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool		= command_pool;
		alloc_info.commandBufferCount = 1;

		VkCommandBuffer cmd;
		if (vkAllocateCommandBuffers(device, &alloc_info, &cmd) != VK_SUCCESS)
			throw Exception("Failed to allocate Vulkan single-time command buffer");

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (vkBeginCommandBuffer(cmd, &begin_info) != VK_SUCCESS)
		{
			vkFreeCommandBuffers(device, command_pool, 1, &cmd);
			throw Exception("Failed to begin Vulkan single-time command buffer");
		}
		return cmd;
	}

	void VulkanDevice::end_single_time_commands(VkCommandBuffer cmd)
	{
		if (vkEndCommandBuffer(cmd) != VK_SUCCESS)
		{
			vkFreeCommandBuffers(device, command_pool, 1, &cmd);
			throw Exception("Failed to end Vulkan single-time command buffer");
		}

		VkSubmitInfo submit_info{};
		submit_info.sType			= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers	= &cmd;

		if (vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
		{
			vkFreeCommandBuffers(device, command_pool, 1, &cmd);
			throw Exception("Failed to submit Vulkan single-time command buffer");
		}
		if (vkQueueWaitIdle(graphics_queue) != VK_SUCCESS)
			throw Exception("Failed to wait for Vulkan graphics queue idle");
		vkFreeCommandBuffers(device, command_pool, 1, &cmd);
	}

	// -------------------------------------------------------------------------
	uint32_t VulkanDevice::find_memory_type(uint32_t type_filter,
											VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties mem_props;
		vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_props);

		for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++)
		{
			if ((type_filter & (1u << i)) &&
				(mem_props.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
		throw Exception("Failed to find suitable Vulkan memory type");
	}

	// -------------------------------------------------------------------------
	VkFormat VulkanDevice::find_depth_format() const
	{
		const std::vector<VkFormat> candidates = {
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT
		};
		for (VkFormat fmt : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physical_device, fmt, &props);
			if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				return fmt;
		}
		throw Exception("Failed to find supported Vulkan depth format");
	}

	// -------------------------------------------------------------------------
	bool VulkanDevice::check_validation_layer_support() const
	{
		uint32_t count = 0;
		if (vkEnumerateInstanceLayerProperties(&count, nullptr) != VK_SUCCESS)
			return false;
		std::vector<VkLayerProperties> available(count);
		if (vkEnumerateInstanceLayerProperties(&count, available.data()) != VK_SUCCESS)
			return false;

		for (const char *layer : validation_layers)
		{
			bool found = false;
			for (const auto &props : available)
				if (strcmp(layer, props.layerName) == 0) { found = true; break; }
			if (!found) return false;
		}
		return true;
	}

	// -------------------------------------------------------------------------
	int VulkanDevice::rate_device(VkPhysicalDevice dev) const
	{
		VkPhysicalDeviceProperties props;
		VkPhysicalDeviceFeatures   feats;
		vkGetPhysicalDeviceProperties(dev, &props);
		vkGetPhysicalDeviceFeatures(dev, &feats);

		// Check required extensions
		uint32_t ext_count = 0;
		if (vkEnumerateDeviceExtensionProperties(dev, nullptr, &ext_count, nullptr) != VK_SUCCESS)
			return -1;
		std::vector<VkExtensionProperties> avail_exts(ext_count);
		if (vkEnumerateDeviceExtensionProperties(dev, nullptr, &ext_count, avail_exts.data()) != VK_SUCCESS)
			return -1;

		for (const char *req : required_device_extensions)
		{
			bool found = false;
			for (const auto &e : avail_exts)
				if (strcmp(req, e.extensionName) == 0) { found = true; break; }
			if (!found) return -1;
		}

		int score = 0;
		if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;
		score += static_cast<int>(props.limits.maxImageDimension2D / 1024);
		return score;
	}

	// -------------------------------------------------------------------------
	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDevice::debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT /*severity*/,
		VkDebugUtilsMessageTypeFlagsEXT		/*type*/,
		const VkDebugUtilsMessengerCallbackDataEXT *data,
		void * /*user_data*/)
	{
		log_event("Vulkan", data->pMessage);
		return VK_FALSE;
	}
}
