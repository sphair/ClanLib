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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/ImageProviders/jpeg_compressor.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/logger.h"

// Seems VC8.0 suddenly defines boolean in its Platform SDK
// Seems VC7.1 also defines that :)
#if _MSC_VER >= 1301
#define HAVE_BOOLEAN
#endif

#ifdef WIN32
#define XMD_H
#endif

#include <cstdio>
extern "C"
{
	#undef FAR
	#include <jpeglib.h>
}

#if BITS_IN_JSAMPLE != 8
#error "CL_JPEGCompressor::write_scanlines expects libjpeg to use 8 bits per color component"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGCompressor_Impl class:

class CL_JPEGCompressor_Impl
{
public:
	CL_JPEGCompressor_Impl()
	: quality(95), size(0, 0), in_color_space(CL_JPEGCompressor::rgb), in_components(3),
	  out_color_space(CL_JPEGCompressor::rgb), out_components(3)
	{
		memset(&cinfo, 0, sizeof(jpeg_compress_struct));
		memset(&dest, 0, sizeof(jpeg_destination_mgr));
		memset(&jerr, 0, sizeof(jpeg_error_mgr));

		dest.next_output_byte = write_buffer;
		dest.free_in_buffer = write_buffer_size;
		dest.init_destination = &on_init_destination;
		dest.empty_output_buffer = &on_empty_output_buffer;
		dest.term_destination = &on_term_destination;
		jpeg_std_error(&jerr);
		jerr.error_exit = &on_error_exit;
		jerr.emit_message = &on_emit_message;
		jerr.output_message = &on_output_message;

		jpeg_create_compress(&cinfo);

		cinfo.err = &jerr;
		cinfo.client_data = this;
		cinfo.dest = &dest;
	}

	~CL_JPEGCompressor_Impl()
	{
		jpeg_destroy_compress(&cinfo);
	}

	J_COLOR_SPACE to_jpeg_color_space(CL_JPEGCompressor::ColorSpace cs) const
	{
		switch (cs)
		{
		case CL_JPEGCompressor::grayscale:
			return JCS_GRAYSCALE;
		case CL_JPEGCompressor::rgb:
			return JCS_RGB;
		case CL_JPEGCompressor::ycbcr:
			return JCS_YCbCr;
		case CL_JPEGCompressor::cmyk:
			return JCS_CMYK;
		case CL_JPEGCompressor::ycck:
			return JCS_YCCK;
		default:
			return JCS_UNKNOWN;
		}
	}

	static void on_init_destination(j_compress_ptr cinfo)
	{
		CL_JPEGCompressor_Impl *self = (CL_JPEGCompressor_Impl *) cinfo->client_data;
		cinfo->dest->next_output_byte = self->write_buffer;
		cinfo->dest->free_in_buffer = self->write_buffer_size;
	}

	static boolean on_empty_output_buffer(j_compress_ptr cinfo)
	{
		CL_JPEGCompressor_Impl *self = (CL_JPEGCompressor_Impl *) cinfo->client_data;
		self->output.write(self->write_buffer, write_buffer_size);
		cinfo->dest->next_output_byte = self->write_buffer;
		cinfo->dest->free_in_buffer = self->write_buffer_size;
		return TRUE;
	}

	static void on_term_destination(j_compress_ptr cinfo)
	{
		CL_JPEGCompressor_Impl *self = (CL_JPEGCompressor_Impl *) cinfo->client_data;
		self->output.write(self->write_buffer, write_buffer_size - cinfo->dest->free_in_buffer);
		cinfo->dest->next_output_byte = self->write_buffer;
		cinfo->dest->free_in_buffer = self->write_buffer_size;
	}

	static void on_error_exit(j_common_ptr cinfo)
	{
		char message[JMSG_LENGTH_MAX];
		cinfo->err->format_message(cinfo, message);
		throw CL_Exception(CL_StringHelp::local8_to_text(message));
	}

	static void on_emit_message(j_common_ptr cinfo, int msg_level)
	{
		// Throw exception if its a warning message - we want cleanly written data:
		if (msg_level == -1)
			on_error_exit(cinfo);
	}

	static void on_output_message(j_common_ptr cinfo)
	{
		char message[JMSG_LENGTH_MAX];
		cinfo->err->format_message(cinfo, message);
		cl_log_event("JPEGCompressor", message);
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_destination_mgr dest;
	struct jpeg_error_mgr jerr;

	int quality;
	CL_IODevice output;
	CL_Size size;
	CL_JPEGCompressor::ColorSpace in_color_space;
	int in_components;
	CL_JPEGCompressor::ColorSpace out_color_space;
	int out_components;
	enum { write_buffer_size = 64*1024 };
	JOCTET write_buffer[write_buffer_size];
};

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGCompressor Construction:

CL_JPEGCompressor::CL_JPEGCompressor()
: impl(new CL_JPEGCompressor_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGCompressor Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGCompressor Operations:

void CL_JPEGCompressor::set_quality(int quality)
{
	impl->quality = quality;
}

void CL_JPEGCompressor::set_output(CL_IODevice output_source)
{
	impl->output = output_source;
}

void CL_JPEGCompressor::set_size(int width, int height)
{
	impl->size = CL_Size(width, height);
}

void CL_JPEGCompressor::set_color_space(ColorSpace in_color_space, int in_components, ColorSpace out_color_space, int out_components)
{
	impl->in_color_space = in_color_space;
	impl->in_components = in_components;
	impl->out_color_space = out_color_space;
	impl->out_components = out_components;
}

void CL_JPEGCompressor::start(bool raw_data)
{
	impl->cinfo.image_width = impl->size.width;
	impl->cinfo.image_height = impl->size.height;
	impl->cinfo.input_components = impl->in_components;
	impl->cinfo.in_color_space = impl->to_jpeg_color_space(impl->in_color_space);
	jpeg_set_defaults(&impl->cinfo);
	impl->cinfo.dct_method = JDCT_FLOAT;
	impl->cinfo.JFIF_major_version = 1;
	impl->cinfo.JFIF_minor_version = 2;
	jpeg_set_colorspace(&impl->cinfo, impl->to_jpeg_color_space(impl->out_color_space));
	impl->cinfo.num_components = impl->out_components;
	jpeg_set_quality(&impl->cinfo, impl->quality, TRUE);
	impl->cinfo.raw_data_in = raw_data ? TRUE : FALSE;
	jpeg_start_compress(&impl->cinfo, TRUE);
}

void CL_JPEGCompressor::write_marker(int marker, const void * const data, int length)
{
	jpeg_write_marker(&impl->cinfo, marker, (const JOCTET * const) data, length);
}

void CL_JPEGCompressor::write_comment_marker(const CL_StringRef &comment)
{
	CL_String8 comment8 = CL_StringHelp::text_to_local8(comment);
	write_marker(marker_com, comment8.data(), comment8.length());
}

void CL_JPEGCompressor::write_scanlines(const unsigned char **data, unsigned int lines)
{
	jpeg_write_scanlines(&impl->cinfo, (JSAMPARRAY) data, lines);
}

void CL_JPEGCompressor::write_raw_data(const unsigned char ***data, unsigned int lines)
{
	jpeg_write_raw_data(&impl->cinfo, (JSAMPIMAGE) data, lines);
}

void CL_JPEGCompressor::finish()
{
	jpeg_finish_compress(&impl->cinfo);
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGCompressor Implementation:
