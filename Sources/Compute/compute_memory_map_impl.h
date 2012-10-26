/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#pragma once

#include "API/Compute/compute_event.h"
#include "API/Compute/compute_buffer.h"
#include "API/Compute/compute_command_queue.h"

namespace clan
{

class ComputeMemoryMap_Impl
{
public:
	ComputeMemoryMap_Impl(void *data, size_t size, const ComputeCommandQueue &queue, const ComputeBuffer &buffer, const ComputeEvent &mapped_event)
	: data(data), size(size), queue(queue), buffer(buffer), mapped_event(mapped_event), mapped(false), unmapped(false)
	{
	}

	~ComputeMemoryMap_Impl()
	{
		unmap();
	}

	void map()
	{
		if (!mapped)
		{
			mapped_event.wait();
			mapped = true;
		}
	}

	ComputeEvent unmap()
	{
		if (!unmapped)
		{
			unmapped = true;
			unmapped_event = queue.unmap(buffer, data, mapped_event);
		}
		return unmapped_event;
	}

	void *data;
	size_t size;
	ComputeCommandQueue queue;
	ComputeBuffer buffer;
	ComputeEvent mapped_event;
	ComputeEvent unmapped_event;
	bool mapped;
	bool unmapped;
};

}
