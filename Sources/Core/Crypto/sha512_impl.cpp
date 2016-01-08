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
#include "sha512_impl.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Crypto/sha384.h"
#include "API/Core/Crypto/sha512.h"
#include "API/Core/Crypto/sha512_224.h"
#include "API/Core/Crypto/sha512_256.h"

#ifndef WIN32
#include <cstring>
#endif

namespace clan
{
	SHA512_Impl::SHA512_Impl(cl_sha_type new_sha_type) : sha_type(new_sha_type)
	{
		reset();
	}

	std::string SHA512_Impl::get_hash(bool uppercase) const
	{
		if (calculated == false)
			throw Exception("SHA-512 hash has not been calculated yet!");

		char digest[128 + 1];
		to_hex64_be(digest, h0, uppercase);
		to_hex64_be(digest + 16, h1, uppercase);
		to_hex64_be(digest + 32, h2, uppercase);

		if (sha_type == cl_sha_512_224)
		{
			// upper 4 bytes of h3
			to_hex_be(digest + 48, (uint32_t)(h3 >> 32), uppercase);
			digest[56] = 0;
		}
		else
		{
			to_hex64_be(digest + 48, h3, uppercase);
			to_hex64_be(digest + 64, h4, uppercase);

			if (sha_type == cl_sha_512_256)
			{
				digest[64] = 0;
			}
			else
			{
				to_hex64_be(digest + 80, h5, uppercase);

				if (sha_type == cl_sha_384)
				{
					digest[96] = 0;
				}
				else
				{
					// Must be cl_sha_512
					to_hex64_be(digest + 96, h6, uppercase);
					to_hex64_be(digest + 112, h7, uppercase);
					digest[128] = 0;
				}
			}
		}
		return digest;
	}

