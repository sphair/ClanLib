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
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/pixel_buffer_provider.h"

namespace clan
{

class PixelBuffer_Impl;

class CPUPixelBufferProvider : public PixelBufferProvider
{
/// \name Construction
/// \{

public:
	CPUPixelBufferProvider();

	~CPUPixelBufferProvider();

	void create(TextureFormat new_format, const Size &new_size, const void *data_ptr, bool only_reference_data);

	void create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage);

/// \}
/// \name Attributes
/// \{

public:
	void *get_data() { return data; }

	int get_pitch() const {return size.width * PixelBuffer::get_bytes_per_pixel(texture_format); }

	Size get_size() const { return size; }

	bool is_gpu() const { return false; }

	TextureFormat get_format() const { return texture_format; };


/// \}
/// \name Operations
/// \{

public:
	void lock(GraphicContext &gc, BufferAccess access) { }

	void unlock() { }

	void upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data);

/// \}
/// \name Implementation
/// \{

private:
	/// \brief Boolean indicating if pixel data should be deleted on destruction.
	bool delete_data;

	/// \brief Pixel data.
	unsigned char *data;

	Size size;
	TextureFormat texture_format;

/// \}
};

}
