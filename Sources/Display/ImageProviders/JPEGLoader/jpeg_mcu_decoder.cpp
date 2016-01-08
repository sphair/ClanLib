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
*/

#include "Display/precomp.h"
#include "jpeg_mcu_decoder.h"
#include "jpeg_loader.h"
#include "API/Core/System/system.h"

#ifndef CL_DISABLE_SSE2
#ifndef ARM_PLATFORM
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#endif // not CL_DISABLE_SSE2

namespace clan
{
	JPEGMCUDecoder::JPEGMCUDecoder(JPEGLoader *loader)
		: loader(loader)
	{
		try
		{
			for (size_t c = 0; c < loader->start_of_frame.components.size(); c++)
				channels.push_back((unsigned char *)System::aligned_alloc(loader->mcu_x*loader->mcu_y * 64, 16));

			/* For float AA&N IDCT method, divisors are equal to quantization
			 * coefficients scaled by scalefactor[row]*scalefactor[col], where
			 *   scalefactor[0] = 1
			 *   scalefactor[k] = cos(k*PI/16) * sqrt(2)    for k=1..7
			 */
			static const float aanscalefactor[8] =
			{
				1.0f, 1.387039845f, 1.306562965f, 1.175875602f,
				1.0f, 0.785694958f, 0.541196100f, 0.275899379f
			};

			for (size_t c = 0; c < loader->start_of_frame.components.size(); c++)
			{
				quant.push_back((float*)System::aligned_alloc(64 * sizeof(float), 16));
				const JPEGQuantizationTable &qtable = loader->quantization_tables[loader->start_of_frame.components[c].quantization_table_selector];
				for (int y = 0; y < 8; y++)
					for (int x = 0; x < 8; x++)
						quant[c][x + y * 8] = aanscalefactor[x] * aanscalefactor[y] * qtable.values[x + y * 8];
			}
		}
		catch (...)
		{
			for (auto & elem : channels)
				System::aligned_free(elem);
			for (auto & elem : quant)
				System::aligned_free(elem);
			throw;
		}
	}

	JPEGMCUDecoder::~JPEGMCUDecoder()
	{
		for (auto & elem : channels)
			System::aligned_free(elem);
		for (auto & elem : quant)
			System::aligned_free(elem);
	}

	void JPEGMCUDecoder::decode(int block)
	{
		for (size_t c = 0; c < channels.size(); c++)
		{
			int scale_x = loader->start_of_frame.components[c].horz_sampling_factor;
			int scale_y = loader->start_of_frame.components[c].vert_sampling_factor;
			int block_size = scale_x * scale_y;
			for (int dct_y = 0; dct_y < scale_y; dct_y++)
			{
				for (int dct_x = 0; dct_x < scale_x; dct_x++)
				{
					short *dct = loader->component_dcts[c].get(block * block_size + dct_x + dct_y * scale_x);

#ifdef CL_DISABLE_SSE2
					idct(dct, channels[c]+dct_x*8+dct_y*scale_x*64, scale_x*8, quant[c]);
#else

#ifndef ARM_PLATFORM
					idct_sse(dct, channels[c] + dct_x * 8 + dct_y*scale_x * 64, scale_x * 8, quant[c]);
#else
					idct(dct, channels[c]+dct_x*8+dct_y*scale_x*64, scale_x*8, quant[c]);
#endif
#endif // not CL_DISABLE_SSE2


				}
			}
		}
	}

