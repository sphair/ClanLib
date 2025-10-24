/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include <cstring>
#include "Core/precomp.h"
#include "inputsource_zip_fileentry.h"
#include "zip_file_entry_generic.h"
#include "zip_compression_method.h"
#include "zip_flags.h"
#include "API/Core/IOData/inputsource_file.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zip_FileEntry construction:

CL_InputSource_Zip_FileEntry::CL_InputSource_Zip_FileEntry(const std::string &filename, const CL_Zip_FileEntry &entry) : inputsource(new CL_InputSource_File(filename)), file_entry(entry)
{
	// indicate the file is little-endian
	inputsource->set_little_endian_mode();
	source_open = false;
	open();
}

CL_InputSource_Zip_FileEntry::CL_InputSource_Zip_FileEntry(const CL_InputSource_Zip_FileEntry &copy)
{
	inputsource = copy.inputsource->clone();
	file_entry = copy.file_entry;
	pos = copy.pos;
	file_header = copy.file_header;
}

CL_InputSource_Zip_FileEntry::~CL_InputSource_Zip_FileEntry()
{
	delete inputsource;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zip_FileEntry attributes:

int CL_InputSource_Zip_FileEntry::tell() const
{
	return pos;
}

int CL_InputSource_Zip_FileEntry::size() const
{
	return file_header.uncompressed_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zip_FileEntry operations:

#define cl_min(a, b) (a < b ? a : b)

int CL_InputSource_Zip_FileEntry::read(void *data, int size)
{
	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		{
			int received = inputsource->read(data, cl_min(size, file_header.uncompressed_size - pos));
			pos += received;
			return received;
		}
		break;

	case zip_compress_deflate:
		zs.next_out = (Bytef *) data;
		zs.avail_out = size;
		// Continue feeding zlib data until we get our data:
		while (zs.avail_out > 0)
		{
			// zlib needs more data:
			if (zs.avail_in == 0 && compressed_pos < file_header.compressed_size)
			{
				// Read some compressed data:
				int received_input = 0;
				while (received_input < 16*1024)
				{
					received_input += inputsource->read(zbuffer, cl_min(16*1024, file_header.compressed_size - compressed_pos));
					if (compressed_pos + received_input == file_header.compressed_size) break;
				}
				compressed_pos += received_input;

				zs.next_in = (Bytef *) zbuffer;
				zs.avail_in = received_input;
			}

			// Decompress data:
			int result = inflate(&zs, 0);
			if (result == Z_STREAM_END) break;
			if (result == Z_NEED_DICT) throw CL_Error("Zlib inflate wants a dictionary!");
			if (result == Z_DATA_ERROR) throw CL_Error("Zip data stream is corrupted");
			if (result == Z_STREAM_ERROR) throw CL_Error("Zip stream structure was inconsistent!");
			if (result == Z_MEM_ERROR) throw CL_Error("Zlib did not have enough memory to decompress file!");
			if (result == Z_BUF_ERROR) throw CL_Error("Not enough data in buffer when Z_FINISH was used");
			if (result != Z_OK) throw CL_Error("Zlib inflate failed while decompressing zip file!");
		}
		pos += size - zs.avail_out;
		return size - zs.avail_out;

	case zip_compress_shrunk:
	case zip_compress_expand_factor_1:
	case zip_compress_expand_factor_2:
	case zip_compress_expand_factor_3:
	case zip_compress_expand_factor_4:
	case zip_compress_implode:
	case zip_compress_tokenize:
	case zip_compress_deflate64:
	case zip_compress_pkware_implode:
		break;
	}

	return 0;
}

void CL_InputSource_Zip_FileEntry::open()
{
	if (source_open) return;

	inputsource->open();
	inputsource->seek(file_entry.impl->record.relative_offset_of_local_header, CL_InputSource::seek_set);
	file_header.load(inputsource);
	
	//This fix allows OS X created .zips to be opened - SAR
	if (file_header.general_purpose_bit_flag  & 8) //if this bit is set, it means the local header data for sizes was not
	{
		//the correct size data is not in the local header.. luckily, we have a real copy from the entry database
		file_header.compressed_size = file_entry.get_compressed_size();
		file_header.uncompressed_size = file_entry.get_uncompressed_size();
	}
	
	pos = 0;
	compressed_pos = 0;

	// Initialize decompression:
	int result = 0;
	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		break;

	case zip_compress_deflate:
		memset(&zs, 0, sizeof(z_stream));
		zs.next_in = Z_NULL;
		zs.avail_in = 0;
		zs.zalloc = Z_NULL;
		zs.zfree = Z_NULL;
		zs.opaque = Z_NULL;
		//result = inflateInit(&zs);
		result = inflateInit2(&zs, -15); // Undocumented: if wbits is negative, zlib skips header check
		if (result != Z_OK) throw CL_Error("Zlib inflateInit failed for zip index!");
		break;

	case zip_compress_shrunk:
	case zip_compress_expand_factor_1:
	case zip_compress_expand_factor_2:
	case zip_compress_expand_factor_3:
	case zip_compress_expand_factor_4:
	case zip_compress_implode:
	case zip_compress_tokenize:
	case zip_compress_deflate64:
	case zip_compress_pkware_implode:
	default:
		throw CL_Error(CL_String::format("Unsupported compression method %1", file_header.compression_method));
	}

	source_open = true;
}

void CL_InputSource_Zip_FileEntry::close()
{
	if (!source_open) return;

	inputsource->close();

	// Clean up decompression:
	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		break;

	case zip_compress_deflate:
		inflateEnd(&zs);
		break;

	case zip_compress_shrunk:
	case zip_compress_expand_factor_1:
	case zip_compress_expand_factor_2:
	case zip_compress_expand_factor_3:
	case zip_compress_expand_factor_4:
	case zip_compress_implode:
	case zip_compress_tokenize:
	case zip_compress_deflate64:
	case zip_compress_pkware_implode:
		break;
	}

	source_open = false;
}

CL_InputSource *CL_InputSource_Zip_FileEntry::clone() const
{
	return new CL_InputSource_Zip_FileEntry(*this);
}

void CL_InputSource_Zip_FileEntry::seek(int seek_pos, SeekEnum seek_type)
{
	int absolute_pos = 0;
	switch (seek_type)
	{
	case seek_set:
		absolute_pos = seek_pos;
		break;

	case seek_cur:
		absolute_pos = pos + seek_pos;
		break;

	case seek_end:
		absolute_pos = file_header.uncompressed_size + seek_pos;
		break;
	}

	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		inputsource->seek(absolute_pos-pos, seek_cur);
		break;

	case zip_compress_deflate:
		// if backward seeking, restart at beginning of stream.
		if (absolute_pos < pos)
		{
			close();
			open();
		}

		char buffer[1024];
		while (absolute_pos > pos)
		{
			int received = read(buffer, cl_min(absolute_pos-pos, 1024));
			if (received == 0) break;
		}
		break;

	case zip_compress_shrunk:
	case zip_compress_expand_factor_1:
	case zip_compress_expand_factor_2:
	case zip_compress_expand_factor_3:
	case zip_compress_expand_factor_4:
	case zip_compress_implode:
	case zip_compress_tokenize:
	case zip_compress_deflate64:
	case zip_compress_pkware_implode:
		break;
	}
}

void CL_InputSource_Zip_FileEntry::push_position()
{
	position_stack.push(pos);
	inputsource->push_position();
}

void CL_InputSource_Zip_FileEntry::pop_position()
{
	inputsource->pop_position();
	pos = position_stack.top();
	position_stack.pop();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zip_FileEntry implementation:
