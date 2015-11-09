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
#include "aes192_decrypt_impl.h"
#include "API/Core/Math/cl_math.h"

#ifndef WIN32
#include <cstring>
#endif

namespace clan
{
	AES192_Decrypt_Impl::AES192_Decrypt_Impl() : initialisation_vector_set(false), cipher_key_set(false), padding_enabled(true), padding_pkcs7(true)
	{
		reset();
	}

	DataBuffer AES192_Decrypt_Impl::get_data() const
	{
		return databuffer;
	}

	void AES192_Decrypt_Impl::reset()
	{
		calculated = false;
		memset(chunk, 0, sizeof(chunk));
		chunk_filled = 0;
		databuffer.set_size(0);
	}

	void AES192_Decrypt_Impl::set_iv(const unsigned char iv[16])
	{
		initialisation_vector_1 = get_word(iv);
		initialisation_vector_2 = get_word(iv + 4);
		initialisation_vector_3 = get_word(iv + 8);
		initialisation_vector_4 = get_word(iv + 12);

		initialisation_vector_set = true;
	}

	void AES192_Decrypt_Impl::set_padding(bool value, bool use_pkcs7)
	{
		padding_enabled = value;
		padding_pkcs7 = use_pkcs7;
	}

	void AES192_Decrypt_Impl::set_key(const unsigned char key[24])
	{
		cipher_key_set = true;
		extract_encrypt_key192(key, key_expanded);
		extract_decrypt_key(key_expanded, aes192_num_rounds_nr);
	}

	void AES192_Decrypt_Impl::add(const void *_data, int size)
	{
		if (calculated)
			reset();

		if (!initialisation_vector_set)
			throw Exception("AES-192 initialisation vector has not been set");

		if (!cipher_key_set)
			throw Exception("AES-192 cipher key has not been set");

		const unsigned char *data = (const unsigned char *)_data;
		int pos = 0;
		while (pos < size)
		{
			int data_left = size - pos;
			int buffer_space = aes192_block_size_bytes - chunk_filled;
			int data_used = min(buffer_space, data_left);
			memcpy(chunk + chunk_filled, data + pos, data_used);
			chunk_filled += data_used;
			pos += data_used;
			if (chunk_filled == aes192_block_size_bytes)
			{
				if ((!padding_enabled) || (pos < size))	// Do not process chunk on the last block if padding is enabled, as calculate() must process it
				{
					process_chunk();
					chunk_filled = 0;
				}
			}
		}

	}

	bool AES192_Decrypt_Impl::calculate()
	{
		if (calculated)
			reset();

		bool return_code = true;

		if (padding_enabled)
		{
			if (chunk_filled == aes192_block_size_bytes)
			{
				process_chunk();
				chunk_filled = 0;
				int current_size = databuffer.get_size();
				if (current_size > 0)
				{
					unsigned char *dest_ptr = (unsigned char *)databuffer.get_data();
					unsigned int pad_byte = dest_ptr[current_size - 1];
					if (padding_pkcs7)
					{
						if ((pad_byte == 0) || (pad_byte > aes192_block_size_bytes))
						{
							return_code = false;	// Invalid pad byte
						}
						else
						{
							databuffer.set_size(current_size - pad_byte);
						}
					}
					else
					{
						pad_byte += 1;	// Include the pad length
						if (current_size < pad_byte)
						{
							return_code = false;	// Not enough data available
						}
						else
						{
							databuffer.set_size(current_size - pad_byte);
						}
					}
				}
				else
				{
					return_code = false;	// Unknown error, for some reason the databuffer does not contain any data
				}
			}
			else
			{
				return_code = false;	// Input data was not a multiple of the block size. Do not attempt to decode it.
			}

		}
		else
		{
			if (chunk_filled)		// Input data was not a multiple of the block size. Do not attempt to decode it.
				return_code = false;
		}

		calculated = true;
		initialisation_vector_set = false;	// Force to reset after each call
		cipher_key_set = false;				// Force to reset after each call (to avoid keeping the cipher key in memory)
		memset(key_expanded, 0, sizeof(key_expanded));

		return true;

	}

