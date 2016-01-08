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

#include "Sound/precomp.h"
#include "API/Sound/sound_sse.h"
#include "API/Core/System/system.h"
#include <cstdlib>
#include <cstring>

#ifndef CL_DISABLE_SSE2
#include <emmintrin.h>
#endif

#ifdef __MINGW32__
#include <malloc.h>
#endif

namespace clan
{
	void *SoundSSE::aligned_alloc(int size)
	{
		return System::aligned_alloc(size, 16);
	}

	void SoundSSE::aligned_free(void *ptr)
	{
		return System::aligned_free(ptr);
	}

	void SoundSSE::unpack_16bit_stereo(short *input, int size, float *output[2])
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 8) * 8;

		__m128i zero = _mm_setzero_si128();
		__m128 constant1 = _mm_set1_ps(1.0f / 32768.0f);
		for (int i = 0; i < sse_size; i += 8)
		{
			__m128i isamples = _mm_loadu_si128((__m128i*)(input + i));

			__m128 samples0 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(zero, isamples), 16));
			__m128 samples1 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(zero, isamples), 16));
			samples0 = _mm_mul_ps(samples0, constant1);
			samples1 = _mm_mul_ps(samples1, constant1);

			__m128 tmp0, tmp1;
			tmp0 = _mm_shuffle_ps(samples0, samples1, _MM_SHUFFLE(2, 0, 2, 0));
			tmp1 = _mm_shuffle_ps(samples0, samples1, _MM_SHUFFLE(3, 1, 3, 1));
			_mm_storeu_ps(output[0] + i / 2, tmp0);
			_mm_storeu_ps(output[1] + i / 2, tmp1);
		}
#else
		const int sse_size = 0;
#endif
		// unpack remaining
		for (int i = sse_size; i < size; i += 2)
		{
			output[0][i / 2] = ((float)input[i]) / 32767.0f;
			output[1][i / 2] = ((float)input[i + 1]) / 32767.0f;
		}
	}

	void SoundSSE::unpack_16bit_mono(short *input, int size, float *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 8) * 8;

		__m128i zero = _mm_setzero_si128();
		__m128 constant1 = _mm_set1_ps(1.0f / 32767.0f);
		for (int i = 0; i < sse_size; i += 8)
		{
			__m128i isamples = _mm_loadu_si128((__m128i*)(input + i));
			__m128 samples0 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(zero, isamples), 16));
			__m128 samples1 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(zero, isamples), 16));
			samples0 = _mm_mul_ps(samples0, constant1);
			samples1 = _mm_mul_ps(samples1, constant1);
			_mm_storeu_ps(output + i + 0, samples0);
			_mm_storeu_ps(output + i + 4, samples1);
		}
#else
		const int sse_size = 0;
#endif

		// unpack remaining
		for (int i = sse_size; i < size; i++)
		{
			output[i] = ((float)input[i]) / 32767.0f;
		}
	}

	void SoundSSE::unpack_8bit_stereo(unsigned char *input, int size, float *output[2])
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 16) * 16;

		__m128i zero = _mm_setzero_si128();
		__m128 constant1 = _mm_set1_ps(1.0f / 128.0f);
		__m128i constant2 = _mm_set1_epi16(128);
		for (int i = 0; i < sse_size; i += 16)
		{
			__m128i isamples = _mm_loadu_si128((__m128i*)(input + i));
			__m128i isamples0 = _mm_sub_epi16(_mm_unpacklo_epi8(isamples, zero), constant2);
			__m128i isamples1 = _mm_sub_epi16(_mm_unpackhi_epi8(isamples, zero), constant2);
			__m128 samples0 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(zero, isamples0), 16));
			__m128 samples1 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(zero, isamples0), 16));
			__m128 samples2 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(zero, isamples1), 16));
			__m128 samples3 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(zero, isamples1), 16));
			samples0 = _mm_mul_ps(samples0, constant1);
			samples1 = _mm_mul_ps(samples1, constant1);
			samples2 = _mm_mul_ps(samples2, constant1);
			samples3 = _mm_mul_ps(samples3, constant1);

			__m128 tmp0, tmp1, tmp2, tmp3;
			tmp0 = _mm_shuffle_ps(samples0, samples1, _MM_SHUFFLE(2, 0, 2, 0));
			tmp1 = _mm_shuffle_ps(samples0, samples1, _MM_SHUFFLE(3, 1, 3, 1));
			tmp2 = _mm_shuffle_ps(samples2, samples3, _MM_SHUFFLE(2, 0, 2, 0));
			tmp3 = _mm_shuffle_ps(samples2, samples3, _MM_SHUFFLE(3, 1, 3, 1));

			_mm_storeu_ps(output[0] + i / 2, tmp0);
			_mm_storeu_ps(output[1] + i / 2, tmp1);
			_mm_storeu_ps(output[0] + i / 2 + 4, tmp2);
			_mm_storeu_ps(output[1] + i / 2 + 4, tmp3);
		}
