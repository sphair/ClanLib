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
#include "vertex_array_buffer.h"

class CL_ElementArrayBufferProvider;
class CL_ElementArrayBuffer_Impl;

/// \brief Element Array Buffer
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ElementArrayBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_ElementArrayBuffer();

	/// \brief Constructs a ElementArrayBuffer
	///
	/// \param gc = Graphic Context
	/// \param size = value
	/// \param usage = Buffer Usage
	CL_ElementArrayBuffer(CL_GraphicContext &gc, int size, CL_BufferUsage usage = cl_usage_static_draw);

	/// \brief Constructs a ElementArrayBuffer
	///
	/// \param gc = Graphic Context
	/// \param data = void
	/// \param size = value
	/// \param usage = Buffer Usage
	CL_ElementArrayBuffer(CL_GraphicContext &gc, void *data, int size, CL_BufferUsage usage = cl_usage_static_draw);

	virtual ~CL_ElementArrayBuffer();

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
	CL_ElementArrayBufferProvider *get_provider() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Handle comparison operator.
	bool operator==(const CL_ElementArrayBuffer &other) const;

	/// \brief Maps element buffer into system memory.
	void lock(CL_BufferAccess access);

	/// \brief Unmaps element buffer.
	void unlock();

	/// \brief Uploads data to element array buffer.
	void upload_data(int offset, void *data, int size);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ElementArrayBuffer_Impl> impl;
/// \}
};

/// \}