	void SHA512_Impl::get_hash(unsigned char *out_hash) const
	{
		if (calculated == false)
			throw Exception("SHA-512 hash has not been calculated yet!");

		out_hash[0] = (unsigned char)((h0 >> 56) & 0xff);
		out_hash[1] = (unsigned char)((h0 >> 48) & 0xff);
		out_hash[2] = (unsigned char)((h0 >> 40) & 0xff);
		out_hash[3] = (unsigned char)((h0 >> 32) & 0xff);
		out_hash[4] = (unsigned char)((h0 >> 24) & 0xff);
		out_hash[5] = (unsigned char)((h0 >> 16) & 0xff);
		out_hash[6] = (unsigned char)((h0 >> 8) & 0xff);
		out_hash[7] = (unsigned char)(h0 & 0xff);
		out_hash[8] = (unsigned char)((h1 >> 56) & 0xff);
		out_hash[9] = (unsigned char)((h1 >> 48) & 0xff);
		out_hash[10] = (unsigned char)((h1 >> 40) & 0xff);
		out_hash[11] = (unsigned char)((h1 >> 32) & 0xff);
		out_hash[12] = (unsigned char)((h1 >> 24) & 0xff);
		out_hash[13] = (unsigned char)((h1 >> 16) & 0xff);
		out_hash[14] = (unsigned char)((h1 >> 8) & 0xff);
		out_hash[15] = (unsigned char)(h1 & 0xff);
		out_hash[16] = (unsigned char)((h2 >> 56) & 0xff);
		out_hash[17] = (unsigned char)((h2 >> 48) & 0xff);
		out_hash[18] = (unsigned char)((h2 >> 40) & 0xff);
		out_hash[19] = (unsigned char)((h2 >> 32) & 0xff);
		out_hash[20] = (unsigned char)((h2 >> 24) & 0xff);
		out_hash[21] = (unsigned char)((h2 >> 16) & 0xff);
		out_hash[22] = (unsigned char)((h2 >> 8) & 0xff);
		out_hash[23] = (unsigned char)(h2 & 0xff);
		out_hash[24] = (unsigned char)((h3 >> 56) & 0xff);
		out_hash[25] = (unsigned char)((h3 >> 48) & 0xff);
		out_hash[26] = (unsigned char)((h3 >> 40) & 0xff);
		out_hash[27] = (unsigned char)((h3 >> 32) & 0xff);

		if (sha_type != cl_sha_512_224)
		{
			out_hash[28] = (unsigned char)((h3 >> 24) & 0xff);
			out_hash[29] = (unsigned char)((h3 >> 16) & 0xff);
			out_hash[30] = (unsigned char)((h3 >> 8) & 0xff);
			out_hash[31] = (unsigned char)(h3 & 0xff);

			if (sha_type != cl_sha_512_256)
			{
				out_hash[32] = (unsigned char)((h4 >> 56) & 0xff);
				out_hash[33] = (unsigned char)((h4 >> 48) & 0xff);
				out_hash[34] = (unsigned char)((h4 >> 40) & 0xff);
				out_hash[35] = (unsigned char)((h4 >> 32) & 0xff);
				out_hash[36] = (unsigned char)((h4 >> 24) & 0xff);
				out_hash[37] = (unsigned char)((h4 >> 16) & 0xff);
				out_hash[38] = (unsigned char)((h4 >> 8) & 0xff);
				out_hash[39] = (unsigned char)(h4 & 0xff);
				out_hash[40] = (unsigned char)((h5 >> 56) & 0xff);
				out_hash[41] = (unsigned char)((h5 >> 48) & 0xff);
				out_hash[42] = (unsigned char)((h5 >> 40) & 0xff);
				out_hash[43] = (unsigned char)((h5 >> 32) & 0xff);
				out_hash[44] = (unsigned char)((h5 >> 24) & 0xff);
				out_hash[45] = (unsigned char)((h5 >> 16) & 0xff);
				out_hash[46] = (unsigned char)((h5 >> 8) & 0xff);
				out_hash[47] = (unsigned char)(h5 & 0xff);

				if (sha_type != cl_sha_384)
				{
					// Must be cl_sha_512
					out_hash[48] = (unsigned char)((h6 >> 56) & 0xff);
					out_hash[49] = (unsigned char)((h6 >> 48) & 0xff);
					out_hash[50] = (unsigned char)((h6 >> 40) & 0xff);
					out_hash[51] = (unsigned char)((h6 >> 32) & 0xff);
					out_hash[52] = (unsigned char)((h6 >> 24) & 0xff);
					out_hash[53] = (unsigned char)((h6 >> 16) & 0xff);
					out_hash[54] = (unsigned char)((h6 >> 8) & 0xff);
					out_hash[55] = (unsigned char)(h6 & 0xff);
					out_hash[56] = (unsigned char)((h7 >> 56) & 0xff);
					out_hash[57] = (unsigned char)((h7 >> 48) & 0xff);
					out_hash[58] = (unsigned char)((h7 >> 40) & 0xff);
					out_hash[59] = (unsigned char)((h7 >> 32) & 0xff);
					out_hash[60] = (unsigned char)((h7 >> 24) & 0xff);
					out_hash[61] = (unsigned char)((h7 >> 16) & 0xff);
					out_hash[62] = (unsigned char)((h7 >> 8) & 0xff);
					out_hash[63] = (unsigned char)(h7 & 0xff);
				}
			}
		}
	}

