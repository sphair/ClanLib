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
**    Mark Page
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../Render/element_array_buffer.h"
#include "../Image/pixel_buffer.h"

/// \brief Element Array Buffer provider.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_PixelBufferProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_PixelBufferProvider() { }

	/// \brief Create
	///
	/// \param data = void
	/// \param new_width = New width
	/// \param new_height = New height
	/// \param direction = Direction
	/// \param new_format = New format
	/// \param usage = Buffer Usage
	virtual void create(const void *data, int new_width, int new_height, CL_PixelBufferDirection direction, CL_TextureFormat new_format, CL_BufferUsage usage) = 0;

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
	/// \brief Maps buffer into system memory.
	virtual void lock(CL_BufferAccess access) = 0;

	/// \brief Unmaps element buffer.
	virtual void unlock() = 0;

	/// \brief Uploads data to buffer.
	virtual void upload_data(const CL_Rect &dest_rect, const void *data) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
