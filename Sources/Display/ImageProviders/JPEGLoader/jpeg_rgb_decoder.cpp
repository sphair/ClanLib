/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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


#ifndef CL_DISABLE_SSE2
#ifndef CL_ARM_PLATFORM
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#endif

CL_JPEGRGBDecoder::CL_JPEGRGBDecoder(CL_JPEGLoader *loader)
: loader(loader), mcu_x(0), mcu_y(0), pixels(0)
{
	mcu_x = loader->mcu_x;
	mcu_y = loader->mcu_y;
	try
	{
		pixels = (unsigned int *) CL_System::aligned_alloc(mcu_x*mcu_y*64*4, 16);
		for (size_t c = 0; c < loader->start_of_frame.components.size(); c++)
			channels.push_back((unsigned char *) CL_System::aligned_alloc(mcu_x*mcu_y*64, 16));
	}
	catch (...)
	{
        CL_System::aligned_free(pixels);
		for (size_t c = 0; c < channels.size(); c++)
			CL_System::aligned_free(channels[c]);
		throw;
	}
}

CL_JPEGRGBDecoder::~CL_JPEGRGBDecoder()
{
    CL_System::aligned_free(pixels);
	for (size_t c = 0; c < channels.size(); c++)
		CL_System::aligned_free(channels[c]);
}

void CL_JPEGRGBDecoder::decode(CL_JPEGMCUDecoder *mcu_decoder)
{
	upsample(mcu_decoder);

	switch (loader->get_colorspace())
	{
	case CL_JPEGLoader::colorspace_grayscale:
		convert_monochrome();
		break;
	case CL_JPEGLoader::colorspace_ycrcb:
		convert_ycrcb_float();
//		convert_ycrcb_sse();
		break;
	case CL_JPEGLoader::colorspace_rgb:
		convert_rgb();
		break;
	case CL_JPEGLoader::colorspace_cmyk:
	case CL_JPEGLoader::colorspace_ycck:
	default:
		throw CL_Exception("Unsupported color space");
	}
}

void CL_JPEGRGBDecoder::upsample(CL_JPEGMCUDecoder *mcu_decoder)
{
	int height = mcu_y*8;
	int width = mcu_x*8;

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
			int step_sx = (h<<16)/mcu_x;
			int step_sy = (v<<16)/mcu_y;
			int sy = step_sy>>1;
			for (int y = 0; y < height; y++)
			{
				const unsigned char *input_line = input+(sy>>16)*v*8;
				int sx = step_sx>>1;
				for (int x = 0; x < width; x++)
				{
					*(output++) = input_line[sx>>16];
					sx += step_sx;
				}
				sy += step_sy;
			}
		}
	}
}

