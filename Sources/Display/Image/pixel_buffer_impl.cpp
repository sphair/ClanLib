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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/exception.h"
#include "pixel_buffer_impl.h"
#include "cpu_pixel_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"
#include "API/Display/Image/pixel_converter.h"

namespace clan
{
	PixelBuffer_Impl::PixelBuffer_Impl()
		: provider(nullptr)
	{
	}

	PixelBuffer_Impl::PixelBuffer_Impl(PixelBufferProvider *provider)
		: provider(provider)
	{
	}

	PixelBuffer_Impl::PixelBuffer_Impl(int width, int height, TextureFormat texture_format, const void *data_ptr, bool only_reference_data)
		: provider(nullptr)	// Default to locked for CPU buffer
	{
		auto cpu_provider = new CPUPixelBufferProvider;
		provider = cpu_provider;

		cpu_provider->create(texture_format, Size(width, height), data_ptr, only_reference_data);

	}

	PixelBuffer_Impl::~PixelBuffer_Impl()
	{
		if (provider)
			delete provider;
	}

	Colorf PixelBuffer_Impl::get_pixel(int x, int y)
	{
		Colorf color(0.0f, 0.0f, 0.0f, 0.0f);

		if (provider->get_format() == tf_rgba8)
		{
			const uint8_t* buf = static_cast<const uint8_t*>(provider->get_data());
			const uint8_t *pos = &buf[y * provider->get_pitch() + x * get_bytes_per_pixel()];

			uint32_t value = *((const uint32_t*)pos);

			float rcp_255 = 1.0f / 255.0f;
			color = Colorf(((value & 0xFF) >> 0) * rcp_255,
				((value & 0xFF00) >> 8) * rcp_255,
				((value & 0xFF0000) >> 16) * rcp_255,
				((value & 0xFF000000) >> 24) * rcp_255);
		}
		else
		{
			throw Exception("Implement me using PixelConverter!");
		}

		return color;
	}

	unsigned int PixelBuffer_Impl::get_bytes_per_block(TextureFormat texture_format)
	{
		switch (texture_format)
		{
		case tf_compressed_rgb_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt3:
		case tf_compressed_srgb_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt3:
			return 8;
		case tf_compressed_rgba_s3tc_dxt5:
		case tf_compressed_srgb_alpha_s3tc_dxt5:
			return 16;
		default:
			throw Exception("cannot obtain block count for this TextureFormat");
		}
	}

	bool PixelBuffer_Impl::is_compressed(TextureFormat texture_format)
	{
		switch (texture_format)
		{
		case tf_compressed_rgb_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt3:
		case tf_compressed_srgb_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt3:
		case tf_compressed_rgba_s3tc_dxt5:
		case tf_compressed_srgb_alpha_s3tc_dxt5:
			return true;
		default:
			return false;
		}
	}

	unsigned int PixelBuffer_Impl::get_data_size(const Size &size, TextureFormat texture_format)
	{
		if (is_compressed(texture_format))
		{
			return ((size.width + 3) / 4) * ((size.height + 3) / 4) * get_bytes_per_block(texture_format);
		}
		else
		{
			return size.width * size.height * get_bytes_per_pixel(texture_format);
		}
	}

