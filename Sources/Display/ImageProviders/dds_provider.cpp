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
#include <iostream>
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/dds_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	PixelBufferSet DDSProvider::load(const std::string &filename, const FileSystem &fs)
	{
		IODevice file = fs.open_file(filename);
		return load(file);
	}

	PixelBufferSet DDSProvider::load(const std::string &fullname)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		return load(filename, vfs);
	}

	PixelBufferSet DDSProvider::load(IODevice &file)
	{
#define fourccvalue(a,b,c,d) ((static_cast<unsigned int>(a)) | (static_cast<unsigned int>(b) << 8) | (static_cast<unsigned int>(c) << 16) | (static_cast<unsigned int>(d) << 24))
#define isbitmask(r,g,b,a) (format_red_bit_mask == (r) && format_green_bit_mask == (g) && format_blue_bit_mask == (b) && format_alpha_bit_mask == (a))

		const int DDS_FOURCC = 0x00000004; // DDPF_FOURCC
		const int DDS_RGB = 0x00000040; // DDPF_RGB
		const int DDS_RGBA = 0x00000041; // DDPF_RGB | DDPF_ALPHAPIXELS
		const int DDS_LUMINANCE = 0x00020000; // DDPF_LUMINANCE
		const int DDS_ALPHA = 0x00000002; // DDPF_ALPHA

		const int DDS_HEADER_FLAGS_TEXTURE = 0x00001007; // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_CL_PIXELFORMAT 
		const int DDS_HEADER_FLAGS_MIPMAP = 0x00020000; // DDSD_MIPMAPCOUNT
		const int DDS_HEADER_FLAGS_VOLUME = 0x00800000; // DDSD_DEPTH
		const int DDS_HEADER_FLAGS_CL_PITCH = 0x00000008; // DDSD_CL_PITCH
		const int DDS_HEADER_FLAGS_LINEARSIZE = 0x00080000; // DDSD_LINEARSIZE

		const int DDS_SURFACE_FLAGS_TEXTURE = 0x00001000; // DDSCAPS_TEXTURE
		const int DDS_SURFACE_FLAGS_MIPMAP = 0x00400008; // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
		const int DDS_SURFACE_FLAGS_CUBEMAP = 0x00000008; // DDSCAPS_COMPLEX

		const int DDS_CUBEMAP_POSITIVEX = 0x00000600; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
		const int DDS_CUBEMAP_NEGATIVEX = 0x00000a00; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
		const int DDS_CUBEMAP_POSITIVEY = 0x00001200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
		const int DDS_CUBEMAP_NEGATIVEY = 0x00002200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
		const int DDS_CUBEMAP_POSITIVEZ = 0x00004200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
		const int DDS_CUBEMAP_NEGATIVEZ = 0x00008200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

		const int DDS_CUBEMAP_ALLFACES = DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX | DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY | DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ;

		const int DDS_FLAGS_VOLUME = 0x00200000; // DDSCAPS2_VOLUME

		const int DDS_D3D11_RESOURCE_DIMENSION_UNKNOWN = 0;
		const int DDS_D3D11_RESOURCE_DIMENSION_BUFFER = 1;
		const int DDS_D3D11_RESOURCE_DIMENSION_TEXTURE1D = 2;
		const int DDS_D3D11_RESOURCE_DIMENSION_TEXTURE2D = 3;
		const int DDS_D3D11_RESOURCE_DIMENSION_TEXTURE3D = 4;

		const int DDS_D3D11_RESOURCE_MISC_GENERATE_MIPS = 0x1;
		const int DDS_D3D11_RESOURCE_MISC_TEXTURECUBE = 0x4;

		const int DDS_D3DFMT_A16B16G16R16 = 36;
		const int DDS_D3DFMT_Q16W16V16U16 = 110;
		const int DDS_D3DFMT_R16F = 111;
		const int DDS_D3DFMT_G16R16F = 112;
		const int DDS_D3DFMT_A16B16G16R16F = 113;
		const int DDS_D3DFMT_R32F = 114;
		const int DDS_D3DFMT_G32R32F = 115;
		const int DDS_D3DFMT_A32B32G32R32F = 116;


		unsigned int magic = file.read_uint32();
		if (magic != fourccvalue('D', 'D', 'S', ' '))
			throw Exception("Not a DDS file");

		unsigned int size = file.read_uint32();
		if (size != (23 + 8) * 4)
			throw Exception("Unsupported DDS file version");
		unsigned int header_flags = file.read_uint32();
		unsigned int height = file.read_uint32();
		unsigned int width = file.read_uint32();
		unsigned int pitch_or_linear_size = file.read_uint32();
		unsigned int depth = file.read_uint32(); // Only used if DDS_FLAGS_VOLUME is set
		unsigned int mipmap_count = file.read_uint32();
		unsigned int reserved1[11];
		file.read(reserved1, 11 * sizeof(unsigned int));

		unsigned int format_size = file.read_uint32();
		if (format_size != 8 * 4)
			throw Exception("Unsupported DDS pixel format version");
		unsigned int format_flags = file.read_uint32();
		unsigned int format_fourcc = file.read_uint32();
		unsigned int format_rgb_bit_count = file.read_uint32();
		unsigned int format_red_bit_mask = file.read_uint32();
		unsigned int format_green_bit_mask = file.read_uint32();
		unsigned int format_blue_bit_mask = file.read_uint32();
		unsigned int format_alpha_bit_mask = file.read_uint32();

		unsigned int surface_flags = file.read_uint32();
		unsigned int cubemap_flags = file.read_uint32();

		unsigned int reserved2[3];
		file.read(reserved2, 3 * sizeof(unsigned int));

		bool dx10_extension = (format_flags & DDS_FOURCC) && format_fourcc == fourccvalue('D', 'X', '1', '0');
		unsigned int dx10_dxgi_format = 0;
		unsigned char dx10_resource_dimension = 0;
		unsigned int dx10_misc_flag = 0;
		unsigned int dx10_array_size = 0;
		unsigned int dx10_reserved = 0;
		if (dx10_extension)
		{
			dx10_dxgi_format = file.read_uint32();
			dx10_resource_dimension = file.read_uint8(); // type is D3D11_RESOURCE_DIMENSION, is this an uint or a byte?
			dx10_misc_flag = file.read_uint32();
			dx10_array_size = file.read_uint32();
			dx10_reserved = file.read_uint32();
		}

		if ((header_flags & DDS_HEADER_FLAGS_TEXTURE) != DDS_HEADER_FLAGS_TEXTURE)
			throw Exception("Unsupported DDS header flags combination");

		TextureDimensions texture_dimensions = TextureDimensions::_2d;
		TextureFormat texture_format = TextureFormat::rgba8;
		int texture_width = width;
		int texture_height = height;
		int texture_slices = 1;
		int texture_levels = 1;

		if ((header_flags & DDS_HEADER_FLAGS_MIPMAP) == DDS_HEADER_FLAGS_MIPMAP)
			texture_levels = mipmap_count;

		if (dx10_extension)
		{
			switch (dx10_resource_dimension)
			{
			default:
			case DDS_D3D11_RESOURCE_DIMENSION_UNKNOWN:
			case DDS_D3D11_RESOURCE_DIMENSION_BUFFER:
				throw Exception("Unsupported DDS resource dimension");
			case DDS_D3D11_RESOURCE_DIMENSION_TEXTURE1D:
				texture_dimensions = dx10_array_size == 1 ? TextureDimensions::_1d : TextureDimensions::_1d_array;
				texture_slices = dx10_array_size;
				break;
			case DDS_D3D11_RESOURCE_DIMENSION_TEXTURE2D:
				texture_dimensions = dx10_array_size == 2 ? TextureDimensions::_2d : TextureDimensions::_2d_array;
				texture_slices = dx10_array_size;
				if (dx10_misc_flag & DDS_D3D11_RESOURCE_MISC_TEXTURECUBE)
					texture_dimensions = TextureDimensions::_cube;
				break;
			case DDS_D3D11_RESOURCE_DIMENSION_TEXTURE3D:
				texture_dimensions = TextureDimensions::_3d;
				texture_slices = dx10_array_size;
				break;
			}

			// find texture_format from dx10_dxgi_format
			throw Exception("Congratulations! You found a .DDS file using the DX10 extension header. You won the prize of having to improve the DDS file loader to be able to handle it");
		}
		else
		{
			if ((surface_flags & DDS_SURFACE_FLAGS_CUBEMAP) == DDS_SURFACE_FLAGS_CUBEMAP && (cubemap_flags & DDS_CUBEMAP_ALLFACES) == DDS_CUBEMAP_ALLFACES)
			{
				texture_dimensions = TextureDimensions::_cube;
				texture_slices = 6;
			}
			else if ((header_flags & DDS_HEADER_FLAGS_VOLUME) == DDS_HEADER_FLAGS_VOLUME)
			{
				texture_dimensions = TextureDimensions::_3d;
				texture_slices = depth;
			}
			else
			{
				texture_dimensions = TextureDimensions::_2d;
				texture_slices = 1;
			}

			if (format_flags & DDS_RGB)
			{
				if (format_rgb_bit_count == 32)
				{
					if (isbitmask(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
						texture_format = TextureFormat::bgra8;
					else if (isbitmask(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
						texture_format = TextureFormat::bgra8; // To do: this is actually bgrx8 (alpha component unused)
					else if (isbitmask(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
						texture_format = TextureFormat::rgba8;
					else if (isbitmask(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000))
						texture_format = TextureFormat::rgba8; // To do: this is actually rgbx8 (alpha component unused)
					//else if (isbitmask(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
					//	texture_format = TextureFormat::bgr10_a2;
					else if (isbitmask(0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000))
						texture_format = TextureFormat::rgb10_a2;
					else if (isbitmask(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
						texture_format = TextureFormat::rg16;
					else if (isbitmask(0xffffffff, 0x00000000, 0x00000000, 0x00000000))
						texture_format = TextureFormat::r32f;
					else
						throw Exception("Unsupported pixel format used by DDS file");
				}
				else if (format_rgb_bit_count == 24)
				{
					if (isbitmask(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
						texture_format = TextureFormat::bgr8;
					else if (isbitmask(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000))
						texture_format = TextureFormat::rgb8;
					else
						throw Exception("Unsupported pixel format used by DDS file");
				}
				else
				{
					throw Exception("Unsupported pixel format used by DDS file");
				}
			}
			else if (format_flags & DDS_LUMINANCE)
			{
				throw Exception("Unsupported pixel format used by DDS file");
			}
			else if (format_flags & DDS_ALPHA)
			{
				throw Exception("Unsupported pixel format used by DDS file");
			}
			else if (format_flags & DDS_FOURCC)
			{
				if (format_fourcc == fourccvalue('D', 'X', 'T', '1'))
					texture_format = TextureFormat::compressed_rgb_s3tc_dxt1; // is this TextureFormat::compressed_rgba_s3tc_dxt1?
				else if (format_fourcc == fourccvalue('D', 'X', 'T', '3'))
					texture_format = TextureFormat::compressed_rgba_s3tc_dxt3;
				else if (format_fourcc == fourccvalue('D', 'X', 'T', '5'))
					texture_format = TextureFormat::compressed_rgba_s3tc_dxt5;
				//else if (format_fourcc == fourccvalue('R', 'G', 'B', 'G'))
				//	texture_format = TextureFormat::rgbg8;
				//else if (format_fourcc == fourccvalue('G', 'R', 'B', 'G'))
				//	texture_format = cl_grgb8;
				else if (format_fourcc == DDS_D3DFMT_A16B16G16R16)
					texture_format = TextureFormat::rgba16;
				else if (format_fourcc == DDS_D3DFMT_Q16W16V16U16)
					texture_format = TextureFormat::rgba16_snorm;
				else if (format_fourcc == DDS_D3DFMT_R16F)
					texture_format = TextureFormat::r16f;
				else if (format_fourcc == DDS_D3DFMT_G16R16F)
					texture_format = TextureFormat::rg16f;
				else if (format_fourcc == DDS_D3DFMT_A16B16G16R16F)
					texture_format = TextureFormat::rgba16f;
				else if (format_fourcc == DDS_D3DFMT_R32F)
					texture_format = TextureFormat::r32f;
				else if (format_fourcc == DDS_D3DFMT_G32R32F)
					texture_format = TextureFormat::rg32f;
				else if (format_fourcc == DDS_D3DFMT_A32B32G32R32F)
					texture_format = TextureFormat::rgba32f;
				else
					throw Exception("Unsupported pixel format used by DDS file");
			}
		}

		TextureFormat original_format = texture_format;
		if (texture_format == TextureFormat::bgra8 || texture_format == TextureFormat::rgb8 || texture_format == TextureFormat::bgr8)
			texture_format = TextureFormat::rgba8;

		PixelBufferSet set(texture_dimensions, texture_format, texture_width, texture_height, texture_slices);

		int bytes_per_pixel = 0;
		int bytes_per_block = 0;
		if (PixelBuffer::is_compressed(original_format))
		{
			bytes_per_block = PixelBuffer::get_bytes_per_block(original_format);
		}
		else
		{
			bytes_per_pixel = PixelBuffer::get_bytes_per_pixel(original_format);
		}

		for (int slice = 0; slice < texture_slices; slice++)
		{
			for (int level = 0; level < texture_levels; level++)
			{
				int mip_width = max(texture_width >> level, 1);
				int mip_height = max(texture_height >> level, 1);

				PixelBuffer buffer(mip_width, mip_height, original_format);

				if (bytes_per_block)
				{
					int blocks_width = (mip_width + 3) / 4;
					int blocks_height = (mip_height + 3) / 4;
					file.read(buffer.get_data(), bytes_per_block * blocks_width * blocks_height);
				}
				else
				{
					file.read(buffer.get_data(), bytes_per_pixel * mip_width * mip_height);
				}

				if (texture_format != original_format)
					buffer = buffer.to_format(texture_format);

				set.set_image(slice, level, buffer);
			}
		}

		return set;
	}
}
