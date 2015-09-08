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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_converter.h"
#include "API/Display/Image/pixel_buffer_lock.h"
#include "API/Display/2D/color.h"
#include "API/Core/System/cl_platform.h"
#include "pixel_buffer_impl.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"
#include "API/Display/Render/transfer_texture.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Image/texture_format.h"
#include "API/Core/Math/half_float.h"

namespace clan
{
	PixelBuffer::PixelBuffer()
	{
	}

	PixelBuffer::PixelBuffer(PixelBufferProvider *provider)
		: impl(std::make_shared<PixelBuffer_Impl>(provider))
	{
	}

	PixelBuffer::PixelBuffer(int width, int height, TextureFormat texture_format, const void *data, bool only_reference_data)
		: impl(std::make_shared<PixelBuffer_Impl>(width, height, texture_format, data, only_reference_data))
	{
	}

	PixelBuffer::PixelBuffer(const std::string &fullname, bool srgb)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		*this = PixelBuffer(filename, vfs, srgb);
	}

	PixelBuffer::PixelBuffer(const std::string &filename, const FileSystem &fs, bool srgb)
	{
		*this = ImageProviderFactory::load(filename, fs, "", srgb);
	}

	PixelBuffer::PixelBuffer(IODevice &file, const std::string &image_type, bool srgb)
	{
		*this = ImageProviderFactory::load(file, image_type, srgb);
	}

	PixelBuffer::~PixelBuffer()
	{
	}

	void PixelBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("PixelBuffer is null");
	}

	int PixelBuffer::get_width() const
	{
		return impl->provider->get_size().width;
	}

	int PixelBuffer::get_height() const
	{
		return impl->provider->get_size().height;
	}

	bool PixelBuffer::is_gpu() const
	{
		return impl->provider->is_gpu();
	}

	int PixelBuffer::get_pitch() const
	{
		return impl->provider->get_pitch();
	}

	void *PixelBuffer::get_data()
	{
		return impl->provider->get_data();
	}

	const void *PixelBuffer::get_data() const
	{
		return impl->provider->get_data();
	}

	bool PixelBuffer::has_transparency() const
	{
		switch (get_format())
		{
		case tf_rgba8:
		case tf_bgra8:
		case tf_rgba2:
		case tf_rgba4:
		case tf_rgb5_a1:
		case tf_rgba8_snorm:
		case tf_rgb10_a2:
		case tf_rgba12:
		case tf_rgba16:
		case tf_rgba16_snorm:
		case tf_srgb8_alpha8:
		case tf_rgba32f:
		case tf_rgba8i:
		case tf_rgba8ui:
		case tf_rgba16i:
		case tf_rgba16ui:
		case tf_rgba32i:
		case tf_rgba32ui:
		case tf_compressed_rgba:
		case tf_compressed_srgb_alpha:
		case tf_compressed_rgba_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt3:
		case tf_compressed_rgba_s3tc_dxt5:
		case tf_compressed_srgb_alpha_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt3:
		case tf_compressed_srgb_alpha_s3tc_dxt5:
			return true;

		case tf_rgb8:
		case tf_bgr8:
		case tf_stencil_index1:
		case tf_stencil_index4:
		case tf_stencil_index8:
		case tf_stencil_index16:
		case tf_r8:
		case tf_r8_snorm:
		case tf_r16:
		case tf_r16_snorm:
		case tf_rg8:
		case tf_rg8_snorm:
		case tf_rg16:
		case tf_rg16_snorm:
		case tf_r3_g3_b2:
		case tf_rgb4:
		case tf_rgb5:
		case tf_rgb8_snorm:
		case tf_rgb10:
		case tf_rgb12:
		case tf_rgb16:
		case tf_rgb16_snorm:
		case tf_srgb8:
		case tf_r16f:
		case tf_rg16f:
		case tf_rgb16f:
		case tf_rgba16f:
		case tf_r32f:
		case tf_rg32f:
		case tf_rgb32f:
		case tf_r11f_g11f_b10f:
		case tf_rgb9_e5:
		case tf_r8i:
		case tf_r8ui:
		case tf_r16i:
		case tf_r16ui:
		case tf_r32i:
		case tf_r32ui:
		case tf_rg8i:
		case tf_rg8ui:
		case tf_rg16i:
		case tf_rg16ui:
		case tf_rg32i:
		case tf_rg32ui:
		case tf_rgb8i:
		case tf_rgb8ui:
		case tf_rgb16i:
		case tf_rgb16ui:
		case tf_rgb32i:
		case tf_rgb32ui:
		case tf_depth_component16:
		case tf_depth_component24:
		case tf_depth_component32:
		case tf_depth_component32f:
		case tf_depth24_stencil8:
		case tf_depth32f_stencil8:
		case tf_compressed_red:
		case tf_compressed_rg:
		case tf_compressed_rgb:
		case tf_compressed_srgb:
		case tf_compressed_red_rgtc1:
		case tf_compressed_signed_red_rgtc1:
		case tf_compressed_rg_rgtc2:
		case tf_compressed_signed_rg_rgtc2:
		case tf_compressed_rgb_s3tc_dxt1:
		case tf_compressed_srgb_s3tc_dxt1:
			return false;

		default:
			throw Exception("Unknown pixel buffer format encountered in PixelBuffer::has_transparency");
		}
	}

	unsigned int PixelBuffer::get_bytes_per_pixel() const
	{
		return impl->get_bytes_per_pixel();
	}

	unsigned int PixelBuffer::get_bytes_per_pixel(TextureFormat texture_format)
	{
		return PixelBuffer_Impl::get_bytes_per_pixel(texture_format);
	}

	unsigned int PixelBuffer::get_bytes_per_block() const
	{
		return impl->get_bytes_per_block();
	}

	unsigned int PixelBuffer::get_bytes_per_block(TextureFormat texture_format)
	{
		return PixelBuffer_Impl::get_bytes_per_block(texture_format);
	}

	unsigned int PixelBuffer::get_data_size() const
	{
		return PixelBuffer_Impl::get_data_size(get_size(), get_format());
	}

	unsigned int PixelBuffer::get_data_size(const Size &size, TextureFormat texture_format)
	{
		return PixelBuffer_Impl::get_data_size(size, texture_format);
	}

	bool PixelBuffer::is_compressed() const
	{
		return PixelBuffer_Impl::is_compressed(impl->get_format());
	}

	bool PixelBuffer::is_compressed(TextureFormat texture_format)
	{
		return PixelBuffer_Impl::is_compressed(texture_format);
	}

	TextureFormat PixelBuffer::get_format() const
	{
		return impl->get_format();
	}

	PixelBufferProvider *PixelBuffer::get_provider() const
	{
		return impl->provider;
	}

	Colorf PixelBuffer::get_pixel(int x, int y)
	{
		return impl->get_pixel(x, y);
	}

	float PixelBuffer::get_pixel_ratio() const
	{
		return impl->pixel_ratio;
	}

	void PixelBuffer::set_pixel_ratio(float ratio)
	{
		impl->pixel_ratio = ratio;
	}

	void PixelBuffer::lock(GraphicContext &gc, BufferAccess access)
	{
		impl->provider->lock(gc, access);
	}

	void PixelBuffer::unlock()
	{
		impl->provider->unlock();
	}

	void PixelBuffer::upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data)
	{
		impl->provider->upload_data(gc, dest_rect, data);
	}

	PixelBuffer PixelBuffer::copy() const
	{
		PixelBuffer pbuf(get_width(), get_height(), get_format());
		void *dst_data = (void *)pbuf.get_data();
		void *src_data = (void *)get_data();
		memcpy(dst_data, src_data, get_height()*get_pitch());
		return pbuf;
	}

	PixelBuffer PixelBuffer::copy(const Rect &rect) const
	{
		Size size = impl->provider->get_size();

		if (rect.left < 0 || rect.top < 0 || rect.right > size.width || rect.bottom > size.height)
			throw Exception("Rectangle passed to PixelBuffer::copy() out of bounds");

		int new_width = rect.get_width();
		int new_height = rect.get_height();

		PixelBuffer pbuf(new_width, new_height, get_format());
		uint8_t *dst_data = (uint8_t *)pbuf.get_data();
		uint8_t *src_data = (uint8_t *)get_data() + (rect.top*size.width + rect.left)*get_bytes_per_pixel();

		int byte_width = new_width * get_bytes_per_pixel();
		int src_pitch = get_pitch();
		int dest_pitch = pbuf.get_pitch();
		for (int y_cnt = 0; y_cnt < new_height; y_cnt++)
		{
			memcpy(dst_data, src_data, byte_width);
			dst_data += dest_pitch;
			src_data += src_pitch;
		}
		return pbuf;
	}

	void PixelBuffer::set_image(const PixelBuffer &source)
	{
		set_subimage(source, Point(0, 0), Rect(Point(0, 0), source.get_size()));
	}

	void PixelBuffer::set_subimage(const PixelBuffer &source, const Point &dest_pos, const Rect &src_rect)
	{
		PixelConverter converter;
		source.impl->convert(*this, Rect(dest_pos, src_rect.get_size()), src_rect, converter);
	}

	void PixelBuffer::set_image(const PixelBuffer &source, PixelConverter &converter)
	{
		set_subimage(source, Point(0, 0), Rect(Point(0, 0), source.get_size()), converter);
	}

	void PixelBuffer::set_subimage(const PixelBuffer &source, const Point &dest_pos, const Rect &src_rect, PixelConverter &converter)
	{
		source.impl->convert(*this, Rect(dest_pos, src_rect.get_size()), src_rect, converter);
	}


	PixelBuffer PixelBuffer::to_cpu(GraphicContext &gc)
	{
		if (is_gpu())
		{
			PixelBuffer cpu_buffer(get_width(), get_height(), get_format());
			PixelBufferLockAny data_cpu(gc, cpu_buffer, access_read_only);
			PixelBufferLockAny data_gpu(gc, *this, access_read_only);
			int bytes_per_row = get_bytes_per_pixel() * get_width();
			int height = get_height();
			for (int y = 0; y < height; y++)
			{
				memcpy(data_cpu.get_row(y), data_gpu.get_row(y), bytes_per_row);
			}
			return cpu_buffer;
		}
		else
		{
			return *this;
		}
	}

	PixelBuffer PixelBuffer::to_gpu(GraphicContext &gc)
	{
		if (!is_gpu())
		{
			return TransferTexture(gc, *this);
		}
		else
		{
			return *this;
		}
	}

	PixelBuffer PixelBuffer::to_format(TextureFormat texture_format) const
	{
		PixelBuffer result(get_width(), get_height(), texture_format);
		result.set_image(*this);
		return result;
	}

	PixelBuffer PixelBuffer::to_format(TextureFormat texture_format, PixelConverter &converter) const
	{
		PixelBuffer result(get_width(), get_height(), texture_format);
		result.set_image(*this, converter);
		return result;
	}


	void PixelBuffer::flip_vertical()
	{
		Size size = impl->provider->get_size();
		if ((size.width == 0) || (size.height <= 1))
			return;

		unsigned int pitch = get_pitch();
		std::vector<unsigned char> line_buffer;
		line_buffer.resize(pitch);

		int num_lines = (size.height - 1) / 2;

		int start_offset = 0;
		int end_offset = (size.height - 1) * pitch;

		char *data = (char *)get_data();

		for (int cnt = 0; cnt < num_lines; cnt++, start_offset += pitch, end_offset -= pitch)
		{
			memcpy(&line_buffer[0], &data[start_offset], pitch);
			memcpy(&data[start_offset], &data[end_offset], pitch);
			memcpy(&data[end_offset], &line_buffer[0], pitch);
		}
	}

	void PixelBuffer::premultiply_alpha()
	{
		if (has_transparency())
		{
			if (get_format() == tf_rgba8 || get_format() == tf_srgb8_alpha8)
			{
				int w = get_width();
				int h = get_height();
				uint32_t *p = (uint32_t *)get_data();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					uint32_t *line = p + index;
					for (int x = 0; x < w; x++)
					{
						uint32_t a = ((line[x] >> 24) & 0xff);
						uint32_t b = ((line[x] >> 16) & 0xff);
						uint32_t g = ((line[x] >> 8) & 0xff);
						uint32_t r = (line[x] & 0xff);

						r = r * a / 255;
						g = g * a / 255;
						b = b * a / 255;

						line[x] = (a << 24) + (b << 16) + (g << 8) + r;
					}
				}
			}
			else if (get_format() == tf_bgra8)
			{
				int w = get_width();
				int h = get_height();
				uint32_t *p = (uint32_t *)get_data();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					uint32_t *line = p + index;
					for (int x = 0; x < w; x++)
					{
						uint32_t a = ((line[x] >> 24) & 0xff);
						uint32_t r = ((line[x] >> 16) & 0xff);
						uint32_t g = ((line[x] >> 8) & 0xff);
						uint32_t b = (line[x] & 0xff);

						r = r * a / 255;
						g = g * a / 255;
						b = b * a / 255;

						line[x] = (a << 24) + (r << 16) + (g << 8) + b;
					}
				}
			}
			else if (get_format() == tf_rgba16)
			{
				int w = get_width();
				int h = get_height();
				uint16_t *p = (uint16_t *)get_data();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					uint16_t *line = p + index;
					for (int x = 0; x < w; x++)
					{
						uint32_t r = line[x * 4];
						uint32_t g = line[x * 4 + 1];
						uint32_t b = line[x * 4 + 2];
						uint32_t a = line[x * 4 + 3];

						r = r * a / 65535;
						g = g * a / 65535;
						b = b * a / 65535;

						line[x * 4] = r;
						line[x * 4 + 1] = g;
						line[x * 4 + 2] = b;
					}
				}
			}
			else if (get_format() == tf_rgba16f)
			{
				int w = get_width();
				int h = get_height();
				unsigned short *p = (unsigned short *)get_data();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					unsigned short *line = p + index;
					for (int x = 0; x < w; x++)
					{
						float r = HalfFloat::half_to_float(line[x * 4]);
						float g = HalfFloat::half_to_float(line[x * 4 + 1]);
						float b = HalfFloat::half_to_float(line[x * 4 + 2]);
						float a = HalfFloat::half_to_float(line[x * 4 + 3]);

						r = r * a;
						g = g * a;
						b = b * a;

						line[x * 4] = HalfFloat::float_to_half(r);
						line[x * 4 + 1] = HalfFloat::float_to_half(g);
						line[x * 4 + 2] = HalfFloat::float_to_half(b);
					}
				}
			}
			else if (get_format() == tf_rgba32f)
			{
				int w = get_width();
				int h = get_height();
				float *p = (float *)get_data();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					float *line = p + index;
					for (int x = 0; x < w; x++)
					{
						float r = line[x * 4];
						float g = line[x * 4 + 1];
						float b = line[x * 4 + 2];
						float a = line[x * 4 + 3];

						r = r * a;
						g = g * a;
						b = b * a;

						line[x * 4] = r;
						line[x * 4 + 1] = g;
						line[x * 4 + 2] = b;
					}
				}
			}
			else
			{
				throw Exception("premultiply_alpha does not support the image format");
			}
		}
	}

	void PixelBuffer::premultiply_gamma(float gamma)
	{
		if (get_format() == tf_rgba8 || get_format() == tf_srgb8_alpha8 || get_format() == tf_bgra8)
		{
			PixelBufferLock4ub lock(*this);
			for (int y = 0; y < lock.get_height(); y++)
			{
				Vec4ub *line = lock.get_row(y);
				for (int x = 0; x < lock.get_width(); x++)
				{
					const float rcp_255 = 1.0f / 255.0f;
					float red = std::pow(line[x].r * rcp_255, gamma);
					float green = std::pow(line[x].g * rcp_255, gamma);
					float blue = std::pow(line[x].b * rcp_255, gamma);
					line[x].r = static_cast<unsigned short>(clamp(red * 255.0f + 0.5f, 0.0f, 255.0f));
					line[x].g = static_cast<unsigned short>(clamp(green * 255.0f + 0.5f, 0.0f, 255.0f));
					line[x].b = static_cast<unsigned short>(clamp(blue * 255.0f + 0.5f, 0.0f, 255.0f));
				}
			}
		}
		else if (get_format() == tf_rgba16)
		{
			PixelBufferLock4us lock(*this);
			for (int y = 0; y < lock.get_height(); y++)
			{
				Vec4us *line = lock.get_row(y);
				for (int x = 0; x < lock.get_width(); x++)
				{
					const float rcp_65535 = 1.0f / 65535.0f;
					float red = std::pow(line[x].r * rcp_65535, gamma);
					float green = std::pow(line[x].g * rcp_65535, gamma);
					float blue = std::pow(line[x].b * rcp_65535, gamma);
					line[x].r = static_cast<unsigned short>(clamp(red * 65535.0f + 0.5f, 0.0f, 65535.0f));
					line[x].g = static_cast<unsigned short>(clamp(green * 65535.0f + 0.5f, 0.0f, 65535.0f));
					line[x].b = static_cast<unsigned short>(clamp(blue * 65535.0f + 0.5f, 0.0f, 65535.0f));
				}
			}
		}
		else if (get_format() == tf_rgba16f)
		{
			PixelBufferLock4hf lock(*this);
			for (int y = 0; y < lock.get_height(); y++)
			{
				Vec4hf *line = lock.get_row(y);
				for (int x = 0; x < lock.get_width(); x++)
				{
					Vec4f v = line[x].to_float();
					v.r = std::pow(v.r, gamma);
					v.g = std::pow(v.g, gamma);
					v.b = std::pow(v.b, gamma);
					line[x] = Vec4hf(v);
				}
			}
		}
		else if (get_format() == tf_rgba32f)
		{
			PixelBufferLock4f lock(*this);
			for (int y = 0; y < lock.get_height(); y++)
			{
				Vec4f *line = lock.get_row(y);
				for (int x = 0; x < lock.get_width(); x++)
				{
					line[x].r = std::pow(line[x].r, gamma);
					line[x].g = std::pow(line[x].g, gamma);
					line[x].b = std::pow(line[x].b, gamma);
				}
			}
		}
	}
}
