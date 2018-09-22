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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/cl_math.h"
#include "aes_impl.h"

#ifndef WIN32
#include <cstring>
#endif

namespace clan
{
	AES_Impl::AES_Impl()
	{
		if (!is_tables_created)
		{
			create_tables();
			is_tables_created = true;
		}
	}

	bool AES_Impl::is_tables_created = false;

	uint32_t AES_Impl::table_e0[256];
	uint32_t AES_Impl::table_e1[256];
	uint32_t AES_Impl::table_e2[256];
	uint32_t AES_Impl::table_e3[256];
	uint32_t AES_Impl::table_d0[256];
	uint32_t AES_Impl::table_d1[256];
	uint32_t AES_Impl::table_d2[256];
	uint32_t AES_Impl::table_d3[256];
	uint32_t AES_Impl::sbox_inverse_substitution_values[256];
	uint32_t AES_Impl::sbox_substitution_values[256];
	uint32_t AES_Impl::rcon_values[10];

	void AES_Impl::extract_encrypt_key128(const unsigned char key[aes128_key_length_bytes], uint32_t key_expanded[aes128_nb_mult_nr_plus1])
	{
		for (int cnt = 0; cnt < aes128_key_length_nk; cnt++)
		{
			key_expanded[cnt] = get_word(key + 4 * cnt);
		}

		uint32_t *key_expanded_ptr = key_expanded;

		for (int cnt = 0; cnt < aes128_num_rounds_nr; cnt++)
		{
			uint32_t temp = key_expanded_ptr[3];
			key_expanded_ptr[4] = key_expanded_ptr[0] ^
				(sbox_substitution_values[(temp >> 16) & 0xff] & 0xff000000) ^
				(sbox_substitution_values[(temp >> 8) & 0xff] & 0x00ff0000) ^
				(sbox_substitution_values[(temp)& 0xff] & 0x0000ff00) ^
				(sbox_substitution_values[(temp >> 24)] & 0x000000ff) ^
				rcon_values[cnt];
			key_expanded_ptr[5] = key_expanded_ptr[1] ^ key_expanded_ptr[4];
			key_expanded_ptr[6] = key_expanded_ptr[2] ^ key_expanded_ptr[5];
			key_expanded_ptr[7] = key_expanded_ptr[3] ^ key_expanded_ptr[6];

			key_expanded_ptr += 4;
		}
	}

	void AES_Impl::extract_encrypt_key192(const unsigned char key[aes192_key_length_bytes], uint32_t key_expanded[aes192_nb_mult_nr_plus1])
	{
		for (int cnt = 0; cnt < aes192_key_length_nk; cnt++)
		{
			key_expanded[cnt] = get_word(key + 4 * cnt);
		}

		uint32_t *key_expanded_ptr = key_expanded;

		for (int cnt = 0; cnt < 8; cnt++)
		{
			uint32_t temp = key_expanded_ptr[5];
			key_expanded_ptr[6] = key_expanded_ptr[0] ^
				(sbox_substitution_values[(temp >> 16) & 0xff] & 0xff000000) ^
				(sbox_substitution_values[(temp >> 8) & 0xff] & 0x00ff0000) ^
				(sbox_substitution_values[(temp)& 0xff] & 0x0000ff00) ^
				(sbox_substitution_values[(temp >> 24)] & 0x000000ff) ^
				rcon_values[cnt];
			key_expanded_ptr[7] = key_expanded_ptr[1] ^ key_expanded_ptr[6];
			key_expanded_ptr[8] = key_expanded_ptr[2] ^ key_expanded_ptr[7];
			key_expanded_ptr[9] = key_expanded_ptr[3] ^ key_expanded_ptr[8];

			if (cnt == (8 - 1))
				break;

			key_expanded_ptr[10] = key_expanded_ptr[4] ^ key_expanded_ptr[9];
			key_expanded_ptr[11] = key_expanded_ptr[5] ^ key_expanded_ptr[10];

			key_expanded_ptr += 6;
		}
	}

