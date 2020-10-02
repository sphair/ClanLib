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
**    Mark Page
*/

#pragma once

#include "pixel_converter_impl.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	class PixelWriter_cast : public PixelWriter
	{
	public:
		template<typename Type>
		Type saturate(float v); // Intentionally not implemented (we only use specializations)
	};

	template<>
	inline unsigned char PixelWriter_cast::saturate(float v)
	{
		return clamp(v + 0.5f, 0.0f, 255.0f);
	}

	template<>
	inline unsigned short PixelWriter_cast::saturate(float v)
	{
		return clamp(v + 0.5f, 0.0f, 65535.0f);
	}

	template<>
	inline unsigned int PixelWriter_cast::saturate(float v)
	{
		if (v < 0)
			return 0;
		else if (v > 0xffffffff)
			return 0xffffffff;
		else
			return static_cast<unsigned int>(v + 0.5f);
	}

	template<>
	inline char PixelWriter_cast::saturate(float v)
	{
		float sign = v >= 0.0f ? 0.5f : -0.5f;
		return clamp(v + sign, -127.0f, 127.0f);
	}

	template<>
	inline short PixelWriter_cast::saturate(float v)
	{
		float sign = v >= 0.0f ? 0.5f : -0.5f;
		return clamp(v + sign, -32767.0f, 32767.0f);
	}

	template<>
	inline int PixelWriter_cast::saturate(float v)
	{
		if (v < -0x7fffffff)
			return -0x7fffffff;
		else if (v > 0x7fffffff)
			return 0x7fffffff;
		else
			return static_cast<int>(v + 0.5f);
	}

	template<>
	inline float PixelWriter_cast::saturate(float v)
	{
		return v;
	}

	template<typename Type>
	class PixelWriter_4cast : public PixelWriter_cast
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec4<Type> *d = static_cast<Vec4<Type> *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = Vec4<Type>(saturate<Type>(input[i].x), saturate<Type>(input[i].y), saturate<Type>(input[i].z), saturate<Type>(input[i].w));
			}
		}
	};

	template<typename Type>
	class PixelWriter_3cast : public PixelWriter_cast
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec3<Type> *d = static_cast<Vec3<Type> *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = Vec3<Type>(saturate<Type>(input[i].x), saturate<Type>(input[i].y), saturate<Type>(input[i].z));
			}
		}
	};

	template<typename Type>
	class PixelWriter_2cast : public PixelWriter_cast
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec2<Type> *d = static_cast<Vec2<Type> *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = Vec2<Type>(saturate<Type>(input[i].x), saturate<Type>(input[i].y));
			}
		}
	};

	template<typename Type>
	class PixelWriter_1cast : public PixelWriter_cast
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Type *d = static_cast<Type *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				d[i] = saturate<Type>(input[i].x);
			}
		}
	};
}
