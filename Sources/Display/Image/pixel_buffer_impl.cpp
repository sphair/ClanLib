/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/exception.h"
#include "pixel_buffer_impl.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"


#ifndef WIN32
#include <cstdlib>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Impl construction:

CL_PixelBuffer_Impl::CL_PixelBuffer_Impl()
: width(0), height(0), delete_data(false), data(0), provider(0), colorkey_enabled(false), colorkey(0), bytes_per_pixel(0)
{
	set_format(cl_rgba8);
}

CL_PixelBuffer_Impl::CL_PixelBuffer_Impl(
	int width, int height, CL_TextureFormat sized_format, const void *data_ptr, bool only_reference_data)
:width(width), height(height), provider(0), delete_data(false), colorkey_enabled(false), colorkey(0), bytes_per_pixel(0)
{
	set_format(sized_format);

	unsigned int pitch = get_pitch();

	if (only_reference_data)
	{
		if (!data_ptr)
			throw CL_Exception("PixelBuffer only_reference_data set without data");

		data = (unsigned char *) data_ptr;
		delete_data = false;
	}
	else
	{
		if (data_ptr)
		{
			delete_data = true;
			data = (unsigned char *) aligned_alloc(pitch*height);
			memcpy(data, data_ptr, pitch*height);
		}
		else
		{
			delete_data = true;
			data = (unsigned char *) aligned_alloc(pitch*height);
		}
	}
}

CL_PixelBuffer_Impl::CL_PixelBuffer_Impl(
	int width, int height, CL_TextureFormat sized_format, const CL_Palette &palette, const void *data_ptr)
:width(width), height(height), provider(0), delete_data(false), colorkey_enabled(false), colorkey(0), bytes_per_pixel(0)
{
	set_format(sized_format);

	CL_PixelBuffer_Impl::palette = palette;

	unsigned int pitch = get_pitch();

	if (data_ptr)
	{
		delete_data = true;
		data = (unsigned char *) aligned_alloc(pitch*height);
		memcpy(data, data_ptr, pitch*height);
	}
	else
	{
		delete_data = true;
		data = (unsigned char *) aligned_alloc(pitch*height);
	}
}

CL_PixelBuffer_Impl::CL_PixelBuffer_Impl(CL_GraphicContext &gc, int new_width, int new_height, CL_PixelBufferDirection new_direction, CL_TextureFormat new_sized_format, const void *new_data, CL_BufferUsage new_usage)
:width(new_width), height(new_height), provider(0), delete_data(false), colorkey_enabled(false), colorkey(0), bytes_per_pixel(0)
{
	set_format(new_sized_format);

	CL_GraphicContextProvider *gc_provider = gc.get_provider();
	provider = gc_provider->alloc_pixel_buffer();
	provider->create(new_data, new_width, new_height, new_direction, new_sized_format, new_usage);
}

CL_PixelBuffer_Impl::~CL_PixelBuffer_Impl()
{
	if (provider)
		provider->destroy();

	if (delete_data)
		aligned_free(data);
}

//////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Impl attributes:
 
CL_Colorf CL_PixelBuffer_Impl::get_pixel(int x, int y)
{
	CL_Colorf color(0.0f, 0.0f, 0.0f, 0.0f);

	cl_uint8* buf = static_cast<cl_uint8*>(get_data());

	if (base_format == cl_formatbase_rgba)
	{
		cl_uint8 *pos = &buf[y * get_pitch() + x * bytes_per_pixel];

		if (sized_format == cl_rgba8 || sized_format == cl_argb8)
		{
			cl_uint32 value = *((cl_uint32*)pos);

			float max_value = 255.0f;
			color = CL_Colorf (((value & red_mask) >> red_bits) / max_value,
				((value & green_mask) >> green_bits) / max_value,
				((value & blue_mask) >> blue_bits) / max_value,
				((value & alpha_mask) >> alpha_bits) / max_value);
		}
	}

	return color;
}


/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Impl operations:

void CL_PixelBuffer_Impl::set_format(CL_TextureFormat new_format)
{
	sized_format = new_format;

	datatype_color = cl_unused;
	datatype_stencil = cl_unused;
	datatype_depth = cl_unused;

	red_bits = 0;
	green_bits = 0;
	blue_bits = 0;
	alpha_bits = 0;
	depth_bits = 0;
	stencil_bits = 0;

	red_mask = 0;
	green_mask = 0;
	blue_mask = 0;
	alpha_mask = 0;
	depth_mask = 0;
	stencil_mask = 0;

	base_format = cl_formatbase_unknown;

	switch (new_format)
	{
		case cl_rgb8:	 //RGB, 8, 8, 8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0xff0000;
			green_mask = 0x00ff00;
			blue_mask = 0x0000ff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgba8:	 //RGBA, 8, 8, 8, 8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			alpha_bits = 8;
			red_mask = 0xff000000;
			green_mask = 0x00ff0000;
			blue_mask = 0x0000ff00;
			alpha_mask = 0x000000ff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_bgr8:	 //BGR, 8, 8, 8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0x0000ff;
			green_mask = 0x00ff00;
			blue_mask = 0xff0000;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}

		case cl_a1_rgb5:	 //A1_RGB5, 1, 5, 5, 5
		{
			alpha_bits = 1;
			red_bits = 5;
			green_bits = 5;
			blue_bits = 5;
			red_mask = CL_PixelFormat::get_bitmask(5, 10);
			green_mask = CL_PixelFormat::get_bitmask(5, 5);
			blue_mask = CL_PixelFormat::get_bitmask(5, 0);
			alpha_mask = CL_PixelFormat::get_bitmask(1, 15);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}

		case cl_argb8:	 //ARGB, 8, 8, 8, 8
		{
			alpha_bits = 8;
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0x00ff0000;
			green_mask = 0x0000ff00;
			blue_mask = 0x000000ff;
			alpha_mask = 0xff000000;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}

		case cl_abgr8:	 //ABGR, 8, 8, 8, 8
		{
			alpha_bits = 8;
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0x000000ff;
			green_mask = 0x0000ff00;
			blue_mask = 0x00ff0000;
			alpha_mask = 0xff000000;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}

		case cl_color_index:
		{
			base_format = cl_formatbase_rgb;
			break;
		}

		case cl_r8:	 //RED, 8
		{
			red_bits = 8;
			red_mask = 0x000000ff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r8_snorm:	 //RED, s8
		{
			red_bits = 8;
			red_mask = 0x000000ff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r16:	 //RED, 16
		{
			red_bits = 16;
			red_mask = 0x0000ffff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r16_snorm:	 //RED, s16
		{
			red_bits = 16;
			red_mask = 0x0000ffff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_rg8:	 //RG, 8, 8
		{
			red_bits = 8;
			green_bits = 8;
			red_mask = 0x0000ff00;
			green_mask = 0x000000ff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg8_snorm:	 //RG, s8, s8
		{
			red_bits = 8;
			green_bits = 8;
			red_mask = 0x0000ff00;
			green_mask = 0x000000ff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg16:	 //RG, 16, 16
		{
			red_bits = 16;
			green_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0x0000ffff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg16_snorm:	 //RG, s16, s16
		{
			red_bits = 16;
			green_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0x0000ffff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_r3_g3_b2:	 //RGB, 3, 3, 2
		{
			red_bits = 3;
			green_bits = 3;
			blue_bits = 2;
			red_mask = CL_PixelFormat::get_bitmask(3, 5);
			green_mask = CL_PixelFormat::get_bitmask(3, 2);
			blue_mask = CL_PixelFormat::get_bitmask(2, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb4:	 //RGB, 4, 4, 4
		{
			red_bits = 4;
			green_bits = 4;
			blue_bits = 4;
			red_mask = CL_PixelFormat::get_bitmask(4, 8);
			green_mask = CL_PixelFormat::get_bitmask(4, 4);
			blue_mask = CL_PixelFormat::get_bitmask(4, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb5:	 //RGB, 5, 5, 5
		{
			red_bits = 5;
			green_bits = 5;
			blue_bits = 5;
			red_mask = CL_PixelFormat::get_bitmask(5, 10);
			green_mask = CL_PixelFormat::get_bitmask(5, 5);
			blue_mask = CL_PixelFormat::get_bitmask(5, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb8_snorm:	 //RGB, s8, s8, s8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0x00ff0000;
			green_mask = 0x0000ff00;
			blue_mask = 0x000000ff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb10:	 //RGB, 10, 10, 10
		{
			red_bits = 10;
			green_bits = 10;
			blue_bits = 10;
			red_mask = CL_PixelFormat::get_bitmask(10, 20);
			green_mask = CL_PixelFormat::get_bitmask(10, 10);
			blue_mask = CL_PixelFormat::get_bitmask(10, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb12:	 //RGB, 12, 12, 12
		{
			red_bits = 12;
			green_bits = 12;
			blue_bits = 12;
			red_mask = CL_PixelFormat::get_bitmask(12, 24);
			green_mask = CL_PixelFormat::get_bitmask(12, 12);
			blue_mask = CL_PixelFormat::get_bitmask(12, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb16:	 //RGB, 16, 16, 16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0x0000ffff;
			blue_mask = 0x0000ffff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb16_snorm:	 //RGB, s16, s16, s16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0x0000ffff;
			blue_mask = 0x0000ffff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgba2:	 //RGBA, 2, 2, 2, 2
		{
			red_bits = 2;
			green_bits = 2;
			blue_bits = 2;
			alpha_bits = 2;
			red_mask = CL_PixelFormat::get_bitmask(2, 6);
			green_mask = CL_PixelFormat::get_bitmask(2, 4);
			blue_mask = CL_PixelFormat::get_bitmask(2, 2);
			alpha_mask = CL_PixelFormat::get_bitmask(2, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba4:	 //RGBA, 4, 4, 4, 4
		{
			red_bits = 4;
			green_bits = 4;
			blue_bits = 4;
			alpha_bits = 4;
			red_mask = CL_PixelFormat::get_bitmask(4, 12);
			green_mask = CL_PixelFormat::get_bitmask(4, 8);
			blue_mask = CL_PixelFormat::get_bitmask(4, 4);
			alpha_mask = CL_PixelFormat::get_bitmask(4, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgb5_a1:	 //RGBA, 5, 5, 5, 1
		{
			red_bits = 5;
			green_bits = 5;
			blue_bits = 5;
			alpha_bits = 1;
			red_mask = CL_PixelFormat::get_bitmask(5, 11);
			green_mask = CL_PixelFormat::get_bitmask(5, 6);
			blue_mask = CL_PixelFormat::get_bitmask(5, 1);
			alpha_mask = CL_PixelFormat::get_bitmask(1, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba8_snorm:	 //RGBA, s8, s8, s8, s8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			alpha_bits = 8;
			red_mask = 0xff000000;
			green_mask = 0x00ff0000;
			blue_mask = 0x0000ff00;
			alpha_mask = 0x000000ff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgb10_a2:	 //RGBA, 10, 10, 10, 2
		{
			red_bits = 10;
			green_bits = 10;
			blue_bits = 10;
			alpha_bits = 2;
			red_mask = CL_PixelFormat::get_bitmask(10, 22);
			green_mask = CL_PixelFormat::get_bitmask(10, 12);
			blue_mask = CL_PixelFormat::get_bitmask(10, 2);
			alpha_mask = CL_PixelFormat::get_bitmask(2, 0);
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba12:	 //RGBA, 12, 12, 12, 12
		{
			red_bits = 12;
			green_bits = 12;
			blue_bits = 12;
			alpha_bits = 12;
			red_mask = 0xfff00000;
			green_mask = 0x000fff00;
			blue_mask = 0;		// Cannot create a mask for this type
			alpha_mask = 0x00000fff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba16:	 //RGBA, 16, 16, 16, 16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			alpha_bits = 16;
			red_mask = 0xfffff000;
			green_mask = 0x0000ffff;
			blue_mask = 0xffff0000;
			alpha_mask = 0x0000ffff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba16_snorm:	 //RGBA, s16, s16, s16, s16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			alpha_bits = 16;
			red_mask = 0xfffff000;
			green_mask = 0x0000ffff;
			blue_mask = 0xffff0000;
			alpha_mask = 0x0000ffff;
			datatype_color = cl_signed_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_srgb8:	 //RGB, 8, 8, 8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0xff0000;
			green_mask = 0xff00;
			blue_mask = 0xff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_srgb8_alpha8:	 //RGBA, 8, 8, 8, 8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			alpha_bits = 8;
			red_mask = 0xff000000;
			green_mask = 0xff0000;
			blue_mask = 0xff00;
			alpha_mask = 0xff;
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_r16f:	 //RED, f16
		{
			red_bits = 16;
			red_mask = 0xffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_rg16f:	 //RG, f16, f16
		{
			red_bits = 16;
			green_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rgb16f:	 //RGB, f16, f16, f16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			blue_mask = 0xffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgba16f:	 //RGBA, f16, f16, f16, f16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			alpha_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			blue_mask = 0xffff0000;
			alpha_mask = 0xffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_r32f:	 //RED, f32
		{
			red_bits = 32;
			red_mask = 0xffffffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_rg32f:	 //RG, f32, f32
		{
			red_bits = 32;
			green_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rgb32f:	 //RGB, f32, f32, f32
		{
			red_bits = 32;
			green_bits = 32;
			blue_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			blue_mask = 0xffffffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgba32f:	 //RGBA, f32, f32, f32, f32
		{
			red_bits = 32;
			green_bits = 32;
			blue_bits = 32;
			alpha_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			blue_mask = 0xffffffff;
			alpha_mask = 0xffffffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_r11f_g11f_b10f:	 //RGB, f11, f11, f10
		{
			red_bits = 11;
			green_bits = 11;
			blue_bits = 10;
			red_mask = CL_PixelFormat::get_bitmask(11, 21);
			green_mask = CL_PixelFormat::get_bitmask(11, 10);
			blue_mask = CL_PixelFormat::get_bitmask(10, 0);
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb9_e5:	 //RGB, 9, 9, 9, 5
		{
			red_bits = 9;
			green_bits = 9;
			blue_bits = 9;
			// Cannot calculate mask for this type
			datatype_color = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_r8i:	 //RED, i8
		{
			red_bits = 8;
			red_mask = 0xff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r8ui:	 //RED, ui8
		{
			red_bits = 8;
			red_mask = 0xff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r16i:	 //RED, i16
		{
			red_bits = 16;
			red_mask = 0xffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r16ui:	 //RED, ui16
		{
			red_bits = 16;
			red_mask = 0xffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r32i:	 //RED, i32
		{
			red_bits = 32;
			red_mask = 0xffffffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_r32ui:	 //RED, ui32
		{
			red_bits = 32;
			red_mask = 0xffffffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_red;
			break;
		}
		case cl_rg8i:	 //RG, i8, i8
		{
			red_bits = 8;
			green_bits = 8;
			red_mask = 0xff00;
			green_mask = 0x00ff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg8ui:	 //RG, ui8, ui8
		{
			red_bits = 8;
			green_bits = 8;
			red_mask = 0xff00;
			green_mask = 0x00ff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg16i:	 //RG, i16, i16
		{
			red_bits = 16;
			green_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0x0000ffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg16ui:	 //RG, ui16, ui16
		{
			red_bits = 16;
			green_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0x0000ffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg32i:	 //RG, i32, i32
		{
			red_bits = 32;
			green_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rg32ui:	 //RG, ui32, ui32
		{
			red_bits = 32;
			green_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rg;
			break;
		}
		case cl_rgb8i:	 //RGB, i8, i8, i8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0xff0000;
			green_mask = 0xff00;
			blue_mask = 0xff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb8ui:	 //RGB, ui8, ui8, ui8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			red_mask = 0xff0000;
			green_mask = 0xff00;
			blue_mask = 0xff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb16i:	 //RGB, i16, i16, i16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			blue_mask = 0xffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb16ui:	 //RGB, ui16, ui16, ui16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			blue_mask = 0xffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb32i:	 //RGB, i32, i32, i32
		{
			red_bits = 32;
			green_bits = 32;
			blue_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			blue_mask = 0xffffffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgb32ui:	 //RGB, ui32, ui32, ui32
		{
			red_bits = 32;
			green_bits = 32;
			blue_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			blue_mask = 0xffffffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgb;
			break;
		}
		case cl_rgba8i:	 //RGBA, i8, i8, i8, i8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			alpha_bits = 8;
			red_mask = 0xff000000;
			green_mask = 0xff0000;
			blue_mask = 0xff00;
			alpha_mask = 0xff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba8ui:	 //RGBA, ui8, ui8, ui8, ui8
		{
			red_bits = 8;
			green_bits = 8;
			blue_bits = 8;
			alpha_bits = 8;
			red_mask = 0xff000000;
			green_mask = 0xff0000;
			blue_mask = 0xff00;
			alpha_mask = 0xff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba16i:	 //RGBA, i16, i16, i16, i16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			alpha_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			blue_mask = 0xffff0000;
			alpha_mask = 0xffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba16ui:	 //RGBA, ui16, ui16, ui16, ui16
		{
			red_bits = 16;
			green_bits = 16;
			blue_bits = 16;
			alpha_bits = 16;
			red_mask = 0xffff0000;
			green_mask = 0xffff;
			blue_mask = 0xffff0000;
			alpha_mask = 0xffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba32i:	 //RGBA, i32, i32, i32, i32
		{
			red_bits = 32;
			green_bits = 32;
			blue_bits = 32;
			alpha_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			blue_mask = 0xffffffff;
			alpha_mask = 0xffffffff;
			datatype_color = cl_signed;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_rgba32ui:	 //RGBA, ui32, ui32, ui32, ui32
		{
			red_bits = 32;
			green_bits = 32;
			blue_bits = 32;
			alpha_bits = 32;
			red_mask = 0xffffffff;
			green_mask = 0xffffffff;
			blue_mask = 0xffffffff;
			alpha_mask = 0xffffffff;
			datatype_color = cl_unsigned;
			base_format = cl_formatbase_rgba;
			break;
		}
		case cl_depth_component16:	 //DEPTH_COMPONENT, 16
		{
			depth_bits = 16;
			depth_mask = 0xffff;
			datatype_depth = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_depth_component;
			break;
		}
		case cl_depth_component24:	 //DEPTH_COMPONENT, 24
		{
			depth_bits = 24;
			depth_mask = 0xffffff;
			datatype_depth = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_depth_component;
			break;
		}
		case cl_depth_component32:	 //DEPTH_COMPONENT, 32
		{
			depth_bits = 32;
			depth_mask = 0xffffffff;
			datatype_depth = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_depth_component;
			break;
		}
		case cl_depth_component32f:	 //DEPTH_COMPONENT, f32
		{
			depth_bits = 32;
			depth_mask = 0xffffffff;
			datatype_color = cl_floating_point;
			base_format = cl_formatbase_depth_component;
			break;
		}
		case cl_depth24_stencil8:	 //DEPTH_STENCIL, 24, 8
		{
			depth_bits = 24;
			stencil_bits = 8;
			depth_mask = 0xffffff00;
			stencil_mask = 0xff;
			datatype_depth = cl_unsigned_normalised_fixed_point;
			datatype_stencil = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_depth_stencil;
			break;
		}
		case cl_depth32f_stencil8:	 //DEPTH_STENCIL, f32, 8
		{
			depth_bits = 32;
			stencil_bits = 8;
			depth_mask = 0xffffffff;
			stencil_mask = 0xff;
			datatype_depth = cl_floating_point;
			datatype_stencil = cl_unsigned_normalised_fixed_point;
			base_format = cl_formatbase_depth_stencil;
			break;
		}

		default:
			throw CL_Exception("cannot obtain pixel count for this sized CL_TextureFormat");
	}

	bytes_per_pixel = ( (red_bits + green_bits + blue_bits + alpha_bits + depth_bits + stencil_bits) + 7) / 8;

	if (new_format == cl_color_index)
		bytes_per_pixel = 1;

}

void CL_PixelBuffer_Impl::convert(CL_PixelBuffer &target, const CL_Rect &dest_rect, const CL_Rect &src_rect) const
{
	if (provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	if (target.get_format() == cl_color_index)
	{
		throw CL_Exception("Converting to indexed pixelformats not supported.");
	}

	if ( dest_rect.get_size() != src_rect.get_size())
	{
		throw CL_Exception("Source and destination rects must have same size. Scaled converting not supported.");
	}

	if (!check_supported_conversion_format())
		throw CL_Exception("Converting from this pixelformat type is currently not supported");

	if (!target.impl->check_supported_conversion_format())
		throw CL_Exception("Converting to this pixelformat type is currently not supported");

	char *src_data = (char *) get_data();
	char *dest_data = (char *) target.get_data();

	int src_pitch = width * bytes_per_pixel;
	int dest_pitch = target.get_width() * target.get_bytes_per_pixel();

	src_data += src_rect.top*src_pitch + src_rect.left*bytes_per_pixel;
	dest_data += dest_rect.top*dest_pitch + dest_rect.left*target.get_bytes_per_pixel();

	if (get_format() == cl_color_index)
	{
		convert_pal(
			src_data,
			src_pitch,
			dest_data,
			target,
			dest_pitch,
			dest_rect.get_size() );
	}
	else
	{
		convert(
			src_data,
			src_pitch,
			dest_data,
			target,
			dest_pitch,
			dest_rect.get_size());
	}
}

void CL_PixelBuffer_Impl::convert(
		void* in_buffer,
		int in_pitch,
		void* out_buffer,
		CL_PixelBuffer& target_buffer,
		int out_pitch,
		const CL_Size& size) const
{
	if (get_format() == cl_color_index)
		throw CL_Exception("Input format is cl_color_index");
	if (target_buffer.get_format() == cl_color_index)
		throw CL_Exception("Output format is cl_color_index");
	
	const int in_bpp = bytes_per_pixel;
	const int out_bpp = target_buffer.get_bytes_per_pixel();
	char* in_p = static_cast<char*>(in_buffer);
	char* out_p = static_cast<char*>(out_buffer);

	if (get_format() == target_buffer.get_format())
	{
		const int in_line_bytes = in_bpp*size.width;
		for (int y = 0; y < size.height; y++, in_p += in_pitch, out_p += out_pitch)
			memcpy(out_p, in_p, in_line_bytes);
	}
	else
	{
		const int in_r_shift = CL_PixelFormat::get_mask_shift(red_mask);
		const int in_g_shift = CL_PixelFormat::get_mask_shift(green_mask);
		const int in_b_shift = CL_PixelFormat::get_mask_shift(blue_mask);
		const int in_a_shift = CL_PixelFormat::get_mask_shift(alpha_mask);

		const int in_r_bits = red_bits;
		const int in_g_bits = green_bits;
		const int in_b_bits = blue_bits;
		const int in_a_bits = alpha_bits;

		const int in_r_bitmask = CL_PixelFormat::get_bitmask(in_r_bits, 0);
		const int in_g_bitmask = CL_PixelFormat::get_bitmask(in_g_bits, 0);
		const int in_b_bitmask = CL_PixelFormat::get_bitmask(in_b_bits, 0);
		const int in_a_bitmask = CL_PixelFormat::get_bitmask(in_a_bits, 0);

		const int out_r_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_red_mask());
		const int out_g_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_green_mask());
		const int out_b_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_blue_mask());
		const int out_a_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_alpha_mask());

		const int out_r_bits = target_buffer.impl->red_bits;
		const int out_g_bits = target_buffer.impl->green_bits;
		const int out_b_bits = target_buffer.impl->blue_bits;
		const int out_a_bits = target_buffer.impl->alpha_bits;

		const bool in_has_ck = colorkey_enabled;
		unsigned int in_ck;
		if (in_has_ck)
			in_ck = colorkey;
		const bool out_has_ck = target_buffer.has_colorkey();
		unsigned int out_ck;
		if (out_has_ck)
			out_ck = target_buffer.get_colorkey();
		
		for (int y = 0; y < size.height; y++, in_p += in_pitch, out_p += out_pitch)
		{
			char* in_p2 = in_p;
			char* out_p2 = out_p;
			for (int x = 0; x < size.width; x++, in_p2 += in_bpp, out_p2 += out_bpp)
			{
				unsigned int in_pixel = 0;
				memcpy(&in_pixel, in_p2, in_bpp);

				// decode
				unsigned int in_r = 0, in_g = 0, in_b = 0, in_a = 0xffffffff;
				if (in_r_bitmask)
				{
					in_r = ((in_pixel >> in_r_shift) & in_r_bitmask);
					if (in_r & 1)
						in_r = (in_r << (32 - in_r_bits)) | CL_PixelFormat::get_bitmask(32 - in_r_bits, 0);
					else
						in_r <<= 32 - in_r_bits;
				}
				if (in_g_bitmask)
				{
					in_g = ((in_pixel >> in_g_shift) & in_g_bitmask);
					if (in_g & 1)
						in_g = (in_g << (32 - in_g_bits)) | CL_PixelFormat::get_bitmask(32 - in_g_bits, 0);
					else
						in_g <<= 32 - in_g_bits;
				}
				if (in_b_bitmask)
				{
					in_b = ((in_pixel >> in_b_shift) & in_b_bitmask);
					if (in_b & 1)
						in_b = (in_b << (32 - in_b_bits)) | CL_PixelFormat::get_bitmask(32 - in_b_bits, 0);
					else
						in_b <<= 32 - in_b_bits;
				}
				if (in_a_bitmask)
				{
					in_a = ((in_pixel >> in_a_shift) & in_a_bitmask);
					if (in_a & 1)
						in_a = (in_a << (32 - in_a_bits)) | CL_PixelFormat::get_bitmask(32 - in_a_bits, 0);
					else
						in_a <<= 32 - in_a_bits;
				}

				unsigned int out_pixel;
				bool encode;
				if (!out_a_bits && (in_a == 0) && out_has_ck)
				{
					out_pixel = out_ck;
					encode = false;
				}
				else if (in_has_ck && (in_pixel == in_ck))
				{
					if (out_has_ck)
					{
						out_pixel = out_ck;
						encode = false;
					}
					else
					{
						in_a = 0;
						encode = true;
					}
				}	
				else
					encode = true;

				// encode
				if (encode)
				{
					out_pixel = 0;
					if (out_r_bits)
						out_pixel |= (in_r >> (32 - out_r_bits)) << out_r_shift;
					if (out_g_bits)
						out_pixel |= (in_g >> (32 - out_g_bits)) << out_g_shift;
					if (out_b_bits)
						out_pixel |= (in_b >> (32 - out_b_bits)) << out_b_shift;
					if (out_a_bits)
						out_pixel |= (in_a >> (32 - out_a_bits)) << out_a_shift;
				}

				memcpy(out_p2, &out_pixel, out_bpp);
			}
		}
	}
}

void CL_PixelBuffer_Impl::convert_pal(
		void* in_buffer,
		int in_pitch,
		void* out_buffer,
		CL_PixelBuffer& target_buffer,
		int out_pitch,
		const CL_Size& size) const
{
	if (get_format() != cl_color_index)
		throw CL_Exception("Input format is not cl_color_index");
	if (target_buffer.get_format() == cl_color_index)
		throw CL_Exception("Output format is cl_color_index");

	const int in_bpp = bytes_per_pixel;
	const int out_bpp = target_buffer.get_bytes_per_pixel();
	char* in_p = static_cast<char*>(in_buffer);
	char* out_p = static_cast<char*>(out_buffer);

	const int out_r_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_red_mask());
	const int out_g_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_green_mask());
	const int out_b_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_blue_mask());
	const int out_a_shift = CL_PixelFormat::get_mask_shift(target_buffer.get_alpha_mask());

	const int out_r_bits = target_buffer.impl->red_bits;
	const int out_g_bits = target_buffer.impl->green_bits;
	const int out_b_bits = target_buffer.impl->blue_bits;
	const int out_a_bits = target_buffer.impl->alpha_bits;

	const bool in_has_ck = colorkey_enabled;
	unsigned int in_ck;
	if (in_has_ck)
		in_ck = colorkey;
	const bool out_has_ck = target_buffer.has_colorkey();
	unsigned int out_ck;
	if (out_has_ck)
		out_ck = target_buffer.get_colorkey();
	
	for (int y = 0; y < size.height; y++, in_p += in_pitch, out_p += out_pitch)
	{
		char* in_p2 = in_p;
		char* out_p2 = out_p;
		for (int x = 0; x < size.width; x++, in_p2 += in_bpp, out_p2 += out_bpp)
		{
			unsigned int in_pixel = 0;
			memcpy(&in_pixel, in_p2, in_bpp);

			// decode
			unsigned int in_r = 0, in_g = 0, in_b = 0, in_a = 0xffffffff;
			in_r = palette.colors[in_pixel].get_red();
			if (in_r & 1)
				in_r = (in_r << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_r <<= 32 - 8;
			in_g = palette.colors[in_pixel].get_green();
			if (in_g & 1)
				in_g = (in_g << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_g <<= 32 - 8;
			in_b = palette.colors[in_pixel].get_blue();
			if (in_b & 1)
				in_b = (in_b << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_b <<= 32 - 8;
			in_a = palette.colors[in_pixel].get_alpha();
			if (in_a & 1)
				in_a = (in_a << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_a <<= 32 - 8;

			unsigned int out_pixel;
			bool encode;
			if (!out_a_bits && (in_a == 0) && out_has_ck)
			{
				out_pixel = out_ck;
				encode = false;
			}
			else if (in_has_ck && (in_pixel == in_ck))
			{
				if (out_has_ck)
				{
					out_pixel = out_ck;
					encode = false;
				}
				else
				{
					in_a = 0;
					encode = true;
				}
			}	
			else
				encode = true;

			// encode
			if (encode)
			{
				out_pixel = 0;
				if (out_r_bits)
					out_pixel |= (in_r >> (32 - out_r_bits)) << out_r_shift;
				if (out_g_bits)
					out_pixel |= (in_g >> (32 - out_g_bits)) << out_g_shift;
				if (out_b_bits)
					out_pixel |= (in_b >> (32 - out_b_bits)) << out_b_shift;
				if (out_a_bits)
					out_pixel |= (in_a >> (32 - out_a_bits)) << out_a_shift;
			}

			memcpy(out_p2, &out_pixel, out_bpp);
		}
	}
}

unsigned int CL_PixelBuffer_Impl::get_pitch() const
{
	return bytes_per_pixel * width;
}

void *CL_PixelBuffer_Impl::aligned_alloc(int size) const
{
	void *ptr;
#ifdef _MSC_VER
	ptr = _aligned_malloc(size, 16);
	if (!ptr)
		throw CL_Exception("Out of memory");

#else
	if (posix_memalign( (void **) &ptr, 16, size))
	{
		throw CL_Exception("Panic! posix_memalign failed");
	}
#endif
	return ptr;
}

void CL_PixelBuffer_Impl::aligned_free(void *ptr) const
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

bool CL_PixelBuffer_Impl::check_supported_conversion_format() const
{
	bool valid = true;

	if (datatype_stencil != cl_unused)
		valid = false;

	if (datatype_depth != cl_unused)
		valid = false;

	if (datatype_color == cl_floating_point)
		valid = false;

	if (bytes_per_pixel > 4)
		valid = false;

	return valid;

}
