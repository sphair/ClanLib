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
*/

#include "Display/precomp.h"
#include "jpeg_rgb_decoder.h"
#include "jpeg_mcu_decoder.h"
#include "jpeg_loader.h"
#include "API/Core/System/system.h"

#ifndef CL_DISABLE_SSE2
#ifndef ARM_PLATFORM
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#endif

namespace clan
{
	JPEGRGBDecoder::JPEGRGBDecoder(JPEGLoader *loader)
		: loader(loader), mcu_x(0), mcu_y(0), pixels(nullptr)
	{
		mcu_x = loader->mcu_x;
		mcu_y = loader->mcu_y;
		try
		{
			pixels = (unsigned int *)System::aligned_alloc(mcu_x*mcu_y * 64 * 4, 16);
			for (auto & elem : loader->start_of_frame.components)
				channels.push_back((unsigned char *)System::aligned_alloc(mcu_x*mcu_y * 64, 16));
		}
		catch (...)
		{
			System::aligned_free(pixels);
			for (auto & elem : channels)
				System::aligned_free(elem);
			throw;
		}
	}

	JPEGRGBDecoder::~JPEGRGBDecoder()
	{
		System::aligned_free(pixels);
		for (auto & elem : channels)
			System::aligned_free(elem);
	}

	void JPEGRGBDecoder::decode(JPEGMCUDecoder *mcu_decoder)
	{
		upsample(mcu_decoder);

		switch (loader->get_colorspace())
		{
		case JPEGLoader::colorspace_grayscale:
			convert_monochrome();
			break;
		case JPEGLoader::colorspace_ycrcb:
#ifndef CL_DISABLE_SSE2
			if (System::detect_cpu_extension(System::sse2))
				convert_ycrcb_sse();
			else
				convert_ycrcb_float();
#else
			convert_ycrcb_float();
#endif
			break;
		case JPEGLoader::colorspace_rgb:
			convert_rgb();
			break;
		case JPEGLoader::colorspace_cmyk:
		case JPEGLoader::colorspace_ycck:
		default:
			throw Exception("Unsupported color space");
		}
	}

	void JPEGRGBDecoder::upsample(JPEGMCUDecoder *mcu_decoder)
	{
		int height = mcu_y * 8;
		int width = mcu_x * 8;

		for (size_t c = 0; c < channels.size(); c++)
		{
			int h = loader->start_of_frame.components[c].horz_sampling_factor;
			int v = loader->start_of_frame.components[c].vert_sampling_factor;
			const unsigned char *input = mcu_decoder->get_channel(c);
			unsigned char *output = channels[c];

			if (mcu_x == h && mcu_y == v)
			{
				memcpy(output, input, width*height);
			}
			else
			{
				int step_sx = (h << 16) / mcu_x;
				int step_sy = (v << 16) / mcu_y;
				int sy = step_sy >> 1;
				for (int y = 0; y < height; y++)
				{
					const unsigned char *input_line = input + (sy >> 16)*v * 8;
					int sx = step_sx >> 1;
					for (int x = 0; x < width; x++)
					{
						*(output++) = input_line[sx >> 16];
						sx += step_sx;
					}
					sy += step_sy;
				}
			}
		}
	}

	void JPEGRGBDecoder::convert_monochrome()
	{
		int height = mcu_y * 8;
		int width = mcu_x * 8;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				unsigned int Y = channels[0][x + y*width];
				pixels[x + y*width] = 0xff000000 + Y + (Y << 8) + (Y << 16);
			}
		}
	}

	/*
	 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
	 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
	 * The conversion equations to be implemented are therefore
	 *	R = Y                + 1.40200 * Cr
	 *	G = Y - 0.34414 * Cb - 0.71414 * Cr
	 *	B = Y + 1.77200 * Cb
	 * where Cb and Cr represent the incoming values less CENTERJSAMPLE.
	 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
	 *
	 */

