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
**    Mark Page
*/

#include "Core/precomp.h"
#include "aes256_encrypt_impl.h"
#include "API/Core/Math/cl_math.h"

#ifndef WIN32
#include <cstring>
#endif

namespace clan
{
	AES256_Encrypt_Impl::AES256_Encrypt_Impl() : initialisation_vector_set(false), cipher_key_set(false), padding_enabled(true), padding_pkcs7(true), padding_num_additional_padded_blocks(0)
	{
		reset();
	}

	DataBuffer AES256_Encrypt_Impl::get_data() const
	{
		return databuffer;
	}

	void AES256_Encrypt_Impl::reset()
	{
		calculated = false;
		memset(chunk, 0, sizeof(chunk));
		chunk_filled = 0;
		databuffer.set_size(0);
	}

	void AES256_Encrypt_Impl::set_iv(const unsigned char iv[16])
	{
		initialisation_vector_1 = get_word(iv);
		initialisation_vector_2 = get_word(iv + 4);
		initialisation_vector_3 = get_word(iv + 8);
		initialisation_vector_4 = get_word(iv + 12);

		initialisation_vector_set = true;
	}

	void AES256_Encrypt_Impl::set_padding(bool value, bool use_pkcs7, unsigned int num_additional_padded_blocks)
	{
		padding_enabled = value;
		padding_pkcs7 = use_pkcs7;
		padding_num_additional_padded_blocks = num_additional_padded_blocks;
		if (padding_num_additional_padded_blocks > 15)
			throw Exception("Number of additional blocks in padding must be 0 to 15");
	}

	void AES256_Encrypt_Impl::set_key(const unsigned char key[32])
	{
		cipher_key_set = true;
		extract_encrypt_key256(key, key_expanded);
	}

	void AES256_Encrypt_Impl::add(const void *_data, int size)
	{
		if (calculated)
			reset();

		if (!initialisation_vector_set)
			throw Exception("AES-256 initialisation vector has not been set");

		if (!cipher_key_set)
			throw Exception("AES-256 cipher key has not been set");

		const unsigned char *data = (const unsigned char *)_data;
		int pos = 0;
		while (pos < size)
		{
			int data_left = size - pos;
			int buffer_space = aes256_block_size_bytes - chunk_filled;
			int data_used = min(buffer_space, data_left);
			memcpy(chunk + chunk_filled, data + pos, data_used);
			chunk_filled += data_used;
			pos += data_used;
			if (chunk_filled == aes256_block_size_bytes)
			{
				process_chunk();
				chunk_filled = 0;
			}
		}
	}

	void AES256_Encrypt_Impl::calculate()
	{
		if (calculated)
			reset();

		if (padding_enabled)
		{
			// Note, chunk_filled is never equal to aes256_block_size_bytes at this point, so we can process an empty block
			if (padding_pkcs7)
			{
				// PKCS#7
				unsigned char pad_size = aes256_block_size_bytes - chunk_filled;
				memset(chunk + chunk_filled, pad_size, pad_size);
				process_chunk();
				chunk_filled = 0;
			}
			else
			{
				// rfc2246
				std::vector<unsigned char> buffer;
				unsigned int pad_size = aes256_block_size_bytes - chunk_filled;
				pad_size += aes128_block_size_bytes * padding_num_additional_padded_blocks;
				buffer.resize(pad_size, pad_size - 1);
				add(&buffer[0], pad_size);
			}
		}
		else
		{
			if (chunk_filled > 0)
				throw Exception("You must provide data with a block size of 16 when padding is disabled");
		}

		calculated = true;
		initialisation_vector_set = false;	// Force to reset after each call
		cipher_key_set = false;				// Force to reset after each call (to avoid keeping the cipher key in memory)
		memset(key_expanded, 0, sizeof(key_expanded));	// Remove the key from memory
	}

