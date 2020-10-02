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

#include "Display/precomp.h"
#include "API/Display/Image/pixel_converter.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/system.h"
#include "pixel_converter_impl.h"
#include "pixel_reader_cast.h"
#include "pixel_reader_half_float.h"
#include "pixel_reader_norm.h"
#include "pixel_reader_special.h"

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
#include "pixel_reader_sse.h"
#endif

#include "pixel_writer_cast.h"
#include "pixel_writer_half_float.h"
#include "pixel_writer_norm.h"
#include "pixel_writer_special.h"

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
#include "pixel_writer_sse.h"
#endif

#include "pixel_filter_gamma.h"
#include "pixel_filter_premultiply_alpha.h"
#include "pixel_filter_swizzle.h"
#include "pixel_filter_rgb_to_ycrcb.h"

namespace clan
{
	PixelConverter::PixelConverter()
		: impl(std::make_shared<PixelConverter_Impl>())
	{
	}

	PixelConverter::~PixelConverter()
	{
	}

	bool PixelConverter::get_premultiply_alpha() const
	{
		return impl->premultiply_alpha;
	}

	bool PixelConverter::flip_vertical() const
	{
		return impl->flip_vertical;
	}

	float PixelConverter::get_gamma() const
	{
		return impl->gamma;
	}

	Vec4i PixelConverter::get_swizzle() const
	{
		return impl->swizzle;
	}

	bool PixelConverter::get_input_is_ycrcb() const
	{
		return impl->input_is_ycrcb;
	}

	bool PixelConverter::get_output_is_ycrcb() const
	{
		return impl->output_is_ycrcb;
	}

	void PixelConverter::set_premultiply_alpha(bool enable)
	{
		impl->premultiply_alpha = enable;
	}

	void PixelConverter::set_flip_vertical(bool enable)
	{
		impl->flip_vertical = enable;
	}

	void PixelConverter::set_gamma(float gamma)
	{
		impl->gamma = gamma;
	}

	void PixelConverter::set_swizzle(int red_source, int green_source, int blue_source, int alpha_source)
	{
		set_swizzle(Vec4i(red_source, green_source, blue_source, alpha_source));
	}

	void PixelConverter::set_swizzle(const Vec4i &swizzle)
	{
		impl->swizzle = swizzle;
	}

	void PixelConverter::set_input_is_ycrcb(bool enable)
	{
		impl->input_is_ycrcb = enable;
	}

	void PixelConverter::set_output_is_ycrcb(bool enable)
	{
		impl->output_is_ycrcb = enable;
	}

	void PixelConverter::convert(void *output, int output_pitch, TextureFormat output_format, const void *input, int input_pitch, TextureFormat input_format, int width, int height)
	{
		bool sse2 = System::detect_cpu_extension(System::sse2);
		bool sse4 = System::detect_cpu_extension(System::sse4_1);

		std::unique_ptr<PixelReader> reader = impl->create_reader(input_format, sse2);
		std::unique_ptr<PixelWriter> writer = impl->create_writer(output_format, sse2, sse4);
		std::vector<std::shared_ptr<PixelFilter> > filters = impl->create_filters(sse2);

		DataBuffer work_buffer(width * sizeof(Vec4f));
		Vec4f *temp = work_buffer.get_data<Vec4f>();
		for (int input_y = 0; input_y < height; input_y++)
		{
			int output_y = impl->flip_vertical ? (height - 1 - input_y) : input_y;

			const char *input_line = static_cast<const char*>(input)+input_pitch * input_y;
			char *output_line = static_cast<char*>(output)+output_pitch * output_y;
			reader->read(input_line, temp, width);
			for (auto & filter : filters)
				filter->filter(temp, width);
			writer->write(output_line, temp, width);
		}
	}

