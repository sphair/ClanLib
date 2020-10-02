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
#include "API/Core/Crypto/random.h"
#include "API/Core/System/databuffer.h"
#include "random_impl.h"

namespace clan
{
	Random::Random(int cache_size) : impl(std::make_shared<Random_Impl>(cache_size))
	{
	}

	void Random::get_random_bytes(unsigned char *out_dest_ptr, int num_bytes)
	{
		impl->get_random_bytes(out_dest_ptr, num_bytes);
	}

	void Random::get_random_bytes_nzero(unsigned char *out_dest_ptr, int num_bytes)
	{
		impl->get_random_bytes_nzero(out_dest_ptr, num_bytes);
	}

	bool Random::get_random_bool()
	{
		return impl->get_random_bool();
	}
}
