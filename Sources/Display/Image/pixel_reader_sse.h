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
#include <xmmintrin.h>

namespace clan
{
	class PixelReaderSSE2_rgba8 : public PixelReader
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			__m128 rcp_255 = _mm_set1_ps(1.0f / 255.0f);
			const Vec4ub *d = static_cast<const Vec4ub *>(input);

			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128i uchar_pixel = _mm_loadu_si128(reinterpret_cast<const __m128i*>(d + i));
				__m128i ushort_pixel0 = _mm_unpacklo_epi8(uchar_pixel, _mm_setzero_si128());
				__m128i ushort_pixel1 = _mm_unpackhi_epi8(uchar_pixel, _mm_setzero_si128());
				__m128i uint_pixel0 = _mm_unpacklo_epi16(ushort_pixel0, _mm_setzero_si128());
				__m128i uint_pixel1 = _mm_unpackhi_epi16(ushort_pixel0, _mm_setzero_si128());
				__m128i uint_pixel2 = _mm_unpacklo_epi16(ushort_pixel1, _mm_setzero_si128());
				__m128i uint_pixel3 = _mm_unpackhi_epi16(ushort_pixel1, _mm_setzero_si128());
				__m128 pixel0 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel0), rcp_255);
				__m128 pixel1 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel1), rcp_255);
				__m128 pixel2 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel2), rcp_255);
				__m128 pixel3 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel3), rcp_255);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i), pixel0);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 1), pixel1);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 2), pixel2);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 3), pixel3);
			}

			const float rcp_255f = 1.0f / 255.0f;
			for (int i = sse_length; i < num_pixels; i++)
			{
				output[i] = Vec4f(d[i].x, d[i].y, d[i].z, d[i].w) * rcp_255f;
			}
		}
	};

	class PixelReaderSSE2_bgra8 : public PixelReader
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			__m128 rcp_255 = _mm_set1_ps(1.0f / 255.0f);
			const Vec4ub *d = static_cast<const Vec4ub *>(input);

			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128i uchar_pixel = _mm_loadu_si128(reinterpret_cast<const __m128i*>(d + i));
				__m128i ushort_pixel0 = _mm_unpacklo_epi8(uchar_pixel, _mm_setzero_si128());
				__m128i ushort_pixel1 = _mm_unpackhi_epi8(uchar_pixel, _mm_setzero_si128());
				__m128i uint_pixel0 = _mm_unpacklo_epi16(ushort_pixel0, _mm_setzero_si128());
				__m128i uint_pixel1 = _mm_unpackhi_epi16(ushort_pixel0, _mm_setzero_si128());
				__m128i uint_pixel2 = _mm_unpacklo_epi16(ushort_pixel1, _mm_setzero_si128());
				__m128i uint_pixel3 = _mm_unpackhi_epi16(ushort_pixel1, _mm_setzero_si128());
				__m128 pixel0 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel0), rcp_255);
				__m128 pixel1 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel1), rcp_255);
				__m128 pixel2 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel2), rcp_255);
				__m128 pixel3 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel3), rcp_255);

				pixel0 = _mm_shuffle_ps(pixel0, pixel0, _MM_SHUFFLE(3, 0, 1, 2));
				pixel1 = _mm_shuffle_ps(pixel1, pixel1, _MM_SHUFFLE(3, 0, 1, 2));
				pixel2 = _mm_shuffle_ps(pixel2, pixel2, _MM_SHUFFLE(3, 0, 1, 2));
				pixel3 = _mm_shuffle_ps(pixel3, pixel3, _MM_SHUFFLE(3, 0, 1, 2));

				_mm_storeu_ps(reinterpret_cast<float*>(output + i), pixel0);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 1), pixel1);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 2), pixel2);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 3), pixel3);
			}

			const float rcp_255f = 1.0f / 255.0f;
			for (int i = sse_length; i < num_pixels; i++)
			{
				output[i] = Vec4f(d[i].z, d[i].y, d[i].x, d[i].w) * rcp_255f;
			}
		}
	};

	class PixelReaderSSE2_rgba16 : public PixelReader
	{
	public:
		void read(const void *input, Vec4f *output, int num_pixels) override
		{
			__m128 rcp_65535 = _mm_set1_ps(1.0f / 65535.0f);
			const Vec4us *d = static_cast<const Vec4us *>(input);

			int sse_length = (num_pixels / 2) * 2;
			for (int i = 0; i < sse_length; i += 2)
			{
				__m128i ushort_pixel = _mm_loadu_si128(reinterpret_cast<const __m128i*>(d + i));
				__m128i uint_pixel0 = _mm_unpacklo_epi16(ushort_pixel, _mm_setzero_si128());
				__m128i uint_pixel1 = _mm_unpackhi_epi16(ushort_pixel, _mm_setzero_si128());
				__m128 pixel0 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel0), rcp_65535);
				__m128 pixel1 = _mm_mul_ps(_mm_cvtepi32_ps(uint_pixel1), rcp_65535);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i), pixel0);
				_mm_storeu_ps(reinterpret_cast<float*>(output + i + 1), pixel1);
			}

			const float rcp_65535f = 1.0f / 65535.0f;
			for (int i = sse_length; i < num_pixels; i++)
			{
				output[i] = Vec4f(d[i].x, d[i].y, d[i].z, d[i].w) * rcp_65535f;
			}
		}
	};
}
