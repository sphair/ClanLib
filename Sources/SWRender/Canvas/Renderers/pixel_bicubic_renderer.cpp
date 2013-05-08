/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "SWRender/precomp.h"
#include "pixel_bicubic_renderer.h"

#ifdef __MINGW32__
#include <malloc.h>
#endif

namespace clan
{

// Implementation of Michael J. Aramini's Efficient Image Magnification by Bicubic Spline Interpolation
// http://members.bellatlantic.net/~vze2vrva/design.html
// http://members.bellatlantic.net/~vze2vrva/magnify_c.txt

PixelBicubicRenderer::PixelBicubicRenderer()
: h_vector(0), dest(0), dest_width(0), dest_height(0), src(0), src_width(0), src_height(0), core(0), num_cores(1)
{
	for (int i = 0; i < 4; i++)
		c_vector[i] = 0;
}

PixelBicubicRenderer::~PixelBicubicRenderer()
{
	reset();
}

void PixelBicubicRenderer::set_dest(unsigned int *data, int width, int height)
{
	dest = data;
	dest_width = width;
	dest_height = height;
}

void PixelBicubicRenderer::set_src(unsigned int *data, int width, int height)
{
	src = data;
	src_width = width;
	src_height = height;
}

void PixelBicubicRenderer::set_core(int new_core, int new_num_cores)
{
	core = new_core;
	num_cores = new_num_cores;
}

void PixelBicubicRenderer::render(int x, int y, int zoom_number, int zoom_denominator)
{
	scale(-0.5f, zoom_number, zoom_denominator, src_width, src_width*4, src_height, src, src_width*zoom_number/zoom_denominator, dest_width*4, src_height*zoom_number/zoom_denominator, dest+x+y*dest_width);
}

void PixelBicubicRenderer::scale(float a, int n, int d, int in_width, int in_pitch, int in_height, const unsigned int *in_data, int out_width, int out_pitch, int out_height, unsigned int *out_data)
{
	prepare(a, n, d, in_width, out_width, out_height);
	int *L = get_L();
	__m128 *row = get_h();
	__m128 *c[4] = { get_c(0), get_c(1), get_c(2), get_c(3) };

	const unsigned char *in_data8 = (const unsigned char *) in_data;
	unsigned char *out_data8 = (unsigned char *) out_data;

	for (int k = find_first_line_for_core(0, core, num_cores); k < out_height; k += num_cores)
	{
		for (int j = 0; j < in_width; j++)
		{
			row[j] = _mm_setzero_ps();
			for (int l = 0; l < 4; l++)
			{
				int index = L[k]+l-1;
				if ((index >= 0) && (index < in_height))
				{
					__m128i in_ipixel = _mm_cvtsi32_si128(*(const int*)(in_data8+index*in_pitch+j*4));
					in_ipixel = _mm_unpacklo_epi8(in_ipixel, _mm_setzero_si128());
					in_ipixel = _mm_unpacklo_epi16(in_ipixel, _mm_setzero_si128());
					row[j] = _mm_add_ps(row[j], _mm_mul_ps(_mm_cvtepi32_ps(in_ipixel), c[3 - l][k]));
				}
			}
		}
		for (int m = 0; m < out_width; m++)
		{
			__m128 x = _mm_set1_ps(0.5f);
			for (int l = 0; l < 4; l++)
			{
				int index = L[m] + l - 1;
				if ((index >= 0) && (index < in_width))
					x = _mm_add_ps(x, _mm_mul_ps(row[index], c[3 - l][m]));
			}

			__m128i ix = _mm_cvtps_epi32(x);
			ix = _mm_packs_epi32(ix, _mm_setzero_si128());
			ix = _mm_packus_epi16(ix, _mm_setzero_si128());
			*(int*)(out_data8+k*out_pitch+m*4) = _mm_cvtsi128_si32(ix);
		}
	}
}

int PixelBicubicRenderer::get_larger_out_dimension(int out_width, int out_height)
{
	return (out_width > out_height) ? out_width : out_height;
}

void PixelBicubicRenderer::prepare(float a, int n, int d, int in_width, int out_width, int out_height)
{
	int larger_out_dimension = get_larger_out_dimension(out_width, out_height);

	reset();
	for (int i=0;i<4;i++)
		c_vector[i] = (float*)aligned_alloc(larger_out_dimension*sizeof(float)*4);
	h_vector = (float*)aligned_alloc(in_width*sizeof(float)*4);
	L_vector.resize(larger_out_dimension);

	int *L = get_L();
	for (int k = 0; k < larger_out_dimension; k++)
		L[k] = (k * d) / n;

	__m128 *c[4] = { get_c(0), get_c(1), get_c(2), get_c(3) };
	for (int k = 0; k < n; k++)
	{
		float x = (float)((k * d) % n) / (float)n;
		c[0][k] = _mm_set1_ps(C0(x, a));
		c[1][k] = _mm_set1_ps(C1(x, a));
		c[2][k] = _mm_set1_ps(C2(x, a));
		c[3][k] = _mm_set1_ps(C3(x, a));
	}
	for (int k = n; k < larger_out_dimension; k++)
		for (int l = 0; l < 4; l++)
			c[l][k] = c[l][k % n];
}

void PixelBicubicRenderer::reset()
{
	aligned_free(h_vector);
	h_vector = 0;
	for (int i = 0; i < 4; i++)
	{
		aligned_free(c_vector[i]);
		c_vector[i] = 0;
	}
}

inline float PixelBicubicRenderer::C0(float t, float a)
{
	return -a * t * t * t + a * t * t;
}

inline float PixelBicubicRenderer::C1(float t, float a)
{
	return -(a + 2.0f) * t * t * t + (2.0f * a + 3.0f) * t * t - a * t;
}

inline float PixelBicubicRenderer::C2(float t, float a)
{
	return (a + 2.0f) * t * t * t - (a + 3.0f) * t * t + 1.0f;
}

inline float PixelBicubicRenderer::C3(float t, float a)
{
	return a * t * t * t - 2.0f * a * t * t + a * t;
}

__m128 *PixelBicubicRenderer::get_h()
{
	return (__m128*)&h_vector[0];
}

int *PixelBicubicRenderer::get_L()
{
	return &L_vector[0];
}

__m128 *PixelBicubicRenderer::get_c(int i)
{
	return (__m128*)&c_vector[i][0];
}

int PixelBicubicRenderer::find_first_line_for_core(int y_start, int core, int num_cores)
{
	int y = y_start / num_cores;
	y *= num_cores;
	y += core;
	if (y < y_start)
		y += num_cores;
	return y;
}

void *PixelBicubicRenderer::aligned_alloc(int size)
{
	void *ptr;
#if defined _MSC_VER || (defined __MINGW32__ && __MSVCRT_VERSION__ >= 0x0700)
	ptr = _aligned_malloc(size, 16);
	if (!ptr)
		throw Exception("Out of memory");
#elif defined __MINGW32__
	ptr = __mingw_aligned_malloc(size, 16);
	if (!ptr)
		throw Exception("Out of memory");	
#else
	if (posix_memalign( (void **) &ptr, 16, size))
	{
		throw Exception("Panic! posix_memalign failed");
	}
#endif
	return ptr;
}

void PixelBicubicRenderer::aligned_free(void *ptr)
{
	if (ptr)
	{
#ifdef _MSC_VER
		_aligned_free(ptr);
#else
		free(ptr);
#endif
	}
}

}
