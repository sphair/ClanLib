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

/// \addtogroup clanCompute_Compute clanCompute Compute
/// \{

#pragma once

#include "compute_buffer.h"
#include "compute_context.h"
#include "compute_command_queue.h"
#include "compute_event.h"

namespace clan
{

/// \brief Compute transfer buffer
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
template<typename Type>
class ComputeTransferBuffer
{
/// \name Construction
/// \{
public:
	ComputeTransferBuffer()
	{
	}

	ComputeTransferBuffer(ComputeContext &context, ComputeCommandQueue &queue, int size)
	: queue(queue), buffer_local(context, size * sizeof(Type), access_write_only, true), buffer_remote(context, size * sizeof(Type), access_read_only)
	{
		memory_map = queue.map_buffer(buffer_local, access_write_only, 0, size * sizeof(Type));
		data = memory_map.data<Type>();
	}

	~ComputeTransferBuffer()
	{
	}
/// \}

/// \name Attributes
/// \{
public:
	Type *data;
	ComputeEvent &get_upload_event() { return upload_event; }
	ComputeBuffer &get_buffer_remote() { return buffer_remote; }
/// \}

/// \name Operations
/// \{
	void upload_range(int start, int length)
	{
		upload_event = queue.write_buffer(buffer_remote, false, start * sizeof(Type), length * sizeof(Type), memory_map.data());
	}
/// \}

/// \name Implementation
/// \{
private:
	ComputeCommandQueue queue;
	ComputeBuffer buffer_local;
	ComputeBuffer buffer_remote;
	ComputeEvent upload_event;
	ComputeMemoryMap memory_map;
/// \}
};

}

/// \}
