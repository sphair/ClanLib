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
#include "API/Core/IOData/security_descriptor.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityDescriptor construction:

CL_SecurityDescriptor::CL_SecurityDescriptor()
: owner(CL_SecurityIdentifier::get_thread_user()),
	primary_group(CL_SecurityIdentifier::get_thread_group()),
	owner_defaulted(true),
	group_defaulted(true),
	dacl_defaulted(true)
{
	// todo: setup dacl to u+rw, g+r, o+r rights
}

CL_SecurityDescriptor::CL_SecurityDescriptor(const CL_SecurityDescriptor &copy)
: owner(copy.owner), primary_group(copy.primary_group),
	discretionary_acl(copy.discretionary_acl),
	owner_defaulted(copy.owner_defaulted),
	group_defaulted(copy.group_defaulted),
	dacl_defaulted(copy.dacl_defaulted)
{
}

CL_SecurityDescriptor::~CL_SecurityDescriptor()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityDescriptor attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityDescriptor operations:

CL_SecurityDescriptor &CL_SecurityDescriptor::operator =(const CL_SecurityDescriptor &copy)
{
	owner = copy.owner;
	primary_group = copy.primary_group;
	discretionary_acl = copy.discretionary_acl;
	owner_defaulted = copy.owner_defaulted;
	group_defaulted = copy.group_defaulted;
	dacl_defaulted = copy.dacl_defaulted;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SecurityDescriptor implementation:
