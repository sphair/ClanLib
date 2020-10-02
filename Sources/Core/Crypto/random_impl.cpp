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
#include "random_impl.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/IOData/file.h"

#ifndef WIN32
#include <cstring>
#endif

namespace clan
{
	Random_Impl::Random_Impl(int cache_size) : random_pool_size(cache_size), random_pool_free(0), random_pool(nullptr), random_bool_bits_free(0)
#ifdef WIN32
		, hProvider(0)
#endif
	{
		random_pool = new unsigned char[random_pool_size];
	}

	Random_Impl::~Random_Impl()
	{
		if (random_pool)
		{
			memset(random_pool, 0, random_pool_size);
			delete[] random_pool;
		}
#ifdef WIN32
		if (hProvider)
			::CryptReleaseContext(hProvider, 0);
#endif
	}

	void Random_Impl::get_random_bytes(unsigned char *out_dest_ptr, int num_bytes)
	{
		while (num_bytes > 0)
		{
			int bytes_to_copy = num_bytes;
			if (bytes_to_copy > random_pool_free)
				bytes_to_copy = random_pool_free;

			if (bytes_to_copy)
			{
				memcpy(out_dest_ptr, random_pool + (random_pool_size - random_pool_free), bytes_to_copy);
				out_dest_ptr += bytes_to_copy;
				random_pool_free -= bytes_to_copy;
				num_bytes -= bytes_to_copy;
			}
			else
			{
				fill_random_pool();
			}
		}
	}

	void Random_Impl::get_random_bytes_nzero(unsigned char *out_dest_ptr, int num_bytes)
	{
		while (num_bytes > 0)
		{
			if (!random_pool_free)
				fill_random_pool();

			unsigned char random_byte = random_pool[random_pool_size - random_pool_free];
			random_pool_free--;
			if (random_byte)	// Only copy non zero bytes
			{
				*(out_dest_ptr++) = random_byte;
				num_bytes--;
			}
		}
	}

	void Random_Impl::fill_random_pool()
	{
		// Note "random_pool_free" should always by zero at this point

#ifdef WIN32
		if (!hProvider)
		{
			if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
			{
				throw Exception("Cannot acquire crypt context");
			}
		}
		if (!::CryptGenRandom(hProvider, random_pool_size, random_pool))
		{
			throw Exception("Cannot generate random numbers");
		}

#else
		File file("/dev/urandom");
		file.read(random_pool, random_pool_size);
		file.close();
#endif

		random_pool_free = random_pool_size;	// Reset it

	}

	bool Random_Impl::get_random_bool()
	{
		if (!random_bool_bits_free)
		{
			get_random_bytes(&random_bool, 1);
			random_bool_bits_free = 8;
		}
		random_bool_bits_free--;

		return ((random_bool >> random_bool_bits_free) & 1);
	}
}
