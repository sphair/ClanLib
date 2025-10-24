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
#include "zip_iodevice_fileentry.h"
#include "zip_file_entry_impl.h"
#include "zip_compression_method.h"
#include "zip_flags.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_format.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ZipIODevice_FileEntry construction:

CL_ZipIODevice_FileEntry::CL_ZipIODevice_FileEntry(CL_IODevice iodevice, const CL_ZipFileEntry &entry)
: iodevice(iodevice), file_entry(entry), zstream_open(false), peeked_data(0)
{
	init();
}

CL_ZipIODevice_FileEntry::~CL_ZipIODevice_FileEntry()
{
	deinit();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipIODevice_FileEntry attributes:

int CL_ZipIODevice_FileEntry::get_size() const
{
	return file_header.uncompressed_size;
}

int CL_ZipIODevice_FileEntry::get_position() const
{
	return (int) pos;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipIODevice_FileEntry operations:

#define cl_min(a,b) ((a)<(b)?(a):(b))

int CL_ZipIODevice_FileEntry::send(const void *data, int len, bool send_all)
{
	throw CL_Exception("Read-only device.");
}

int CL_ZipIODevice_FileEntry::receive(void *buffer, int size, bool receive_all)
{
	if (size == 0)
		return 0;
	if (peeked_data.get_size() > 0)
	{
		int peek_amount = cl_min(size, peeked_data.get_size());
		memcpy(buffer, peeked_data.get_data(), peek_amount);
		memmove(peeked_data.get_data(), peeked_data.get_data()+peek_amount, peeked_data.get_size()-peek_amount);
		peeked_data.set_size(peeked_data.get_size()-peek_amount);
		if (peek_amount <= size)
			return peek_amount + receive((char*) buffer+peek_amount, size-peek_amount, receive_all);
	}

	return lowlevel_read(buffer, size, receive_all);
}

int CL_ZipIODevice_FileEntry::peek(void *data, int len)
{
	if (peeked_data.get_size() >= len)
	{
		memcpy(data, peeked_data.get_data(), len);
		return len;
	}
	else
	{
		int old_size = peeked_data.get_size();
		try
		{
			peeked_data.set_size(len);
			int bytes_read = lowlevel_read(peeked_data.get_data()+old_size, len-old_size, false);
			peeked_data.set_size(old_size+bytes_read);
			memcpy(data, peeked_data.get_data(), peeked_data.get_size());
			return peeked_data.get_size();
		}
		catch (const CL_Exception& e)
		{
			peeked_data.set_size(old_size);
			throw;
		}
	}
}

bool CL_ZipIODevice_FileEntry::seek(int seek_pos, CL_IODevice::SeekMode mode)
{
	cl_byte64 absolute_pos = 0;
	switch (mode)
	{
	case CL_IODevice::seek_set:
		absolute_pos = seek_pos;
		break;

	case CL_IODevice::seek_cur:
 		absolute_pos = pos + seek_pos;
		break;

	case CL_IODevice::seek_end:
		absolute_pos = file_header.uncompressed_size + seek_pos;
		break;
	}

	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		iodevice.seek(int(absolute_pos-pos), CL_IODevice::seek_cur);
		break;

	case zip_compress_deflate:
		// if backward seeking, restart at beginning of stream.
		if (absolute_pos < pos)
		{
			deinit();
			init();
		}

		char buffer[1024];
		while (absolute_pos > pos)
		{
			int received = receive(buffer, int(cl_min(absolute_pos-pos, 1024)), true);
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
		return false;
	}
	return true;
}

CL_IODeviceProvider *CL_ZipIODevice_FileEntry::duplicate()
{
	CL_ZipIODevice_FileEntry *new_provider = new CL_ZipIODevice_FileEntry(this, file_entry);
	return new_provider;
}


/////////////////////////////////////////////////////////////////////////////
// CL_ZipIODevice_FileEntry implementation:

void CL_ZipIODevice_FileEntry::init()
{
	iodevice.seek(file_entry.impl->record.relative_offset_of_local_header, CL_IODevice::seek_set);
	file_header.load(iodevice);

	//This fix allows OS X created .zips to be opened - SAR
	if (file_header.general_purpose_bit_flag  & CL_ZIP_CRC32_IN_FILE_DESCRIPTOR) //if this bit is set, it means the local header data for sizes was not
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
		if (result != Z_OK) throw CL_Exception("Zlib inflateInit failed for zip index!");
		zstream_open = true;
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
		throw CL_Exception(cl_format("Unsupported compression method %1", file_header.compression_method));
	}
}

void CL_ZipIODevice_FileEntry::deinit()
{
	// Clean up decompression:
	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		break;

	case zip_compress_deflate:
		if (zstream_open)
			inflateEnd(&zs);
		zstream_open = false;
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

int CL_ZipIODevice_FileEntry::lowlevel_read(void *data, int size, bool read_all)
{
	switch (file_header.compression_method)
	{
	case zip_compress_store: // no compression
		{
			int received = iodevice.receive(data, int(cl_min(size, file_header.uncompressed_size - pos)), read_all);
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
					received_input += iodevice.receive(zbuffer, int(cl_min(16*1024, file_header.compressed_size - compressed_pos)), true);
					if (compressed_pos + received_input == file_header.compressed_size) break;
				}
				compressed_pos += received_input;

				zs.next_in = (Bytef *) zbuffer;
				zs.avail_in = received_input;
			}

			// Decompress data:
			int result = inflate(&zs, 0);
			if (result == Z_STREAM_END) break;
			if (result == Z_NEED_DICT) throw CL_Exception("Zlib inflate wants a dictionary!");
			if (result == Z_DATA_ERROR) throw CL_Exception("Zip data stream is corrupted");
			if (result == Z_STREAM_ERROR) throw CL_Exception("Zip stream structure was inconsistent!");
			if (result == Z_MEM_ERROR) throw CL_Exception("Zlib did not have enough memory to decompress file!");
			if (result == Z_BUF_ERROR) throw CL_Exception("Not enough data in buffer when Z_FINISH was used");
			if (result != Z_OK) throw CL_Exception("Zlib inflate failed while decompressing zip file!");
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
