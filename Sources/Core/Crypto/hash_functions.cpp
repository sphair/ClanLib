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

#include "Core/precomp.h"
#include "API/Core/Crypto/hash_functions.h"
#include "API/Core/System/databuffer.h"
#include "Core/Zip/miniz.h"

namespace clan
{
	uint32_t HashFunctions::crc32(const void *data, int size, uint32_t running_crc/*=0*/)
	{
		uint32_t crc = running_crc;
		if (crc == 0)
			crc = mz_crc32(0L, nullptr, 0);

		return mz_crc32(running_crc, (const unsigned char*)data, size);;
	}

	uint32_t HashFunctions::adler32(const void *data, int size, uint32_t running_adler32/*=0*/)
	{
		uint32_t adler = running_adler32;
		if (adler == 0)
			adler = mz_adler32(0L, nullptr, 0);

		return mz_adler32(adler, (const unsigned char*)data, size);
	}

	std::string HashFunctions::md5(const void *data, int size, bool uppercase)
	{
		SHA1 md5;
		md5.add(data, size);
		md5.calculate();
		return md5.get_hash(uppercase);
	}

	std::string HashFunctions::md5(const std::string &data, bool uppercase)
	{
		return md5(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::md5(const DataBuffer &data, bool uppercase)
	{
		return md5(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::md5(const void *data, int size, unsigned char out_hash[16])
	{
		SHA1 md5;
		md5.add(data, size);
		md5.calculate();
		md5.get_hash(out_hash);
	}

	void HashFunctions::md5(const DataBuffer &data, unsigned char out_hash[16])
	{
		md5(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::md5(const std::string &data, unsigned char out_hash[16])
	{
		md5(data.data(), data.length(), out_hash);
	}


	std::string HashFunctions::sha1(const void *data, int size, bool uppercase)
	{
		SHA1 sha1;
		sha1.add(data, size);
		sha1.calculate();
		return sha1.get_hash(uppercase);
	}

	std::string HashFunctions::sha1(const std::string &data, bool uppercase)
	{
		return sha1(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha1(const DataBuffer &data, bool uppercase)
	{
		return sha1(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha1(const void *data, int size, unsigned char out_hash[20])
	{
		SHA1 sha1;
		sha1.add(data, size);
		sha1.calculate();
		sha1.get_hash(out_hash);
	}

	void HashFunctions::sha1(const DataBuffer &data, unsigned char out_hash[20])
	{
		sha1(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha1(const std::string &data, unsigned char out_hash[20])
	{
		sha1(data.data(), data.length(), out_hash);
	}

	std::string HashFunctions::sha224(const void *data, int size, bool uppercase)
	{
		SHA224 sha224;
		sha224.add(data, size);
		sha224.calculate();
		return sha224.get_hash(uppercase);
	}

	std::string HashFunctions::sha224(const std::string &data, bool uppercase)
	{
		return sha224(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha224(const DataBuffer &data, bool uppercase)
	{
		return sha224(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha224(const void *data, int size, unsigned char out_hash[28])
	{
		SHA224 sha224;
		sha224.add(data, size);
		sha224.calculate();
		sha224.get_hash(out_hash);
	}

	void HashFunctions::sha224(const DataBuffer &data, unsigned char out_hash[28])
	{
		sha224(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha224(const std::string &data, unsigned char out_hash[28])
	{
		sha224(data.data(), data.length(), out_hash);
	}

	std::string HashFunctions::sha256(const void *data, int size, bool uppercase)
	{
		SHA256 sha256;
		sha256.add(data, size);
		sha256.calculate();
		return sha256.get_hash(uppercase);
	}

	std::string HashFunctions::sha256(const std::string &data, bool uppercase)
	{
		return sha256(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha256(const DataBuffer &data, bool uppercase)
	{
		return sha256(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha256(const void *data, int size, unsigned char out_hash[32])
	{
		SHA256 sha256;
		sha256.add(data, size);
		sha256.calculate();
		sha256.get_hash(out_hash);
	}

	void HashFunctions::sha256(const DataBuffer &data, unsigned char out_hash[32])
	{
		sha256(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha256(const std::string &data, unsigned char out_hash[32])
	{
		sha256(data.data(), data.length(), out_hash);
	}


	std::string HashFunctions::sha384(const void *data, int size, bool uppercase)
	{
		SHA384 sha384;
		sha384.add(data, size);
		sha384.calculate();
		return sha384.get_hash(uppercase);
	}

	std::string HashFunctions::sha384(const std::string &data, bool uppercase)
	{
		return sha384(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha384(const DataBuffer &data, bool uppercase)
	{
		return sha384(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha384(const void *data, int size, unsigned char out_hash[48])
	{
		SHA384 sha384;
		sha384.add(data, size);
		sha384.calculate();
		sha384.get_hash(out_hash);
	}

	void HashFunctions::sha384(const DataBuffer &data, unsigned char out_hash[48])
	{
		sha384(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha384(const std::string &data, unsigned char out_hash[48])
	{
		sha384(data.data(), data.length(), out_hash);
	}

	std::string HashFunctions::sha512(const void *data, int size, bool uppercase)
	{
		SHA512 sha512;
		sha512.add(data, size);
		sha512.calculate();
		return sha512.get_hash(uppercase);
	}

	std::string HashFunctions::sha512(const std::string &data, bool uppercase)
	{
		return sha512(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha512(const DataBuffer &data, bool uppercase)
	{
		return sha512(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha512(const void *data, int size, unsigned char out_hash[64])
	{
		SHA512 sha512;
		sha512.add(data, size);
		sha512.calculate();
		sha512.get_hash(out_hash);
	}

	void HashFunctions::sha512(const DataBuffer &data, unsigned char out_hash[64])
	{
		sha512(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha512(const std::string &data, unsigned char out_hash[64])
	{
		sha512(data.data(), data.length(), out_hash);
	}

	std::string HashFunctions::sha512_224(const void *data, int size, bool uppercase)
	{
		SHA512_224 sha512_224;
		sha512_224.add(data, size);
		sha512_224.calculate();
		return sha512_224.get_hash(uppercase);
	}

	std::string HashFunctions::sha512_224(const std::string &data, bool uppercase)
	{
		return sha512_224(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha512_224(const DataBuffer &data, bool uppercase)
	{
		return sha512_224(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha512_224(const void *data, int size, unsigned char out_hash[28])
	{
		SHA512_224 sha512_224;
		sha512_224.add(data, size);
		sha512_224.calculate();
		sha512_224.get_hash(out_hash);
	}

	void HashFunctions::sha512_224(const DataBuffer &data, unsigned char out_hash[28])
	{
		sha512_224(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha512_224(const std::string &data, unsigned char out_hash[28])
	{
		sha512_224(data.data(), data.length(), out_hash);
	}

	std::string HashFunctions::sha512_256(const void *data, int size, bool uppercase)
	{
		SHA512_256 sha512_256;
		sha512_256.add(data, size);
		sha512_256.calculate();
		return sha512_256.get_hash(uppercase);
	}

	std::string HashFunctions::sha512_256(const std::string &data, bool uppercase)
	{
		return sha512_256(data.data(), data.length(), uppercase);
	}

	std::string HashFunctions::sha512_256(const DataBuffer &data, bool uppercase)
	{
		return sha512_256(data.get_data(), data.get_size(), uppercase);
	}

	void HashFunctions::sha512_256(const void *data, int size, unsigned char out_hash[32])
	{
		SHA512_256 sha512_256;
		sha512_256.add(data, size);
		sha512_256.calculate();
		sha512_256.get_hash(out_hash);
	}

	void HashFunctions::sha512_256(const DataBuffer &data, unsigned char out_hash[32])
	{
		sha512_256(data.get_data(), data.get_size(), out_hash);
	}

	void HashFunctions::sha512_256(const std::string &data, unsigned char out_hash[32])
	{
		sha512_256(data.data(), data.length(), out_hash);
	}
}
