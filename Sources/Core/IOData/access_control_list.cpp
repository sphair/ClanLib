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
#include "API/Core/IOData/access_control_list.h"

/////////////////////////////////////////////////////////////////////////////
// CL_AccessControlList Construction:

CL_AccessControlList::CL_AccessControlList()
{
}

CL_AccessControlList::~CL_AccessControlList()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_AccessControlList Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_AccessControlList Operations:

void CL_AccessControlList::add_access_allowed(const CL_SecurityIdentifier &sid, AccessMask access_mask)
{
}
	
void CL_AccessControlList::add_access_denied(const CL_SecurityIdentifier &sid, AccessMask access_mask)
{
}
	
void CL_AccessControlList::add_user_access_allowed(const CL_String &name, AccessMask access_mask)
{
}
	
void CL_AccessControlList::add_user_access_denied(const CL_String &name, AccessMask access_mask)
{
}

void CL_AccessControlList::add_group_access_allowed(const CL_String &name, AccessMask access_mask)
{
}
	
void CL_AccessControlList::add_group_access_denied(const CL_String &name, AccessMask access_mask)
{
}
	
void CL_AccessControlList::remove_access_allowed(const CL_SecurityIdentifier &sid)
{
}
	
void CL_AccessControlList::remove_access_denied(const CL_SecurityIdentifier &sid)
{
}
	
void CL_AccessControlList::remove_user_allowed(const CL_String &name)
{
}
	
void CL_AccessControlList::remove_user_denied(const CL_String &name)
{
}
	
void CL_AccessControlList::remove_group_allowed(const CL_String &name)
{
}
	
void CL_AccessControlList::remove_group_denied(const CL_String &name)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_AccessControlList Implementation:
