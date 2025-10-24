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

/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{

#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"
#include <vector>

class CL_DNSResourceRecord;
class CL_DNSPacket;
class CL_DNSResolver_Impl;

/// \brief DNS resolver.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_DNSResolver
{
/// \name Construction
/// \{

public:
	CL_DNSResolver();

	~CL_DNSResolver();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	std::vector<CL_DNSResourceRecord> lookup_resource(
		const CL_String &domain_name,
		const CL_String &resource_type,
		int timeout);

	CL_DNSPacket perform_query(
		CL_DNSPacket &packet,
		int timeout,
		const CL_String &dns_server_name);

	CL_DNSPacket perform_query(
		const CL_String &domain_name,
		const CL_String &resource_type,
		int timeout,
		const CL_String &dns_server_name);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DNSResolver_Impl> impl;
/// \}
};

/// \}
