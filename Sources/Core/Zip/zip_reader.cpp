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

#include "Core/precomp.h"
#include "API/Core/Zip/zip_reader.h"
#include "zip_archive_impl.h"
#include "zip_local_file_header.h"
#include "zip_compression_method.h"
#include <zlib.h>

/////////////////////////////////////////////////////////////////////////////
// CL_ZipReader_Impl class:

class CL_ZipReader_Impl
{
public:
	CL_ZipReader_Impl(CL_IODevice &input)
	: input(input), zstream_open(false), compressed_pos(0)
	{
	}

	~CL_ZipReader_Impl()
	{
		if (zstream_open)
			inflateEnd(&zs);
	}

	cl_byte64 deflate_read(void *data, cl_byte64 size, bool read_all);
	
	CL_IODevice input;
	CL_ZipLocalFileHeader local_header;
	z_stream zs;
	char zbuffer[16*1024];
	bool zstream_open;
	cl_byte64 compressed_pos;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ZipReader Construction:

CL_ZipReader::CL_ZipReader(CL_IODevice &input)
: impl(new CL_ZipReader_Impl(input))
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipReader Operations:

bool CL_ZipReader::read_local_file_header(bool allow_data_descriptor)
{
	try
	{
		impl->local_header.load(impl->input);
	}
	catch (const CL_Exception&)
	{
		return false;
	}

	if (has_data_descriptor() && !allow_data_descriptor)
		throw CL_Exception("Zip file entry uses a data descriptor");
	if ((impl->local_header.general_purpose_bit_flag & CL_ZIP_ENCRYPTED) ||
		(impl->local_header.general_purpose_bit_flag & CL_ZIP_STRONG_ENCRYPTED))
		throw CL_Exception("Zip file entry is encrypted");

	if (impl->zstream_open)
		inflateEnd(&impl->zs);
	impl->zstream_open = false;

	if (impl->local_header.compression_method == zip_compress_deflate)
	{
		memset(&impl->zs, 0, sizeof(z_stream));
		int result = inflateInit2(&impl->zs, -15); // Undocumented: if wbits is negative, zlib skips header check
		if (result != Z_OK)
			throw CL_Exception("Zlib inflateInit failed for zip index!");
		impl->zstream_open = true;
		impl->compressed_pos = 0;
	}
	else if (impl->local_header.compression_method != zip_compress_store)
	{
		throw CL_Exception("Zip file entry is compressed with an unsupported compression method");
	}

	return true;
}

CL_String CL_ZipReader::get_filename()
{
	return impl->local_header.filename;
}

bool CL_ZipReader::has_data_descriptor() const
{
	return (impl->local_header.general_purpose_bit_flag & CL_ZIP_CRC32_IN_FILE_DESCRIPTOR);
}

cl_byte64 CL_ZipReader::get_compressed_size() const
{
	return impl->local_header.compressed_size;
}

cl_byte64 CL_ZipReader::get_uncompressed_size() const
{
	return impl->local_header.uncompressed_size;
}

void CL_ZipReader::set_data_descriptor_data(cl_byte64 compressed_size, cl_byte64 uncompressed_size, cl_ubyte32 crc32)
{
	impl->local_header.compressed_size = compressed_size;
	impl->local_header.uncompressed_size = uncompressed_size;
	impl->local_header.crc32 = crc32;
}

cl_byte64 CL_ZipReader::read_file_data(void *data, cl_byte64 size, bool read_all)
{
	if (impl->zstream_open)
	{
		return impl->deflate_read(data, size, read_all);
	}
	else
	{
		return impl->input.read(data, size, read_all);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipReader Implementation:

#define cl_min(a,b) ((a)<(b)?(a):(b))

cl_byte64 CL_ZipReader_Impl::deflate_read(void *data, cl_byte64 size, bool read_all)
{
	zs.next_out = (Bytef *) data;
	zs.avail_out = size;
	// Continue feeding zlib data until we get our data:
	while (zs.avail_out > 0)
	{
		// zlib needs more data:
		if (zs.avail_in == 0 && compressed_pos < local_header.compressed_size)
		{
			// Read some compressed data:
			int received_input = 0;
			while (received_input < 16*1024)
			{
				received_input += input.receive(zbuffer, int(cl_min(16*1024, local_header.compressed_size - compressed_pos)), true);
				if (compressed_pos + received_input == local_header.compressed_size) break;
			}
			compressed_pos += received_input;

			zs.next_in = (Bytef *) zbuffer;
			zs.avail_in = received_input;
		}

		// Decompress data:
		int result = inflate(&zs, (compressed_pos == local_header.compressed_size) ? Z_FINISH : Z_NO_FLUSH);
		if (result == Z_STREAM_END) break;
		if (result == Z_NEED_DICT) throw CL_Exception("Zlib inflate wants a dictionary!");
		if (result == Z_DATA_ERROR) throw CL_Exception("Zip data stream is corrupted");
		if (result == Z_STREAM_ERROR) throw CL_Exception("Zip stream structure was inconsistent!");
		if (result == Z_MEM_ERROR) throw CL_Exception("Zlib did not have enough memory to decompress file!");
		if (result == Z_BUF_ERROR) throw CL_Exception("Not enough data in buffer when Z_FINISH was used");
		if (result != Z_OK) throw CL_Exception("Zlib inflate failed while decompressing zip file!");
	}
	return size - zs.avail_out;
}
