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

#include "API/Core/Math/vec4.h"
#include "API/Core/Math/half_float_vector.h"
#include <memory>
#include <vector>

namespace clan
{
	class PixelReader
	{
	public:
		virtual ~PixelReader() { }
		virtual void read(const void *input, Vec4f *output, int num_pixels) = 0;
	};

	class PixelWriter
	{
	public:
		virtual ~PixelWriter() { }
		virtual void write(void *output, Vec4f *input, int num_pixels) = 0;
	};

	class PixelFilter
	{
	public:
		virtual ~PixelFilter() { }
		virtual void filter(Vec4f *pixels, int num_pixels) = 0;
	};

	class PixelConverter_Impl
	{
	public:
		PixelConverter_Impl() : premultiply_alpha(false), flip_vertical(false), gamma(1.0f), swizzle(0, 1, 2, 3), input_is_ycrcb(false), output_is_ycrcb(false) { }

		std::unique_ptr<PixelReader> create_reader(TextureFormat format, bool sse2);
		std::unique_ptr<PixelWriter> create_writer(TextureFormat format, bool sse2, bool sse4);
		std::vector<std::shared_ptr<PixelFilter> > create_filters(bool sse2);

		bool premultiply_alpha;
		bool flip_vertical;
		float gamma;
		Vec4i swizzle;
		bool input_is_ycrcb;
		bool output_is_ycrcb;
	};
}
