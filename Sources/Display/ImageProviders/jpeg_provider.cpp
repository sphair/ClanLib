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
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include <iostream>
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "jpeg_provider_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGProvider construction:

CL_PixelBuffer CL_JPEGProvider::load(
	const CL_String &filename,
	const CL_VirtualDirectory &directory)
{
	CL_JPEGProvider_Impl jpeg(filename, directory);
	return CL_PixelBuffer(jpeg.width, jpeg.height, jpeg.pitch, jpeg.format, jpeg.palette, jpeg.get_data());
}

CL_PixelBuffer CL_JPEGProvider::load(
	CL_IODevice &file)
{
	CL_JPEGProvider_Impl jpeg(file);
	return CL_PixelBuffer(jpeg.width, jpeg.height, jpeg.pitch, jpeg.format, jpeg.palette, jpeg.get_data());
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
	throw CL_Exception(cl_text("CL_JPEGProvider::save() using CL_IODevice is not implemented"));
}

void CL_JPEGProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &filename,
	CL_VirtualDirectory &directory,
	int quality)
{
	if (buffer.get_format() != CL_PixelFormat::bgr888)
	{
		CL_PixelBuffer newbuf(
			buffer.get_width(), buffer.get_height(), 
			buffer.get_width()*3, CL_PixelFormat::bgr888);
		buffer.convert(newbuf);
		buffer = newbuf;
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	/* Step 1: allocate and initialize JPEG compression object */

	/* We have to set up the error handler first, in case the initialization
	 * step fails.  (Unlikely, but it could happen if you are out of memory.)
	 * This routine fills in the contents of struct jerr, and returns jerr's
	 * address which we place into the link field in cinfo.
	 */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */

	/* Here we use the library-supplied code to send compressed data to a
	 * stdio stream.  You can also write your own code to do something else.
	 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	 * requires it in order to write binary files.
	 */

#ifdef _CRT_INSECURE_DEPRECATE
	errno_t result = fopen_s(&outfile, CL_StringHelp::text_to_local8(filename).c_str(), "wb");
	if (result != 0)
		throw CL_Exception(cl_text("Can't open ") + filename);
#else
	if ((outfile = fopen(CL_StringHelp::text_to_local8(filename).c_str(), "wb")) == NULL)
	{
		throw CL_Exception(cl_text("Can't open ") + filename);
	}
#endif
	jpeg_stdio_dest(&cinfo, outfile);

	/* Step 3: set parameters for compression */

	/* First we supply a description of the input image.
	 * Four fields of the cinfo struct must be filled in:
	 */
	cinfo.image_width  = buffer.get_width(); 	/* image width and height, in pixels */
	cinfo.image_height = buffer.get_height();
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	/* Now use the library's routine to set default compression parameters.
	 * (You must set at least cinfo.in_color_space before calling this,
	 * since the defaults depend on the source color space.)
	 */
	jpeg_set_defaults(&cinfo);
	/* Now you can set any non-default parameters you wish to.
	 * Here we just illustrate the use of quality (quantization table) scaling:
	 */
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	/* Step 4: Start compressor */

	/* TRUE ensures that we will write a complete interchange-JPEG file.
	 * Pass TRUE unless you are very sure of what you're doing.
	 */
	jpeg_start_compress(&cinfo, TRUE);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */

	/* Here we use the library's state variable cinfo.next_scanline as the
	 * loop counter, so that we don't have to keep track ourselves.
	 * To keep things simple, we pass one scanline per call; you can pass
	 * more if you wish, though.
	 */
	row_stride = buffer.get_width() * 3;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could pass
		 * more than one scanline at a time if that's more convenient.
		 */
		row_pointer[0] = &static_cast<unsigned char*>(buffer.get_data())[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	fclose(outfile);

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	/* And we're done! */
}

/* EOF */
