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

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"
#include <emmintrin.h>

/// \brief SSE accelerated rendering operations for ARGB8888
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_BlitARGB8SSE
{
//!Operations
public:
	static void copy_pixels(unsigned int *dest, const unsigned int *src);
	static void load_pixel(__m128i &xmm, const unsigned int &pixel);
	static void load_pixels(__m128i &xmm, const unsigned int *pixels);
	static void load_pixels(__m128i &xmm, const unsigned int &p1, unsigned int &p2);
	static void load_pixel_linear(__m128i &xmm, const unsigned int &p1, const unsigned int &p2, const unsigned int &p3, const unsigned int &p4, unsigned int ifracx, unsigned int ifracy);
	static void set_one(__m128i &xmm);
	static void set_half(__m128i &xmm);
	static void set_color(__m128i &xmm, unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha);
	static void set_color(__m128i &xmm, unsigned short r1, unsigned short g1, unsigned short b1, unsigned short a1, unsigned short r2, unsigned short g2, unsigned short b2, unsigned short a2);

#ifdef _MSC_VER
	static void multiply_color(__m128i &src, __m128i &primcolor);
#else
	// Fix to compile on gcc
	static void multiply_color(__m128i &src, __m128i primcolor);
#endif
	static void blend_normal(__m128i &dest, __m128i &src, __m128i &one, __m128i &half);
	static void blend_premultiplied(__m128i &dest, __m128i &src, __m128i &one, __m128i &half);
	static void blend_lcd(__m128i &dest, __m128i &src, __m128i &one, __m128i &half, __m128i &color);
	static void store_pixel(unsigned int &pixel, __m128i &xmm);
	static void store_pixels(unsigned int *pixels, __m128i &xmm);

	static void pixels_to_channels(__m128i &red, __m128i &green, __m128i &blue, __m128i &alpha, const __m128i &src0, const __m128i &src1);
	static void channels_to_pixels(__m128i &dest0, __m128i &dest1, __m128i &red, __m128i &green, __m128i &blue, __m128i &alpha);
//	static void sample_nearest(__m128i &out0, __m128i tx, __m128i ty, const unsigned int *data, int width);
};

inline void CL_BlitARGB8SSE::copy_pixels(unsigned int *dest, const unsigned int *src)
{
	__m128i src0;
	src0 = _mm_loadl_epi64((const __m128i *) src);
	_mm_storel_epi64((__m128i *) dest, src0);
}

inline void CL_BlitARGB8SSE::load_pixel(__m128i &xmm, const unsigned int &pixel)
{
	xmm = _mm_cvtsi32_si128(pixel);
	xmm = _mm_unpacklo_epi8(xmm, _mm_setzero_si128());
}

inline void CL_BlitARGB8SSE::load_pixels(__m128i &xmm, const unsigned int *pixels)
{
	xmm = _mm_loadl_epi64((const __m128i *) pixels);
	xmm = _mm_unpacklo_epi8(xmm, _mm_setzero_si128());
}

inline void CL_BlitARGB8SSE::load_pixels(__m128i &xmm, const unsigned int &p1, unsigned int &p2)
{
	xmm = _mm_set_epi32(0, 0, p2, p1);
	xmm = _mm_unpacklo_epi8(xmm, _mm_setzero_si128());
}

inline void CL_BlitARGB8SSE::load_pixel_linear(__m128i &xmm, const unsigned int &pixel1, const unsigned int &pixel2, const unsigned int &pixel3, const unsigned int &pixel4, unsigned int ifracx, unsigned int ifracy)
{
	__m128i src0, src1, src2, src3;
	__m128i frac0, frac1, frac2, frac3;
	__m128i fracx, inv_fracx, fracy, inv_fracy;
	__m128i half = _mm_set1_epi16(64);
	fracx = _mm_set1_epi16(ifracx);
	fracy = _mm_set1_epi16(ifracy);
	inv_fracx = _mm_set1_epi16(0x80-ifracx);
	inv_fracy = _mm_set1_epi16(0x80-ifracy);
	frac0 = _mm_srli_epi16(_mm_mullo_epi16(inv_fracx, inv_fracy), 7);
	frac1 = _mm_srli_epi16(_mm_mullo_epi16(fracx, inv_fracy), 7);
	frac2 = _mm_srli_epi16(_mm_mullo_epi16(inv_fracx, fracy), 7);
	frac3 = _mm_srli_epi16(_mm_mullo_epi16(fracx, fracy), 7);
	src0 = _mm_mullo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(pixel1), _mm_setzero_si128()), frac0);
	src1 = _mm_mullo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(pixel2), _mm_setzero_si128()), frac1);
	src2 = _mm_mullo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(pixel3), _mm_setzero_si128()), frac2);
	src3 = _mm_mullo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128(pixel4), _mm_setzero_si128()), frac3);
	xmm = _mm_srli_epi16(_mm_add_epi16(_mm_add_epi16(_mm_add_epi16(_mm_add_epi16(src0, src1), src2), src3), half), 7);
}