	void SHA512_Impl::reset()
	{

		if (sha_type == cl_sha_384)
		{
			h0 = 0xcbbb9d5dc1059ed8ll;
			h1 = 0x629a292a367cd507ll;
			h2 = 0x9159015a3070dd17ll;
			h3 = 0x152fecd8f70e5939ll;
			h4 = 0x67332667ffc00b31ll;
			h5 = 0x8eb44a8768581511ll;
			h6 = 0xdb0c2e0d64f98fa7ll;
			h7 = 0x47b5481dbefa4fa4ll;
		}
		else
			if (sha_type == cl_sha_512)
			{
				h0 = 0x6a09e667f3bcc908ll;
				h1 = 0xbb67ae8584caa73bll;
				h2 = 0x3c6ef372fe94f82bll;
				h3 = 0xa54ff53a5f1d36f1ll;
				h4 = 0x510e527fade682d1ll;
				h5 = 0x9b05688c2b3e6c1fll;
				h6 = 0x1f83d9abfb41bd6bll;
				h7 = 0x5be0cd19137e2179ll;
			}
			else
				if (sha_type == cl_sha_512_224)
				{
					h0 = 0x8C3D37C819544DA2ll;
					h1 = 0x73E1996689DCD4D6ll;
					h2 = 0x1DFAB7AE32FF9C82ll;
					h3 = 0x679DD514582F9FCFll;
					h4 = 0x0F6D2B697BD44DA8ll;
					h5 = 0x77E36F7304C48942ll;
					h6 = 0x3F9D85A86A1D36C8ll;
					h7 = 0x1112E6AD91D692A1ll;
				}
				else
					if (sha_type == cl_sha_512_256)
					{
						h0 = 0x22312194FC2BF72Cll;
						h1 = 0x9F555FA3C84C64C2ll;
						h2 = 0x2393B86B6F53B151ll;
						h3 = 0x963877195940EABDll;
						h4 = 0x96283EE2A88EFFE3ll;
						h5 = 0xBE5E1E2553863992ll;
						h6 = 0x2B0199FC2C85B8AAll;
						h7 = 0x0EB72DDC81C52CA2ll;
					}
					else
					{
						// Note we can support SHA512_T, but there is no requirement at the moment since it's not an approved SHA hash
						throw Exception("SHA512_Impl, unsupported sha type for 512 bits");
					}

		memset(chunk, 0, block_size);
		chunk_filled = 0;
		length_message = 0;
		calculated = false;
		hmac_enabled = false;

	}

