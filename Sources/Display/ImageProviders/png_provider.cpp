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
**    (if your name is missing here, please add it)
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "png_provider_impl.h"
#include <cerrno>
#include <cstdio>


class CustomIOFunctions
{
public:
	static void read(png_structp png_ptr, png_bytep data, png_size_t length);
	static void write(png_structp png_ptr, png_bytep data, png_size_t length);
	static void flush(png_structp png_ptr);
};

void CustomIOFunctions::read( png_structp png_ptr, png_bytep data, png_size_t length )
{
	CL_IODevice *iodev = (CL_IODevice*)png_get_io_ptr(png_ptr);
	iodev->read(data, length);
}

void CustomIOFunctions::write( png_structp png_ptr, png_bytep data, png_size_t length )
{
	CL_IODevice *iodev = (CL_IODevice*)png_get_io_ptr(png_ptr);
	iodev->write(data, length);
}

void CustomIOFunctions::flush( png_structp png_ptr )
{
	// CL_IODevice doesn't have a flush().
}

/////////////////////////////////////////////////////////////////////////////
// CL_PNGProvider construction:

CL_PixelBuffer CL_PNGProvider::load(
	const CL_String &filename,
	const CL_VirtualDirectory &directory)
{
	CL_PNGProvider_Impl png(filename, directory);
	CL_PixelBuffer pbuff(png.width, png.height, png.sized_format, png.palette, png.get_data());
	pbuff.set_colorkey(png.uses_src_colorkey(), png.get_src_colorkey());
	return pbuff;
}

CL_PixelBuffer CL_PNGProvider::load(
	const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	return CL_PNGProvider::load(filename, dir);
}

CL_PixelBuffer CL_PNGProvider::load(CL_IODevice &iodev)
{
	CL_PNGProvider_Impl png(iodev);
	CL_PixelBuffer pbuff(png.width, png.height, png.sized_format, png.palette, png.get_data());
	pbuff.set_colorkey(png.uses_src_colorkey(), png.get_src_colorkey());
	return pbuff;
}

void CL_PNGProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &filename,
	CL_VirtualDirectory &directory)
{
	CL_IODevice file = directory.open_file(filename, CL_File::create_always);
	save(buffer, file);
}

void CL_PNGProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	CL_PNGProvider::save(buffer, filename, dir);

}

void CL_PNGProvider::save(CL_PixelBuffer buffer, CL_IODevice &iodev)
{
	if (buffer.get_format() != cl_abgr8)
	{
		CL_PixelBuffer newbuf(
			buffer.get_width(),
			buffer.get_height(), 
			cl_abgr8);
		buffer.convert(newbuf);
		buffer = newbuf;
	}

	png_structp png_ptr;
	png_infop info_ptr;

	png_ptr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);

	png_set_read_fn(png_ptr, &iodev, &CustomIOFunctions::read);
	png_set_write_fn(png_ptr, &iodev, &CustomIOFunctions::write, CustomIOFunctions::flush);

	#ifndef PNG_COLOR_TYPE_RGBA
	#define PNG_COLOR_TYPE_RGBA PNG_COLOR_TYPE_RGB_ALPHA
	#endif

	png_set_IHDR(
		png_ptr, info_ptr, 
		buffer.get_width(), buffer.get_height(), 8 /* bitdepth */,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE, 
		PNG_COMPRESSION_TYPE_BASE, 
		PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	png_uint_32 height = buffer.get_height();
	png_uint_32 row_bytes = buffer.get_width()*4;

	png_byte* image = new png_byte[height * row_bytes];
	png_bytep* row_pointers = new png_bytep[height];

	// fill the image with data
	for (int i = 0; i < buffer.get_width()*buffer.get_height()*4; ++i)
		image[i] = static_cast<unsigned char*>(buffer.get_data())[i];

	// generate row pointers
	for (unsigned int k = 0; k < height; k++)
		row_pointers[k] = image + (k * row_bytes);

	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct( &png_ptr, &info_ptr );

	delete[] image;
	delete[] row_pointers;
}



