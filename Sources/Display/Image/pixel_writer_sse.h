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

#if defined(__SSE4_1__)
#include <smmintrin.h>
#else
#include <emmintrin.h>
#endif

namespace clan
{
	class PixelWriterSSE2_rgba8 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec4ub *d = static_cast<Vec4ub *>(output);

			__m128 value255f = _mm_set1_ps(255.0f);
			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128 pixel0 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i));
				__m128 pixel1 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 1));
				__m128 pixel2 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 2));
				__m128 pixel3 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 3));

				pixel0 = _mm_mul_ps(pixel0, value255f);
				pixel1 = _mm_mul_ps(pixel1, value255f);
				pixel2 = _mm_mul_ps(pixel2, value255f);
				pixel3 = _mm_mul_ps(pixel3, value255f);

				__m128i ushort_pixel0 = _mm_packs_epi32(_mm_cvttps_epi32(pixel0), _mm_cvttps_epi32(pixel1));
				__m128i ushort_pixel1 = _mm_packs_epi32(_mm_cvttps_epi32(pixel2), _mm_cvttps_epi32(pixel3));
				__m128i uchar_pixel = _mm_packus_epi16(ushort_pixel0, ushort_pixel1);
				_mm_storeu_si128(reinterpret_cast<__m128i*>(d + i), uchar_pixel);
			}

			for (int i = sse_length; i < num_pixels; i++)
			{
				Vec4f v = input[i] * 255.0f + 0.5f;
				d[i] = Vec4ub((int)clamp(v.x, 0, 255), (int)clamp(v.y, 0, 255), (int)clamp(v.z, 0, 255), (int)clamp(v.w, 0, 255));
			}
		}
	};

	class PixelWriterSSE2_bgra8 : public PixelWriter
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels) override
		{
			Vec4ub *d = static_cast<Vec4ub *>(output);

			__m128 value255f = _mm_set1_ps(255.0f);
			__m128 half = _mm_set1_ps(0.5f);
			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128 pixel0 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i));
				__m128 pixel1 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 1));
				__m128 pixel2 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 2));
				__m128 pixel3 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 3));

				pixel0 = _mm_add_ps(_mm_mul_ps(pixel0, value255f), half);
				pixel1 = _mm_add_ps(_mm_mul_ps(pixel1, value255f), half);
				pixel2 = _mm_add_ps(_mm_mul_ps(pixel2, value255f), half);
				pixel3 = _mm_add_ps(_mm_mul_ps(pixel3, value255f), half);

				pixel0 = _mm_shuffle_ps(pixel0, pixel0, _MM_SHUFFLE(3, 0, 1, 2));
				pixel1 = _mm_shuffle_ps(pixel1, pixel1, _MM_SHUFFLE(3, 0, 1, 2));
				pixel2 = _mm_shuffle_ps(pixel2, pixel2, _MM_SHUFFLE(3, 0, 1, 2));
				pixel3 = _mm_shuffle_ps(pixel3, pixel3, _MM_SHUFFLE(3, 0, 1, 2));

				__m128i ushort_pixel0 = _mm_packs_epi32(_mm_cvttps_epi32(pixel0), _mm_cvttps_epi32(pixel1));
				__m128i ushort_pixel1 = _mm_packs_epi32(_mm_cvttps_epi32(pixel2), _mm_cvttps_epi32(pixel3));
				__m128i uchar_pixel = _mm_packus_epi16(ushort_pixel0, ushort_pixel1);
				_mm_storeu_si128(reinterpret_cast<__m128i*>(d + i), uchar_pixel);
			}

			for (int i = sse_length; i < num_pixels; i++)
			{
				Vec4f v = input[i] * 255.0f + 0.5f;
				d[i] = Vec4ub((int)clamp(v.z, 0, 255), (int)clamp(v.y, 0, 255), (int)clamp(v.x, 0, 255), (int)clamp(v.w, 0, 255));
			}
		}
	};

#if defined(__SSE4_1__)

	class PixelWriterSSE4_rgba16 : public PixelWriter // _mm_packus_epi32 requires SSE4
	{
	public:
		void write(void *output, Vec4f *input, int num_pixels)
		{
			Vec4us *d = static_cast<Vec4us *>(output);

			__m128 value65535f = _mm_set1_ps(65535.0f);
			__m128 half = _mm_set1_ps(0.5f);
			int sse_length = (num_pixels / 2) * 2;
			for (int i = 0; i < sse_length; i += 2)
			{
				__m128 pixel0 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i));
				__m128 pixel1 = _mm_loadu_ps(reinterpret_cast<const float*>(input + i + 1));

				pixel0 = _mm_add_ps(_mm_mul_ps(pixel0, value65535f), half);
				pixel1 = _mm_add_ps(_mm_mul_ps(pixel1, value65535f), half);

				__m128i ushort_pixel0 = _mm_packus_epi32(_mm_cvttps_epi32(pixel0), _mm_cvttps_epi32(pixel1));
				_mm_storeu_si128(reinterpret_cast<__m128i*>(d + i), ushort_pixel0);
			}

			for (int i = sse_length; i < num_pixels; i++)
			{
				Vec4f v = input[i] * 65535.0f + 0.5f;
				d[i] = Vec4us((int)clamp(v.x, 0, 65535), (int)clamp(v.y, 0, 65535), (int)clamp(v.z, 0, 65535), (int)clamp(v.w, 0, 65535));
			}
		}
	};

#endif

}
