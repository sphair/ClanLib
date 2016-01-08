/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"

namespace clan
{
	class GraphicContext;
	class PixelBufferProvider;

	/// \brief Pixel data implementation interface.
	class PixelBuffer_Impl
	{
	public:
		PixelBuffer_Impl();
		PixelBuffer_Impl(PixelBufferProvider *provider);

		PixelBuffer_Impl(int width, int height, TextureFormat texture_format, const void *data_ptr, bool only_reference_data);
		~PixelBuffer_Impl();

		Colorf get_pixel(int x, int y);

		PixelBufferProvider *provider;

		float pixel_ratio = 0.0f;

		/// \brief Returns the pixel format
		TextureFormat get_format() const { return provider->get_format(); }

		/// \brief Returns the number of bytes per pixel
		unsigned int get_bytes_per_pixel() const { return get_bytes_per_pixel(get_format()); }

		/// \brief Returns the number of bytes per compression block
		unsigned int get_bytes_per_block() const { return get_bytes_per_block(get_format()); }

		/// \brief Returns the number of bytes per pixel
		///
		/// \return Bytes per pixel. Exception thrown if not available (hint, use is_compressed() )
		static unsigned int get_bytes_per_pixel(TextureFormat texture_format);

		static unsigned int get_data_size(const Size &size, TextureFormat texture_format);

		/// \brief Returns the number of bytes per compression block
		///
		/// \return Bytes per block. Exception thrown if not available (hint, use is_compressed() )
		static unsigned int get_bytes_per_block(TextureFormat texture_format);

		static bool is_compressed(TextureFormat texture_format);

		void convert(PixelBuffer &target, const Rect &dest_rect, const Rect &src_rect, PixelConverter &converter) const;
	};
}