inline void CL_BlitARGB8SSE::set_one(__m128i &xmm)
{
	xmm =  _mm_set1_epi16(0x0100);
}

inline void CL_BlitARGB8SSE::set_half(__m128i &xmm)
{
	xmm =  _mm_set1_epi16(0x007f);
}

inline void CL_BlitARGB8SSE::set_color(__m128i &xmm, unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha)
{
	xmm = _mm_set_epi16(alpha, red, green, blue, alpha, red, green, blue);
}

inline void CL_BlitARGB8SSE::set_color(__m128i &xmm, unsigned short r1, unsigned short g1, unsigned short b1, unsigned short a1, unsigned short r2, unsigned short g2, unsigned short b2, unsigned short a2)
{
	xmm = _mm_set_epi16(a2, r2, g2, b2, a1, r1, g1, b1);
}

#ifdef _MSC_VER
inline void CL_BlitARGB8SSE::multiply_color(__m128i &src, __m128i &primcolor)
{
	src = _mm_mullo_epi16(src, primcolor);
	src = _mm_srli_epi16(src, 8);
}
#else
	// For some reason "primcolor" cannot be a reference on gcc
inline void CL_BlitARGB8SSE::multiply_color(__m128i &src, __m128i primcolor)
{
	src = _mm_mullo_epi16(src, primcolor);
	src = _mm_srli_epi16(src, 8);
}
#endif

#define cl_blitargb8sse_multiply_color(src, primcolor) \
{ \
	src = _mm_mullo_epi16(src, primcolor); \
	src = _mm_srli_epi16(src, 8); \
}

inline void CL_BlitARGB8SSE::blend_normal(__m128i &dest, __m128i &src, __m128i &one, __m128i &half)
{
	__m128i src_alpha, invsrc_alpha;

	src_alpha = src;
	src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
	src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);

	invsrc_alpha = _mm_sub_epi16(one, src_alpha);

	src = _mm_mullo_epi16(src, src_alpha);
	dest = _mm_mullo_epi16(dest, invsrc_alpha);

	dest = _mm_add_epi16(dest, src);
	dest = _mm_add_epi16(dest, half); // round up
	dest = _mm_srli_epi16(dest, 8);
}

#define cl_blitargb8sse_blend_normal(dest, src, one, half) \
{ \
	__m128i src_alpha, invsrc_alpha; \
\
	src_alpha = src; \
	src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff); \
	src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff); \
\
	invsrc_alpha = _mm_sub_epi16(one, src_alpha); \
\
	src = _mm_mullo_epi16(src, src_alpha); \
	dest = _mm_mullo_epi16(dest, invsrc_alpha); \
\
	dest = _mm_add_epi16(dest, src); \
	dest = _mm_add_epi16(dest, half); \
	dest = _mm_srli_epi16(dest, 8); \
}

inline void CL_BlitARGB8SSE::blend_premultiplied(__m128i &dest, __m128i &src, __m128i &one, __m128i &half)
{
	__m128i src_alpha, invsrc_alpha;

	src_alpha = src;
	src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
	src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);

	invsrc_alpha = _mm_sub_epi16(one, src_alpha);

	dest = _mm_mullo_epi16(dest, invsrc_alpha);
	dest = _mm_add_epi16(dest, half); // round up
	dest = _mm_srli_epi16(dest, 8);
	dest = _mm_add_epi16(dest, src);
}

inline void CL_BlitARGB8SSE::blend_lcd(__m128i &dest, __m128i &src, __m128i &one, __m128i &half, __m128i &color)
{
	__m128i invsrc;
	invsrc = _mm_sub_epi16(one, _mm_add_epi16(_mm_srli_epi16(src, 7), src));

	dest = _mm_add_epi16(_mm_mullo_epi16(src, color), _mm_mullo_epi16(dest, invsrc));
	dest = _mm_add_epi16(dest, half); // round up
	dest = _mm_srli_epi16(dest, 8);
}

inline void CL_BlitARGB8SSE::store_pixel(unsigned int &pixel, __m128i &xmm)
{
	xmm = _mm_packus_epi16(xmm, _mm_setzero_si128());
	pixel = _mm_cvtsi128_si32(xmm);
}

inline void CL_BlitARGB8SSE::store_pixels(unsigned int *pixels, __m128i &xmm)
{
	xmm = _mm_packus_epi16(xmm, _mm_setzero_si128());
	_mm_storel_epi64((__m128i *) pixels, xmm);
}

