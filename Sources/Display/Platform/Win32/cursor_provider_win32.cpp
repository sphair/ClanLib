/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "cursor_provider_win32.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/memory_device.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Window/cursor_description.h"
#ifdef __MINGW32__
#include "API/Display/Window/display_window.h"
#endif
#include "win32_window.h"

namespace clan
{
	CursorProvider_Win32::CursorProvider_Win32(const CursorDescription &cursor_description)
		: handle(0)
	{
		handle = create_cursor(cursor_description);
	}

	CursorProvider_Win32::~CursorProvider_Win32()
	{
		if (handle)
			DestroyCursor(handle);
	}

	HCURSOR CursorProvider_Win32::create_cursor(const CursorDescription &cursor_description)
	{
		if (cursor_description.get_frames().empty())
			throw Exception("Cannot create cursor with no image frames");
		DataBuffer ani_file = create_ani_file(cursor_description);
		int desired_width = cursor_description.get_frames().front().rect.get_width();
		int desired_height = cursor_description.get_frames().front().rect.get_height();
		HICON icon = CreateIconFromResourceEx((PBYTE)ani_file.get_data(), ani_file.get_size(), FALSE, 0x00030000, desired_width, desired_height, LR_DEFAULTCOLOR);
		return (HCURSOR)icon;
	}

	DataBuffer CursorProvider_Win32::create_ico_file(const PixelBuffer &image)
	{
		return create_ico_helper(image, Rectf(image.get_size()), 1, Point(0, 0));
	}

	DataBuffer CursorProvider_Win32::create_cur_file(const PixelBuffer &image, const Rect &rect, const Point &hotspot)
	{
		return create_ico_helper(image, rect, 2, hotspot);
	}

	DataBuffer CursorProvider_Win32::create_ico_helper(const PixelBuffer &image, const Rect &rect, WORD type, const Point &hotspot)
	{
		std::vector<PixelBuffer> images;
		std::vector<Rect> rects;
		std::vector<Point> hotspots;
		images.push_back(image);
		rects.push_back(rect);
		hotspots.push_back(hotspot);
		return create_ico_helper(images, rects, type, hotspots);
	}

	DataBuffer CursorProvider_Win32::create_ico_helper(const std::vector<PixelBuffer> &images, const std::vector<Rect> &rects, WORD type, const std::vector<Point> &hotspots)
	{
		DataBuffer buf;
		buf.set_capacity(32 * 1024);
		MemoryDevice device(buf);

		ICONHEADER header;
		memset(&header, 0, sizeof(ICONHEADER));
		header.idType = type;
		header.idCount = images.size();
		device.write(&header, sizeof(ICONHEADER));

		std::vector<PixelBuffer> bmp_images;
		for (size_t i = 0; i < images.size(); i++)
			bmp_images.push_back(Win32Window::create_bitmap_data(images[i], rects[i]));

		for (size_t i = 0; i < bmp_images.size(); i++)
		{
			IconDirectoryEntry entry;
			memset(&entry, 0, sizeof(IconDirectoryEntry));
			entry.bWidth = bmp_images[i].get_width();
			entry.bHeight = bmp_images[i].get_height();
			entry.bColorCount = 0;
			entry.wPlanes = 1;
			entry.wBitCount = 32;
			entry.dwBytesInRes = sizeof(BITMAPINFOHEADER) + bmp_images[i].get_pitch() * bmp_images[i].get_height();
			entry.dwImageOffset = size_header + size_direntry*bmp_images.size();
			if (type == 2)
			{
				entry.XHotspot = hotspots[i].x;
				entry.YHotspot = hotspots[i].y;
			}
			device.write(&entry, sizeof(IconDirectoryEntry));
		}

		for (size_t i = 0; i < bmp_images.size(); i++)
		{
			BITMAPINFOHEADER bmp_header;
			memset(&bmp_header, 0, sizeof(BITMAPINFOHEADER));
			bmp_header.biSize = sizeof(BITMAPINFOHEADER);
			bmp_header.biWidth = bmp_images[i].get_width();
			bmp_header.biHeight = bmp_images[i].get_height() * 2; // why on earth do I have to multiply this by two??
			bmp_header.biPlanes = 1;
			bmp_header.biBitCount = 32;
			bmp_header.biCompression = BI_RGB;
			bmp_header.biSizeImage = bmp_images[i].get_pitch() * bmp_images[i].get_height();
			device.write(&bmp_header, sizeof(BITMAPINFOHEADER));
			device.write(bmp_images[i].get_data(), bmp_images[i].get_pitch() * bmp_images[i].get_height());
		}

		return device.get_data();
	}

