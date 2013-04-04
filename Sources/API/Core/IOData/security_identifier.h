/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include "../System/databuffer.h"

namespace clan
{

/// \brief Security identifier.
class CL_API_CORE SecurityIdentifier
{
/// \name Construction
/// \{

public:
	/// \brief Security identifier types.
	enum Type
	{
		type_user,
		type_group,
		type_domain,
		type_alias,
		type_well_known_group,
		type_deleted_account,
		type_invalid,
		type_unknown,
		type_computer
	};

	/// \brief Constructs a SecurityIdentifier
	///
	/// \param name = String
	SecurityIdentifier(const std::string &name);

	/// \brief Constructs a SecurityIdentifier
	///
	/// \param name = String
	/// \param type = Type
	SecurityIdentifier(const std::string &name, Type type);

#ifdef WIN32

	/// \brief Constructs a SecurityIdentifier
	///
	/// \param sid = PSID
	SecurityIdentifier(const PSID sid);
#else

	/// \brief Constructs a SecurityIdentifier
	///
	/// \param sid = value
	/// \param type = Type
	SecurityIdentifier(unsigned int sid, Type type);
#endif

	/// \brief Constructs a SecurityIdentifier
	///
	/// \param copy = Security Identifier
	SecurityIdentifier(const SecurityIdentifier &copy);

	~SecurityIdentifier();

	/// \brief Get Thread user
	///
	/// \return thread_user
	static SecurityIdentifier get_thread_user();

	/// \brief Get Thread group
	///
	/// \return thread_group
	static SecurityIdentifier get_thread_group();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Get friendly name of security identifier.
	std::string get_name() const;

	/// \brief Get domain name identifier belongs to.
	std::string get_domain_name() const;

	/// \brief Get security identifier type.
	Type get_type() const;

/// \}
/// \name Operations
/// \{

public:
	SecurityIdentifier &operator =(const SecurityIdentifier &copy);

/// \}
/// \name Implementation
/// \{

private:
#ifdef WIN32
	DataBuffer sid;
#else
	unsigned int sid;
#endif

	Type type;
/// \}
};

}

/// \}
