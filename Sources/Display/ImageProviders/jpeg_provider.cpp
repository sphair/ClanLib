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
**    (if your name is missing here, please add it)
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include <iostream>
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "jpeg_provider_impl.h"
#include "API/Display/ImageProviders/jpeg_compressor.h"

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGProvider construction:

CL_PixelBuffer CL_JPEGProvider::load(
	const CL_String &filename,
	const CL_VirtualDirectory &directory)
{
	CL_JPEGProvider_Impl jpeg(filename, directory);
	return CL_PixelBuffer(jpeg.width, jpeg.height, jpeg.sized_format, jpeg.palette, jpeg.get_data());
}

CL_PixelBuffer CL_JPEGProvider::load(
	CL_IODevice &file)
{
	CL_JPEGProvider_Impl jpeg(file);
	return CL_PixelBuffer(jpeg.width, jpeg.height, jpeg.sized_format, jpeg.palette, jpeg.get_data());
}

CL_PixelBuffer CL_JPEGProvider::load(
	const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	return CL_JPEGProvider::load(filename, vfs.get_root_directory());
}

void CL_JPEGProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &fullname,
	int quality)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	return CL_JPEGProvider::save(buffer, filename, dir, quality);
}

void CL_JPEGProvider::save(
	CL_PixelBuffer buffer,
	CL_IODevice &file,
	int quality)
{
	if (buffer.get_format() != cl_bgr8)
	{
		CL_PixelBuffer newbuf(
			buffer.get_width(), buffer.get_height(), 
			cl_bgr8);
		buffer.convert(newbuf);
		buffer = newbuf;
	}

	CL_JPEGCompressor cp;
	cp.set_output(file);
	cp.set_size(buffer.get_width(), buffer.get_height());
	cp.set_quality(quality);

	cp.start();
	unsigned char *data = (unsigned char *)buffer.get_data();
	int pitch = buffer.get_width()*3;

	for (int y=0; y<buffer.get_height(); y++)
	{
		const unsigned char *row_ptr = data + y*pitch;
		cp.write_scanlines(&row_ptr, 1);
	}

	cp.finish();
}

void CL_JPEGProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &filename,
	CL_VirtualDirectory &directory,
	int quality)
{
	CL_IODevice iodev = directory.open_file(filename, CL_File::create_always, CL_File::access_write);
	save(buffer, iodev, quality);
}
