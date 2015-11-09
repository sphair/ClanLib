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

#pragma once

#include "sha.h"

namespace clan
{
	class SHA512_Impl : private SHA
	{
	public:
		SHA512_Impl(cl_sha_type new_sha_type);

		std::string get_hash(bool uppercase = false) const;
		void get_hash(unsigned char *out_hash) const;

		void reset();
		void set_hmac(const void *key_data, int key_size);
		void add(const void *data, int size);
		void calculate();

	private:
		inline uint64_t sigma_rr28_rr34_rr39(uint64_t value) const
		{
			return (rightrotate_uint64(value, 28) ^ rightrotate_uint64(value, 34) ^ rightrotate_uint64(value, 39));
		}

		inline uint64_t sigma_rr14_rr18_rr41(uint64_t value) const
		{
			return (rightrotate_uint64(value, 14) ^ rightrotate_uint64(value, 18) ^ rightrotate_uint64(value, 41));
		}

		inline uint64_t sigma_rr1_rr8_sr7(uint64_t value) const
		{
			return (rightrotate_uint64(value, 1) ^ rightrotate_uint64(value, 8) ^ (value >> 7));
		}

		inline uint64_t sigma_rr19_rr61_sr6(uint64_t value) const
		{
			return (rightrotate_uint64(value, 19) ^ rightrotate_uint64(value, 61) ^ (value >> 6));
		}

		inline uint64_t sha_ch(uint64_t x, uint64_t y, uint64_t z) const
		{
			return (((x)& ((y) ^ (z))) ^ (z));
		}

		inline uint64_t sha_maj(uint64_t x, uint64_t y, uint64_t z) const
		{
			return  (((x)& ((y) | (z))) | ((y)& (z)));
		}

		void process_chunk();

		uint64_t h0, h1, h2, h3, h4, h5, h6, h7;
		const static int block_size = 128;
		unsigned char chunk[128];
		int chunk_filled;
		ubyte128 length_message;
		bool calculated;

		cl_sha_type sha_type;
		bool hmac_enabled;
		unsigned char hmac_key_chunk[block_size];
	};
}
