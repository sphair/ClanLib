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

#include "pixel_converter_impl.h"

namespace clan
{
	class PixelWriter_bgra8 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec4ub *d = static_cast<Vec4ub *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec4f v = input[i] * 255.0f + 0.5f;
				d[i] = Vec4ub((int)clamp(v.z, 0, 255), (int)clamp(v.y, 0, 255), (int)clamp(v.x, 0, 255), (int)clamp(v.w, 0, 255));
			}
		}
	};

	class PixelWriter_bgr8 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec3ub *d = static_cast<Vec3ub *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec3f v = Vec3f(input[i]) * 255.0f + 0.5f;
				d[i] = Vec3ub((int)clamp(v.z, 0, 255), (int)clamp(v.y, 0, 255), (int)clamp(v.x, 0, 255));
			}
		}
	};

	class PixelWriter_r3_g3_b2 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			unsigned char *d = static_cast<unsigned char *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec3f v = Vec3f(input[i].x * 7.0f + 0.5f, input[i].y * 7.0f + 0.5f, input[i].z * 3.0f + 0.5f);
				d[i] = (unsigned char)(((int)clamp(v.x, 0, 7) << 5) | ((int)clamp(v.y, 0, 7) << 2) | ((int)clamp(v.z, 0, 3)) << 0);
			}
		}
	};

	class PixelWriter_rgb4 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			unsigned short *d = static_cast<unsigned short *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec3f v = Vec3f(input[i].x * 15.0f + 0.5f, input[i].y * 15.0f + 0.5f, input[i].z * 15.0f + 0.5f);
				d[i] = (unsigned short)(((int)clamp(v.x, 0, 15) << 12) | ((int)clamp(v.y, 0, 15) << 8) | ((int)clamp(v.z, 0, 15)) << 4);
			}
		}
	};

	class PixelWriter_rgb5 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			unsigned short *d = static_cast<unsigned short *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec3f v = Vec3f(input[i].x * 31.0f + 0.5f, input[i].y * 31.0f + 0.5f, input[i].z * 31.0f + 0.5f);
				d[i] = (unsigned short)(((int)clamp(v.x, 0, 31) << 11) | ((int)clamp(v.y, 0, 31) << 6) | ((int)clamp(v.z, 0, 31)) << 1);
			}
		}
	};

	class PixelWriter_rgb10 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			uint32_t *d = static_cast<uint32_t *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec3f v = Vec3f(input[i].x * 1023.0f + 0.5f, input[i].y * 1023.0f + 0.5f, input[i].z * 1023.0f + 0.5f);
				d[i] = (uint32_t)(((int)clamp(v.x, 0, 1023) << 22) | ((int)clamp(v.y, 0, 1023) << 12) | ((int)clamp(v.z, 0, 1023)) << 2);
			}
		}
	};


	class PixelWriter_rgba4 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			unsigned short *d = static_cast<unsigned short *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec4f v = Vec4f(input[i].x * 15.0f + 0.5f, input[i].y * 15.0f + 0.5f, input[i].z * 15.0f + 0.5f, input[i].a * 15.0f + 0.5f);
				d[i] = (unsigned short)(((int)clamp(v.x, 0, 15) << 12) | ((int)clamp(v.y, 0, 15) << 8) | ((int)clamp(v.z, 0, 15)) << 4 | ((int)clamp(v.a, 0, 15)) << 0);
			}
		}
	};

	class PixelWriter_rgb5_a1 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			unsigned short *d = static_cast<unsigned short *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec4f v = Vec4f(input[i].x * 31.0f + 0.5f, input[i].y * 31.0f + 0.5f, input[i].z * 31.0f + 0.5f, input[i].a + 0.5f);
				d[i] = (unsigned short)(((int)clamp(v.x, 0, 31) << 11) | ((int)clamp(v.y, 0, 31) << 6) | ((int)clamp(v.z, 0, 31)) << 1 | ((int)clamp(v.a, 0, 1)) << 0);
			}
		}
	};

	class PixelWriter_rgb10_a2 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			uint32_t *d = static_cast<uint32_t *>(output);
			for (int i = 0; i < num_pixels; i++)
			{
				Vec4f v = Vec4f(input[i].x * 1023.0f + 0.5f, input[i].y * 1023.0f + 0.5f, input[i].z * 1023.0f + 0.5f, input[i].a * 3.0f + 0.5f);
				d[i] = (uint32_t)(((int)clamp(v.x, 0, 1023) << 22) | ((int)clamp(v.y, 0, 1023) << 12) | ((int)clamp(v.z, 0, 1023)) << 2 | ((int)clamp(v.a, 0, 3)) << 0);
			}
		}
	};
}
