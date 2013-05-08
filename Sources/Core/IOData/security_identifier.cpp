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

#include "Core/precomp.h"
#include "API/Core/IOData/security_identifier.h"
#include "API/Core/System/exception.h"
#ifdef WIN32
#include <AclAPI.h>
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SecurityIdentifier Construction:

SecurityIdentifier::SecurityIdentifier(const std::string &name)
: type(type_invalid)
{
	throw Exception("SecurityIdentifier(const std::string &name) not implemented");
}
	
SecurityIdentifier::SecurityIdentifier(const std::string &name, Type type)
: type(type)
{
	throw Exception("SecurityIdentifier(const std::string &name) not implemented");
}

#ifdef WIN32
SecurityIdentifier::SecurityIdentifier(const PSID new_sid)
: type(type_unknown)
{
	sid.set_size(GetLengthSid(new_sid));
	memcpy(sid.get_data(), new_sid, sid.get_size());
}
#else
SecurityIdentifier::SecurityIdentifier(unsigned int sid, Type type)
: sid(sid), type(type)
{
}
#endif

SecurityIdentifier::SecurityIdentifier(const SecurityIdentifier &copy)
: sid(copy.sid), type(copy.type)
{
}

SecurityIdentifier::~SecurityIdentifier()
{
}

SecurityIdentifier SecurityIdentifier::get_thread_user()
{
#ifdef WIN32
	PSID pSid = 0;
	PSECURITY_DESCRIPTOR securityDescriptor = 0;
	DWORD result = GetSecurityInfo(
		GetCurrentThread(),
		SE_KERNEL_OBJECT,
		OWNER_SECURITY_INFORMATION,
		&pSid,
		0,
		0,
		0,
		&securityDescriptor);
	if (result != ERROR_SUCCESS)
		throw Exception("Unable to get thread user SID");

	SecurityIdentifier identifier(pSid);
	LocalFree(securityDescriptor);
	return identifier;
#else
	return SecurityIdentifier(getuid(), type_user);
#endif
}
	
SecurityIdentifier SecurityIdentifier::get_thread_group()
{
#ifdef WIN32
	PSID pSid = 0;
	PSECURITY_DESCRIPTOR securityDescriptor = 0;
	DWORD result = GetSecurityInfo(
		GetCurrentThread(),
		SE_KERNEL_OBJECT,
		GROUP_SECURITY_INFORMATION,
		0,
		&pSid,
		0,
		0,
		&securityDescriptor);
	if (result != ERROR_SUCCESS)
		throw Exception("Unable to get thread user SID");

	SecurityIdentifier identifier(pSid);
	LocalFree(securityDescriptor);
	return identifier;
#else
	return SecurityIdentifier(getgid(), type_group);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// SecurityIdentifier Attributes:

std::string SecurityIdentifier::get_name() const
{
	return std::string();
}

std::string SecurityIdentifier::get_domain_name() const
{
	return std::string();
}

SecurityIdentifier::Type SecurityIdentifier::get_type() const
{
	return type;
}

/////////////////////////////////////////////////////////////////////////////
// SecurityIdentifier Operations:

SecurityIdentifier &SecurityIdentifier::operator =(const SecurityIdentifier &copy)
{
	sid = copy.sid;
	type = copy.type;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// SecurityIdentifier Implementation:

}