	std::unique_ptr<PixelReader> PixelConverter_Impl::create_reader(TextureFormat format, bool sse2)
	{
		switch (format)
		{
		case TextureFormat::bgra8:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelReader>(new PixelReaderSSE2_bgra8());
			else
#endif
				return std::unique_ptr<PixelReader>(new PixelReader_bgra8());
		case TextureFormat::bgr8:
			return std::unique_ptr<PixelReader>(new PixelReader_bgr8());
		case TextureFormat::stencil_index1:
		case TextureFormat::stencil_index4:
		case TextureFormat::stencil_index8:
		case TextureFormat::stencil_index16:
			break;

		case TextureFormat::r8:
			return std::unique_ptr<PixelReader>(new PixelReader_1norm<unsigned char>());
		case TextureFormat::r8_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_1norm<char>());
		case TextureFormat::r16:
			return std::unique_ptr<PixelReader>(new PixelReader_1norm<unsigned short>());
		case TextureFormat::r16_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_1norm<short>());
		case TextureFormat::rg8:
			return std::unique_ptr<PixelReader>(new PixelReader_2norm<unsigned char>());
		case TextureFormat::rg8_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_2norm<char>());
		case TextureFormat::rg16:
			return std::unique_ptr<PixelReader>(new PixelReader_2norm<unsigned short>());
		case TextureFormat::rg16_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_2norm<short>());

		case TextureFormat::r3_g3_b2:
			return std::unique_ptr<PixelReader>(new PixelReader_r3_g3_b2());
		case TextureFormat::rgb4:
			return std::unique_ptr<PixelReader>(new PixelReader_rgb4());
		case TextureFormat::rgb5:
			return std::unique_ptr<PixelReader>(new PixelReader_rgb5());
		case TextureFormat::rgb8:
			return std::unique_ptr<PixelReader>(new PixelReader_3norm<unsigned char>());
		case TextureFormat::rgb8_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_3norm<char>());
		case TextureFormat::rgb10:
			return std::unique_ptr<PixelReader>(new PixelReader_rgb10());
		case TextureFormat::rgb12:
			break;
		case TextureFormat::rgb16:
			return std::unique_ptr<PixelReader>(new PixelReader_3norm<unsigned short>());
		case TextureFormat::rgb16_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_3norm<short>());
		case TextureFormat::rgba2:
			break;
		case TextureFormat::rgba4:
			return std::unique_ptr<PixelReader>(new PixelReader_rgba4());
		case TextureFormat::rgb5_a1:
			return std::unique_ptr<PixelReader>(new PixelReader_rgb5_a1());
		case TextureFormat::rgba8:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelReader>(new PixelReaderSSE2_rgba8());
			else
#endif
				return std::unique_ptr<PixelReader>(new PixelReader_4norm<unsigned char>());
		case TextureFormat::rgba8_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_4norm<char>());
		case TextureFormat::rgb10_a2:
			return std::unique_ptr<PixelReader>(new PixelReader_rgb10_a2());
		case TextureFormat::rgba12:
			break;
		case TextureFormat::rgba16:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelReader>(new PixelReaderSSE2_rgba16());
			else
#endif
				return std::unique_ptr<PixelReader>(new PixelReader_4norm<unsigned short>());
		case TextureFormat::rgba16_snorm:
			return std::unique_ptr<PixelReader>(new PixelReader_4norm<short>());
		case TextureFormat::srgb8:
			return std::unique_ptr<PixelReader>(new PixelReader_3norm<unsigned char>()); // TBD: should we add a 2.2 gamma filter?
		case TextureFormat::srgb8_alpha8:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelReader>(new PixelReaderSSE2_rgba8());
			else
