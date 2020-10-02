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

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
#include <emmintrin.h>
#endif

namespace clan
{
	class PixelFilterRGBToYCrCb : public PixelFilter
	{
	public:
		void filter(Vec4f *pixels, int num_pixels) override
		{
			for (int i = 0; i < num_pixels; i++)
			{
				float y = (0.299f * pixels[i].r) + (0.587f * pixels[i].g) + (0.114f * pixels[i].b);
				float cb = 0.5f - (0.168736f * pixels[i].r) - (0.331264f * pixels[i].g) + (0.5f * pixels[i].b);
				float cr = 0.5f + (0.5f * pixels[i].r) - (0.418688f * pixels[i].g) - (0.081312f * pixels[i].b);
				pixels[i] = Vec4f(y, cr, cb, pixels[i].a);
			}
		}
	};

	class PixelFilterYCrCbToRGB : public PixelFilter
	{
	public:
		void filter(Vec4f *pixels, int num_pixels) override
		{
			for (int i = 0; i < num_pixels; i++)
			{
				// xyz = ycrcb
				float r = pixels[i].x + 1.402f * (pixels[i].y - 0.5f);
				float g = pixels[i].x - 0.34414f * (pixels[i].z - 0.5f) - 0.71414f * (pixels[i].y - 0.5f);
				float b = pixels[i].x + 1.772f * (pixels[i].z - 0.5f);
				pixels[i] = Vec4f(r, g, b, pixels[i].a);
			}
		}
	};

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2

	class PixelFilterSSE2_RGBToYCrCb : public PixelFilter
	{
	public:
		void filter(Vec4f *pixels, int num_pixels) override
		{
			__m128 constant_0_299 = _mm_set1_ps(0.299f);
			__m128 constant_0_587 = _mm_set1_ps(0.587f);
			__m128 constant_0_114 = _mm_set1_ps(0.114f);
			__m128 constant_0_5 = _mm_set1_ps(0.5f);
			__m128 constant_0_168736 = _mm_set1_ps(0.168736f);
			__m128 constant_0_331264 = _mm_set1_ps(0.331264f);
			__m128 constant_0_418688 = _mm_set1_ps(0.418688f);
			__m128 constant_0_081312 = _mm_set1_ps(0.081312f);

			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128 pixel0 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i));
				__m128 pixel1 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i + 1));
				__m128 pixel2 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i + 2));
				__m128 pixel3 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i + 3));

				_MM_TRANSPOSE4_PS(pixel0, pixel1, pixel2, pixel3); // after this line: pixel0 = red, pixel1 = green, pixel2 = blue, pixel3 = alpha

				__m128 y = _mm_add_ps(_mm_add_ps(_mm_mul_ps(constant_0_299, pixel0), _mm_mul_ps(constant_0_587, pixel1)), _mm_mul_ps(constant_0_114, pixel2));
				__m128 cb = _mm_add_ps(_mm_sub_ps(_mm_sub_ps(constant_0_5, _mm_mul_ps(constant_0_168736, pixel0)), _mm_mul_ps(constant_0_331264, pixel1)), _mm_mul_ps(constant_0_5, pixel2));
				__m128 cr = _mm_sub_ps(_mm_sub_ps(_mm_add_ps(constant_0_5, _mm_mul_ps(constant_0_5, pixel0)), _mm_mul_ps(constant_0_418688, pixel1)), _mm_mul_ps(constant_0_081312, pixel2));

				_MM_TRANSPOSE4_PS(y, cr, cb, pixel3);

				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i), y);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 1), cr);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 2), cb);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 3), pixel3);
			}

			for (int i = sse_length; i < num_pixels; i++)
			{
				float y = (0.299f * pixels[i].r) + (0.587f * pixels[i].g) + (0.114f * pixels[i].b);
				float cb = 0.5f - (0.168736f * pixels[i].r) - (0.331264f * pixels[i].g) + (0.5f * pixels[i].b);
				float cr = 0.5f + (0.5f * pixels[i].r) - (0.418688f * pixels[i].g) - (0.081312f * pixels[i].b);
				pixels[i] = Vec4f(y, cr, cb, pixels[i].a);
			}
		}
	};

	class PixelFilterSSE2_YCrCbToRGB : public PixelFilter
	{
	public:
		void filter(Vec4f *pixels, int num_pixels) override
		{
			__m128 constant_1_402 = _mm_set1_ps(1.402f);
			__m128 constant_0_5 = _mm_set1_ps(0.5f);
			__m128 constant_0_34414 = _mm_set1_ps(0.34414f);
			__m128 constant_0_71414 = _mm_set1_ps(0.71414f);
			__m128 constant_1_772 = _mm_set1_ps(1.772f);

			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128 pixel0 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i));
				__m128 pixel1 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i + 1));
				__m128 pixel2 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i + 2));
				__m128 pixel3 = _mm_loadu_ps(reinterpret_cast<const float*>(pixels + i + 3));

				_MM_TRANSPOSE4_PS(pixel0, pixel1, pixel2, pixel3); // after this line: pixel0 = y, pixel1 = cr, pixel2 = cb, pixel3 = alpha

				__m128 r = _mm_add_ps(pixel0, _mm_mul_ps(constant_1_402, _mm_sub_ps(pixel1, constant_0_5)));
				__m128 g = _mm_sub_ps(_mm_sub_ps(pixel0, _mm_mul_ps(constant_0_34414, _mm_sub_ps(pixel2, constant_0_5))), _mm_mul_ps(constant_0_71414, _mm_sub_ps(pixel1, constant_0_5)));
				__m128 b = _mm_add_ps(pixel0, _mm_mul_ps(constant_1_772, _mm_sub_ps(pixel2, constant_0_5)));

				_MM_TRANSPOSE4_PS(r, g, b, pixel3);

				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i), r);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 1), g);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 2), b);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 3), pixel3);
			}

			for (int i = sse_length; i < num_pixels; i++)
			{
				// xyz = ycrcb
				float r = pixels[i].x + 1.402f * (pixels[i].y - 0.5f);
				float g = pixels[i].x - 0.34414f * (pixels[i].z - 0.5f) - 0.71414f * (pixels[i].y - 0.5f);
				float b = pixels[i].x + 1.772f * (pixels[i].z - 0.5f);
				pixels[i] = Vec4f(r, g, b, pixels[i].a);
			}
		}
	};

#endif

}