	void JPEGMCUDecoder::idct(short *inptr, unsigned char *outptr, int pitch, float *quantptr)
	{
		float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
		float tmp10, tmp11, tmp12, tmp13;
		float z5, z10, z11, z12, z13;
		float * wsptr;
		int ctr;
		float workspace[64]; /* buffers data between passes */

		/* Pass 1: process columns from input, store into work array. */

		wsptr = workspace;
		for (ctr = 8; ctr > 0; ctr--)
		{
			/* Due to quantization, we will usually find that many of the input
			 * coefficients are zero, especially the AC terms.  We can exploit this
			 * by short-circuiting the IDCT calculation for any column in which all
			 * the AC terms are zero.  In that case each output is equal to the
			 * DC coefficient (with scale factor as needed).
			 * With typical images and quantization tables, half or more of the
			 * column DCT calculations can be simplified this way.
			 */

			/* Even part */

			tmp0 = (float)inptr[8 * 0] * quantptr[8 * 0];
			tmp1 = (float)inptr[8 * 2] * quantptr[8 * 2];
			tmp2 = (float)inptr[8 * 4] * quantptr[8 * 4];
			tmp3 = (float)inptr[8 * 6] * quantptr[8 * 6];

			tmp10 = tmp0 + tmp2;	/* phase 3 */
			tmp11 = tmp0 - tmp2;

			tmp13 = tmp1 + tmp3;	/* phases 5-3 */
			tmp12 = (tmp1 - tmp3) * 1.414213562f - tmp13; /* 2*c4 */

			tmp0 = tmp10 + tmp13;	/* phase 2 */
			tmp3 = tmp10 - tmp13;
			tmp1 = tmp11 + tmp12;
			tmp2 = tmp11 - tmp12;

			/* Odd part */

			tmp4 = inptr[8 * 1] * quantptr[8 * 1];
			tmp5 = inptr[8 * 3] * quantptr[8 * 3];
			tmp6 = inptr[8 * 5] * quantptr[8 * 5];
			tmp7 = inptr[8 * 7] * quantptr[8 * 7];

			z13 = tmp6 + tmp5;		/* phase 6 */
			z10 = tmp6 - tmp5;
			z11 = tmp4 + tmp7;
			z12 = tmp4 - tmp7;

			tmp7 = z11 + z13;		/* phase 5 */
			tmp11 = (z11 - z13) * 1.414213562f; /* 2*c4 */

			z5 = (z10 + z12) * 1.847759065f; /* 2*c2 */
			tmp10 = 1.082392200f * z12 - z5; /* 2*(c2-c6) */
			tmp12 = -2.613125930f * z10 + z5; /* -2*(c2+c6) */

			tmp6 = tmp12 - tmp7;	/* phase 2 */
			tmp5 = tmp11 - tmp6;
			tmp4 = tmp10 + tmp5;

			wsptr[8 * 0] = tmp0 + tmp7;
			wsptr[8 * 7] = tmp0 - tmp7;
			wsptr[8 * 1] = tmp1 + tmp6;
			wsptr[8 * 6] = tmp1 - tmp6;
			wsptr[8 * 2] = tmp2 + tmp5;
			wsptr[8 * 5] = tmp2 - tmp5;
			wsptr[8 * 4] = tmp3 + tmp4;
			wsptr[8 * 3] = tmp3 - tmp4;

			inptr++;			/* advance pointers to next column */
			quantptr++;
			wsptr++;
		}

		/* Pass 2: process rows from work array, store into output array. */
		/* Note that we must descale the results by a factor of 8 == 2**3. */

		wsptr = workspace;
		for (ctr = 0; ctr < 8; ctr++)
		{
			/* Rows of zeroes can be exploited in the same way as we did with columns.
			 * However, the column calculation has created many nonzero AC terms, so
			 * the simplification applies less often (typically 5% to 10% of the time).
			 * And testing floats for zero is relatively expensive, so we don't bother.
			 */

			/* Even part */

			tmp10 = wsptr[0] + wsptr[4];
			tmp11 = wsptr[0] - wsptr[4];

			tmp13 = wsptr[2] + wsptr[6];
			tmp12 = (wsptr[2] - wsptr[6]) * 1.414213562f - tmp13;

			tmp0 = tmp10 + tmp13;
			tmp3 = tmp10 - tmp13;
			tmp1 = tmp11 + tmp12;
			tmp2 = tmp11 - tmp12;

			/* Odd part */

			z13 = wsptr[5] + wsptr[3];
			z10 = wsptr[5] - wsptr[3];
			z11 = wsptr[1] + wsptr[7];
			z12 = wsptr[1] - wsptr[7];

			tmp7 = z11 + z13;
			tmp11 = (z11 - z13) * 1.414213562f;

			z5 = (z10 + z12) * 1.847759065f; /* 2*c2 */
			tmp10 = 1.082392200f * z12 - z5; /* 2*(c2-c6) */
			tmp12 = -2.613125930f * z10 + z5; /* -2*(c2+c6) */

			tmp6 = tmp12 - tmp7;
			tmp5 = tmp11 - tmp6;
			tmp4 = tmp10 + tmp5;

			/* Final output stage: scale down by a factor of 8 and range-limit */

#define DESCALE (1.0f / 8.0f)
			outptr[0] = float_to_int((tmp0 + tmp7) * DESCALE);
			outptr[7] = float_to_int((tmp0 - tmp7) * DESCALE);
			outptr[1] = float_to_int((tmp1 + tmp6) * DESCALE);
			outptr[6] = float_to_int((tmp1 - tmp6) * DESCALE);
			outptr[2] = float_to_int((tmp2 + tmp5) * DESCALE);
			outptr[5] = float_to_int((tmp2 - tmp5) * DESCALE);
			outptr[4] = float_to_int((tmp3 + tmp4) * DESCALE);
			outptr[3] = float_to_int((tmp3 - tmp4) * DESCALE);

			wsptr += 8;		/* advance pointer to next row */
			outptr += pitch;
		}
	}

#ifndef CL_DISABLE_SSE2

#ifndef ARM_PLATFORM
	void JPEGMCUDecoder::idct_sse(short *inptr, unsigned char *outptr, int pitch, float *quantptr)
	{
		__m128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
		__m128 tmp10, tmp11, tmp12, tmp13;
		__m128 z5, z10, z11, z12, z13;
		float * wsptr;
		int ctr;
#ifdef WIN32
		__declspec(align(16)) float workspace[64]; /* buffers data between passes */
#else
		__attribute__ ((aligned (16)))  float workspace[64]; /* buffers data between passes */
#endif

		__m128 constant1 = _mm_set1_ps(1.414213562f);
		__m128 constant2 = _mm_set1_ps(1.847759065f);
		__m128 constant3 = _mm_set1_ps(1.082392200f);
		__m128 constant4 = _mm_set1_ps(-2.613125930f);
		__m128 constant5 = _mm_set1_ps(128.0f);
		__m128 descale = _mm_set1_ps(1.0f / 8.0f);

		/* Pass 1: process columns from input, store into work array. */

		wsptr = workspace;
		for (ctr = 8; ctr > 0; ctr -= 4)
		{
			/* Due to quantization, we will usually find that many of the input
			 * coefficients are zero, especially the AC terms.  We can exploit this
			 * by short-circuiting the IDCT calculation for any column in which all
			 * the AC terms are zero.  In that case each output is equal to the
			 * DC coefficient (with scale factor as needed).
			 * With typical images and quantization tables, half or more of the
			 * column DCT calculations can be simplified this way.
			 */

			/* Even part */

			tmp0 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 0])), 16)), _mm_load_ps(&quantptr[8 * 0]));
			tmp1 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 2])), 16)), _mm_load_ps(&quantptr[8 * 2]));
			tmp2 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 4])), 16)), _mm_load_ps(&quantptr[8 * 4]));
			tmp3 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 6])), 16)), _mm_load_ps(&quantptr[8 * 6]));

			tmp10 = _mm_add_ps(tmp0, tmp2); /* phase 3 */
			tmp11 = _mm_sub_ps(tmp0, tmp2);

			tmp13 = _mm_add_ps(tmp1, tmp3); /* phases 5-3 */
			tmp12 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(tmp1, tmp3), constant1), tmp13);

			tmp0 = _mm_add_ps(tmp10, tmp13); /* phase 2 */
			tmp3 = _mm_sub_ps(tmp10, tmp13);
			tmp1 = _mm_add_ps(tmp11, tmp12);
			tmp2 = _mm_sub_ps(tmp11, tmp12);

			/* Odd part */

			tmp4 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 1])), 16)), _mm_load_ps(&quantptr[8 * 1]));
			tmp5 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 3])), 16)), _mm_load_ps(&quantptr[8 * 3]));
			tmp6 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 5])), 16)), _mm_load_ps(&quantptr[8 * 5]));
			tmp7 = _mm_mul_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), _mm_loadl_epi64((__m128i*)&inptr[8 * 7])), 16)), _mm_load_ps(&quantptr[8 * 7]));

			z13 = _mm_add_ps(tmp6, tmp5); /* phase 6 */
			z10 = _mm_sub_ps(tmp6, tmp5);
			z11 = _mm_add_ps(tmp4, tmp7);
			z12 = _mm_sub_ps(tmp4, tmp7);

			tmp7 = _mm_add_ps(z11, z13); /* phase 5 */
			tmp11 = _mm_mul_ps(_mm_sub_ps(z11, z13), constant1); /* 2*c4 */

			z5 = _mm_mul_ps(_mm_add_ps(z10, z12), constant2);    /* 2*c2 */
			tmp10 = _mm_sub_ps(_mm_mul_ps(constant3, z12), z5);  /* 2*(c2-c6) */
			tmp12 = _mm_add_ps(_mm_mul_ps(constant4, z10), z5);  /* -2*(c2+c6) */

			tmp6 = _mm_sub_ps(tmp12, tmp7); /* phase 2 */
			tmp5 = _mm_sub_ps(tmp11, tmp6);
			tmp4 = _mm_add_ps(tmp10, tmp5);

			_mm_store_ps(&wsptr[8 * 0], _mm_add_ps(tmp0, tmp7));
			_mm_store_ps(&wsptr[8 * 7], _mm_sub_ps(tmp0, tmp7));
			_mm_store_ps(&wsptr[8 * 1], _mm_add_ps(tmp1, tmp6));
			_mm_store_ps(&wsptr[8 * 6], _mm_sub_ps(tmp1, tmp6));
			_mm_store_ps(&wsptr[8 * 2], _mm_add_ps(tmp2, tmp5));
			_mm_store_ps(&wsptr[8 * 5], _mm_sub_ps(tmp2, tmp5));
			_mm_store_ps(&wsptr[8 * 4], _mm_add_ps(tmp3, tmp4));
			_mm_store_ps(&wsptr[8 * 3], _mm_sub_ps(tmp3, tmp4));

			inptr += 4;			/* advance pointers to next column */
			quantptr += 4;
			wsptr += 4;
		}

		/* Pass 2: process rows from work array, store into output array. */
		/* Note that we must descale the results by a factor of 8 == 2**3. */

		/* Note that we must descale the results by a factor of 8 == 2**3. */

		wsptr = workspace;
		for (ctr = 0; ctr < 2; ctr++)
		{
			__m128 w0 = _mm_load_ps(wsptr + 0 * 8);
			__m128 w4 = _mm_load_ps(wsptr + 0 * 8 + 4);
			__m128 w1 = _mm_load_ps(wsptr + 1 * 8);
			__m128 w5 = _mm_load_ps(wsptr + 1 * 8 + 4);
			__m128 w2 = _mm_load_ps(wsptr + 2 * 8);
			__m128 w6 = _mm_load_ps(wsptr + 2 * 8 + 4);
			__m128 w3 = _mm_load_ps(wsptr + 3 * 8);
			__m128 w7 = _mm_load_ps(wsptr + 3 * 8 + 4);

			_MM_TRANSPOSE4_PS(w0, w1, w2, w3);
			_MM_TRANSPOSE4_PS(w4, w5, w6, w7);


			/* Rows of zeros can be exploited in the same way as we did with columns.
			 * However, the column calculation has created many nonzero AC terms, so
			 * the simplification applies less often (typically 5% to 10% of the time).
			 * And testing floats for zero is relatively expensive, so we don't bother.
			 */

			/* Even part */

			tmp10 = _mm_add_ps(w0, w4); // wsptr[0] + wsptr[4];
			tmp11 = _mm_sub_ps(w0, w4); // wsptr[0] - wsptr[4];

			tmp13 = _mm_add_ps(w2, w6); // wsptr[2] + wsptr[6];
			tmp12 = _mm_sub_ps(_mm_mul_ps(_mm_sub_ps(w2, w6), constant1), tmp13); // (wsptr[2] - wsptr[6]) * ((float) 1.414213562) - tmp13;

			tmp0 = _mm_add_ps(tmp10, tmp13);
			tmp3 = _mm_sub_ps(tmp10, tmp13);
			tmp1 = _mm_add_ps(tmp11, tmp12);
			tmp2 = _mm_sub_ps(tmp11, tmp12);

			/* Odd part */

			z13 = _mm_add_ps(w5, w3); // wsptr[5] + wsptr[3];
			z10 = _mm_sub_ps(w5, w3); // wsptr[5] - wsptr[3];
			z11 = _mm_add_ps(w1, w7); // wsptr[1] + wsptr[7];
			z12 = _mm_sub_ps(w1, w7); // wsptr[1] - wsptr[7];

			tmp7 = _mm_add_ps(z11, z13);
			tmp11 = _mm_mul_ps(_mm_sub_ps(z11, z13), constant1);

			z5 = _mm_mul_ps(_mm_add_ps(z10, z12), constant2);   /* 2*c2 */
			tmp10 = _mm_sub_ps(_mm_mul_ps(constant3, z12), z5); /* 2*(c2-c6) */
			tmp12 = _mm_add_ps(_mm_mul_ps(constant4, z10), z5); /* -2*(c2+c6) */

			tmp6 = _mm_sub_ps(tmp12, tmp7);
			tmp5 = _mm_sub_ps(tmp11, tmp6);
			tmp4 = _mm_add_ps(tmp10, tmp5);

			/* Final output stage: scale down by a factor of 8 and range-limit */

			__m128 output0 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(tmp0, tmp7), descale), constant5);
			__m128 output7 = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(tmp0, tmp7), descale), constant5);
			__m128 output1 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(tmp1, tmp6), descale), constant5);
			__m128 output6 = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(tmp1, tmp6), descale), constant5);
			__m128 output2 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(tmp2, tmp5), descale), constant5);
			__m128 output5 = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(tmp2, tmp5), descale), constant5);
			__m128 output4 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(tmp3, tmp4), descale), constant5);
			__m128 output3 = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(tmp3, tmp4), descale), constant5);

			_MM_TRANSPOSE4_PS(output0, output1, output2, output3);
			_MM_TRANSPOSE4_PS(output4, output5, output6, output7);

