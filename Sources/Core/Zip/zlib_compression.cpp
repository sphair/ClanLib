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
#include "API/Core/Zip/zlib_compression.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/memory_device.h"

#define INCLUDED_FROM_ZLIB_COMPRESSION_CPP
#include "miniz.h"

namespace clan
{
	DataBuffer ZLibCompression::compress(const DataBuffer &data, bool raw, int compression_level, CompressionMode mode)
	{
		const int window_bits = 15;

		DataBuffer zbuffer(1024 * 1024);
		MemoryDevice output;

		int strategy = MZ_DEFAULT_STRATEGY;
		switch (mode)
		{
		case default_strategy: strategy = MZ_DEFAULT_STRATEGY; break;
		case filtered: strategy = MZ_FILTERED; break;
		case huffman_only: strategy = MZ_HUFFMAN_ONLY; break;
		case rle: strategy = MZ_RLE; break;
		case fixed: strategy = MZ_FIXED; break;
		}

		mz_stream zs = { nullptr };
		int result = mz_deflateInit2(&zs, compression_level, MZ_DEFLATED, raw ? -window_bits : window_bits, 8, strategy); // Undocumented: if wbits is negative, zlib skips header check
		if (result != MZ_OK)
			throw Exception("Zlib deflateInit failed");

		try
		{
			zs.next_in = (unsigned char *)data.get_data();
			zs.avail_in = data.get_size();
			while (true)
			{
				zs.next_out = (unsigned char *)zbuffer.get_data();
				zs.avail_out = zbuffer.get_size();

				int result = mz_deflate(&zs, MZ_FINISH);
				if (result == MZ_NEED_DICT) throw Exception("Zlib deflate wants a dictionary!");
				if (result == MZ_DATA_ERROR) throw Exception("Zip data stream is corrupted");
				if (result == MZ_STREAM_ERROR) throw Exception("Zip stream structure was inconsistent!");
				if (result == MZ_MEM_ERROR) throw Exception("Zlib did not have enough memory to compress file!");
				if (result == MZ_BUF_ERROR) throw Exception("Not enough data in buffer when Z_FINISH was used");
				if (result != MZ_OK && result != MZ_STREAM_END) throw Exception("Zlib deflate failed while compressing zip file!");
				int zsize = zbuffer.get_size() - zs.avail_out;
				if (zsize == 0)
					break;
				output.write(zbuffer.get_data(), zsize);
				if (result == MZ_STREAM_END)
					break;
			}
			mz_deflateEnd(&zs);
		}
		catch (...)
		{
			mz_deflateEnd(&zs);
			throw;
		}

		return output.get_data();
	}

	DataBuffer ZLibCompression::decompress(const DataBuffer &data, bool raw)
	{
		const int window_bits = 15;

		DataBuffer zbuffer(1024 * 1024);
		MemoryDevice output;

		mz_stream zs = { nullptr };
		int result = mz_inflateInit2(&zs, raw ? -window_bits : window_bits);
		if (result != MZ_OK)
			throw Exception("Zlib inflateInit failed");


		zs.next_in = (unsigned char *)data.get_data();
		zs.avail_in = data.get_size();

		// Continue feeding zlib data until we get our data:
		while (true)
		{
			zs.next_out = (unsigned char *)zbuffer.get_data();
			zs.avail_out = zbuffer.get_size();

			// Decompress data:
			int result = mz_inflate(&zs, 0);
			if (result == MZ_NEED_DICT) throw Exception("Zlib inflate wants a dictionary!");
			if (result == MZ_DATA_ERROR) throw Exception("Zip data stream is corrupted");
			if (result == MZ_STREAM_ERROR) throw Exception("Zip stream structure was inconsistent!");
			if (result == MZ_MEM_ERROR) throw Exception("Zlib did not have enough memory to decompress file!");
			if (result == MZ_BUF_ERROR) throw Exception("Not enough data in buffer when Z_FINISH was used");
			if (result != MZ_OK && result != MZ_STREAM_END) throw Exception("Zlib inflate failed while decompressing zip file!");

			output.write(zbuffer.get_data(), zbuffer.get_size() - zs.avail_out);

			if (result == MZ_STREAM_END)
				break;
		}
		mz_inflateEnd(&zs);

		return output.get_data();
	}
}
