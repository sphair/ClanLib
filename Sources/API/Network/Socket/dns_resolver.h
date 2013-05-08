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

/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{

#pragma once

#include "../api_network.h"
#include <memory>
#include <vector>

namespace clan
{

class DNSResourceRecord;
class DNSPacket;
class DNSResolver_Impl;

/// \brief DNS resolver.
class CL_API_NETWORK DNSResolver
{
/// \name Construction
/// \{

public:
	DNSResolver();

	~DNSResolver();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	std::vector<DNSResourceRecord> lookup_resource(
		const std::string &domain_name,
		const std::string &resource_type,
		int timeout);

	DNSPacket perform_query(
		DNSPacket &packet,
		int timeout,
		const std::string &dns_server_name);

	DNSPacket perform_query(
		const std::string &domain_name,
		const std::string &resource_type,
		int timeout,
		const std::string &dns_server_name);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<DNSResolver_Impl> impl;
/// \}
};

}

/// \}
