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
*/

#pragma once

/// \addtogroup clanCrypto_System clanCrypto System
/// \{

#include <prio.h>
#include <certt.h>
#include <keyt.h>

class CL_Certificate;
class CL_PrivateKey;

/// \brief PK11 password handler
///
/// \xmlonly !group=Crypto/System! !header=crypto.h! \endxmlonly
class PK11PasswordHandler
{
public:
	virtual ~PK11PasswordHandler() { return; }

	/// \brief Get password
	///
	/// \param slot = PK11 Slot Info
	/// \param retry = bool
	/// \param out_password = String
	///
	/// \return bool
	virtual bool get_password(PK11SlotInfo *slot, bool retry, CL_String &out_password)=0;
};

/// \brief PK11 password handler
///
/// \xmlonly !group=Crypto/System! !header=crypto.h! \endxmlonly
class PK11PasswordHandler_Password : public PK11PasswordHandler
{
public:
	PK11PasswordHandler_Password(const CL_String &password) : password(password) { return; }

	/// \brief Get password
	///
	/// \param slot = PK11 Slot Info
	/// \param retry = bool
	/// \param out_password = String
	///
	/// \return bool
	virtual bool get_password(PK11SlotInfo *slot, bool retry, CL_String &out_password)
	{
		if (retry)
			return false;
		out_password = password;
		return true;
	}
	CL_String password;
};

/// \brief Crypto library setup
///
/// \xmlonly !group=Crypto/System! !header=crypto.h! \endxmlonly
class CL_CryptoSetup
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a CryptoSetup
	///
	/// \param config_dir = String
	/// \param read_write = bool
	CL_CryptoSetup(const CL_String &config_dir, bool read_write = false);

	~CL_CryptoSetup();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	void config_server_sid_cache(
		int max_cache_entries = 256,
		unsigned int timeout = 0,
		unsigned int ssl3_timeout = 0,
		const CL_String &directory = CL_String());

	CL_Certificate find_cert_from_nickname(
		const CL_String &nickname,
		PK11PasswordHandler *pw_handler);

	CL_Certificate find_cert_from_nickname(
		const CL_String &nickname,
		const CL_String &password = CL_String());

	CL_PrivateKey find_key_by_any_cert(
		const CL_Certificate &cert,
		PK11PasswordHandler *pw_handler);

/// \}
/// \name Implementation
/// \{

private:
	static CL_CryptoSetup *instance;

	/// \brief Pk11 password func
	///
	/// \param slot = PK11 Slot Info
	/// \param retry = PRBool
	/// \param arg = void
	///
	/// \return char
	static char *pk11_password_func(PK11SlotInfo *slot, PRBool retry, void *arg);
/// \}
};

/// \}