#endif
				return std::unique_ptr<PixelReader>(new PixelReader_4norm<char>()); // TBD: should we add a 2.2 gamma filter?
		case TextureFormat::r16f:
			return std::unique_ptr<PixelReader>(new PixelReader_1hf());
		case TextureFormat::rg16f:
			return std::unique_ptr<PixelReader>(new PixelReader_2hf());
		case TextureFormat::rgb16f:
			return std::unique_ptr<PixelReader>(new PixelReader_3hf());
		case TextureFormat::rgba16f:
			return std::unique_ptr<PixelReader>(new PixelReader_4hf());
		case TextureFormat::r32f:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<float>());
		case TextureFormat::rg32f:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<float>());
		case TextureFormat::rgb32f:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<float>());
		case TextureFormat::rgba32f:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<float>());
		case TextureFormat::r11f_g11f_b10f:
		case TextureFormat::rgb9_e5:
		case TextureFormat::r8i:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<char>());
		case TextureFormat::r8ui:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<unsigned char>());
		case TextureFormat::r16i:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<short>());
		case TextureFormat::r16ui:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<unsigned short>());
		case TextureFormat::r32i:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<int>());
		case TextureFormat::r32ui:
			return std::unique_ptr<PixelReader>(new PixelReader_1cast<unsigned int>());
		case TextureFormat::rg8i:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<char>());
		case TextureFormat::rg8ui:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<unsigned char>());
		case TextureFormat::rg16i:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<short>());
		case TextureFormat::rg16ui:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<unsigned short>());
		case TextureFormat::rg32i:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<int>());
		case TextureFormat::rg32ui:
			return std::unique_ptr<PixelReader>(new PixelReader_2cast<unsigned int>());
		case TextureFormat::rgb8i:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<char>());
		case TextureFormat::rgb8ui:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<unsigned char>());
		case TextureFormat::rgb16i:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<short>());
		case TextureFormat::rgb16ui:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<unsigned short>());
		case TextureFormat::rgb32i:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<int>());
		case TextureFormat::rgb32ui:
			return std::unique_ptr<PixelReader>(new PixelReader_3cast<unsigned int>());
		case TextureFormat::rgba8i:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<char>());
		case TextureFormat::rgba8ui:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<unsigned char>());
		case TextureFormat::rgba16i:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<short>());
		case TextureFormat::rgba16ui:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<unsigned short>());
		case TextureFormat::rgba32i:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<int>());
		case TextureFormat::rgba32ui:
			return std::unique_ptr<PixelReader>(new PixelReader_4cast<unsigned int>());
		case TextureFormat::depth_component16:
		case TextureFormat::depth_component24:
		case TextureFormat::depth_component32:
		case TextureFormat::depth_component32f:
		case TextureFormat::depth24_stencil8:
		case TextureFormat::depth32f_stencil8:
		case TextureFormat::compressed_red:
		case TextureFormat::compressed_rg:
		case TextureFormat::compressed_rgb:
		case TextureFormat::compressed_rgba:
		case TextureFormat::compressed_srgb:
		case TextureFormat::compressed_srgb_alpha:
		case TextureFormat::compressed_red_rgtc1:
		case TextureFormat::compressed_signed_red_rgtc1:
		case TextureFormat::compressed_rg_rgtc2:
		case TextureFormat::compressed_signed_rg_rgtc2:
		case TextureFormat::compressed_rgb_s3tc_dxt1:
		case TextureFormat::compressed_rgba_s3tc_dxt1:
		case TextureFormat::compressed_rgba_s3tc_dxt3:
		case TextureFormat::compressed_rgba_s3tc_dxt5:
		case TextureFormat::compressed_srgb_s3tc_dxt1:
		case TextureFormat::compressed_srgb_alpha_s3tc_dxt1:
		case TextureFormat::compressed_srgb_alpha_s3tc_dxt3:
		case TextureFormat::compressed_srgb_alpha_s3tc_dxt5:
		default:
			break;
		};
		throw Exception("Pixel format not yet supported");
	}

	std::unique_ptr<PixelWriter> PixelConverter_Impl::create_writer(TextureFormat format, bool sse2, bool sse4)
	{
		switch (format)
		{
		case TextureFormat::bgra8:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelWriter>(new PixelWriterSSE2_bgra8());
			else
#endif
				return std::unique_ptr<PixelWriter>(new PixelWriter_bgra8());
		case TextureFormat::bgr8:
			return std::unique_ptr<PixelWriter>(new PixelWriter_bgr8());
		case TextureFormat::stencil_index1:
		case TextureFormat::stencil_index4:
		case TextureFormat::stencil_index8:
		case TextureFormat::stencil_index16:
			break;

		case TextureFormat::r8:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1norm<unsigned char>());
		case TextureFormat::r8_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1norm<char>());
		case TextureFormat::r16:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1norm<unsigned short>());
		case TextureFormat::r16_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1norm<short>());
		case TextureFormat::rg8:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2norm<unsigned char>());
		case TextureFormat::rg8_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2norm<char>());
		case TextureFormat::rg16:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2norm<unsigned short>());
		case TextureFormat::rg16_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2norm<short>());

		case TextureFormat::r3_g3_b2:
			return std::unique_ptr<PixelWriter>(new PixelWriter_r3_g3_b2());
		case TextureFormat::rgb4:
			return std::unique_ptr<PixelWriter>(new PixelWriter_rgb4());
		case TextureFormat::rgb5:
			return std::unique_ptr<PixelWriter>(new PixelWriter_rgb5());
		case TextureFormat::rgb8:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3norm<unsigned char>());
		case TextureFormat::rgb8_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3norm<char>());
		case TextureFormat::rgb10:
			return std::unique_ptr<PixelWriter>(new PixelWriter_rgb10());
		case TextureFormat::rgb12:
			break;
		case TextureFormat::rgb16:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3norm<unsigned short>());
		case TextureFormat::rgb16_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3norm<short>());
		case TextureFormat::rgba2:
			break;
		case TextureFormat::rgba4:
			return std::unique_ptr<PixelWriter>(new PixelWriter_rgba4());
		case TextureFormat::rgb5_a1:
			return std::unique_ptr<PixelWriter>(new PixelWriter_rgb5_a1());
		case TextureFormat::rgba8:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelWriter>(new PixelWriterSSE2_rgba8());
			else
