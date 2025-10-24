/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "security_identifier.h"
#include "access_control_list.h"

/// \brief Security descriptor.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_SecurityDescriptor
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a security descriptor.
	CL_SecurityDescriptor();

	/// \brief Constructs a SecurityDescriptor
	///
	/// \param copy = Security Descriptor
	CL_SecurityDescriptor(const CL_SecurityDescriptor &copy);

	~CL_SecurityDescriptor();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	CL_SecurityDescriptor &operator =(const CL_SecurityDescriptor &copy);

/// \}
/// \name Implementation
/// \{

private:
	CL_SecurityIdentifier owner;

	CL_SecurityIdentifier primary_group;

	CL_AccessControlList discretionary_acl;

	bool owner_defaulted;

	bool group_defaulted;

	bool dacl_defaulted;
/// \}
};

/// \}
