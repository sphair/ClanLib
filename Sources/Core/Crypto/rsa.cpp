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
#include "API/Core/Crypto/rsa.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Crypto/secret.h"
#include "rsa_impl.h"

namespace clan
{
	void RSA::create_keypair(Random &random, Secret &out_private_exponent, DataBuffer &out_public_exponent, DataBuffer &out_modulus, int key_size_in_bits, int public_exponent_value)
	{
		RSA_Impl rsa_impl;
		rsa_impl.create_keypair(random, out_private_exponent, out_public_exponent, out_modulus, key_size_in_bits, public_exponent_value);
	}

	DataBuffer RSA::encrypt(int block_type, Random &random, const DataBuffer &in_public_exponent, const DataBuffer &in_modulus, const Secret &in_data)
	{
		return RSA_Impl::encrypt(block_type, random, in_public_exponent.get_data(), in_public_exponent.get_size(), in_modulus.get_data(), in_modulus.get_size(), in_data.get_data(), in_data.get_size());
	}

	Secret RSA::decrypt(const Secret &in_private_exponent, const DataBuffer &in_modulus, const DataBuffer &in_data)
	{
		return RSA_Impl::decrypt(in_private_exponent, in_modulus.get_data(), in_modulus.get_size(), in_data.get_data(), in_data.get_size());
	}

	DataBuffer RSA::encrypt(int block_type, Random &random, const void *in_public_exponent, unsigned int in_public_exponent_size, const void *in_modulus, unsigned int in_modulus_size, const void *in_data, unsigned int in_data_size)
	{
		return RSA_Impl::encrypt(block_type, random, in_public_exponent, in_public_exponent_size, in_modulus, in_modulus_size, in_data, in_data_size);
	}

	Secret RSA::decrypt(const Secret &in_private_exponent, const void *in_modulus, unsigned int in_modulus_size, const void *in_data, unsigned int in_data_size)
	{
		return RSA_Impl::decrypt(in_private_exponent, in_modulus, in_modulus_size, in_data, in_data_size);
	}
}