	void AES192_Decrypt_Impl::process_chunk()
	{
		const uint32_t *key_expanded_ptr = key_expanded;

		uint32_t chunk1 = get_word(chunk);
		uint32_t chunk2 = get_word(chunk + 4);
		uint32_t chunk3 = get_word(chunk + 8);
		uint32_t chunk4 = get_word(chunk + 12);

		uint32_t s0 = chunk1 ^ key_expanded_ptr[0];
		uint32_t s1 = chunk2 ^ key_expanded_ptr[1];
		uint32_t s2 = chunk3 ^ key_expanded_ptr[2];
		uint32_t s3 = chunk4 ^ key_expanded_ptr[3];

		uint32_t t0;
		uint32_t t1;
		uint32_t t2;
		uint32_t t3;

		// Round 1
		t0 = table_d0[s0 >> 24] ^ table_d1[(s3 >> 16) & 0xff] ^ table_d2[(s2 >> 8) & 0xff] ^ table_d3[s1 & 0xff] ^ key_expanded_ptr[4];
		t1 = table_d0[s1 >> 24] ^ table_d1[(s0 >> 16) & 0xff] ^ table_d2[(s3 >> 8) & 0xff] ^ table_d3[s2 & 0xff] ^ key_expanded_ptr[5];
		t2 = table_d0[s2 >> 24] ^ table_d1[(s1 >> 16) & 0xff] ^ table_d2[(s0 >> 8) & 0xff] ^ table_d3[s3 & 0xff] ^ key_expanded_ptr[6];
		t3 = table_d0[s3 >> 24] ^ table_d1[(s2 >> 16) & 0xff] ^ table_d2[(s1 >> 8) & 0xff] ^ table_d3[s0 & 0xff] ^ key_expanded_ptr[7];
		// Round 2
		s0 = table_d0[t0 >> 24] ^ table_d1[(t3 >> 16) & 0xff] ^ table_d2[(t2 >> 8) & 0xff] ^ table_d3[t1 & 0xff] ^ key_expanded_ptr[8];
		s1 = table_d0[t1 >> 24] ^ table_d1[(t0 >> 16) & 0xff] ^ table_d2[(t3 >> 8) & 0xff] ^ table_d3[t2 & 0xff] ^ key_expanded_ptr[9];
		s2 = table_d0[t2 >> 24] ^ table_d1[(t1 >> 16) & 0xff] ^ table_d2[(t0 >> 8) & 0xff] ^ table_d3[t3 & 0xff] ^ key_expanded_ptr[10];
		s3 = table_d0[t3 >> 24] ^ table_d1[(t2 >> 16) & 0xff] ^ table_d2[(t1 >> 8) & 0xff] ^ table_d3[t0 & 0xff] ^ key_expanded_ptr[11];
		// Round 3
		t0 = table_d0[s0 >> 24] ^ table_d1[(s3 >> 16) & 0xff] ^ table_d2[(s2 >> 8) & 0xff] ^ table_d3[s1 & 0xff] ^ key_expanded_ptr[12];
		t1 = table_d0[s1 >> 24] ^ table_d1[(s0 >> 16) & 0xff] ^ table_d2[(s3 >> 8) & 0xff] ^ table_d3[s2 & 0xff] ^ key_expanded_ptr[13];
		t2 = table_d0[s2 >> 24] ^ table_d1[(s1 >> 16) & 0xff] ^ table_d2[(s0 >> 8) & 0xff] ^ table_d3[s3 & 0xff] ^ key_expanded_ptr[14];
		t3 = table_d0[s3 >> 24] ^ table_d1[(s2 >> 16) & 0xff] ^ table_d2[(s1 >> 8) & 0xff] ^ table_d3[s0 & 0xff] ^ key_expanded_ptr[15];
		// Round 4
		s0 = table_d0[t0 >> 24] ^ table_d1[(t3 >> 16) & 0xff] ^ table_d2[(t2 >> 8) & 0xff] ^ table_d3[t1 & 0xff] ^ key_expanded_ptr[16];
		s1 = table_d0[t1 >> 24] ^ table_d1[(t0 >> 16) & 0xff] ^ table_d2[(t3 >> 8) & 0xff] ^ table_d3[t2 & 0xff] ^ key_expanded_ptr[17];
		s2 = table_d0[t2 >> 24] ^ table_d1[(t1 >> 16) & 0xff] ^ table_d2[(t0 >> 8) & 0xff] ^ table_d3[t3 & 0xff] ^ key_expanded_ptr[18];
		s3 = table_d0[t3 >> 24] ^ table_d1[(t2 >> 16) & 0xff] ^ table_d2[(t1 >> 8) & 0xff] ^ table_d3[t0 & 0xff] ^ key_expanded_ptr[19];
		// Round 5
		t0 = table_d0[s0 >> 24] ^ table_d1[(s3 >> 16) & 0xff] ^ table_d2[(s2 >> 8) & 0xff] ^ table_d3[s1 & 0xff] ^ key_expanded_ptr[20];
		t1 = table_d0[s1 >> 24] ^ table_d1[(s0 >> 16) & 0xff] ^ table_d2[(s3 >> 8) & 0xff] ^ table_d3[s2 & 0xff] ^ key_expanded_ptr[21];
		t2 = table_d0[s2 >> 24] ^ table_d1[(s1 >> 16) & 0xff] ^ table_d2[(s0 >> 8) & 0xff] ^ table_d3[s3 & 0xff] ^ key_expanded_ptr[22];
		t3 = table_d0[s3 >> 24] ^ table_d1[(s2 >> 16) & 0xff] ^ table_d2[(s1 >> 8) & 0xff] ^ table_d3[s0 & 0xff] ^ key_expanded_ptr[23];
		// Round 6
		s0 = table_d0[t0 >> 24] ^ table_d1[(t3 >> 16) & 0xff] ^ table_d2[(t2 >> 8) & 0xff] ^ table_d3[t1 & 0xff] ^ key_expanded_ptr[24];
		s1 = table_d0[t1 >> 24] ^ table_d1[(t0 >> 16) & 0xff] ^ table_d2[(t3 >> 8) & 0xff] ^ table_d3[t2 & 0xff] ^ key_expanded_ptr[25];
		s2 = table_d0[t2 >> 24] ^ table_d1[(t1 >> 16) & 0xff] ^ table_d2[(t0 >> 8) & 0xff] ^ table_d3[t3 & 0xff] ^ key_expanded_ptr[26];
		s3 = table_d0[t3 >> 24] ^ table_d1[(t2 >> 16) & 0xff] ^ table_d2[(t1 >> 8) & 0xff] ^ table_d3[t0 & 0xff] ^ key_expanded_ptr[27];
		// Round 7
		t0 = table_d0[s0 >> 24] ^ table_d1[(s3 >> 16) & 0xff] ^ table_d2[(s2 >> 8) & 0xff] ^ table_d3[s1 & 0xff] ^ key_expanded_ptr[28];
		t1 = table_d0[s1 >> 24] ^ table_d1[(s0 >> 16) & 0xff] ^ table_d2[(s3 >> 8) & 0xff] ^ table_d3[s2 & 0xff] ^ key_expanded_ptr[29];
		t2 = table_d0[s2 >> 24] ^ table_d1[(s1 >> 16) & 0xff] ^ table_d2[(s0 >> 8) & 0xff] ^ table_d3[s3 & 0xff] ^ key_expanded_ptr[30];
		t3 = table_d0[s3 >> 24] ^ table_d1[(s2 >> 16) & 0xff] ^ table_d2[(s1 >> 8) & 0xff] ^ table_d3[s0 & 0xff] ^ key_expanded_ptr[31];
		// Round 8
		s0 = table_d0[t0 >> 24] ^ table_d1[(t3 >> 16) & 0xff] ^ table_d2[(t2 >> 8) & 0xff] ^ table_d3[t1 & 0xff] ^ key_expanded_ptr[32];
		s1 = table_d0[t1 >> 24] ^ table_d1[(t0 >> 16) & 0xff] ^ table_d2[(t3 >> 8) & 0xff] ^ table_d3[t2 & 0xff] ^ key_expanded_ptr[33];
		s2 = table_d0[t2 >> 24] ^ table_d1[(t1 >> 16) & 0xff] ^ table_d2[(t0 >> 8) & 0xff] ^ table_d3[t3 & 0xff] ^ key_expanded_ptr[34];
		s3 = table_d0[t3 >> 24] ^ table_d1[(t2 >> 16) & 0xff] ^ table_d2[(t1 >> 8) & 0xff] ^ table_d3[t0 & 0xff] ^ key_expanded_ptr[35];
		// Round 9
		t0 = table_d0[s0 >> 24] ^ table_d1[(s3 >> 16) & 0xff] ^ table_d2[(s2 >> 8) & 0xff] ^ table_d3[s1 & 0xff] ^ key_expanded_ptr[36];
		t1 = table_d0[s1 >> 24] ^ table_d1[(s0 >> 16) & 0xff] ^ table_d2[(s3 >> 8) & 0xff] ^ table_d3[s2 & 0xff] ^ key_expanded_ptr[37];
		t2 = table_d0[s2 >> 24] ^ table_d1[(s1 >> 16) & 0xff] ^ table_d2[(s0 >> 8) & 0xff] ^ table_d3[s3 & 0xff] ^ key_expanded_ptr[38];
		t3 = table_d0[s3 >> 24] ^ table_d1[(s2 >> 16) & 0xff] ^ table_d2[(s1 >> 8) & 0xff] ^ table_d3[s0 & 0xff] ^ key_expanded_ptr[39];
		// Round 10
		s0 = table_d0[t0 >> 24] ^ table_d1[(t3 >> 16) & 0xff] ^ table_d2[(t2 >> 8) & 0xff] ^ table_d3[t1 & 0xff] ^ key_expanded_ptr[40];
		s1 = table_d0[t1 >> 24] ^ table_d1[(t0 >> 16) & 0xff] ^ table_d2[(t3 >> 8) & 0xff] ^ table_d3[t2 & 0xff] ^ key_expanded_ptr[41];
		s2 = table_d0[t2 >> 24] ^ table_d1[(t1 >> 16) & 0xff] ^ table_d2[(t0 >> 8) & 0xff] ^ table_d3[t3 & 0xff] ^ key_expanded_ptr[42];
		s3 = table_d0[t3 >> 24] ^ table_d1[(t2 >> 16) & 0xff] ^ table_d2[(t1 >> 8) & 0xff] ^ table_d3[t0 & 0xff] ^ key_expanded_ptr[43];
		// Round 11
		t0 = table_d0[s0 >> 24] ^ table_d1[(s3 >> 16) & 0xff] ^ table_d2[(s2 >> 8) & 0xff] ^ table_d3[s1 & 0xff] ^ key_expanded_ptr[44];
		t1 = table_d0[s1 >> 24] ^ table_d1[(s0 >> 16) & 0xff] ^ table_d2[(s3 >> 8) & 0xff] ^ table_d3[s2 & 0xff] ^ key_expanded_ptr[45];
		t2 = table_d0[s2 >> 24] ^ table_d1[(s1 >> 16) & 0xff] ^ table_d2[(s0 >> 8) & 0xff] ^ table_d3[s3 & 0xff] ^ key_expanded_ptr[46];
		t3 = table_d0[s3 >> 24] ^ table_d1[(s2 >> 16) & 0xff] ^ table_d2[(s1 >> 8) & 0xff] ^ table_d3[s0 & 0xff] ^ key_expanded_ptr[47];

		key_expanded_ptr += aes192_num_rounds_nr * 4;

		// Apply last round
		s0 = (sbox_inverse_substitution_values[(t0 >> 24)] & 0xff000000) ^ (sbox_inverse_substitution_values[(t3 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_inverse_substitution_values[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_inverse_substitution_values[(t1)& 0xff] & 0x000000ff) ^ key_expanded_ptr[0];
		s1 = (sbox_inverse_substitution_values[(t1 >> 24)] & 0xff000000) ^ (sbox_inverse_substitution_values[(t0 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_inverse_substitution_values[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_inverse_substitution_values[(t2)& 0xff] & 0x000000ff) ^ key_expanded_ptr[1];
		s2 = (sbox_inverse_substitution_values[(t2 >> 24)] & 0xff000000) ^ (sbox_inverse_substitution_values[(t1 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_inverse_substitution_values[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_inverse_substitution_values[(t3)& 0xff] & 0x000000ff) ^ key_expanded_ptr[2];
		s3 = (sbox_inverse_substitution_values[(t3 >> 24)] & 0xff000000) ^ (sbox_inverse_substitution_values[(t2 >> 16) & 0xff] & 0x00ff0000) ^ (sbox_inverse_substitution_values[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (sbox_inverse_substitution_values[(t0)& 0xff] & 0x000000ff) ^ key_expanded_ptr[3];

		s0 ^= initialisation_vector_1;
		s1 ^= initialisation_vector_2;
		s2 ^= initialisation_vector_3;
		s3 ^= initialisation_vector_4;
		store_block(s0, s1, s2, s3, databuffer);

		initialisation_vector_1 = chunk1;
		initialisation_vector_2 = chunk2;
		initialisation_vector_3 = chunk3;
		initialisation_vector_4 = chunk4;
	}
}