	unsigned int PixelBuffer_Impl::get_bytes_per_pixel(TextureFormat texture_format)
	{
		unsigned int count;
		switch (texture_format)
		{
		case tf_r8: count = 8;	break; //RED 8
		case tf_r8_snorm: count = 8; break; //RED, s8
		case tf_r16: count = 16; break; //RED, 16
		case tf_r16_snorm: count = 16; break; //RED, s16
		case tf_rg8: count = 8 + 8; break; //RG, 8, 8
		case tf_rg8_snorm: count = 8 + 8; break; //RG, s8, s8
		case tf_rg16: count = 16 + 16; break; //RG, 16, 16
		case tf_rg16_snorm: count = 16 + 16; break; //RG, s16, s16
		case tf_r3_g3_b2: count = 3 + 3 + 2; break; //RGB, 3, 3, 2
		case tf_rgb4: count = 4 + 4 + 4; break; //RGB, 4, 4, 4
		case tf_rgb5: count = 5 + 5 + 5; break; //RGB, 5, 5, 5
		case tf_rgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case tf_bgr8: count = 8 + 8 + 8; break; //BGR, 8, 8, 8
		case tf_rgb8_snorm: count = 8 + 8 + 8; break; //RGB, s8, s8, s8
		case tf_rgb10: count = 10 + 10 + 10; break; //RGB, 10, 10, 10
		case tf_rgb12: count = 12 + 12 + 12; break; //RGB, 12, 12, 12
		case tf_rgb16: count = 16 + 16 + 16; break; //RGB, 16, 16, 16
		case tf_rgb16_snorm: count = 16 + 16 + 16; break; //RGB, s16, s16, s16
		case tf_rgba2: count = 2 + 2 + 2 + 2; break; //RGBA, 2, 2, 2, 2
		case tf_rgba4: count = 4 + 4 + 4 + 4; break; //RGBA, 4, 4, 4, 4
		case tf_rgb5_a1: count = 5 + 5 + 5 + 1; break; //RGBA, 5, 5, 5, 1
		case tf_rgba8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case tf_rgba8_snorm: count = 8 + 8 + 8 + 8; break; //RGBA, s8, s8, s8, s8
		case tf_bgra8: count = 8 + 8 + 8 + 8; break; //BGRA, 8, 8, 8, 8
		case tf_rgb10_a2: count = 10 + 10 + 10 + 2; break; //RGBA, 10, 10, 10, 2
		case tf_rgba12: count = 12 + 12 + 12 + 12; break; //RGBA, 12, 12, 12, 12
		case tf_rgba16: count = 16 + 16 + 16 + 16; break; //RGBA, 16, 16, 16, 16
		case tf_rgba16_snorm: count = 16 + 16 + 16 + 16; break; //RGBA, s16, s16, s16, s16
		case tf_srgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case tf_srgb8_alpha8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case tf_r16f: count = 16; break; //RED, f16
		case tf_rg16f: count = 16 + 16; break; //RG, f16, f16
		case tf_rgb16f: count = 16 + 16 + 16; break; //RGB, f16, f16, f16
		case tf_rgba16f: count = 16 + 16 + 16 + 16; break; //RGBA, f16, f16, f16, f16
		case tf_r32f: count = 32; break; //RED, f32
		case tf_rg32f: count = 32 + 32; break; //RG, f32, f32
		case tf_rgb32f: count = 32 + 32 + 32; break; //RGB, f32, f32, f32
		case tf_rgba32f: count = 32 + 32 + 32 + 32; break; //RGBA, f32, f32, f32, f32
		case tf_r11f_g11f_b10f: count = 11 + 11 + 10; break; //RGB, f11, f11, f10
		case tf_rgb9_e5: count = 9 + 9 + 9 + 5; break; //RGB, 9, 9, 9, 5
		case tf_r8i: count = 8; break; //RED, i8
		case tf_r8ui: count = 8; break; //RED, ui8
		case tf_r16i: count = 16; break; //RED, i16
		case tf_r16ui: count = 16; break; //RED, ui16
		case tf_r32i: count = 32; break; //RED, i32
		case tf_r32ui: count = 32; break; //RED, ui32
		case tf_rg8i: count = 8 + 8; break; //RG, i8, i8
		case tf_rg8ui: count = 8 + 8; break; //RG, ui8, ui8
		case tf_rg16i: count = 16 + 16; break; //RG, i16, i16
		case tf_rg16ui: count = 16 + 16; break; //RG, ui16, ui16
		case tf_rg32i: count = 32 + 32; break; //RG, i32, i32
		case tf_rg32ui: count = 32 + 32; break; //RG, ui32, ui32
		case tf_rgb8i: count = 8 + 8 + 8; break; //RGB, i8, i8, i8
		case tf_rgb8ui: count = 8 + 8 + 8; break; //RGB, ui8, ui8, ui8
		case tf_rgb16i: count = 16 + 16 + 16; break; //RGB, i16, i16, i16
		case tf_rgb16ui: count = 16 + 16 + 16; break; //RGB, ui16, ui16, ui16
		case tf_rgb32i: count = 32 + 32 + 32; break; //RGB, i32, i32, i32
		case tf_rgb32ui: count = 32 + 32 + 32; break; //RGB, ui32, ui32, ui32
		case tf_rgba8i: count = 8 + 8 + 8 + 8; break; //RGBA, i8, i8, i8, i8
		case tf_rgba8ui: count = 8 + 8 + 8 + 8; break; //RGBA, ui8, ui8, ui8, ui8
		case tf_rgba16i: count = 16 + 16 + 16 + 16; break; //RGBA, i16, i16, i16, i16
		case tf_rgba16ui: count = 16 + 16 + 16 + 16; break; //RGBA, ui16, ui16, ui16, ui16
		case tf_rgba32i: count = 32 + 32 + 32 + 32; break; //RGBA, i32, i32, i32, i32
		case tf_rgba32ui: count = 32 + 32 + 32 + 32; break; //RGBA, ui32, ui32, ui32, ui32
		case tf_depth_component16: count = 16; break; //DEPTH_COMPONENT, 16
		case tf_depth_component24: count = 24; break; //DEPTH_COMPONENT, 24
		case tf_depth_component32: count = 32; break; //DEPTH_COMPONENT, 32
		case tf_depth_component32f: count = 32; break; //DEPTH_COMPONENT, f32
		case tf_depth24_stencil8: count = 24 + 8; break; //DEPTH_STENCIL, 24, 8
		case tf_depth32f_stencil8: count = 32 + 8; break; //DEPTH_STENCIL, f32, 8
		default:
			throw Exception("cannot obtain pixel count for this TextureFormat");
		}

		return (count + 7) / 8;
	}

	void PixelBuffer_Impl::convert(PixelBuffer &target, const Rect &dest_rect, const Rect &src_rect, PixelConverter &converter) const
	{
		if (dest_rect.get_size() != src_rect.get_size())
		{
			throw Exception("Source and destination rects must have same size. Scaled converting not supported.");
		}

		char *src_data = (char *)provider->get_data();
		char *dest_data = (char *)target.get_data();

		int src_pitch = provider->get_size().width * get_bytes_per_pixel();
		int dest_pitch = target.get_width() * target.get_bytes_per_pixel();

		src_data += src_rect.top*src_pitch + src_rect.left*get_bytes_per_pixel();
		dest_data += dest_rect.top*dest_pitch + dest_rect.left*target.get_bytes_per_pixel();

		converter.convert(dest_data, dest_pitch, target.get_format(), src_data, src_pitch, get_format(), dest_rect.get_width(), dest_rect.get_height());
	}
}
