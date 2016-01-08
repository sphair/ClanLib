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
	class PixelFilterGamma : public PixelFilter
	{
	public:
		PixelFilterGamma(float gamma) : gamma(gamma) { }
		void filter(Vec4f *pixels, int num_pixels) override
		{
			for (int i = 0; i < num_pixels; i++)
			{
				pixels[i] = Vec4f(std::pow(pixels[i].x, gamma), std::pow(pixels[i].y, gamma), std::pow(pixels[i].z, gamma), pixels[i].w);
			}
		}

	private:
		float gamma;
	};

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
	class PixelFilterGammaSSE2 : public PixelFilter
	{
	public:
		PixelFilterGammaSSE2(float gamma) : gamma(gamma) { }
		void filter(Vec4f *pixels, int num_pixels) override
		{
			__m128 b = _mm_set1_ps(gamma);

			int sse_length = (num_pixels / 4) * 4;
			for (int i = 0; i < sse_length; i += 4)
			{
				__m128 red = _mm_loadu_ps(reinterpret_cast<float*>(pixels + i));
				__m128 green = _mm_loadu_ps(reinterpret_cast<float*>(pixels + i + 1));
				__m128 blue = _mm_loadu_ps(reinterpret_cast<float*>(pixels + i + 2));
				__m128 alpha = _mm_loadu_ps(reinterpret_cast<float*>(pixels + i + 3));
				_MM_TRANSPOSE4_PS(red, green, blue, alpha);

				red = powf4(red, b);
				green = powf4(green, b);
				blue = powf4(blue, b);

				_MM_TRANSPOSE4_PS(red, green, blue, alpha);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i), red);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 1), green);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 2), blue);
				_mm_storeu_ps(reinterpret_cast<float*>(pixels + i + 3), alpha);
			}

			for (int i = sse_length; i < num_pixels; i++)
			{
				pixels[i] = Vec4f(std::pow(pixels[i].x, gamma), std::pow(pixels[i].y, gamma), std::pow(pixels[i].z, gamma), pixels[i].w);
			}
		}

		// Polynomial SSE2 pow(a,b)
		// Minimax Approximations and the Remez Algorithm
		// http://jrfonseca.blogspot.dk/2008/09/fast-sse2-pow-tables-or-polynomials.html

		#define EXP_POLY_DEGREE 3

		#define POLY0(x, c0) _mm_set1_ps(c0)
		#define POLY1(x, c0, c1) _mm_add_ps(_mm_mul_ps(POLY0(x, c1), x), _mm_set1_ps(c0))
		#define POLY2(x, c0, c1, c2) _mm_add_ps(_mm_mul_ps(POLY1(x, c1, c2), x), _mm_set1_ps(c0))
		#define POLY3(x, c0, c1, c2, c3) _mm_add_ps(_mm_mul_ps(POLY2(x, c1, c2, c3), x), _mm_set1_ps(c0))
		#define POLY4(x, c0, c1, c2, c3, c4) _mm_add_ps(_mm_mul_ps(POLY3(x, c1, c2, c3, c4), x), _mm_set1_ps(c0))
		#define POLY5(x, c0, c1, c2, c3, c4, c5) _mm_add_ps(_mm_mul_ps(POLY4(x, c1, c2, c3, c4, c5), x), _mm_set1_ps(c0))

		static inline __m128 powf4(__m128 x, __m128 y)
		{
			return exp2f4(_mm_mul_ps(log2f4(x), y));
		}

		static inline __m128 exp2f4(__m128 x)
		{
			__m128i ipart;
			__m128 fpart, expipart, expfpart;

			x = _mm_min_ps(x, _mm_set1_ps(129.00000f));
			x = _mm_max_ps(x, _mm_set1_ps(-126.99999f));

			/* ipart = int(x - 0.5) */
			ipart = _mm_cvtps_epi32(_mm_sub_ps(x, _mm_set1_ps(0.5f)));

			/* fpart = x - ipart */
			fpart = _mm_sub_ps(x, _mm_cvtepi32_ps(ipart));

			/* expipart = (float) (1 << ipart) */
			expipart = _mm_castsi128_ps(_mm_slli_epi32(_mm_add_epi32(ipart, _mm_set1_epi32(127)), 23));

			/* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
			#if EXP_POLY_DEGREE == 5
			expfpart = POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
			#elif EXP_POLY_DEGREE == 4
			expfpart = POLY4(fpart, 1.0000026f, 6.9300383e-1f, 2.4144275e-1f, 5.2011464e-2f, 1.3534167e-2f);
			#elif EXP_POLY_DEGREE == 3
			expfpart = POLY3(fpart, 9.9992520e-1f, 6.9583356e-1f, 2.2606716e-1f, 7.8024521e-2f);
			#elif EXP_POLY_DEGREE == 2
			expfpart = POLY2(fpart, 1.0017247f, 6.5763628e-1f, 3.3718944e-1f);
			#else
			#error
			#endif

			return _mm_mul_ps(expipart, expfpart);
		}

		#define LOG_POLY_DEGREE 5

		static inline __m128 log2f4(__m128 x)
		{
			__m128i exp = _mm_set1_epi32(0x7F800000);
			__m128i mant = _mm_set1_epi32(0x007FFFFF);

			__m128 one = _mm_set1_ps(1.0f);
			__m128i i = _mm_castps_si128(x);
			__m128 e = _mm_cvtepi32_ps(_mm_sub_epi32(_mm_srli_epi32(_mm_and_si128(i, exp), 23), _mm_set1_epi32(127)));
			__m128 m = _mm_or_ps(_mm_castsi128_ps(_mm_and_si128(i, mant)), one);
			__m128 p;

			/* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
			#if LOG_POLY_DEGREE == 6
			p = POLY5( m, 3.1157899f, -3.3241990f, 2.5988452f, -1.2315303f,  3.1821337e-1f, -3.4436006e-2f);
			#elif LOG_POLY_DEGREE == 5
			p = POLY4(m, 2.8882704548164776201f, -2.52074962577807006663f, 1.48116647521213171641f, -0.465725644288844778798f, 0.0596515482674574969533f);
			#elif LOG_POLY_DEGREE == 4
			p = POLY3(m, 2.61761038894603480148f, -1.75647175389045657003f, 0.688243882994381274313f, -0.107254423828329604454f);
			#elif LOG_POLY_DEGREE == 3
			p = POLY2(m, 2.28330284476918490682f, -1.04913055217340124191f, 0.204446009836232697516f);
			#else
			#error
			#endif

			/* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
			p = _mm_mul_ps(p, _mm_sub_ps(m, one));

			return _mm_add_ps(p, e);
		}

	private:
		float gamma;
	};
#endif
}
