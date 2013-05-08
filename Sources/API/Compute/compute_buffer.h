/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "api_compute.h"
#include <memory>
#include "../Core/Math/vec4.h"
#include "../Display/Image/buffer_usage.h"

namespace clan
{

class PixelBuffer;
class VertexArrayBuffer;
class ElementArrayBuffer;
class Texture;
class RenderBuffer;
class ComputeContext;
class ComputeBuffer;
class ComputeBuffer_Impl;

/// \brief Compute buffer
class CL_API_COMPUTE ComputeBuffer
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	ComputeBuffer();

	/// \brief Constructs a compute program kernel
	///
	/// \param program = Compute program containing the kernel function
	/// \param kernel_name = Name of kernel function
	ComputeBuffer(ComputeContext &context, int size, BufferAccess access = access_read_write, bool alloc_host_pointer = false);

	~ComputeBuffer();

	static ComputeBuffer from_pixel_buffer(ComputeContext &context, PixelBuffer &pixel_buffer, BufferAccess access = access_read_only);
	static ComputeBuffer from_vertex_buffer(ComputeContext &context, VertexArrayBuffer &vertex_buffer, BufferAccess access = access_read_only);
	static ComputeBuffer from_element_buffer(ComputeContext &context, ElementArrayBuffer &element_buffer, BufferAccess access = access_read_only);
	static ComputeBuffer from_texture(ComputeContext &context, Texture &texture, int mipmap_level = 0, BufferAccess access = access_read_only);
	static ComputeBuffer from_render_buffer(ComputeContext &context, RenderBuffer &render_buffer, BufferAccess access = access_read_only);
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;
/// \}

/// \name Operations
/// \{
public:
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<ComputeBuffer_Impl> impl;

	friend class ComputeKernel;
	friend class ComputeCommandQueue;
	friend class ComputeCommandQueue_Impl;
/// \}
};

}

/// \}
