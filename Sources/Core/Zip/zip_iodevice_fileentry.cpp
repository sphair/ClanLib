/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Core/Math/cl_math.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	ZipIODevice_FileEntry::ZipIODevice_FileEntry(IODevice iodevice, const ZipFileEntry &entry)
		: iodevice(iodevice), file_entry(entry), zstream_open(false), peeked_data(0)
	{
		init();
	}

	ZipIODevice_FileEntry::~ZipIODevice_FileEntry()
	{
		deinit();
	}

	size_t ZipIODevice_FileEntry::get_size() const
	{
		return size_t(file_header.uncompressed_size);
	}

	size_t ZipIODevice_FileEntry::get_position() const
	{
		return size_t(pos);
	}

	size_t ZipIODevice_FileEntry::send(const void *data, size_t len, bool send_all)
	{
		throw Exception("Read-only device.");
	}

	size_t ZipIODevice_FileEntry::receive(void *buffer, size_t size, bool receive_all)
	{
		if (size == 0)
			return 0;
		if (peeked_data.get_size() > 0)
		{
			size_t peek_amount = min(size, peeked_data.get_size());
			memcpy(buffer, peeked_data.get_data(), peek_amount);
			memmove(peeked_data.get_data(), peeked_data.get_data() + peek_amount, peeked_data.get_size() - peek_amount);
			peeked_data.set_size(peeked_data.get_size() - peek_amount);
			if (peek_amount <= size)
				return peek_amount + receive((char*)buffer + peek_amount, size - peek_amount, receive_all);
		}

		return lowlevel_read(buffer, size, receive_all);
	}

	size_t ZipIODevice_FileEntry::peek(void *data, size_t len)
	{
		if (peeked_data.get_size() >= len)
		{
			memcpy(data, peeked_data.get_data(), len);
			return len;
		}
		else
		{
			size_t old_size = peeked_data.get_size();
			try
			{
				peeked_data.set_size(len);
				size_t bytes_read = lowlevel_read(peeked_data.get_data() + old_size, len - old_size, false);
				peeked_data.set_size(old_size + bytes_read);
				memcpy(data, peeked_data.get_data(), peeked_data.get_size());
				return peeked_data.get_size();
			}
			catch (const Exception& e)
			{
				peeked_data.set_size(old_size);
				throw;
			}
		}
	}

	bool ZipIODevice_FileEntry::seek(int seek_pos, IODevice::SeekMode mode)
	{
		int64_t absolute_pos = 0;
		switch (mode)
		{
		case IODevice::seek_set:
			absolute_pos = seek_pos;
			break;

		case IODevice::seek_cur:
			absolute_pos = pos + seek_pos;
			break;

		case IODevice::seek_end:
			absolute_pos = file_header.uncompressed_size + seek_pos;
			break;
		}

		switch (file_header.compression_method)
		{
		case zip_compress_store: // no compression
			iodevice.seek(int(absolute_pos - pos), IODevice::seek_cur);
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
				size_t received = receive(buffer, size_t(min(absolute_pos - pos, (int64_t)1024)), true);
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

	IODeviceProvider *ZipIODevice_FileEntry::duplicate()
	{
		ZipIODevice_FileEntry *new_provider = new ZipIODevice_FileEntry(this, file_entry);
		return new_provider;
	}


	void ZipIODevice_FileEntry::init()
	{
		iodevice.seek(file_entry.impl->record.relative_offset_of_local_header, IODevice::seek_set);
		file_header.load(iodevice);

		//This fix allows OS X created .zips to be opened - SAR
		if (file_header.general_purpose_bit_flag  & ZIP_CRC32_IN_FILE_DESCRIPTOR) //if this bit is set, it means the local header data for sizes was not
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
			memset(&zs, 0, sizeof(mz_stream));
			zs.next_in = nullptr;
			zs.avail_in = 0;
			zs.zalloc = nullptr;
			zs.zfree = nullptr;
			zs.opaque = nullptr;
			//result = inflateInit(&zs);
			result = mz_inflateInit2(&zs, -15); // Undocumented: if wbits is negative, zlib skips header check
			if (result != MZ_OK) throw Exception("Zlib inflateInit failed for zip index!");
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
			throw Exception(string_format("Unsupported compression method %1", file_header.compression_method));
		}
	}

	void ZipIODevice_FileEntry::deinit()
	{
		// Clean up decompression:
		switch (file_header.compression_method)
		{
		case zip_compress_store: // no compression
			break;

		case zip_compress_deflate:
			if (zstream_open)
				mz_inflateEnd(&zs);
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

	size_t ZipIODevice_FileEntry::lowlevel_read(void *data, size_t size, bool read_all)
	{
		switch (file_header.compression_method)
		{
		case zip_compress_store: // no compression
		{
			size_t received = iodevice.receive(data, size_t(min((int64_t)size, file_header.uncompressed_size - pos)), read_all);
			pos += received;
			return received;
		}
		break;

		case zip_compress_deflate:
			zs.next_out = (unsigned char *)data;
			zs.avail_out = (unsigned int)size;
			// Continue feeding zlib data until we get our data:
			while (zs.avail_out > 0)
			{
				// zlib needs more data:
				if (zs.avail_in == 0 && compressed_pos < file_header.compressed_size)
				{
					// Read some compressed data:
					size_t received_input = 0;
					while (received_input < 16 * 1024)
					{
						received_input += iodevice.receive(zbuffer, size_t(min((int64_t)16 * 1024, file_header.compressed_size - compressed_pos)), true);
						if (compressed_pos + received_input == file_header.compressed_size) break;
					}
					compressed_pos += received_input;

					zs.next_in = (unsigned char *)zbuffer;
					zs.avail_in = (unsigned int)received_input;
				}

				// Decompress data:
				int result = mz_inflate(&zs, 0);
				if (result == MZ_STREAM_END) break;
				if (result == MZ_NEED_DICT) throw Exception("Zlib inflate wants a dictionary!");
				if (result == MZ_DATA_ERROR) throw Exception("Zip data stream is corrupted");
				if (result == MZ_STREAM_ERROR) throw Exception("Zip stream structure was inconsistent!");
				if (result == MZ_MEM_ERROR) throw Exception("Zlib did not have enough memory to decompress file!");
				if (result == MZ_BUF_ERROR) throw Exception("Not enough data in buffer when Z_FINISH was used");
				if (result != MZ_OK) throw Exception("Zlib inflate failed while decompressing zip file!");
			}
			pos += size - size_t(zs.avail_out);
			return size - size_t(zs.avail_out);

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
}