#define STORE_SSE_BYTE_CL_PITCH(ptr, pitch, o0, o1, o2, o3) \
					{ \
			__m128i tmpMerge0 = _mm_packs_epi32(_mm_cvttps_epi32(o0), _mm_cvttps_epi32(o1)); \
			__m128i tmpMerge1 = _mm_packs_epi32(_mm_cvttps_epi32(o2), _mm_cvttps_epi32(o3)); \
			__m128i t = _mm_packus_epi16(tmpMerge0, tmpMerge1); \
			_mm_storel_epi64((__m128i*)(ptr), t); \
			ptr+=pitch; \
			t = _mm_srli_si128(t, 8); \
			_mm_storel_epi64((__m128i*)(ptr), t); \
			ptr+=pitch; \
					}

			STORE_SSE_BYTE_CL_PITCH(outptr, pitch, output0, output4, output1, output5);
			STORE_SSE_BYTE_CL_PITCH(outptr, pitch, output2, output6, output3, output7);

			wsptr += 8 * 4; /* advance pointer to next row */
		}
	}
#endif
#endif // not CL_DISABLE_SSE2

	unsigned char JPEGMCUDecoder::float_to_int(float f)
	{
		unsigned char i;
		float v = f + 128.0f;
		if (v < 0.0f)
			i = 0;
		else if (v > 255.0f)
			i = 255;
		else
			i = (unsigned char)v;
		return i;
	}
}
