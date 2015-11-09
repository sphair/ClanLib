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

namespace clan
{
	class PixelWriter_norm : public PixelWriter
	{
	public:
		template<typename Type>
		static Type unnorm(float v); // Intentionally not implemented (we only use specializations)
	};

	template<>
	inline unsigned char PixelWriter_norm::unnorm(float v)
	{
		return clamp(v * 255.0f + 0.5f, 0.0f, 255.0f);
	}

	template<>
	inline unsigned short PixelWriter_norm::unnorm(float v)
	{
		return clamp(v * 65535.0f + 0.5f, 0.0f, 65535.0f);
	}

	template<>
	inline char PixelWriter_norm::unnorm(float v)
	{
		float sign = v >= 0.0f ? 0.5f : -0.5f;
		return clamp(v * 127.0f + sign, 0.0f, 127.0f);
	}

	template<>
	inline short PixelWriter_norm::unnorm(float v)
	{
		float sign = v >= 0.0f ? 0.5f : -0.5f;
		return clamp(v * 32767.0f + sign, 0.0f, 32767.0f);
	}

	template<typename Type>
	class PixelWriter_4norm : public PixelWriter_norm
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec4<Type> *d = static_cast<Vec4<Type> *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = Vec4<Type>(unnorm<Type>(input[i].x), unnorm<Type>(input[i].y), unnorm<Type>(input[i].z), unnorm<Type>(input[i].w));
			}
		}
	};

	template<typename Type>
	class PixelWriter_3norm : public PixelWriter_norm
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec3<Type> *d = static_cast<Vec3<Type> *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = Vec3<Type>(unnorm<Type>(input[i].x), unnorm<Type>(input[i].y), unnorm<Type>(input[i].z));
			}
		}
	};

	template<typename Type>
	class PixelWriter_2norm : public PixelWriter_norm
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec2<Type> *d = static_cast<Vec2<Type> *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = Vec2<Type>(unnorm<Type>(input[i].x), unnorm<Type>(input[i].y));
			}
		}
	};

	template<typename Type>
	class PixelWriter_1norm : public PixelWriter_norm
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Type *d = static_cast<Type *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = (unnorm<Type>(input[i].x));
			}
		}
	};
}