#ifndef CL_DISABLE_SSE2
#ifndef ARM_PLATFORM
	void JPEGRGBDecoder::convert_ycrcb_sse()
	{
		int height = mcu_y * 8;
		int width = mcu_x * 8;
		for (int y = 0; y < height; y++)
		{
			unsigned char *c_line[3] =
			{
				&channels[0][y*width],
				&channels[1][y*width],
				&channels[2][y*width]
			};
			unsigned int *p_line = pixels + y * width;
			for (int x = 0; x < width; x += 4)
			{
				__m128i c0 = _mm_cvtsi32_si128(*reinterpret_cast<unsigned int*>(c_line[0] + x));
				__m128i c1 = _mm_cvtsi32_si128(*reinterpret_cast<unsigned int*>(c_line[1] + x));
				__m128i c2 = _mm_cvtsi32_si128(*reinterpret_cast<unsigned int*>(c_line[2] + x));

				c0 = _mm_unpacklo_epi8(c0, _mm_setzero_si128());
				c0 = _mm_unpacklo_epi16(c0, _mm_setzero_si128());
				c1 = _mm_unpacklo_epi8(c1, _mm_setzero_si128());
				c1 = _mm_unpacklo_epi16(c1, _mm_setzero_si128());
				c2 = _mm_unpacklo_epi8(c2, _mm_setzero_si128());
				c2 = _mm_unpacklo_epi16(c2, _mm_setzero_si128());

				__m128 Y = _mm_cvtepi32_ps(c0);
				__m128 Cb = _mm_cvtepi32_ps(c1);
				__m128 Cr = _mm_cvtepi32_ps(c2);
				Cr = _mm_sub_ps(Cr, _mm_set1_ps(128.0f));
				Cb = _mm_sub_ps(Cb, _mm_set1_ps(128.0f));

				__m128 R = _mm_add_ps(Y, _mm_mul_ps(_mm_set1_ps(1.40200f), Cr));
				__m128 G = _mm_sub_ps(_mm_sub_ps(Y, _mm_mul_ps(_mm_set1_ps(0.34414f), Cb)), _mm_mul_ps(_mm_set1_ps(0.71414f), Cr));
				__m128 B = _mm_add_ps(Y, _mm_mul_ps(_mm_set1_ps(1.77200f), Cb));

				R = _mm_add_ps(_mm_min_ps(_mm_max_ps(R, _mm_setzero_ps()), _mm_set1_ps(255.0f)), _mm_set1_ps(0.5f));
				G = _mm_add_ps(_mm_min_ps(_mm_max_ps(G, _mm_setzero_ps()), _mm_set1_ps(255.0f)), _mm_set1_ps(0.5f));
				B = _mm_add_ps(_mm_min_ps(_mm_max_ps(B, _mm_setzero_ps()), _mm_set1_ps(255.0f)), _mm_set1_ps(0.5f));

				_mm_store_si128(reinterpret_cast<__m128i*>(p_line + x), _mm_add_epi32(_mm_set1_epi32(0xff000000), _mm_add_epi32(_mm_add_epi32(_mm_cvttps_epi32(B), _mm_slli_epi32(_mm_cvttps_epi32(G), 8)), _mm_slli_epi32(_mm_cvttps_epi32(R), 16))));
			}
		}
	}
#endif
#endif	//not CL_DISABLE_SSE2

	void JPEGRGBDecoder::convert_ycrcb_float()
	{
		int height = mcu_y * 8;
		int width = mcu_x * 8;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float Y = channels[0][x + y*width];
				float Cb = channels[1][x + y*width];
				float Cr = channels[2][x + y*width];
				Cr -= 128.0f;
				Cb -= 128.0f;

				float R = Y + 1.40200f * Cr;
				float G = Y - 0.34414f * Cb - 0.71414f * Cr;
				float B = Y + 1.77200f * Cb;

				R = max(R, 0.0f);
				R = min(R, 255.0f);
				G = max(G, 0.0f);
				G = min(G, 255.0f);
				B = max(B, 0.0f);
				B = min(B, 255.0f);

				R += 0.5f;
				G += 0.5f;
				B += 0.5f;

				pixels[x + y*width] = 0xff000000 + ((unsigned int)B) + (((unsigned int)G) << 8) + (((unsigned int)R) << 16);
			}
		}
	}

	void JPEGRGBDecoder::convert_rgb()
	{
		int height = mcu_y * 8;
		int width = mcu_x * 8;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int R = channels[0][x + y*width];
				int G = channels[1][x + y*width];
				int B = channels[2][x + y*width];
				pixels[x + y*width] = 0xff000000 + ((unsigned int)B) + (((unsigned int)G) << 8) + (((unsigned int)R) << 16);
			}
		}
	}
}
