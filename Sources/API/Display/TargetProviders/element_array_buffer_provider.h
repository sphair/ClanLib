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

#include "../Render/element_array_buffer.h"

/// \brief Element Array Buffer provider.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ElementArrayBufferProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_ElementArrayBufferProvider() { }

	/// \brief Constructs a element array buffer.
	virtual void create(int size, CL_BufferUsage usage) = 0;

	/// \brief Create
	///
	/// \param data = void
	/// \param size = value
	/// \param usage = Buffer Usage
	virtual void create(void *data, int size, CL_BufferUsage usage) = 0;

	/// \brief Destroys the element array buffer provider.
	virtual void destroy()=0;

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Retrieves a pointer to the mapped buffer.
	virtual void *get_data() = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Maps element buffer into system memory.
	virtual void lock(CL_BufferAccess access) = 0;

	/// \brief Unmaps element buffer.
	virtual void unlock() = 0;

	/// \brief Uploads data to element array buffer.
	virtual void upload_data(int offset, void *data, int size) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
