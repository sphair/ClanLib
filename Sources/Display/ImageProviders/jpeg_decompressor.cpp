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
*/

#include "Display/precomp.h"
#include "API/Display/ImageProviders/jpeg_decompressor.h"
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
#error "CL_JPEGDecompressor::read_scanlines expects libjpeg to use 8 bits per color component"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGDecompressor_Impl class:

class CL_JPEGDecompressor_Impl
{
public:
	CL_JPEGDecompressor_Impl()
	{
		memset(&cinfo, 0, sizeof(jpeg_decompress_struct));
		memset(&src, 0, sizeof(jpeg_source_mgr));
		memset(&jerr, 0, sizeof(jpeg_error_mgr));

		src.init_source = &on_init_source;
		src.fill_input_buffer = &on_fill_input_buffer;
		src.skip_input_data = &on_skip_input_data;
		src.resync_to_restart = &on_resync_to_restart;
		src.term_source = &on_term_source;
		jpeg_std_error(&jerr);
		jerr.error_exit = &on_error_exit;
		jerr.emit_message = &on_emit_message;
		jerr.output_message = &on_output_message;

		cinfo.err = &jerr;
		cinfo.client_data = this;
		cinfo.src = &src;
		jpeg_create_decompress(&cinfo);
	}

	~CL_JPEGDecompressor_Impl()
	{
		jpeg_destroy_decompress(&cinfo);
	}

	static void on_init_source(j_decompress_ptr cinfo)
	{
		CL_JPEGDecompressor_Impl *self = (CL_JPEGDecompressor_Impl *) cinfo->client_data;
	}

	static boolean on_fill_input_buffer(j_decompress_ptr cinfo)
	{
		CL_JPEGDecompressor_Impl *self = (CL_JPEGDecompressor_Impl *) cinfo->client_data;
		return TRUE;
	}

	static void on_skip_input_data(j_decompress_ptr cinfo, long num_bytes)
	{
		CL_JPEGDecompressor_Impl *self = (CL_JPEGDecompressor_Impl *) cinfo->client_data;
	}

	static boolean on_resync_to_restart(j_decompress_ptr cinfo, int desired)
	{
		CL_JPEGDecompressor_Impl *self = (CL_JPEGDecompressor_Impl *) cinfo->client_data;
		return TRUE;
	}

	static void on_term_source(j_decompress_ptr cinfo)
	{
		CL_JPEGDecompressor_Impl *self = (CL_JPEGDecompressor_Impl *) cinfo->client_data;
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
		cl_log_event("JPEGDecompressor", message);
	}

	struct jpeg_decompress_struct cinfo;
	struct jpeg_source_mgr src;
	struct jpeg_error_mgr jerr;

	CL_IODevice iodevice;
};

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGDecompressor Construction:

CL_JPEGDecompressor::CL_JPEGDecompressor(CL_IODevice input_source)
: impl(new CL_JPEGDecompressor_Impl)
{
	impl->iodevice = input_source;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGDecompressor Attributes:

int CL_JPEGDecompressor::get_output_width() const
{
	return impl->cinfo.output_width;
}

int CL_JPEGDecompressor::get_output_height() const
{
	return impl->cinfo.output_height;
}

int CL_JPEGDecompressor::get_output_components() const
{
	return impl->cinfo.output_components;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGDecompressor Operations:

void CL_JPEGDecompressor::record_marker(SpecialMarker marker_code, unsigned int length_limit)
{
	jpeg_save_markers(&impl->cinfo, marker_code, length_limit);
}

std::vector<CL_JPEGDecompressor::SavedMarker> CL_JPEGDecompressor::get_saved_markers() const
{
	std::vector<SavedMarker> markers;

	jpeg_saved_marker_ptr cur = impl->cinfo.marker_list;
	while (cur)
	{
		SavedMarker saved_marker;
		saved_marker.marker = cur->marker;
		saved_marker.data = cur->data;
		saved_marker.data_length = cur->data_length;
		saved_marker.original_length = cur->original_length;
		markers.push_back(saved_marker);
		cur = cur->next;
	}
	return markers;
}

void CL_JPEGDecompressor::start(bool raw_data)
{
	jpeg_read_header(&impl->cinfo, TRUE);
	impl->cinfo.dct_method = JDCT_FLOAT;
	impl->cinfo.raw_data_out = raw_data ? TRUE : FALSE;
	jpeg_start_decompress(&impl->cinfo);
}

unsigned int CL_JPEGDecompressor::read_scanlines(unsigned char **data, unsigned int lines)
{
	return jpeg_read_scanlines(&impl->cinfo, (JSAMPARRAY) data, lines);
}

unsigned int CL_JPEGDecompressor::read_raw_data(const unsigned char ***data, unsigned int lines)
{
	return jpeg_read_raw_data(&impl->cinfo, (JSAMPIMAGE) data, lines);
}

void CL_JPEGDecompressor::finish()
{
	jpeg_finish_decompress(&impl->cinfo);
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGDecompressor Implementation:
