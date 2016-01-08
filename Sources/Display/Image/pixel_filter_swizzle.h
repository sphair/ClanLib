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
	class PixelFilterSwizzle : public PixelFilter
	{
	public:
		PixelFilterSwizzle(const Vec4i &swizzle)
		{
			red.x = (swizzle.x == 0) ? 1.0f : 0.0f;
			red.y = (swizzle.x == 1) ? 1.0f : 0.0f;
			red.z = (swizzle.x == 2) ? 1.0f : 0.0f;
			red.w = (swizzle.x == 3) ? 1.0f : 0.0f;

			green.x = (swizzle.y == 0) ? 1.0f : 0.0f;
			green.y = (swizzle.y == 1) ? 1.0f : 0.0f;
			green.z = (swizzle.y == 2) ? 1.0f : 0.0f;
			green.w = (swizzle.y == 3) ? 1.0f : 0.0f;

			blue.x = (swizzle.z == 0) ? 1.0f : 0.0f;
			blue.y = (swizzle.z == 1) ? 1.0f : 0.0f;
			blue.z = (swizzle.z == 2) ? 1.0f : 0.0f;
			blue.w = (swizzle.z == 3) ? 1.0f : 0.0f;

			alpha.x = (swizzle.w == 0) ? 1.0f : 0.0f;
			alpha.y = (swizzle.w == 1) ? 1.0f : 0.0f;
			alpha.z = (swizzle.w == 2) ? 1.0f : 0.0f;
			alpha.w = (swizzle.w == 3) ? 1.0f : 0.0f;
		}

		void filter(Vec4f *pixels, int num_pixels) override
		{
			for (int i = 0; i < num_pixels; i++)
			{
				pixels[i] = Vec4f(
					red.x * pixels[i].x + red.y * pixels[i].y + red.z * pixels[i].z + red.w * pixels[i].w,
					green.x * pixels[i].x + green.y * pixels[i].y + green.z * pixels[i].z + green.w * pixels[i].w,
					blue.x * pixels[i].x + blue.y * pixels[i].y + blue.z * pixels[i].z + blue.w * pixels[i].w,
					alpha.x * pixels[i].x + alpha.y * pixels[i].y + alpha.z * pixels[i].z + alpha.w * pixels[i].w);
			}
		}

	private:
		Vec4f red, green, blue, alpha;
	};

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2

	class PixelFilterSwizzleSSE2 : public PixelFilter
	{
	public:
		PixelFilterSwizzleSSE2(const Vec4i &swizzle)
		{
			red_mask = _mm_castsi128_ps(_mm_set_epi32(
				swizzle.x == 0 ? 0xffffffff : 0,
				swizzle.y == 0 ? 0xffffffff : 0,
				swizzle.z == 0 ? 0xffffffff : 0,
				swizzle.w == 0 ? 0xffffffff : 0));

			green_mask = _mm_castsi128_ps(_mm_set_epi32(
				swizzle.x == 1 ? 0xffffffff : 0,
				swizzle.y == 1 ? 0xffffffff : 0,
				swizzle.z == 1 ? 0xffffffff : 0,
				swizzle.w == 1 ? 0xffffffff : 0));

			blue_mask = _mm_castsi128_ps(_mm_set_epi32(
				swizzle.x == 2 ? 0xffffffff : 0,
				swizzle.y == 2 ? 0xffffffff : 0,
				swizzle.z == 2 ? 0xffffffff : 0,
				swizzle.w == 2 ? 0xffffffff : 0));

			alpha_mask = _mm_castsi128_ps(_mm_set_epi32(
				swizzle.x == 3 ? 0xffffffff : 0,
				swizzle.y == 3 ? 0xffffffff : 0,
				swizzle.z == 3 ? 0xffffffff : 0,
				swizzle.w == 3 ? 0xffffffff : 0));
		}

		void filter(Vec4f *pixels, int num_pixels) override
		{
			for (int i = 0; i < num_pixels; i++)
			{
				__m128 pixel = _mm_loadu_ps(reinterpret_cast<float*>(pixels + i));

				__m128 red = _mm_shuffle_ps(pixel, pixel, _MM_SHUFFLE(0, 0, 0, 0));
				__m128 green = _mm_shuffle_ps(pixel, pixel, _MM_SHUFFLE(1, 1, 1, 1));
				__m128 blue = _mm_shuffle_ps(pixel, pixel, _MM_SHUFFLE(2, 2, 2, 2));
				__m128 alpha = _mm_shuffle_ps(pixel, pixel, _MM_SHUFFLE(3, 3, 3, 3));

				pixel = _mm_or_ps(_mm_or_ps(_mm_or_ps(_mm_and_ps(red_mask, red), _mm_and_ps(green_mask, green)), _mm_and_ps(blue_mask, blue)), _mm_and_ps(alpha_mask, alpha));

				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i), pixel);
			}
		}

	private:
		__m128 red_mask, green_mask, blue_mask, alpha_mask;
	};
#endif

}