inline void CL_BlitARGB8SSE::pixels_to_channels(__m128i &red, __m128i &green, __m128i &blue, __m128i &alpha, const __m128i &src0, const __m128i &src1)
{
	__m128i alpha_mask = _mm_set1_epi32(0xff000000);
	__m128i red_mask = _mm_set1_epi32(0x00ff0000);
	__m128i green_mask = _mm_set1_epi32(0x0000ff00);
	__m128i blue_mask = _mm_set1_epi32(0x000000ff);

	alpha = _mm_srli_si128(_mm_and_si128(alpha_mask, src0), 1);
	alpha = _mm_or_si128(alpha, _mm_srli_si128(_mm_and_si128(alpha_mask, src1), 3));

	red = _mm_and_si128(red_mask, src0);
	red = _mm_or_si128(red, _mm_srli_si128(_mm_and_si128(red_mask, src1), 2));

	green = _mm_slli_si128(_mm_and_si128(green_mask, src0), 1);
	green = _mm_or_si128(green, _mm_srli_si128(_mm_and_si128(green_mask, src1), 1));

	blue = _mm_slli_si128(_mm_and_si128(blue_mask, src0), 2);
	blue = _mm_or_si128(blue, _mm_and_si128(blue_mask, src1));
}

inline void CL_BlitARGB8SSE::channels_to_pixels(__m128i &dest0, __m128i &dest1, __m128i &red, __m128i &green, __m128i &blue, __m128i &alpha)
{
	__m128i alpha_mask = _mm_set1_epi32(0xff000000);
	__m128i red_mask = _mm_set1_epi32(0x00ff0000);
	__m128i green_mask = _mm_set1_epi32(0x0000ff00);
	__m128i blue_mask = _mm_set1_epi32(0x000000ff);

	dest0 = _mm_and_si128(alpha_mask, _mm_slli_si128(alpha, 1));
	dest1 = _mm_and_si128(alpha_mask, _mm_slli_si128(alpha, 3));

	dest0 = _mm_or_si128(dest0, _mm_and_si128(red_mask, red));
	dest1 = _mm_or_si128(dest1, _mm_and_si128(red_mask, _mm_slli_si128(red, 2)));

	dest0 = _mm_or_si128(dest0, _mm_and_si128(green_mask, _mm_srli_si128(green, 1)));
	dest1 = _mm_or_si128(dest1, _mm_and_si128(green_mask, _mm_slli_si128(green, 1)));

	dest0 = _mm_or_si128(dest0, _mm_and_si128(blue_mask, _mm_srli_si128(blue, 2)));
	dest1 = _mm_or_si128(dest1, _mm_and_si128(blue_mask, blue));
}

#ifdef _MSC_VER

#define cl_blitargb8sse_sample_nearest(out0, tx, ty, data, width) \
{ \
	__declspec(align(16)) unsigned int x[4], y[4]; \
	_mm_store_si128((__m128i*) x, _mm_srai_epi32(tx, 16)); \
	_mm_store_si128((__m128i*) y, _mm_srai_epi32(ty, 16)); \
	out0 = _mm_set_epi32(data[x[0]+y[0]*width], data[x[1]+y[1]*width], data[x[2]+y[2]*width], data[x[3]+y[3]*width]); \
}

#else

#define cl_blitargb8sse_sample_nearest(out0, tx, ty, data, width) \
{ \
	__attribute__ ((aligned(16))) unsigned int x[4], y[4]; \
	_mm_store_si128((__m128i*) x, _mm_srai_epi32(tx, 16)); \
	_mm_store_si128((__m128i*) y, _mm_srai_epi32(ty, 16)); \
	out0 = _mm_set_epi32(data[x[0]+y[0]*width], data[x[1]+y[1]*width], data[x[2]+y[2]*width], data[x[3]+y[3]*width]); \
}

#endif

// Sadly it seems that the Visual C++ 2008 compiler is unable to optimize CL_BlitARGB8SSE::texture_repeat properly
// when implemented as an inline function.  Maybe it is the branching or the loops that does it?
// Implemented as a macro instead.
#define cl_blitargb8sse_texture_repeat(tx, ty, width, height) \
{ \
	while (true) \
	{ \
		__m128i compare_result = _mm_cmplt_epi32(tx, _mm_setzero_si128()); \
		if (_mm_movemask_epi8(compare_result)) \
			tx = _mm_add_epi32(tx, _mm_and_si128(compare_result, width)); \
		else \
			break; \
	} \
	while (true) \
	{ \
		__m128i compare_result = _mm_cmplt_epi32(tx, width); \
		if (_mm_movemask_epi8(compare_result)!=0xffff) \
			tx = _mm_sub_epi32(tx, _mm_andnot_si128(compare_result, width)); \
		else \
			break; \
	} \
	while (true) \
	{ \
		__m128i compare_result = _mm_cmplt_epi32(ty, _mm_setzero_si128()); \
		if (_mm_movemask_epi8(compare_result)) \
			ty = _mm_add_epi32(ty, _mm_and_si128(compare_result, height)); \
		else \
			break; \
	} \
	while (true) \
	{ \
		__m128i compare_result = _mm_cmplt_epi32(ty, height); \
		if (_mm_movemask_epi8(compare_result)!=0xffff) \
			ty = _mm_sub_epi32(ty, _mm_andnot_si128(compare_result, height)); \
		else \
			break; \
	} \
}

/// \}