	void AES256_Encrypt_Impl::process_chunk()
	{
		const uint32_t *key_expanded_ptr = key_expanded;

		/* Electronic Codebook Mode
		uint32_t s0 = get_word(chunk) ^ key_expanded_ptr[0];
		uint32_t s1 = get_word(chunk + 4) ^ key_expanded_ptr[1];
		uint32_t s2 = get_word(chunk + 8) ^ key_expanded_ptr[2];
		uint32_t s3 = get_word(chunk + 12) ^ key_expanded_ptr[3];
		*/

		// Cipher Block Chaining Mode
		uint32_t s0 = initialisation_vector_1 ^ get_word(chunk) ^ key_expanded_ptr[0];
		uint32_t s1 = initialisation_vector_2 ^ get_word(chunk + 4) ^ key_expanded_ptr[1];
		uint32_t s2 = initialisation_vector_3 ^ get_word(chunk + 8) ^ key_expanded_ptr[2];
		uint32_t s3 = initialisation_vector_4 ^ get_word(chunk + 12) ^ key_expanded_ptr[3];

		uint32_t t0;
		uint32_t t1;
		uint32_t t2;
		uint32_t t3;

		// round 1
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[4];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[5];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[6];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[7];
		// round 2
		s0 = table_e0[t0 >> 24] ^ table_e1[(t1 >> 16) & 0xff] ^ table_e2[(t2 >> 8) & 0xff] ^ table_e3[t3 & 0xff] ^ key_expanded_ptr[8];
		s1 = table_e0[t1 >> 24] ^ table_e1[(t2 >> 16) & 0xff] ^ table_e2[(t3 >> 8) & 0xff] ^ table_e3[t0 & 0xff] ^ key_expanded_ptr[9];
		s2 = table_e0[t2 >> 24] ^ table_e1[(t3 >> 16) & 0xff] ^ table_e2[(t0 >> 8) & 0xff] ^ table_e3[t1 & 0xff] ^ key_expanded_ptr[10];
		s3 = table_e0[t3 >> 24] ^ table_e1[(t0 >> 16) & 0xff] ^ table_e2[(t1 >> 8) & 0xff] ^ table_e3[t2 & 0xff] ^ key_expanded_ptr[11];
		// round 3
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[12];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[13];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[14];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[15];
		// round 4
		s0 = table_e0[t0 >> 24] ^ table_e1[(t1 >> 16) & 0xff] ^ table_e2[(t2 >> 8) & 0xff] ^ table_e3[t3 & 0xff] ^ key_expanded_ptr[16];
		s1 = table_e0[t1 >> 24] ^ table_e1[(t2 >> 16) & 0xff] ^ table_e2[(t3 >> 8) & 0xff] ^ table_e3[t0 & 0xff] ^ key_expanded_ptr[17];
		s2 = table_e0[t2 >> 24] ^ table_e1[(t3 >> 16) & 0xff] ^ table_e2[(t0 >> 8) & 0xff] ^ table_e3[t1 & 0xff] ^ key_expanded_ptr[18];
		s3 = table_e0[t3 >> 24] ^ table_e1[(t0 >> 16) & 0xff] ^ table_e2[(t1 >> 8) & 0xff] ^ table_e3[t2 & 0xff] ^ key_expanded_ptr[19];
		// round 5
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[20];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[21];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[22];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[23];
		// round 6
		s0 = table_e0[t0 >> 24] ^ table_e1[(t1 >> 16) & 0xff] ^ table_e2[(t2 >> 8) & 0xff] ^ table_e3[t3 & 0xff] ^ key_expanded_ptr[24];
		s1 = table_e0[t1 >> 24] ^ table_e1[(t2 >> 16) & 0xff] ^ table_e2[(t3 >> 8) & 0xff] ^ table_e3[t0 & 0xff] ^ key_expanded_ptr[25];
		s2 = table_e0[t2 >> 24] ^ table_e1[(t3 >> 16) & 0xff] ^ table_e2[(t0 >> 8) & 0xff] ^ table_e3[t1 & 0xff] ^ key_expanded_ptr[26];
		s3 = table_e0[t3 >> 24] ^ table_e1[(t0 >> 16) & 0xff] ^ table_e2[(t1 >> 8) & 0xff] ^ table_e3[t2 & 0xff] ^ key_expanded_ptr[27];
		// round 7
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[28];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[29];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[30];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[31];
		// round 8
		s0 = table_e0[t0 >> 24] ^ table_e1[(t1 >> 16) & 0xff] ^ table_e2[(t2 >> 8) & 0xff] ^ table_e3[t3 & 0xff] ^ key_expanded_ptr[32];
		s1 = table_e0[t1 >> 24] ^ table_e1[(t2 >> 16) & 0xff] ^ table_e2[(t3 >> 8) & 0xff] ^ table_e3[t0 & 0xff] ^ key_expanded_ptr[33];
		s2 = table_e0[t2 >> 24] ^ table_e1[(t3 >> 16) & 0xff] ^ table_e2[(t0 >> 8) & 0xff] ^ table_e3[t1 & 0xff] ^ key_expanded_ptr[34];
		s3 = table_e0[t3 >> 24] ^ table_e1[(t0 >> 16) & 0xff] ^ table_e2[(t1 >> 8) & 0xff] ^ table_e3[t2 & 0xff] ^ key_expanded_ptr[35];
		// round 9
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[36];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[37];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[38];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[39];
		// round 10
		s0 = table_e0[t0 >> 24] ^ table_e1[(t1 >> 16) & 0xff] ^ table_e2[(t2 >> 8) & 0xff] ^ table_e3[t3 & 0xff] ^ key_expanded_ptr[40];
		s1 = table_e0[t1 >> 24] ^ table_e1[(t2 >> 16) & 0xff] ^ table_e2[(t3 >> 8) & 0xff] ^ table_e3[t0 & 0xff] ^ key_expanded_ptr[41];
		s2 = table_e0[t2 >> 24] ^ table_e1[(t3 >> 16) & 0xff] ^ table_e2[(t0 >> 8) & 0xff] ^ table_e3[t1 & 0xff] ^ key_expanded_ptr[42];
		s3 = table_e0[t3 >> 24] ^ table_e1[(t0 >> 16) & 0xff] ^ table_e2[(t1 >> 8) & 0xff] ^ table_e3[t2 & 0xff] ^ key_expanded_ptr[43];
		// round 11
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[44];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[45];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[46];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[47];
		// round 12
		s0 = table_e0[t0 >> 24] ^ table_e1[(t1 >> 16) & 0xff] ^ table_e2[(t2 >> 8) & 0xff] ^ table_e3[t3 & 0xff] ^ key_expanded_ptr[48];
		s1 = table_e0[t1 >> 24] ^ table_e1[(t2 >> 16) & 0xff] ^ table_e2[(t3 >> 8) & 0xff] ^ table_e3[t0 & 0xff] ^ key_expanded_ptr[49];
		s2 = table_e0[t2 >> 24] ^ table_e1[(t3 >> 16) & 0xff] ^ table_e2[(t0 >> 8) & 0xff] ^ table_e3[t1 & 0xff] ^ key_expanded_ptr[50];
		s3 = table_e0[t3 >> 24] ^ table_e1[(t0 >> 16) & 0xff] ^ table_e2[(t1 >> 8) & 0xff] ^ table_e3[t2 & 0xff] ^ key_expanded_ptr[51];
		// round 13
		t0 = table_e0[s0 >> 24] ^ table_e1[(s1 >> 16) & 0xff] ^ table_e2[(s2 >> 8) & 0xff] ^ table_e3[s3 & 0xff] ^ key_expanded_ptr[52];
		t1 = table_e0[s1 >> 24] ^ table_e1[(s2 >> 16) & 0xff] ^ table_e2[(s3 >> 8) & 0xff] ^ table_e3[s0 & 0xff] ^ key_expanded_ptr[53];
		t2 = table_e0[s2 >> 24] ^ table_e1[(s3 >> 16) & 0xff] ^ table_e2[(s0 >> 8) & 0xff] ^ table_e3[s1 & 0xff] ^ key_expanded_ptr[54];
		t3 = table_e0[s3 >> 24] ^ table_e1[(s0 >> 16) & 0xff] ^ table_e2[(s1 >> 8) & 0xff] ^ table_e3[s2 & 0xff] ^ key_expanded_ptr[55];

		key_expanded_ptr += aes256_num_rounds_nr * 4;

		// Apply last round
		s0 = (sbox_substitution_values[(t0 >> 24)] & 0xff000000) ^ (sbox_substitution_values[(t1 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_substitution_values[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_substitution_values[(t3)& 0xff] & 0x000000ff) ^ key_expanded_ptr[0];
		s1 = (sbox_substitution_values[(t1 >> 24)] & 0xff000000) ^ (sbox_substitution_values[(t2 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_substitution_values[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_substitution_values[(t0)& 0xff] & 0x000000ff) ^ key_expanded_ptr[1];
		s2 = (sbox_substitution_values[(t2 >> 24)] & 0xff000000) ^ (sbox_substitution_values[(t3 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_substitution_values[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_substitution_values[(t1)& 0xff] & 0x000000ff) ^ key_expanded_ptr[2];
		s3 = (sbox_substitution_values[(t3 >> 24)] & 0xff000000) ^ (sbox_substitution_values[(t0 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_substitution_values[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_substitution_values[(t2)& 0xff] & 0x000000ff) ^ key_expanded_ptr[3];

		store_block(s0, s1, s2, s3, databuffer);

		initialisation_vector_1 = s0;
		initialisation_vector_2 = s1;
		initialisation_vector_3 = s2;
		initialisation_vector_4 = s3;
	}
}
