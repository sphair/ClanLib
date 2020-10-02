/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
	class MD5_Impl : private SHA
	{
	public:
		MD5_Impl();

		std::string get_hash(bool uppercase = false) const;
		void get_hash(unsigned char out_hash[16]) const;
		void reset();
		void add(const void *data, int size);
		void calculate();
		void set_hmac(const void *key_data, int key_size);

	private:
		void process_chunk();

		const static int block_size = 64;

		inline uint32_t leftrotate_uint32(uint32_t value, int shift) const
		{
			return (value << shift) | (value >> (32 - shift));
		}

		uint32_t h0, h1, h2, h3;
		unsigned char chunk[block_size];
		int chunk_filled;
		unsigned int length_message;
		bool calculated;

		bool hmac_enabled;
		unsigned char hmac_key_chunk[block_size];
	};
}
