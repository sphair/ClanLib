/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../../Core/System/sharedptr.h"
#include "graphic_context.h"
#include "../api_display.h"

#include "../Image/buffer_usage.h"

class CL_VertexArrayBufferProvider;
class CL_VertexArrayBuffer_Impl;

/// \brief Vertex Array Buffer
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_VertexArrayBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_VertexArrayBuffer();

	/// \brief Constructs a VertexArrayBuffer
	///
	/// \param gc = Graphic Context
	/// \param size = value
	/// \param usage = Buffer Usage
	CL_VertexArrayBuffer(CL_GraphicContext &gc, int size, CL_BufferUsage usage = cl_usage_static_draw);

	/// \brief Constructs a VertexArrayBuffer
	///
	/// \param gc = Graphic Context
	/// \param data = void
	/// \param size = value
	/// \param usage = Buffer Usage
	CL_VertexArrayBuffer(CL_GraphicContext &gc, void *data, int size, CL_BufferUsage usage = cl_usage_static_draw);

	virtual ~CL_VertexArrayBuffer();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Retrieves a pointer to the mapped buffer.
	void *get_data();

	/// \brief Get Provider
	///
	/// \return provider
	CL_VertexArrayBufferProvider *get_provider() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Handle comparison operator.
	bool operator==(const CL_VertexArrayBuffer &other) const;

	/// \brief Maps vertex buffer into system memory.
	void lock(CL_BufferAccess access);

	/// \brief Unmaps vertex buffer.
	void unlock();

	/// \brief Uploads data to vertex array buffer.
	void upload_data(int offset, void *data, int size);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_VertexArrayBuffer_Impl> impl;
/// \}
};

/// \}