	void AES_Impl::extract_encrypt_key256(const unsigned char key[aes256_key_length_bytes], uint32_t key_expanded[aes256_nb_mult_nr_plus1])
	{
		for (int cnt = 0; cnt < aes256_key_length_nk; cnt++)
		{
			key_expanded[cnt] = get_word(key + 4 * cnt);
		}

		uint32_t *key_expanded_ptr = key_expanded;

		for (int cnt = 0; cnt < 7; cnt++)
		{
			uint32_t temp = key_expanded_ptr[7];
			key_expanded_ptr[8] = key_expanded_ptr[0] ^
				(sbox_substitution_values[(temp >> 16) & 0xff] & 0xff000000) ^
				(sbox_substitution_values[(temp >> 8) & 0xff] & 0x00ff0000) ^
				(sbox_substitution_values[(temp)& 0xff] & 0x0000ff00) ^
				(sbox_substitution_values[(temp >> 24)] & 0x000000ff) ^
				rcon_values[cnt];
			key_expanded_ptr[9] = key_expanded_ptr[1] ^ key_expanded_ptr[8];
			key_expanded_ptr[10] = key_expanded_ptr[2] ^ key_expanded_ptr[9];
			key_expanded_ptr[11] = key_expanded_ptr[3] ^ key_expanded_ptr[10];

			if (cnt == (7 - 1))
				break;

			temp = key_expanded_ptr[11];
			key_expanded_ptr[12] = key_expanded_ptr[4] ^
				(sbox_substitution_values[(temp >> 24) & 0xff] & 0xff000000) ^
				(sbox_substitution_values[(temp >> 16) & 0xff] & 0x00ff0000) ^
				(sbox_substitution_values[(temp >> 8) & 0xff] & 0x0000ff00) ^
				(sbox_substitution_values[(temp)& 0xff] & 0x000000ff);
			key_expanded_ptr[13] = key_expanded_ptr[5] ^ key_expanded_ptr[12];
			key_expanded_ptr[14] = key_expanded_ptr[6] ^ key_expanded_ptr[13];
			key_expanded_ptr[15] = key_expanded_ptr[7] ^ key_expanded_ptr[14];
			key_expanded_ptr += 8;

		}
	}
	void AES_Impl::store_block(uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3, DataBuffer &databuffer)
	{
		// (Note AES 128, 192 and 256 all have the same block size)

		// Store the data
		int current_size = databuffer.get_size();
		int current_capacity = databuffer.get_capacity();
		int available = current_capacity - current_size;
		if (available < aes128_block_size_bytes)	// Increase capacity required
		{
			databuffer.set_capacity(current_capacity + 1024);	// Increase in blocks of 1K
		}
		databuffer.set_size(current_size + aes128_block_size_bytes);
		unsigned char *dest_ptr = (unsigned char *)databuffer.get_data();
		dest_ptr += current_size;

		put_word(s0, dest_ptr);
		put_word(s1, dest_ptr + 4);
		put_word(s2, dest_ptr + 8);
		put_word(s3, dest_ptr + 12);
	}

