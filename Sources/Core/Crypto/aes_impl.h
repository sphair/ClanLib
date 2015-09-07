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

namespace clan
{
	class AES_Impl
	{
	public:
		AES_Impl();

		static void create_tables();

		static const int aes128_key_length_nk = 4;
		static const int aes128_block_size_nb = 4;
		static const int aes128_num_rounds_nr = 10;
		static const int aes192_key_length_nk = 6;
		static const int aes192_block_size_nb = 4;
		static const int aes192_num_rounds_nr = 12;
		static const int aes256_key_length_nk = 8;
		static const int aes256_block_size_nb = 4;
		static const int aes256_num_rounds_nr = 14;

		static const int aes128_key_length_bytes = aes128_key_length_nk * 4;
		static const int aes128_nb_mult_nr_plus1 = aes128_block_size_nb * (aes128_num_rounds_nr + 1);
		static const int aes128_block_size_bytes = aes128_block_size_nb * 4;
		static const int aes192_key_length_bytes = aes192_key_length_nk * 4;
		static const int aes192_nb_mult_nr_plus1 = aes192_block_size_nb * (aes192_num_rounds_nr + 1);
		static const int aes192_block_size_bytes = aes192_block_size_nb * 4;
		static const int aes256_key_length_bytes = aes256_key_length_nk * 4;
		static const int aes256_nb_mult_nr_plus1 = aes256_block_size_nb * (aes256_num_rounds_nr + 1);
		static const int aes256_block_size_bytes = aes256_block_size_nb * 4;

		static uint32_t sbox_substitution_values[256];
		static uint32_t sbox_inverse_substitution_values[256];
		static uint32_t rcon_values[10];

		// Combining SubBytes and ShiftRows with MixColumns, and transforming them into a sequence of table lookups
		static uint32_t table_e0[256];
		static uint32_t table_e1[256];
		static uint32_t table_e2[256];
		static uint32_t table_e3[256];
		static uint32_t table_d0[256];
		static uint32_t table_d1[256];
		static uint32_t table_d2[256];
		static uint32_t table_d3[256];

		void extract_encrypt_key128(const unsigned char key[aes128_key_length_bytes], uint32_t key_expanded[aes128_nb_mult_nr_plus1]);
		void extract_encrypt_key192(const unsigned char key[aes192_key_length_bytes], uint32_t key_expanded[aes192_nb_mult_nr_plus1]);
		void extract_encrypt_key256(const unsigned char key[aes256_key_length_bytes], uint32_t key_expanded[aes256_nb_mult_nr_plus1]);
		void extract_decrypt_key(uint32_t *key_expanded, int num_rounds);
		void store_block(uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3, DataBuffer &databuffer);

		inline uint32_t get_word(const unsigned char *data) const
		{
			return ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]));
		}

		inline void put_word(uint32_t source_value, unsigned char *dest_data) const
		{
			dest_data[0] = (unsigned char)(source_value >> 24);
			dest_data[1] = (unsigned char)(source_value >> 16);
			dest_data[2] = (unsigned char)(source_value >> 8);
			dest_data[3] = (unsigned char)(source_value);
		}

		static inline int x_time(int value)
		{
			return ((value << 1) ^ ((value & 0x80) ? 0x1B : 0x00));
		}

		static inline int mul_value(int x, int y, const int *power, const int *logarithm)
		{
			if (x && y)
			{
				return power[(logarithm[x] + logarithm[y]) % 255];
			}
			else
			{
				return 0;
			}
		}

	private:
		static bool is_tables_created;
	};
}
