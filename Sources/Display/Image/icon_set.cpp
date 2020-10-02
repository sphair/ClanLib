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
#include "API/Display/Image/icon_set.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/memory_device.h"

namespace clan
{
	class IconSet_Impl
	{
	public:
		std::vector<PixelBuffer> images;

		static DataBuffer create_ico_helper(const std::vector<PixelBuffer> &images, int type, const std::vector<Point> &hotspots);
		static PixelBuffer create_bitmap_data(const PixelBuffer &image);

		struct IconHeader
		{
			uint16_t idReserved;
			uint16_t idType; // 1 = ICO, 2 = CUR
			uint16_t idCount;
		};

		struct IconDirectoryEntry
		{
			uint8_t bWidth;
			uint8_t bHeight;
			uint8_t bColorCount;
			uint8_t bReserved;
			union
			{
				uint16_t wPlanes;   // ICO format
				int16_t XHotspot;   // CUR format
			};
			union
			{
				uint16_t wBitCount; // ICO format
				int16_t YHotspot;   // CUR format
			};
			uint32_t dwBytesInRes;
			uint32_t dwImageOffset;
			/** uint16_t nID; // Mentioned by http://msdn2.microsoft.com/en-us/library/ms997538.aspx but not in other ICO docs.*/
		};

		struct IconBitmapInfoHeader
		{
			uint32_t biSize;
			int32_t biWidth;
			int32_t biHeight;
			uint16_t biPlanes;
			uint16_t biBitCount;
			uint32_t biCompression;
			uint32_t biSizeImage;
			int32_t biXPelsPerMeter;
			int32_t biYPelsPerMeter;
			uint32_t biClrUsed;
			uint32_t biClrImportant;
		};

		enum IconBitmapInfoCompressionType
		{
			bi_rgb = 0,
			bi_rle8 = 1,
			bi_rle4 = 2,
			bi_bitfields = 3,
			bi_jpeg = 4,
			bi_png = 5
		};

		enum { size_header = 6, size_direntry = 16, size_bitmap_info = 40 };
	};

	IconSet::IconSet()
		: impl(std::make_shared<IconSet_Impl>())
	{
	}

	std::vector<PixelBuffer> IconSet::get_images() const
	{
		return impl->images;
	}

	void IconSet::add_image(const PixelBuffer &image)
	{
		impl->images.push_back(image);
	}

	DataBuffer IconSet::create_win32_icon()
	{
		return impl->create_ico_helper(impl->images, 1, std::vector<Point>());
	}

	DataBuffer IconSet_Impl::create_ico_helper(const std::vector<PixelBuffer> &images, int type, const std::vector<Point> &hotspots)
	{
		DataBuffer buf;
		buf.set_capacity(32 * 1024);
		MemoryDevice device(buf);

		IconHeader header;
		memset(&header, 0, sizeof(IconHeader));
		header.idType = type;
		header.idCount = images.size();
		device.write(&header, sizeof(IconHeader));

		std::vector<PixelBuffer> bmp_images;
		for (auto & image : images)
			bmp_images.push_back(create_bitmap_data(image));

		int image_offset = size_header + size_direntry*bmp_images.size();
		for (size_t i = 0; i < bmp_images.size(); i++)
		{
			IconDirectoryEntry entry;
			memset(&entry, 0, sizeof(IconDirectoryEntry));
			entry.bWidth = bmp_images[i].get_width();
			entry.bHeight = bmp_images[i].get_height();
			entry.bColorCount = 0;
			entry.wPlanes = 1;
			entry.wBitCount = 32;
			entry.dwBytesInRes = size_bitmap_info + bmp_images[i].get_pitch() * bmp_images[i].get_height();
			entry.dwImageOffset = image_offset;
			if (type == 2)
			{
				entry.XHotspot = hotspots[i].x;
				entry.YHotspot = hotspots[i].y;
			}
			device.write(&entry, size_direntry);
			image_offset += entry.dwBytesInRes;
		}

		for (auto & bmp_image : bmp_images)
		{
			IconBitmapInfoHeader bmp_header;
			memset(&bmp_header, 0, sizeof(IconBitmapInfoHeader));
			bmp_header.biSize = size_bitmap_info;
			bmp_header.biWidth = bmp_image.get_width();
			bmp_header.biHeight = bmp_image.get_height() * 2; // why on earth do I have to multiply this by two??
			bmp_header.biPlanes = 1;
			bmp_header.biBitCount = 32;
			bmp_header.biCompression = bi_rgb;
			device.write(&bmp_header, size_bitmap_info);
			device.write(bmp_image.get_data(), bmp_image.get_pitch() * bmp_image.get_height());
		}

		return device.get_data();
	}

	PixelBuffer IconSet_Impl::create_bitmap_data(const PixelBuffer &image)
	{
		// Convert pixel buffer to DIB compatible format:

		PixelBuffer bmp_image = image.to_format(TextureFormat::bgra8);

		// Note that the APIs use pre-multiplied alpha, which means that the red,
		// green and blue channel values in the bitmap must be pre-multiplied with
		// the alpha channel value. For example, if the alpha channel value is x,
		// the red, green and blue channels must be multiplied by x and divided by
		// 0xff prior to the call.
		int w = bmp_image.get_width();
		int h = bmp_image.get_height();
		unsigned int *p = (unsigned int *)bmp_image.get_data();
		for (int y = 0; y < h; y++)
		{
			int index = y * w;
			unsigned int *line = p + index;
			for (int x = 0; x < w; x++)
			{
				unsigned int a = ((line[x] >> 24) & 0xff);
				unsigned int r = ((line[x] >> 16) & 0xff);
				unsigned int g = ((line[x] >> 8) & 0xff);
				unsigned int b = (line[x] & 0xff);

				r = r * a / 255;
				g = g * a / 255;
				b = b * a / 255;

				line[x] = (a << 24) + (r << 16) + (g << 8) + b;
			}
		}

		// Flip image upside down
		for (int y = 0; y < h / 2; y++)
		{
			for (int x = 0; x < w; x++)
			{
				unsigned int l1 = p[y*w + x];
				unsigned int l2 = p[(w - 1 - y)*w + x];
				p[(w - 1 - y)*w + x] = l1;
				p[y*w + x] = l2;
			}
		}

		return bmp_image;
	}
}
