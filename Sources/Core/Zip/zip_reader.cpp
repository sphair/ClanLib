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
#include "API/Core/Zip/zip_reader.h"
#include "zip_archive_impl.h"
#include "zip_local_file_header.h"
#include "zip_compression_method.h"
#include "API/Core/Math/cl_math.h"
#include "Core/Zip/miniz.h"

namespace clan
{
	class ZipReader_Impl
	{
	public:
		ZipReader_Impl(IODevice &input)
			: input(input), zstream_open(false), compressed_pos(0)
		{
		}

		~ZipReader_Impl()
		{
			if (zstream_open)
				mz_inflateEnd(&zs);
		}

		int64_t deflate_read(void *data, int64_t size, bool read_all);

		IODevice input;
		ZipLocalFileHeader local_header;
		mz_stream zs;
		char zbuffer[16 * 1024];
		bool zstream_open;
		int64_t compressed_pos;
	};

	ZipReader::ZipReader(IODevice &input)
		: impl(std::make_shared<ZipReader_Impl>(input))
	{
	}

	bool ZipReader::read_local_file_header(bool allow_data_descriptor)
	{
		try
		{
			impl->local_header.load(impl->input);
		}
		catch (const Exception&)
		{
			return false;
		}

		if (has_data_descriptor() && !allow_data_descriptor)
			throw Exception("Zip file entry uses a data descriptor");
		if ((impl->local_header.general_purpose_bit_flag & ZIP_ENCRYPTED) ||
			(impl->local_header.general_purpose_bit_flag & ZIP_STRONG_ENCRYPTED))
			throw Exception("Zip file entry is encrypted");

		if (impl->zstream_open)
			mz_inflateEnd(&impl->zs);
		impl->zstream_open = false;

		if (impl->local_header.compression_method == zip_compress_deflate)
		{
			memset(&impl->zs, 0, sizeof(mz_stream));
			int result = mz_inflateInit2(&impl->zs, -15); // Undocumented: if wbits is negative, zlib skips header check
			if (result != MZ_OK)
				throw Exception("Zlib inflateInit failed for zip index!");
			impl->zstream_open = true;
			impl->compressed_pos = 0;
		}
		else if (impl->local_header.compression_method != zip_compress_store)
		{
			throw Exception("Zip file entry is compressed with an unsupported compression method");
		}

		return true;
	}

	std::string ZipReader::get_filename()
	{
		return impl->local_header.filename;
	}

	bool ZipReader::has_data_descriptor() const
	{
		return (impl->local_header.general_purpose_bit_flag & ZIP_CRC32_IN_FILE_DESCRIPTOR);
	}

	int64_t ZipReader::get_compressed_size() const
	{
		return impl->local_header.compressed_size;
	}

	int64_t ZipReader::get_uncompressed_size() const
	{
		return impl->local_header.uncompressed_size;
	}

	void ZipReader::set_data_descriptor_data(int64_t compressed_size, int64_t uncompressed_size, uint32_t crc32)
	{
		impl->local_header.compressed_size = compressed_size;
		impl->local_header.uncompressed_size = uncompressed_size;
		impl->local_header.crc32 = crc32;
	}

	int64_t ZipReader::read_file_data(void *data, int64_t size, bool read_all)
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

	int64_t ZipReader_Impl::deflate_read(void *data, int64_t size, bool read_all)
	{
		zs.next_out = (unsigned char *)data;
		zs.avail_out = size;
		// Continue feeding zlib data until we get our data:
		while (zs.avail_out > 0)
		{
			// zlib needs more data:
			if (zs.avail_in == 0 && compressed_pos < local_header.compressed_size)
			{
				// Read some compressed data:
				int received_input = 0;
				while (received_input < 16 * 1024)
				{
					received_input += input.receive(zbuffer, int(min((int64_t)16 * 1024, local_header.compressed_size - compressed_pos)), true);
					if (compressed_pos + received_input == local_header.compressed_size) break;
				}
				compressed_pos += received_input;

				zs.next_in = (unsigned char *)zbuffer;
				zs.avail_in = received_input;
			}

			// Decompress data:
			int result = mz_inflate(&zs, (compressed_pos == local_header.compressed_size) ? MZ_FINISH : MZ_NO_FLUSH);
			if (result == MZ_STREAM_END) break;
			if (result == MZ_NEED_DICT) throw Exception("Zlib inflate wants a dictionary!");
			if (result == MZ_DATA_ERROR) throw Exception("Zip data stream is corrupted");
			if (result == MZ_STREAM_ERROR) throw Exception("Zip stream structure was inconsistent!");
			if (result == MZ_MEM_ERROR) throw Exception("Zlib did not have enough memory to decompress file!");
			if (result == MZ_BUF_ERROR) throw Exception("Not enough data in buffer when Z_FINISH was used");
			if (result != MZ_OK) throw Exception("Zlib inflate failed while decompressing zip file!");
		}
		return size - zs.avail_out;
	}
}
