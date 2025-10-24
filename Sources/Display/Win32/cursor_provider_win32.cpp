/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/sprite_description.h"
#include "win32_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Construction:

CL_CursorProvider_Win32::CL_CursorProvider_Win32(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
: handle(0)
{
	handle = create_cursor(sprite_description, hotspot);
}

CL_CursorProvider_Win32::~CL_CursorProvider_Win32()
{
	if (handle)
		DestroyCursor(handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Implementation:

HCURSOR CL_CursorProvider_Win32::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot) const
{
	if (sprite_description.get_frames().empty())
		throw CL_Exception(cl_text("Cannot create cursor with no image frames"));
	CL_DataBuffer ani_file = create_ani_file(sprite_description, hotspot);
	int desired_width = sprite_description.get_frames().front().rect.get_width();
	int desired_height = sprite_description.get_frames().front().rect.get_height();
	HICON icon = CreateIconFromResourceEx((PBYTE) ani_file.get_data(), ani_file.get_size(), FALSE, 0x00030000, desired_width, desired_height, LR_DEFAULTCOLOR);
	return (HCURSOR) icon;
}

CL_DataBuffer CL_CursorProvider_Win32::create_ico_file(const CL_PixelBufferRef &image) const
{
	return create_ico_helper(image, 1, CL_Point(0, 0));
}

CL_DataBuffer CL_CursorProvider_Win32::create_cur_file(const CL_PixelBufferRef &image, const CL_Point &hotspot) const
{
	return create_ico_helper(image, 2, hotspot);
}

CL_DataBuffer CL_CursorProvider_Win32::create_ico_helper(const CL_PixelBufferRef &image, WORD type, const CL_Point &hotspot) const
{
	CL_PixelBuffer bmp_image = CL_Win32Window::create_bitmap_data(image);
	int image_size = bmp_image.get_pitch() * bmp_image.get_height();

	BITMAPINFOHEADER bmp_header;
	memset(&bmp_header, 0, sizeof(BITMAPINFOHEADER));
	bmp_header.biSize = sizeof(BITMAPINFOHEADER);
	bmp_header.biWidth = bmp_image.get_width();
	bmp_header.biHeight = bmp_image.get_height() * 2; // why on earth do I have to multiply this by two??
	bmp_header.biPlanes = 1;
	bmp_header.biBitCount = 32;
	bmp_header.biCompression = BI_RGB;
	//bmp_header.biSizeImage = image_size;

	ICONHEADER header;
	memset(&header, 0, sizeof(ICONHEADER));
	header.idType = type;
	header.idCount = 1;

	IconDirectoryEntry entry;
	memset(&entry, 0, sizeof(IconDirectoryEntry));
	entry.bWidth = bmp_image.get_width();
	entry.bHeight = bmp_image.get_height();
	entry.bColorCount = 0;
	entry.wPlanes = 1;
	entry.wBitCount = 32;
	entry.dwBytesInRes = sizeof(BITMAPINFOHEADER) + image_size;
	entry.dwImageOffset = size_header + size_direntry;
	if (type == 2)
	{
		entry.XHotspot = hotspot.x;
		entry.YHotspot = hotspot.y;
	}

	CL_DataBuffer ico_file(size_header + size_direntry + sizeof(BITMAPINFOHEADER) + image_size);
	char *d = ico_file.get_data();
	memcpy(d, &header, size_header);
	memcpy(d + size_header, &entry, size_direntry);
	memcpy(d + size_header + size_direntry, &bmp_header, sizeof(BITMAPINFOHEADER));
	char *p = (char *) bmp_image.get_data();
	int h = bmp_image.get_height();
	int pitch = (int) bmp_image.get_pitch();
	for (int y = 0; y < h; y++)
		memcpy(d + size_header + size_direntry + sizeof(BITMAPINFOHEADER) + y * pitch, p + (h-y-1)*pitch, pitch);
	return ico_file;
}

CL_DataBuffer CL_CursorProvider_Win32::create_ani_file(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot) const
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

	const std::vector<CL_SpriteDescriptionFrame> &frames = sprite_description.get_frames();
	for (std::vector<CL_SpriteDescriptionFrame>::size_type i = 0; i < frames.size(); i++)
	{
		if (frames[i].type != CL_SpriteDescriptionFrame::type_pixelbuffer)
			throw CL_Exception(cl_text("Only pixel buffer sprite frames currently supported for cursors"));

		CL_DataBuffer ico_file = create_cur_file(frames[i].pixelbuffer.get_subimage(frames[i].rect), hotspot);
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
	ani_header.cFrames = ani_frames.icons.size();
	ani_header.cSteps = steps.size();
	ani_header.cx = hotspot.x;
	ani_header.cy = hotspot.y;

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

	CL_DataBuffer ani_file(size);
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
	DWORD *rate = (DWORD *) (data + 8);
	for (DWORD i = 0; i < ani_header.cSteps; i++)
		rate[i] = rates[i];
	data += size_rate;
	
	set_riff_header(data, "seq ", size_seq);
	DWORD *seq = (DWORD *) (data + 8);
	for (DWORD i = 0; i < ani_header.cSteps; i++)
		seq[i] = steps[i];
	data += size_rate;

	set_riff_header(data, "LIST", size_list2);
	ani_frames.write(data + 8);
	data += size_list2;

	return ani_file;
}

void CL_CursorProvider_Win32::set_riff_header(char *data, const char *type, DWORD size) const
{
	memcpy(data, type, 4);
	DWORD *s = (DWORD *) (data + 4);
	*s = size - 8;
}
