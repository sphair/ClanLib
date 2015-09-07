/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#include "Core/precomp.h"
#include "sha1_impl.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Crypto/sha1.h"

#ifndef WIN32
#include <cstring>
#endif

namespace clan
{
	SHA1_Impl::SHA1_Impl()
	{
		reset();
	}

	std::string SHA1_Impl::get_hash(bool uppercase) const
	{
		if (calculated == false)
			throw Exception("SHA-1 hash has not been calculated yet!");

		char digest[41];
		to_hex_be(digest, h0, uppercase);
		to_hex_be(digest + 8, h1, uppercase);
		to_hex_be(digest + 16, h2, uppercase);
		to_hex_be(digest + 24, h3, uppercase);
		to_hex_be(digest + 32, h4, uppercase);
		digest[40] = 0;
		return digest;
	}

	void SHA1_Impl::get_hash(unsigned char out_hash[20]) const
	{
		if (calculated == false)
			throw Exception("SHA-1 hash has not been calculated yet!");

		out_hash[0] = (unsigned char)((h0 >> 24) & 0xff);
		out_hash[1] = (unsigned char)((h0 >> 16) & 0xff);
		out_hash[2] = (unsigned char)((h0 >> 8) & 0xff);
		out_hash[3] = (unsigned char)(h0 & 0xff);
		out_hash[4] = (unsigned char)((h1 >> 24) & 0xff);
		out_hash[5] = (unsigned char)((h1 >> 16) & 0xff);
		out_hash[6] = (unsigned char)((h1 >> 8) & 0xff);
		out_hash[7] = (unsigned char)(h1 & 0xff);
		out_hash[8] = (unsigned char)((h2 >> 24) & 0xff);
		out_hash[9] = (unsigned char)((h2 >> 16) & 0xff);
		out_hash[10] = (unsigned char)((h2 >> 8) & 0xff);
		out_hash[11] = (unsigned char)(h2 & 0xff);
		out_hash[12] = (unsigned char)((h3 >> 24) & 0xff);
		out_hash[13] = (unsigned char)((h3 >> 16) & 0xff);
		out_hash[14] = (unsigned char)((h3 >> 8) & 0xff);
		out_hash[15] = (unsigned char)(h3 & 0xff);
		out_hash[16] = (unsigned char)((h4 >> 24) & 0xff);
		out_hash[17] = (unsigned char)((h4 >> 16) & 0xff);
		out_hash[18] = (unsigned char)((h4 >> 8) & 0xff);
		out_hash[19] = (unsigned char)(h4 & 0xff);
	}

	void SHA1_Impl::reset()
	{
		//  FIPS 180-3 section 5.3.1
		h0 = 0x67452301;
		h1 = 0xEFCDAB89;
		h2 = 0x98BADCFE;
		h3 = 0x10325476;
		h4 = 0xC3D2E1F0;
		memset(chunk, 0, block_size);
		chunk_filled = 0;
		length_message = 0;
		calculated = false;
		hmac_enabled = false;

	}

	void SHA1_Impl::add(const void *_data, int size)
	{
		if (calculated)
			reset();

		const unsigned char *data = (const unsigned char *)_data;
		int pos = 0;
		while (pos < size)
		{
			int data_left = size - pos;
			int buffer_space = block_size - chunk_filled;
			int data_used = min(buffer_space, data_left);
			memcpy(chunk + chunk_filled, data + pos, data_used);
			chunk_filled += data_used;
			pos += data_used;
			if (chunk_filled == block_size)
			{
				process_chunk();
				chunk_filled = 0;
			}
		}
		length_message += size * (uint64_t)8;
	}

	void SHA1_Impl::set_hmac(const void *key_data, int key_size)
	{
		memset(hmac_key_chunk, 0, block_size);

		int key_chunk_filled;

		if (key_size > block_size)
		{
			SHA1 sha1;
			sha1.add(key_data, key_size);
			sha1.calculate();
			key_chunk_filled = SHA1::hash_size;
			sha1.get_hash(hmac_key_chunk);
		}
		else
		{
			memcpy(hmac_key_chunk, key_data, key_size);
			key_chunk_filled = key_size;
		}

		for (auto & elem : hmac_key_chunk)	// XOR key with inner pad values
		{
			elem ^= 0x36;
		}

		add(hmac_key_chunk, block_size);	// Add the inner HMAC

		for (auto & elem : hmac_key_chunk)	// XOR key with outer pad values
		{
			elem ^= 0x36;	// Undo the inner pad
			elem ^= 0x5c;
		}
		hmac_enabled = true;	// This has to be after the add(), as that function may call reset()

	}

	void SHA1_Impl::calculate()
	{
		if (calculated)
			reset();

		// append a single "1" bit to message
		// append "0" bits until message length ≡ 448 ≡ -64 (mod 512)
		// append length of message, in bits as 64-bit big-endian integer to message

		unsigned char end_data[block_size * 2];
		memset(end_data, 0, block_size * 2);
		end_data[0] = 128;

		int size = block_size - chunk_filled;
		if (size < 9)
			size += block_size;

		unsigned int length_upper = (unsigned int)(length_message >> 32);
		unsigned int length_lower = (unsigned int)(length_message & 0xffffffff);

		end_data[size - 8] = (length_upper & 0xff000000) >> 24;
		end_data[size - 7] = (length_upper & 0x00ff0000) >> 16;
		end_data[size - 6] = (length_upper & 0x0000ff00) >> 8;
		end_data[size - 5] = (length_upper & 0x000000ff);
		end_data[size - 4] = (length_lower & 0xff000000) >> 24;
		end_data[size - 3] = (length_lower & 0x00ff0000) >> 16;
		end_data[size - 2] = (length_lower & 0x0000ff00) >> 8;
		end_data[size - 1] = (length_lower & 0x000000ff);

		add(end_data, size);

		if (chunk_filled != 0)
			throw Exception("Error in SHA1_Impl class. Still chunk data at end of calculate");

		calculated = true;

		if (hmac_enabled)
		{
			unsigned char temp_hash[SHA1::hash_size];
			get_hash(temp_hash);
			reset();
			add(hmac_key_chunk, block_size);	// Add the outer HMAC
			add(temp_hash, SHA1::hash_size);
			calculate();
		}
	}

	void SHA1_Impl::process_chunk()
	{
		int i;
		unsigned int w[80];

		for (i = 0; i < 16; i++)
		{
			unsigned int b1 = chunk[i * 4];
			unsigned int b2 = chunk[i * 4 + 1];
			unsigned int b3 = chunk[i * 4 + 2];
			unsigned int b4 = chunk[i * 4 + 3];
			w[i] = (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
		}

		for (i = 16; i < 80; i++)
			w[i] = leftrotate_uint32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;

		for (i = 0; i < 80; i++)
		{
			uint32_t f, k;
			if (i < 20)
			{
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if (i < 40)
			{
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (i < 60)
			{
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else
			{
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}

			uint32_t temp = leftrotate_uint32(a, 5) + f + e + k + w[i];
			e = d;
			d = c;
			c = leftrotate_uint32(b, 30);
			b = a;
			a = temp;
		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
	}
}
