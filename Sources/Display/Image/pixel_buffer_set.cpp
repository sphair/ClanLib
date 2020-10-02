/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer_set.h"
#include "API/Display/Image/pixel_buffer.h"

namespace clan
{
	class PixelBufferSet_Impl
	{
	public:
		PixelBufferSet_Impl(TextureDimensions dimensions, TextureFormat format, int width, int height, int slices)
			: dimensions(dimensions), format(format), width(width), height(height), slices(slices), base_level(-1), max_level(-1)
		{
		}

		TextureDimensions dimensions;
		TextureFormat format;
		int width;
		int height;
		std::vector<std::vector<PixelBuffer> > slices;
		int base_level;
		int max_level;
	};

	PixelBufferSet::PixelBufferSet()
	{
	}

	PixelBufferSet::PixelBufferSet(TextureDimensions dimensions, TextureFormat format, int width, int height, int slices)
		: impl(std::make_shared<PixelBufferSet_Impl>(dimensions, format, width, height, slices))
	{
	}

	PixelBufferSet::PixelBufferSet(const PixelBuffer &image)
		: impl(std::make_shared<PixelBufferSet_Impl>(TextureDimensions::_2d, image.get_format(), image.get_width(), image.get_height(), 1))
	{
		set_image(0, 0, image);
	}

	void PixelBufferSet::throw_if_null() const
	{
		if (!impl)
			throw Exception("PixelBufferSet is null");
	}

	TextureDimensions PixelBufferSet::get_dimensions() const
	{
		throw_if_null();
		return impl->dimensions;
	}

	TextureFormat PixelBufferSet::get_format() const
	{
		throw_if_null();
		return impl->format;
	}

	int PixelBufferSet::get_width() const
	{
		throw_if_null();
		return impl->width;
	}

	int PixelBufferSet::get_height() const
	{
		throw_if_null();
		return impl->height;
	}

	int PixelBufferSet::get_slice_count() const
	{
		throw_if_null();
		return impl->slices.size();
	}

	int PixelBufferSet::get_base_level() const
	{
		throw_if_null();
		return impl->base_level;
	}

	int PixelBufferSet::get_max_level() const
	{
		throw_if_null();
		return impl->max_level;
	}

	PixelBuffer PixelBufferSet::get_image(int slice, int level)
	{
		throw_if_null();

		if (slice < 0 || slice >= (int)impl->slices.size() || level < 0)
			throw Exception("Out of bounds");

		if (impl->slices[slice].size() > (size_t)level)
		{
			return impl->slices[slice][level];
		}
		else
		{
			return PixelBuffer();
		}
	}

	void PixelBufferSet::set_image(int slice, int level, const PixelBuffer &image)
	{
		throw_if_null();

		if (slice < 0 || slice >= (int)impl->slices.size() || level < 0)
			throw Exception("Out of bounds");

		if (impl->slices[slice].size() <= (size_t)level)
			impl->slices[slice].resize(level + 1);
		impl->slices[slice][level] = image;

		if (impl->base_level == -1)
			impl->base_level = level;
		else
			impl->base_level = min(impl->base_level, level);

		if (impl->max_level == -1)
			impl->max_level = level;
		else
			impl->max_level = max(impl->max_level, level);
	}
}