void CL_JPEGRGBDecoder::convert_monochrome()
{
	int height = mcu_y*8;
	int width = mcu_x*8;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned int Y = channels[0][x+y*width];
			pixels[x+y*width] = 0xff000000 + Y + (Y<<8) + (Y<<16);
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

#ifndef CL_ARM_PLATFORM
void CL_JPEGRGBDecoder::convert_ycrcb_sse()
{
	__m128 constant_r_cr = _mm_set1_ps(1.40200f);
	__m128 constant_b_cb = _mm_set1_ps(1.77200f);
	__m128 constant_g_cr = _mm_set1_ps(0.71414f);
	__m128 constant_g_cb = _mm_set1_ps(0.34414f);
	__m128 centerjsample = _mm_set1_ps((float) 128);
	__m128 min_value = _mm_set1_ps(0.0f);
	__m128 max_value = _mm_set1_ps(255.0f);
	__m128i ypack8, cbpack8, crpack8, ypack4, cbpack4, crpack4;
	__m128 y, cb, cr, r, g, b, zero;

#ifdef WIN32
	__declspec(align(16)) unsigned char temp[32];
#else
	__attribute__ ((aligned (16))) unsigned char temp[32];
#endif

	int height = mcu_y*8;
	int width = mcu_x*8;
	for (int row = 0; row < height; row++)
	{
		unsigned char *outptr = (unsigned char*) (pixels+row*width);
		unsigned char *inptr0 = channels[0]+row*width;
		unsigned char *inptr1 = channels[1]+row*width;
		unsigned char *inptr2 = channels[2]+row*width;

		for (int x = 0; x+7 < width; x+=8)
		{
			ypack8 = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*) (&inptr0[x])), _mm_setzero_si128());
			cbpack8 = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*) (&inptr1[x])), _mm_setzero_si128());
			crpack8 = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*) (&inptr2[x])), _mm_setzero_si128());
			
			ypack4 = _mm_unpacklo_epi16(ypack8, _mm_setzero_si128());
			cbpack4 = _mm_unpacklo_epi16(cbpack8, _mm_setzero_si128());
			crpack4 = _mm_unpacklo_epi16(crpack8, _mm_setzero_si128());

			y = _mm_cvtepi32_ps(ypack4);
			cb = _mm_sub_ps(_mm_cvtepi32_ps(cbpack4), centerjsample);
			cr = _mm_sub_ps(_mm_cvtepi32_ps(crpack4), centerjsample);

			r = _mm_add_ps(y, _mm_mul_ps(constant_r_cr, cr));
			g = _mm_sub_ps(_mm_sub_ps(y, _mm_mul_ps(constant_g_cb, cb)), _mm_mul_ps(constant_g_cr, cr));
			b = _mm_add_ps(y, _mm_mul_ps(cb, constant_b_cb));
			zero = _mm_setzero_ps();

			r = _mm_max_ps(_mm_min_ps(r, max_value), min_value);
			g = _mm_max_ps(_mm_min_ps(g, max_value), min_value);
			b = _mm_max_ps(_mm_min_ps(b, max_value), min_value);

			_mm_store_si128((__m128i*)temp, _mm_packus_epi16(
				_mm_packs_epi32(_mm_cvtps_epi32(r), _mm_cvtps_epi32(g)),
				_mm_packs_epi32(_mm_cvtps_epi32(b), _mm_cvtps_epi32(zero))));

			ypack4 = _mm_unpackhi_epi16(ypack8, _mm_setzero_si128());
			cbpack4 = _mm_unpackhi_epi16(cbpack8, _mm_setzero_si128());
			crpack4 = _mm_unpackhi_epi16(crpack8, _mm_setzero_si128());

			y = _mm_cvtepi32_ps(ypack4);
			cb = _mm_sub_ps(_mm_cvtepi32_ps(cbpack4), centerjsample);
			cr = _mm_sub_ps(_mm_cvtepi32_ps(crpack4), centerjsample);

			r = _mm_add_ps(y, _mm_mul_ps(constant_r_cr, cr));
			g = _mm_sub_ps(_mm_sub_ps(y, _mm_mul_ps(constant_g_cb, cb)), _mm_mul_ps(constant_g_cr, cr));
			b = _mm_add_ps(y, _mm_mul_ps(cb, constant_b_cb));
			zero = _mm_setzero_ps();

			r = _mm_max_ps(_mm_min_ps(r, max_value), min_value);
			g = _mm_max_ps(_mm_min_ps(g, max_value), min_value);
			b = _mm_max_ps(_mm_min_ps(b, max_value), min_value);

			_mm_store_si128((__m128i*)(temp+16), _mm_packus_epi16(
				_mm_packs_epi32(_mm_cvtps_epi32(r), _mm_cvtps_epi32(g)),
				_mm_packs_epi32(_mm_cvtps_epi32(b), _mm_cvtps_epi32(zero))));

			for (int i = 0; i < 4; i++)
			{
				outptr[0] = temp[i+8];
				outptr[1] = temp[i+4];
				outptr[2] = temp[i];
				outptr[3] = 0xff;
				outptr += 4;
			}
			for (int i = 0; i < 4; i++)
			{
				outptr[0] = temp[i+16+8];
				outptr[1] = temp[i+16+4];
				outptr[2] = temp[i+16];
				outptr[3] = 0xff;
				outptr += 4;
			}
		}
	}
}
#endif
#endif	//not CL_DISABLE_SSE2

void CL_JPEGRGBDecoder::convert_ycrcb_float()
{
	int height = mcu_y*8;
	int width = mcu_x*8;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float Y = channels[0][x+y*width];
			float Cb = channels[1][x+y*width];
			float Cr = channels[2][x+y*width];
			Cr -= 128.0f;
			Cb -= 128.0f;

			float R = Y + 1.40200f * Cr;
			float G = Y - 0.34414f * Cb - 0.71414f * Cr;
			float B = Y + 1.77200f * Cb;

			R = cl_max(R, 0.0f);
			R = cl_min(R, 255.0f);
			G = cl_max(G, 0.0f);
			G = cl_min(G, 255.0f);
			B = cl_max(B, 0.0f);
			B = cl_min(B, 255.0f);

			R += 0.5f;
			G += 0.5f;
			B += 0.5f;

			pixels[x+y*width] = 0xff000000 + ((unsigned int)B) + (((unsigned int)G)<<8) + (((unsigned int)R)<<16);
		}
	}
}

void CL_JPEGRGBDecoder::convert_rgb()
{
	int height = mcu_y*8;
	int width = mcu_x*8;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int R = channels[0][x+y*width];
			int G = channels[1][x+y*width];
			int B = channels[2][x+y*width];
			pixels[x+y*width] = 0xff000000 + ((unsigned int)B) + (((unsigned int)G)<<8) + (((unsigned int)R)<<16);
		}
	}
}
