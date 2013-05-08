/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include <memory>
#include "security_identifier.h"

namespace clan
{

class AccessControlList_Impl;

/// \brief Access Control List (ACL).
class CL_API_CORE AccessControlList
{
/// \name Construction
/// \{

public:
	AccessControlList();

	~AccessControlList();

/// \}
/// \name Attributes
/// \{

public:
	// Mask defining type of access applying.
	typedef unsigned int AccessMask;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Add access allow entry.
	void add_access_allowed(const SecurityIdentifier &sid, AccessMask access_mask);

	/// \brief Add access deny entry.
	void add_access_denied(const SecurityIdentifier &sid, AccessMask access_mask);

	/// \brief Add user access allow entry.
	void add_user_access_allowed(const std::string &name, AccessMask access_mask);

	/// \brief Add user access deny entry.
	void add_user_access_denied(const std::string &name, AccessMask access_mask);

	/// \brief Add group access allow entry.
	void add_group_access_allowed(const std::string &name, AccessMask access_mask);

	/// \brief Add user access deny entry.
	void add_group_access_denied(const std::string &name, AccessMask access_mask);

	/// \brief Remove access allow entry.
	void remove_access_allowed(const SecurityIdentifier &sid);

	/// \brief Remove access deny entry.
	void remove_access_denied(const SecurityIdentifier &sid);

	/// \brief Remove user access allow entry.
	void remove_user_allowed(const std::string &name);

	/// \brief Remove user access deny entry.
	void remove_user_denied(const std::string &name);

	/// \brief Remove group access allow entry.
	void remove_group_allowed(const std::string &name);

	/// \brief Remove group access deny entry.
	void remove_group_denied(const std::string &name);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<AccessControlList_Impl> impl;
/// \}
};

}

/// \}
