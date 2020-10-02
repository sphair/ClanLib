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
#include "API/Core/Crypto/tls_client.h"
#include "tls_client_impl.h"

namespace clan
{
	TLSClient::TLSClient()
		: impl(std::make_shared<TLSClient_Impl>())
	{
	}

	const void *TLSClient::get_decrypted_data() const
	{
		return impl->get_decrypted_data();
	}

	int TLSClient::get_decrypted_data_available() const
	{
		return impl->get_decrypted_data_available();
	}

	const void *TLSClient::get_encrypted_data() const
	{
		return impl->get_encrypted_data();
	}

	int TLSClient::get_encrypted_data_available() const
	{
		return impl->get_encrypted_data_available();
	}

	int TLSClient::encrypt(const void *data, int size)
	{
		return impl->encrypt(data, size);
	}

	int TLSClient::decrypt(const void *data, int size)
	{
		return impl->decrypt(data, size);
	}

	void TLSClient::decrypted_data_consumed(int size)
	{
		impl->decrypted_data_consumed(size);
	}

	void TLSClient::encrypted_data_consumed(int size)
	{
		impl->encrypted_data_consumed(size);
	}
}
