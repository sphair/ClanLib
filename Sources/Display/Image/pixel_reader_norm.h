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

#pragma once

#include "pixel_converter_impl.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	class PixelReader_norm : public PixelReader
	{
	public:
		inline static float normalize(unsigned char v)  { return v * (1.0f / 0xff); }
		inline static float normalize(unsigned short v) { return v * (1.0f / 0xffff); }

		inline static float normalize(char v)  { return min(v * (1.0f / 0x7f), 1.0f); }
		inline static float normalize(short v) { return min(v * (1.0f / 0x7fff), 1.0f); }
	};

	template<typename Type>
	class PixelReader_4norm : public PixelReader_norm
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			const Vec4<Type> *d = static_cast<const Vec4<Type> *>(input);
			for (int i = 0; i < num_pixels; i++)
			{
				output[i] = Vec4f(normalize(d[i].x), normalize(d[i].y), normalize(d[i].z), normalize(d[i].w));
			}
		}
	};

	template<typename Type>
	class PixelReader_3norm : public PixelReader_norm
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			const Vec3<Type> *d = static_cast<const Vec3<Type> *>(input);
			for (int i = 0; i < num_pixels; i++)
			{
				output[i] = Vec4f(normalize(d[i].x), normalize(d[i].y), normalize(d[i].z), 1.0f);
			}
		}
	};

	template<typename Type>
	class PixelReader_2norm : public PixelReader_norm
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			const Vec2<Type> *d = static_cast<const Vec2<Type> *>(input);
			for (int i = 0; i < num_pixels; i++)
			{
				output[i] = Vec4f(normalize(d[i].x), normalize(d[i].y), 0.0f, 1.0f);
			}
		}
	};

	template<typename Type>
	class PixelReader_1norm : public PixelReader_norm
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			const Type *d = static_cast<const Type *>(input);
			for (int i = 0; i < num_pixels; i++)
			{
				output[i] = Vec4f(normalize(d[i]), 0.0f, 0.0f, 1.0f);
			}
		}
	};
}