#else
		const int sse_size = 0;
#endif
		// unpack remaining
		for (int i = sse_size; i < size; i += 2)
		{
			int value = input[i];
			output[0][i / 2] = ((float)(value - 128)) / 128.0f;

			value = input[i + 1];
			output[1][i / 2] = ((float)(value - 128)) / 128.0f;
		}
	}

	void SoundSSE::unpack_8bit_mono(unsigned char *input, int size, float *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 16) * 16;

		__m128i zero = _mm_setzero_si128();
		__m128 constant1 = _mm_set1_ps(1.0f / 128.0f);
		__m128i constant2 = _mm_set1_epi16(128);
		for (int i = 0; i < sse_size; i += 16)
		{
			__m128i isamples = _mm_loadu_si128((__m128i*)(input + i));
			__m128i isamples0 = _mm_sub_epi16(_mm_unpacklo_epi8(isamples, zero), constant2);
			__m128i isamples1 = _mm_sub_epi16(_mm_unpackhi_epi8(isamples, zero), constant2);
			__m128 samples0 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(zero, isamples0), 16));
			__m128 samples1 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(zero, isamples0), 16));
			__m128 samples2 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(zero, isamples1), 16));
			__m128 samples3 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(zero, isamples1), 16));
			samples0 = _mm_mul_ps(samples0, constant1);
			samples1 = _mm_mul_ps(samples1, constant1);
			samples2 = _mm_mul_ps(samples2, constant1);
			samples3 = _mm_mul_ps(samples3, constant1);
			_mm_storeu_ps(output + i + 0, samples0);
			_mm_storeu_ps(output + i + 4, samples1);
			_mm_storeu_ps(output + i + 8, samples2);
			_mm_storeu_ps(output + i + 12, samples3);
		}
#else
		const int sse_size = 0;
#endif
		// unpack remaining
		for (int i = sse_size; i < size; i++)
		{
			int value = input[i];
			output[i] = ((float)(value - 128)) / 128.0f;
		}
	}

	void SoundSSE::pack_16bit_stereo(float *input[2], int size, short *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;

		__m128 constant1 = _mm_set1_ps(32767);
		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 samples0 = _mm_loadu_ps(input[0] + i);
			__m128 samples1 = _mm_loadu_ps(input[1] + i);
			samples0 = _mm_mul_ps(samples0, constant1);
			samples1 = _mm_mul_ps(samples1, constant1);
			__m128 tmp0, tmp1;
			tmp0 = _mm_unpacklo_ps(samples0, samples1);
			tmp1 = _mm_unpackhi_ps(samples0, samples1);
			__m128i isamples0 = _mm_cvtps_epi32(tmp0);
			__m128i isamples1 = _mm_cvtps_epi32(tmp1);
			__m128i isamples = _mm_packs_epi32(isamples0, isamples1);
			_mm_storeu_si128((__m128i*)(output + i * 2), isamples);
		}

#else
		const int sse_size = 0;
#endif

		// Pack remaining
		for (int i = sse_size; i < size; i++)
		{
			output[i * 2] = input[0][i] * 32767;
			output[i * 2 + 1] = input[1][i] * 32767;
		}
	}

	void SoundSSE::pack_float_stereo(float *input[2], int size, float *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;

		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 samples0 = _mm_loadu_ps(input[0] + i);
			__m128 samples1 = _mm_loadu_ps(input[1] + i);
			__m128 tmp0, tmp1;
			tmp0 = _mm_unpacklo_ps(samples0, samples1);
			tmp1 = _mm_unpackhi_ps(samples0, samples1);
			_mm_storeu_ps(output + i * 2, tmp0);
			_mm_storeu_ps(output + i * 2 + 4, tmp1);
		}

#else
		const int sse_size = 0;
#endif

		// Pack remaining
		for (int i = sse_size; i < size; i++)
		{
			output[i * 2] = input[0][i];
			output[i * 2 + 1] = input[1][i];
		}
	}

	void SoundSSE::copy_float(float *input, int size, float *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;

		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 s = _mm_loadu_ps(input + i);
			_mm_storeu_ps(output + i, s);
		}

#else
		const int sse_size = 0;
