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

	class PixelFilterPremultiplyAlpha : public PixelFilter
	{
	public:
		void filter(Vec4f *pixels, int num_pixels) override
		{
			for (int i = 0; i < num_pixels; i++)
			{
				pixels[i] = Vec4f(pixels[i].r * pixels[i].a, pixels[i].g * pixels[i].a, pixels[i].b * pixels[i].a, pixels[i].a);
			}
		}
	};

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2

	class PixelFilterPremultiplyAlphaSSE2 : public PixelFilter
	{
	public:
		void filter(Vec4f *pixels, int num_pixels) override
		{
			__m128 alpha_mask = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0xffffffff));
			for (int i = 0; i < num_pixels; i++)
			{
				__m128 pixel = _mm_loadu_ps(reinterpret_cast<float*>(pixels + i));

				__m128 alpha = _mm_shuffle_ps(pixel, pixel, _MM_SHUFFLE(3, 3, 3, 3));
				pixel = _mm_or_ps(_mm_and_ps(pixel, alpha_mask), _mm_andnot_ps(_mm_mul_ps(pixel, alpha), alpha_mask));

				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i), pixel);
			}
		}
	};
#endif

}
