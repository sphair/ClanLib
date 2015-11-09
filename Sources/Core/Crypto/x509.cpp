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
#include "x509.h"
#include "x509_impl.h"
#include "API/Core/System/databuffer.h"

namespace clan
{
	X509::X509()
	{
	}

	X509::X509(unsigned char *base_ptr, unsigned int length)
		: impl(std::make_shared<X509_Impl>())
	{
		impl->parse(base_ptr, length);
	}

	void X509::throw_if_null() const
	{
		if (!impl)
			throw Exception("X509 is null");
	}

	void X509::get_rsa_public_key(DataBuffer &output_exponent, DataBuffer &output_modulus) const
	{
		throw_if_null();
		impl->get_rsa_public_key(output_exponent, output_modulus);
	}
}