#endif

		// Copy remaining
		for (int i = sse_size; i < size; i++)
			output[i] = input[i];
	}

	void SoundSSE::multiply_float(float *channel, int size, float volume)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;

		__m128 volume0 = _mm_set1_ps(volume);
		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 s = _mm_loadu_ps(channel + i);
			s = _mm_mul_ps(s, volume0);
			_mm_storeu_ps(channel + i, s);
		}
#else
		const int sse_size = 0;
#endif

		for (int i = sse_size; i < size; i++)
			channel[i] *= volume;
	}

	void SoundSSE::set_float(float *channel, int size, float value)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;

		__m128 value0 = _mm_set1_ps(value);
		for (int i = 0; i < sse_size; i += 4)
		{
			_mm_storeu_ps(channel + i, value0);
		}
#else
		const int sse_size = 0;
#endif

		for (int i = sse_size; i < size; i++)
			channel[i] = value;
	}

	void SoundSSE::mix_one_to_one(float *input, int size, float *output, float volume)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;
		__m128 volume0 = _mm_set1_ps(volume);
		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 sample0 = _mm_loadu_ps(input + i);
			__m128 sample1 = _mm_loadu_ps(output + i);
			_mm_storeu_ps(output + i, _mm_add_ps(_mm_mul_ps(sample0, volume0), sample1));
		}

#else
		const int sse_size = 0;
#endif

		for (int i = sse_size; i < size; i++)
		{
			output[i] += input[i] * volume;
		}
	}

	void SoundSSE::mix_one_to_many(float *input, int size, float **output, float *volume, int channels)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;
		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 sample0 = _mm_loadu_ps(input + i);
			for (int j = 0; j < channels; j++)
			{
				__m128 sample1 = _mm_loadu_ps(output[j] + i);
				__m128 volume0 = _mm_set1_ps(volume[j]);
				_mm_storeu_ps(output[j] + i, _mm_add_ps(_mm_mul_ps(sample0, volume0), sample1));
			}
		}

#else
		const int sse_size = 0;
#endif

		for (int i = sse_size; i < size; i++)
		{
			float sample0 = input[i];
			for (int j = 0; j < channels; j++)
			{
				output[j][i] += sample0 * volume[j];
			}
		}
	}

	void SoundSSE::mix_many_to_one(float **input, float *volume, int channels, int size, float *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;
		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 sample0 = _mm_loadu_ps(output + i);
			for (int j = 0; j < channels; j++)
			{
				__m128 sample1 = _mm_loadu_ps(input[j] + i);
				__m128 volume0 = _mm_set1_ps(volume[j]);
				sample0 = _mm_add_ps(_mm_mul_ps(sample1, volume0), sample0);
			}
			_mm_storeu_ps(output + i, sample0);
		}

#else
		const int sse_size = 0;
#endif

		for (int i = sse_size; i < size; i++)
		{
			float sample0 = output[i];
			for (int j = 0; j < channels; j++)
			{
				sample0 += input[j][i] * volume[j];
			}
			output[i] = sample0;
		}
	}

	void SoundSSE::unpack_float_stereo(float *input, int size, float *output[2])
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 8) * 8;

		for (int i = 0; i < sse_size; i += 8)
		{
			__m128 samples0 = _mm_loadu_ps(input + i);
			__m128 samples1 = _mm_loadu_ps(input + i + 4);
			__m128 tmp0 = _mm_shuffle_ps(samples0, samples1, _MM_SHUFFLE(2, 0, 2, 0));
			__m128 tmp1 = _mm_shuffle_ps(samples0, samples1, _MM_SHUFFLE(3, 1, 3, 1));

			_mm_storeu_ps(output[0] + i / 2, tmp0);
			_mm_storeu_ps(output[1] + i / 2, tmp1);
		}

#else
		const int sse_size = 0;
#endif

		// unpack remaining
		for (int i = sse_size; i < size; i += 2)
		{
			output[0][i / 2] = input[i];
			output[1][i / 2] = input[i + 1];
		}
	}

	void SoundSSE::unpack_float_mono(float *input, int size, float *output)
	{
#ifndef CL_DISABLE_SSE2
		int sse_size = (size / 4) * 4;

		for (int i = 0; i < sse_size; i += 4)
		{
			__m128 samples0 = _mm_loadu_ps(input + i);
			_mm_storeu_ps(output + i, samples0);
		}

#else
		const int sse_size = 0;
#endif

		// unpack remaining
		if (sse_size < size)
			memcpy(output, input, (size - sse_size)*sizeof(float));
	}
}
