/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Crypto/aes256_encrypt.h"
#include "API/Core/System/databuffer.h"
#include "aes256_encrypt_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_AES256_Encrypt Construction:

CL_AES256_Encrypt::CL_AES256_Encrypt()
: impl(new CL_AES256_Encrypt_Impl())
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_AES256_Encrypt Attributes:

CL_DataBuffer CL_AES256_Encrypt::get_data() const
{
	return impl->get_data();
}

/////////////////////////////////////////////////////////////////////////////
// CL_AES256_Encrypt Operations:

void CL_AES256_Encrypt::reset()
{
	impl->reset();
}

void CL_AES256_Encrypt::set_iv(const unsigned char iv[16])
{
	impl->set_iv(iv);
}

void CL_AES256_Encrypt::set_key(const unsigned char key[32])
{
	impl->set_key(key);
}

void CL_AES256_Encrypt::set_padding(bool value, bool use_pkcs7, unsigned int num_additional_padded_blocks)
{
	impl->set_padding(value, use_pkcs7, num_additional_padded_blocks);
}

void CL_AES256_Encrypt::add(const void *data, int size)
{
	impl->add(data, size);
}

void CL_AES256_Encrypt::add(const CL_DataBuffer &data)
{
	add(data.get_data(), data.get_size());
}

void CL_AES256_Encrypt::calculate()
{
	impl->calculate();
}

/////////////////////////////////////////////////////////////////////////////
// CL_AES256_Encrypt Implementation:
