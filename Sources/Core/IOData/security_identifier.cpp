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

#include "Core/precomp.h"
#include "API/Core/IOData/security_identifier.h"
#include "API/Core/System/exception.h"
#ifdef WIN32
#include <AclAPI.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityIdentifier Construction:

CL_SecurityIdentifier::CL_SecurityIdentifier(const CL_String &name)
: type(type_invalid)
{
	throw CL_Exception("CL_SecurityIdentifier(const CL_String &name) not implemented");
}
	
CL_SecurityIdentifier::CL_SecurityIdentifier(const CL_String &name, Type type)
: type(type)
{
	throw CL_Exception("CL_SecurityIdentifier(const CL_String &name) not implemented");
}

#ifdef WIN32
CL_SecurityIdentifier::CL_SecurityIdentifier(const PSID new_sid)
: type(type_unknown)
{
	sid.set_size(GetLengthSid(new_sid));
	memcpy(sid.get_data(), new_sid, sid.get_size());
}
#else
CL_SecurityIdentifier::CL_SecurityIdentifier(unsigned int sid, Type type)
: sid(sid), type(type)
{
}
#endif

CL_SecurityIdentifier::CL_SecurityIdentifier(const CL_SecurityIdentifier &copy)
: sid(copy.sid), type(copy.type)
{
}

CL_SecurityIdentifier::~CL_SecurityIdentifier()
{
}

CL_SecurityIdentifier CL_SecurityIdentifier::get_thread_user()
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
		throw CL_Exception("Unable to get thread user SID");

	CL_SecurityIdentifier identifier(pSid);
	LocalFree(securityDescriptor);
	return identifier;
#else
	return CL_SecurityIdentifier(getuid(), type_user);
#endif
}
	
CL_SecurityIdentifier CL_SecurityIdentifier::get_thread_group()
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
		throw CL_Exception("Unable to get thread user SID");

	CL_SecurityIdentifier identifier(pSid);
	LocalFree(securityDescriptor);
	return identifier;
#else
	return CL_SecurityIdentifier(getgid(), type_group);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityIdentifier Attributes:

CL_String CL_SecurityIdentifier::get_name() const
{
	return CL_String();
}

CL_String CL_SecurityIdentifier::get_domain_name() const
{
	return CL_String();
}

CL_SecurityIdentifier::Type CL_SecurityIdentifier::get_type() const
{
	return type;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityIdentifier Operations:

CL_SecurityIdentifier &CL_SecurityIdentifier::operator =(const CL_SecurityIdentifier &copy)
{
	sid = copy.sid;
	type = copy.type;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityIdentifier Implementation:
