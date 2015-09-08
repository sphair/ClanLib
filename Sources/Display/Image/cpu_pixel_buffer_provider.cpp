/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Display/precomp.h"
#include "cpu_pixel_buffer_provider.h"
#include "pixel_buffer_impl.h"
#include "API/Core/System/system.h"

namespace clan
{
	CPUPixelBufferProvider::CPUPixelBufferProvider() : delete_data(false), data(nullptr)
	{
	}

	CPUPixelBufferProvider::~CPUPixelBufferProvider()
	{
		if (delete_data)
			System::aligned_free(data);
	}

	void CPUPixelBufferProvider::create(TextureFormat new_format, const Size &new_size, const void *data_ptr, bool only_reference_data)
	{
		size = new_size;
		texture_format = new_format;

		unsigned int data_size = PixelBuffer::get_data_size(size, new_format);

		if (only_reference_data)
		{
			if (!data_ptr)
				throw Exception("PixelBuffer only_reference_data set without data");

			data = (unsigned char *)data_ptr;
			delete_data = false;
		}
		else
		{
			if (data_ptr)
			{
				delete_data = true;
				data = (unsigned char *)System::aligned_alloc(data_size, 16);
				memcpy(data, data_ptr, data_size);
			}
			else
			{
				delete_data = true;
				data = (unsigned char *)System::aligned_alloc(data_size, 16);
			}
		}
	}

	void CPUPixelBufferProvider::create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage)
	{
		// GPU only, this is never called
		throw Exception("Never call me");
	}

	void CPUPixelBufferProvider::upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data)
	{
		// Handle the simple base
		if ((dest_rect.left == 0) && (dest_rect.get_width() == size.width))
		{
			unsigned int data_size = PixelBuffer::get_data_size(Size(size.width, dest_rect.get_height()), texture_format);
			memcpy(this->data, data, data_size);
		}
		else
		{
			// Need to upload in blocks here
			throw Exception("CPUPixelBufferProvider::upload_data() Implement me for this situation");
		}
	}
}