	DataBuffer CursorProvider_Win32::create_ani_file(const CursorDescription &cursor_description)
	{
		/*
			"RIFF" {Length of File}
			"ACON"
			"LIST" {Length of List}
			"INFO"
			"INAM" {Length of Title} {Data}
			"IART" {Length of Author} {Data}
			"anih" {Length of ANI header (36 bytes)} {Data}   ; (see ANI Header TypeDef )
			"rate" {Length of rate block} {Data}      ; ea. rate is a long (length is 1 to cSteps)
			"seq " {Length of sequence block} {Data} ; ea. seq is a long (length is 1 to cSteps)
			"LIST" {Length of List}
			"fram"
			"icon" {Length of Icon} {Data}      ; 1st in list
			...
			"icon" {Length of Icon} {Data}      ; Last in list  (1 to cFrames)
			*/

		ANIFrames ani_frames;
		std::vector<DWORD> rates;
		std::vector<DWORD> steps;

		const std::vector<CursorDescriptionFrame> &frames = cursor_description.get_frames();
		for (std::vector<CursorDescriptionFrame>::size_type i = 0; i < frames.size(); i++)
		{
			DataBuffer ico_file = create_cur_file(frames[i].pixelbuffer, frames[i].rect, cursor_description.get_hotspot());
			ani_frames.icons.push_back(ico_file);
			DWORD rate = static_cast<DWORD>(frames[i].delay * 60);
			if (rate == 0)
				rate = 1;
			rates.push_back(rate);
			steps.push_back(i);
		}

		ANIHeader ani_header;
		memset(&ani_header, 0, sizeof(ANIHeader));
		ani_header.cbSizeOf = sizeof(ANIHeader);
		ani_header.flags = AF_ICON;
		ani_header.JifRate = 30;
		ani_header.cBitCount = 32;
		ani_header.cPlanes = 1;
		ani_header.cFrames = ani_frames.icons.size();
		ani_header.cSteps = steps.size();
		ani_header.cx = cursor_description.get_hotspot().x;
		ani_header.cy = cursor_description.get_hotspot().y;

		ANIInfo ani_info;
		ani_info.author = "clanlib";
		ani_info.title = "clanlib";


		int size_file_header = 8 + 4;
		int size_list1 = 8 + ani_info.length();
		int size_anih = 8 + sizeof(ANIHeader);
		int size_rate = 8 + ani_header.cSteps * 4;
		int size_seq = 8 + ani_header.cSteps * 4;
		int size_list2 = 8 + ani_frames.length();
		int size = size_file_header + size_list1 + size_anih + size_rate + size_seq + size_list2;

		DataBuffer ani_file(size);
		char *data = ani_file.get_data();

		set_riff_header(data, "RIFF", size);
		memcpy(data + 8, "ACON", 4);
		data += size_file_header;

		set_riff_header(data, "LIST", size_list1);
		ani_info.write(data + 8);
		data += size_list1;

		set_riff_header(data, "anih", size_anih);
		memcpy(data + 8, &ani_header, sizeof(ANIHeader));
		data += size_anih;

		set_riff_header(data, "rate", size_rate);
		DWORD *rate = (DWORD *)(data + 8);
		for (DWORD i = 0; i < ani_header.cSteps; i++)
			rate[i] = rates[i];
		data += size_rate;

		set_riff_header(data, "seq ", size_seq);
		DWORD *seq = (DWORD *)(data + 8);
		for (DWORD i = 0; i < ani_header.cSteps; i++)
			seq[i] = steps[i];
		data += size_rate;

		set_riff_header(data, "LIST", size_list2);
		ani_frames.write(data + 8);
		data += size_list2;

		return ani_file;
	}

	void CursorProvider_Win32::set_riff_header(char *data, const char *type, DWORD size)
	{
		memcpy(data, type, 4);
		DWORD *s = (DWORD *)(data + 4);
		*s = size - 8;
	}
}
