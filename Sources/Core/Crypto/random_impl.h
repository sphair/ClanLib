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

#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"

namespace clan
{
	class Random_Impl
	{
	public:
		Random_Impl(int cache_size);
		~Random_Impl();

		void get_random_bytes(unsigned char *out_dest_ptr, int num_bytes);
		void get_random_bytes_nzero(unsigned char *out_dest_ptr, int num_bytes);
		bool get_random_bool();

	private:
		void fill_random_pool();

		int random_pool_size;
		int random_pool_free;
		unsigned char *random_pool;
#ifdef WIN32
		HCRYPTPROV hProvider;
#endif
		unsigned char random_bool;
		int random_bool_bits_free;
	};
}
