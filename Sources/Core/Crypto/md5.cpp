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
#include "API/Core/Crypto/md5.h"
#include "API/Core/System/databuffer.h"
#include "md5_impl.h"

namespace clan
{
	MD5::MD5()
		: impl(std::make_shared<MD5_Impl>())
	{
	}

	std::string MD5::get_hash(bool uppercase) const
	{
		return impl->get_hash(uppercase);
	}

	void MD5::get_hash(unsigned char out_hash[16]) const
	{
		impl->get_hash(out_hash);
	}

	void MD5::reset()
	{
		impl->reset();
	}

	void MD5::add(const void *data, int size)
	{
		impl->add(data, size);
	}

	void MD5::add(const DataBuffer &data)
	{
		add(data.get_data(), data.get_size());
	}

	void MD5::calculate()
	{
		impl->calculate();
	}

	void MD5::set_hmac(const void *key_data, int key_size)
	{
		impl->set_hmac(key_data, key_size);
	}
}