	void AES_Impl::extract_decrypt_key(uint32_t *key_expanded, int num_rounds)
	{
		// Invert the order of the round keys
		for (int start_value = 0, end_value = 4 * num_rounds; start_value < end_value; start_value += 4, end_value -= 4)
		{
			uint32_t temp;
			temp = key_expanded[start_value]; key_expanded[start_value] = key_expanded[end_value]; key_expanded[end_value] = temp;
			temp = key_expanded[start_value + 1]; key_expanded[start_value + 1] = key_expanded[end_value + 1]; key_expanded[end_value + 1] = temp;
			temp = key_expanded[start_value + 2]; key_expanded[start_value + 2] = key_expanded[end_value + 2]; key_expanded[end_value + 2] = temp;
			temp = key_expanded[start_value + 3]; key_expanded[start_value + 3] = key_expanded[end_value + 3]; key_expanded[end_value + 3] = temp;
		}

		// Apply the inverse MixColumn transform to all round keys but the first and the last
		uint32_t *key_expanded_ptr = key_expanded;
		for (int cnt = 1; cnt < num_rounds; cnt++)
		{
			key_expanded_ptr += 4;
			key_expanded_ptr[0] =
				table_d0[sbox_substitution_values[(key_expanded_ptr[0] >> 24)] & 0xff] ^
				table_d1[sbox_substitution_values[(key_expanded_ptr[0] >> 16) & 0xff] & 0xff] ^
				table_d2[sbox_substitution_values[(key_expanded_ptr[0] >> 8) & 0xff] & 0xff] ^
				table_d3[sbox_substitution_values[(key_expanded_ptr[0]) & 0xff] & 0xff];
			key_expanded_ptr[1] =
				table_d0[sbox_substitution_values[(key_expanded_ptr[1] >> 24)] & 0xff] ^
				table_d1[sbox_substitution_values[(key_expanded_ptr[1] >> 16) & 0xff] & 0xff] ^
				table_d2[sbox_substitution_values[(key_expanded_ptr[1] >> 8) & 0xff] & 0xff] ^
				table_d3[sbox_substitution_values[(key_expanded_ptr[1]) & 0xff] & 0xff];
			key_expanded_ptr[2] =
				table_d0[sbox_substitution_values[(key_expanded_ptr[2] >> 24)] & 0xff] ^
				table_d1[sbox_substitution_values[(key_expanded_ptr[2] >> 16) & 0xff] & 0xff] ^
				table_d2[sbox_substitution_values[(key_expanded_ptr[2] >> 8) & 0xff] & 0xff] ^
				table_d3[sbox_substitution_values[(key_expanded_ptr[2]) & 0xff] & 0xff];
			key_expanded_ptr[3] =
				table_d0[sbox_substitution_values[(key_expanded_ptr[3] >> 24)] & 0xff] ^
				table_d1[sbox_substitution_values[(key_expanded_ptr[3] >> 16) & 0xff] & 0xff] ^
				table_d2[sbox_substitution_values[(key_expanded_ptr[3] >> 8) & 0xff] & 0xff] ^
				table_d3[sbox_substitution_values[(key_expanded_ptr[3]) & 0xff] & 0xff];
		}
	}


	void AES_Impl::create_tables()
	{
		size_t power[256];
		size_t logarithm[256];

		size_t cnt, x;

		// Calculate power and logarithm helper tables over GF(2^8)
		for (cnt = 0, x = 1; cnt < 256; cnt++)
		{
			power[cnt] = x;
			logarithm[x] = cnt;
			x = (x ^ x_time(x)) & 0xff;
		}

		// Calculate the rcon table
		for (cnt = 0, x = 1; cnt < 10; cnt++)
		{
			rcon_values[cnt] = x << 24;
			x = x_time(x) & 0xFF;
		}

		// Create the SBox's
		sbox_substitution_values[0] = 0x63636363;
		sbox_inverse_substitution_values[0x63] = 0;
		for (cnt = 1; cnt < 256; cnt++)
		{
			x = power[255 - logarithm[cnt]];
			int y = x; y = ((y << 1) | (y >> 7)) & 0xFF;
			x ^= y; y = ((y << 1) | (y >> 7)) & 0xFF;
			x ^= y; y = ((y << 1) | (y >> 7)) & 0xFF;
			x ^= y; y = ((y << 1) | (y >> 7)) & 0xFF;
			x ^= y ^ 0x63;

			x = x & 0xFF;
			sbox_substitution_values[cnt] = x << 24 | x << 16 | x << 8 | x;
			sbox_inverse_substitution_values[x] = cnt << 24 | cnt << 16 | cnt << 8 | cnt;

		}

		// Create the encrypt tables
		for (cnt = 0; cnt < 256; cnt++)
		{
			// Get the sbox value
			x = sbox_substitution_values[cnt] & 0xFF;
			int y = x_time(x) & 0xFF;
			int z = (y ^ x) & 0xFF;

			table_e0[cnt] = z | x << 8 | x << 16 | y << 24;
			table_e1[cnt] = x | x << 8 | y << 16 | z << 24;
			table_e2[cnt] = x | y << 8 | z << 16 | x << 24;
			table_e3[cnt] = y | z << 8 | x << 16 | x << 24;

			x = sbox_inverse_substitution_values[cnt] & 0xFF;
			int a = mul_value(0xe, x, power, logarithm);
			int b = mul_value(0x9, x, power, logarithm);
			int c = mul_value(0xd, x, power, logarithm);
			int d = mul_value(0xb, x, power, logarithm);
			table_d0[cnt] = d | c << 8 | b << 16 | a << 24;
			table_d1[cnt] = c | b << 8 | a << 16 | d << 24;
			table_d2[cnt] = b | a << 8 | d << 16 | c << 24;
			table_d3[cnt] = a | d << 8 | c << 16 | b << 24;

		}
	}
}