#endif
				return std::unique_ptr<PixelWriter>(new PixelWriter_4norm<unsigned char>());
		case TextureFormat::rgba8_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4norm<char>());
		case TextureFormat::rgb10_a2:
			return std::unique_ptr<PixelWriter>(new PixelWriter_rgb10_a2());
		case TextureFormat::rgba12:
			break;
		case TextureFormat::rgba16:
#if defined(__SSE4_1__)
			if (sse4)
				return std::unique_ptr<PixelWriter>(new PixelWriterSSE4_rgba16());
			else
				return std::unique_ptr<PixelWriter>(new PixelWriter_4norm<unsigned short>());
#else
			return std::unique_ptr<PixelWriter>(new PixelWriter_4norm<unsigned short>());
#endif
		case TextureFormat::rgba16_snorm:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4norm<short>());
		case TextureFormat::srgb8:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3norm<unsigned char>()); // TBD: should we add a 2.2 gamma filter?
		case TextureFormat::srgb8_alpha8:
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				return std::unique_ptr<PixelWriter>(new PixelWriterSSE2_rgba8());
			else
#endif
				return std::unique_ptr<PixelWriter>(new PixelWriter_4norm<char>()); // TBD: should we add a 2.2 gamma filter?
		case TextureFormat::r16f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1hf());
		case TextureFormat::rg16f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2hf());
		case TextureFormat::rgb16f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3hf());
		case TextureFormat::rgba16f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4hf());
		case TextureFormat::r32f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<float>());
		case TextureFormat::rg32f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<float>());
		case TextureFormat::rgb32f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<float>());
		case TextureFormat::rgba32f:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<float>());
		case TextureFormat::r11f_g11f_b10f:
		case TextureFormat::rgb9_e5:
		case TextureFormat::r8i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<char>());
		case TextureFormat::r8ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<unsigned char>());
		case TextureFormat::r16i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<short>());
		case TextureFormat::r16ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<unsigned short>());
		case TextureFormat::r32i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<int>());
		case TextureFormat::r32ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_1cast<unsigned int>());
		case TextureFormat::rg8i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<char>());
		case TextureFormat::rg8ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<unsigned char>());
		case TextureFormat::rg16i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<short>());
		case TextureFormat::rg16ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<unsigned short>());
		case TextureFormat::rg32i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<int>());
		case TextureFormat::rg32ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_2cast<unsigned int>());
		case TextureFormat::rgb8i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<char>());
		case TextureFormat::rgb8ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<unsigned char>());
		case TextureFormat::rgb16i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<short>());
		case TextureFormat::rgb16ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<unsigned short>());
		case TextureFormat::rgb32i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<int>());
		case TextureFormat::rgb32ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_3cast<unsigned int>());
		case TextureFormat::rgba8i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<char>());
		case TextureFormat::rgba8ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<unsigned char>());
		case TextureFormat::rgba16i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<short>());
		case TextureFormat::rgba16ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<unsigned short>());
		case TextureFormat::rgba32i:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<int>());
		case TextureFormat::rgba32ui:
			return std::unique_ptr<PixelWriter>(new PixelWriter_4cast<unsigned int>());
		case TextureFormat::depth_component16:
		case TextureFormat::depth_component24:
		case TextureFormat::depth_component32:
		case TextureFormat::depth_component32f:
		case TextureFormat::depth24_stencil8:
		case TextureFormat::depth32f_stencil8:
		case TextureFormat::compressed_red:
		case TextureFormat::compressed_rg:
		case TextureFormat::compressed_rgb:
		case TextureFormat::compressed_rgba:
		case TextureFormat::compressed_srgb:
		case TextureFormat::compressed_srgb_alpha:
		case TextureFormat::compressed_red_rgtc1:
		case TextureFormat::compressed_signed_red_rgtc1:
		case TextureFormat::compressed_rg_rgtc2:
		case TextureFormat::compressed_signed_rg_rgtc2:
		case TextureFormat::compressed_rgb_s3tc_dxt1:
		case TextureFormat::compressed_rgba_s3tc_dxt1:
		case TextureFormat::compressed_rgba_s3tc_dxt3:
		case TextureFormat::compressed_rgba_s3tc_dxt5:
		case TextureFormat::compressed_srgb_s3tc_dxt1:
		case TextureFormat::compressed_srgb_alpha_s3tc_dxt1:
		case TextureFormat::compressed_srgb_alpha_s3tc_dxt3:
		case TextureFormat::compressed_srgb_alpha_s3tc_dxt5:
		default:
			break;
		};
		throw Exception("Pixel format not yet supported");
	}

	std::vector<std::shared_ptr<PixelFilter> > PixelConverter_Impl::create_filters(bool sse2)
	{
		std::vector<std::shared_ptr<PixelFilter> > filters;

		if (input_is_ycrcb)
		{
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterSSE2_YCrCbToRGB()));
			else
#endif
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterYCrCbToRGB()));
		}

		if (premultiply_alpha)
		{
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterPremultiplyAlphaSSE2()));
			else
#endif
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterPremultiplyAlpha()));
		}

		if (gamma != 1.0f)
		{
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterGammaSSE2(gamma)));
			else
#endif
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterGamma(gamma)));
		}

		if (swizzle != Vec4i(0, 1, 2, 3))
		{
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterSwizzleSSE2(swizzle)));
			else
#endif
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterSwizzle(swizzle)));
		}

		if (output_is_ycrcb)
		{
#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
			if (sse2)
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterSSE2_RGBToYCrCb()));
			else
#endif
				filters.push_back(std::shared_ptr<PixelFilter>(new PixelFilterRGBToYCrCb()));
		}

		return filters;
	}
}