	void SHA512_Impl::add(const void *_data, int size)
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
		length_message = length_message + (size * (uint64_t)8);
	}

	void SHA512_Impl::set_hmac(const void *key_data, int key_size)
	{
		memset(hmac_key_chunk, 0, block_size);

		int key_chunk_filled;

		if (key_size > block_size)
		{
			if (sha_type == cl_sha_384)
			{
				SHA384 sha384;
				sha384.add(key_data, key_size);
				sha384.calculate();
				key_chunk_filled = SHA384::hash_size;
				sha384.get_hash(hmac_key_chunk);
			}
			else if (sha_type == cl_sha_512)
			{
				SHA512 sha512;
				sha512.add(key_data, key_size);
				sha512.calculate();
				key_chunk_filled = SHA512::hash_size;
				sha512.get_hash(hmac_key_chunk);
			}
			else if (sha_type == cl_sha_512_224)
			{
				SHA512_224 sha512_224;
				sha512_224.add(key_data, key_size);
				sha512_224.calculate();
				key_chunk_filled = SHA512_224::hash_size;
				sha512_224.get_hash(hmac_key_chunk);
			}
			else if (sha_type == cl_sha_512_256)
			{
				SHA512_256 sha512_256;
				sha512_256.add(key_data, key_size);
				sha512_256.calculate();
				key_chunk_filled = SHA512_256::hash_size;
				sha512_256.get_hash(hmac_key_chunk);
			}
			else
			{
				throw Exception("SHA512_Impl, unsupported sha type for hmac");
			}

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

	void SHA512_Impl::calculate()
	{
		if (calculated)
			reset();

		// append a single "1" bit to message
		// append "0" bits until message length ≡ 896 ≡ -128 (mod 1024)
		// append length of message, in bits as 64-bit big-endian integer to message

		unsigned char end_data[block_size * 2];
		memset(end_data, 0, block_size * 2);
		end_data[0] = 128;

		int size = block_size - chunk_filled;
		if (size < 17)
			size += block_size;

		uint64_t length_upper = length_message.high;
		uint64_t length_lower = length_message.low;

		end_data[size - 16] = (length_upper & 0xff00000000000000ll) >> 56;
		end_data[size - 15] = (length_upper & 0x00ff000000000000ll) >> 48;
		end_data[size - 14] = (length_upper & 0x0000ff0000000000ll) >> 40;
		end_data[size - 13] = (length_upper & 0x000000ff00000000ll) >> 32;
		end_data[size - 12] = (length_upper & 0x00000000ff000000ll) >> 24;
		end_data[size - 11] = (length_upper & 0x0000000000ff0000ll) >> 16;
		end_data[size - 10] = (length_upper & 0x000000000000ff00ll) >> 8;
		end_data[size - 9] = (length_upper & 0x00000000000000ffll);
		end_data[size - 8] = (length_lower & 0xff00000000000000ll) >> 56;
		end_data[size - 7] = (length_lower & 0x00ff000000000000ll) >> 48;
		end_data[size - 6] = (length_lower & 0x0000ff0000000000ll) >> 40;
		end_data[size - 5] = (length_lower & 0x000000ff00000000ll) >> 32;
		end_data[size - 4] = (length_lower & 0x00000000ff000000ll) >> 24;
		end_data[size - 3] = (length_lower & 0x0000000000ff0000ll) >> 16;
		end_data[size - 2] = (length_lower & 0x000000000000ff00ll) >> 8;
		end_data[size - 1] = (length_lower & 0x00000000000000ffll);

		add(end_data, size);

		if (chunk_filled != 0)
			throw Exception("Error in SHA512_Impl class. Still chunk data at end of calculate");

		calculated = true;

		if (hmac_enabled)
		{
			int hash_size;
			if (sha_type == cl_sha_384)
			{
				hash_size = SHA384::hash_size;
			}
			else if (sha_type == cl_sha_512)
			{
				hash_size = SHA512::hash_size;
			}
			else if (sha_type == cl_sha_512_224)
			{
				hash_size = SHA512_224::hash_size;
			}
			else if (sha_type == cl_sha_512_256)
			{
				hash_size = SHA512_256::hash_size;
			}
			else
			{
				throw Exception("SHA512_Impl, unsupported sha type for hmac");
			}

			unsigned char temp_hash[SHA512::hash_size];
			get_hash(temp_hash);
			reset();
			add(hmac_key_chunk, block_size);	// Add the outer HMAC
			add(temp_hash, hash_size);
			calculate();
		}
	}

	void SHA512_Impl::process_chunk()
	{
		// Constants defined in FIPS 180-3, section 4.2.3
		static const uint64_t constant_K[80] = {
			0x428A2F98D728AE22ull, 0x7137449123EF65CDull, 0xB5C0FBCFEC4D3B2Full,
			0xE9B5DBA58189DBBCull, 0x3956C25BF348B538ull, 0x59F111F1B605D019ull,
			0x923F82A4AF194F9Bull, 0xAB1C5ED5DA6D8118ull, 0xD807AA98A3030242ull,
			0x12835B0145706FBEull, 0x243185BE4EE4B28Cull, 0x550C7DC3D5FFB4E2ull,
			0x72BE5D74F27B896Full, 0x80DEB1FE3B1696B1ull, 0x9BDC06A725C71235ull,
			0xC19BF174CF692694ull, 0xE49B69C19EF14AD2ull, 0xEFBE4786384F25E3ull,
			0x0FC19DC68B8CD5B5ull, 0x240CA1CC77AC9C65ull, 0x2DE92C6F592B0275ull,
			0x4A7484AA6EA6E483ull, 0x5CB0A9DCBD41FBD4ull, 0x76F988DA831153B5ull,
			0x983E5152EE66DFABull, 0xA831C66D2DB43210ull, 0xB00327C898FB213Full,
			0xBF597FC7BEEF0EE4ull, 0xC6E00BF33DA88FC2ull, 0xD5A79147930AA725ull,
			0x06CA6351E003826Full, 0x142929670A0E6E70ull, 0x27B70A8546D22FFCull,
			0x2E1B21385C26C926ull, 0x4D2C6DFC5AC42AEDull, 0x53380D139D95B3DFull,
			0x650A73548BAF63DEull, 0x766A0ABB3C77B2A8ull, 0x81C2C92E47EDAEE6ull,
			0x92722C851482353Bull, 0xA2BFE8A14CF10364ull, 0xA81A664BBC423001ull,
			0xC24B8B70D0F89791ull, 0xC76C51A30654BE30ull, 0xD192E819D6EF5218ull,
			0xD69906245565A910ull, 0xF40E35855771202Aull, 0x106AA07032BBD1B8ull,
			0x19A4C116B8D2D0C8ull, 0x1E376C085141AB53ull, 0x2748774CDF8EEB99ull,
			0x34B0BCB5E19B48A8ull, 0x391C0CB3C5C95A63ull, 0x4ED8AA4AE3418ACBull,
			0x5B9CCA4F7763E373ull, 0x682E6FF3D6B2B8A3ull, 0x748F82EE5DEFB2FCull,
			0x78A5636F43172F60ull, 0x84C87814A1F0AB72ull, 0x8CC702081A6439ECull,
			0x90BEFFFA23631E28ull, 0xA4506CEBDE82BDE9ull, 0xBEF9A3F7B2C67915ull,
			0xC67178F2E372532Bull, 0xCA273ECEEA26619Cull, 0xD186B8C721C0C207ull,
			0xEADA7DD6CDE0EB1Eull, 0xF57D4F7FEE6ED178ull, 0x06F067AA72176FBAull,
			0x0A637DC5A2C898A6ull, 0x113F9804BEF90DAEull, 0x1B710B35131C471Bull,
			0x28DB77F523047D84ull, 0x32CAAB7B40C72493ull, 0x3C9EBE0A15C9BEBCull,
			0x431D67C49C100D4Cull, 0x4CC5D4BECB3E42B6ull, 0x597F299CFC657E2Aull,
			0x5FCB6FAB3AD6FAECull, 0x6C44198C4A475817ull

		};

		int i;
		uint64_t w[80];

		for (i = 0; i < 16; i++)
		{
			uint64_t b1 = chunk[i * 8];
			uint64_t b2 = chunk[i * 8 + 1];
			uint64_t b3 = chunk[i * 8 + 2];
			uint64_t b4 = chunk[i * 8 + 3];
			uint64_t b5 = chunk[i * 8 + 4];
			uint64_t b6 = chunk[i * 8 + 5];
			uint64_t b7 = chunk[i * 8 + 6];
			uint64_t b8 = chunk[i * 8 + 7];
			w[i] = (b1 << 56) + (b2 << 48) + (b3 << 40) + (b4 << 32) + (b5 << 24) + (b6 << 16) + (b7 << 8) + b8;
		}

		for (i = 16; i < 80; i++)
		{
			w[i] = sigma_rr19_rr61_sr6(w[i - 2]) + w[i - 7] + sigma_rr1_rr8_sr7(w[i - 15]) + w[i - 16];
		}

		uint64_t a = h0;
		uint64_t b = h1;
		uint64_t c = h2;
		uint64_t d = h3;
		uint64_t e = h4;
		uint64_t f = h5;
		uint64_t g = h6;
		uint64_t h = h7;

		for (i = 0; i < 80; i++)
		{
			uint64_t t1, t2;

			t1 = h + sigma_rr14_rr18_rr41(e) + sha_ch(e, f, g) + constant_K[i] + w[i];
			t2 = sigma_rr28_rr34_rr39(a) + sha_maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + t1;
			d = c;
			c = b;
			b = a;
			a = t1 + t2;
		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
		h5 += f;
		h6 += g;
		h7 += h;
	}
}
