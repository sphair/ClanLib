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

// VMA uses volk's dynamic dispatch — we must tell it not to expect static Vulkan symbols.
#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

// VMA's dynamic function pointer struct matches volk's global table.
// We provide a helper that fills VmaVulkanFunctions from the volk globals.

#include "volk.h"

// vk_mem_alloc.h is a single-header library. Include it after the configuration
// defines above. The actual include is done either with VMA_IMPLEMENTATION
// (in vulkan_vma.cpp) or without it (everywhere else that needs VmaAllocator).
#include "vk_mem_alloc.h"

namespace clan
{
	/// Fill a VmaVulkanFunctions struct from volk's loaded function pointers.
	/// Call this just before vmaCreateAllocator().
	inline VmaVulkanFunctions make_vma_vulkan_functions()
	{
		VmaVulkanFunctions f{};
		f.vkGetInstanceProcAddr				= vkGetInstanceProcAddr;
		f.vkGetDeviceProcAddr					= vkGetDeviceProcAddr;
		f.vkGetPhysicalDeviceProperties		= vkGetPhysicalDeviceProperties;
		f.vkGetPhysicalDeviceMemoryProperties	= vkGetPhysicalDeviceMemoryProperties;
		f.vkAllocateMemory						= vkAllocateMemory;
		f.vkFreeMemory							= vkFreeMemory;
		f.vkMapMemory							= vkMapMemory;
		f.vkUnmapMemory						= vkUnmapMemory;
		f.vkFlushMappedMemoryRanges			= vkFlushMappedMemoryRanges;
		f.vkInvalidateMappedMemoryRanges		= vkInvalidateMappedMemoryRanges;
		f.vkBindBufferMemory					= vkBindBufferMemory;
		f.vkBindImageMemory					= vkBindImageMemory;
		f.vkGetBufferMemoryRequirements		= vkGetBufferMemoryRequirements;
		f.vkGetImageMemoryRequirements			= vkGetImageMemoryRequirements;
		f.vkCreateBuffer						= vkCreateBuffer;
		f.vkDestroyBuffer						= vkDestroyBuffer;
		f.vkCreateImage						= vkCreateImage;
		f.vkDestroyImage						= vkDestroyImage;
		f.vkCmdCopyBuffer						= vkCmdCopyBuffer;
		// Vulkan 1.1 / KHR promoted functions (required by VMA >= 3.x)
		f.vkGetBufferMemoryRequirements2KHR	= vkGetBufferMemoryRequirements2;
		f.vkGetImageMemoryRequirements2KHR		= vkGetImageMemoryRequirements2;
		f.vkBindBufferMemory2KHR				= vkBindBufferMemory2;
		f.vkBindImageMemory2KHR				= vkBindImageMemory2;
		f.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2;
		return f;
	}
} // namespace clan
