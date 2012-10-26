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

#include "Core/precomp.h"
#include "API/Core/IOData/access_control_list.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// AccessControlList Construction:

AccessControlList::AccessControlList()
{
}

AccessControlList::~AccessControlList()
{
}

/////////////////////////////////////////////////////////////////////////////
// AccessControlList Attributes:

/////////////////////////////////////////////////////////////////////////////
// AccessControlList Operations:

void AccessControlList::add_access_allowed(const SecurityIdentifier &sid, AccessMask access_mask)
{
}
	
void AccessControlList::add_access_denied(const SecurityIdentifier &sid, AccessMask access_mask)
{
}
	
void AccessControlList::add_user_access_allowed(const std::string &name, AccessMask access_mask)
{
}
	
void AccessControlList::add_user_access_denied(const std::string &name, AccessMask access_mask)
{
}

void AccessControlList::add_group_access_allowed(const std::string &name, AccessMask access_mask)
{
}
	
void AccessControlList::add_group_access_denied(const std::string &name, AccessMask access_mask)
{
}
	
void AccessControlList::remove_access_allowed(const SecurityIdentifier &sid)
{
}
	
void AccessControlList::remove_access_denied(const SecurityIdentifier &sid)
{
}
	
void AccessControlList::remove_user_allowed(const std::string &name)
{
}
	
void AccessControlList::remove_user_denied(const std::string &name)
{
}
	
void AccessControlList::remove_group_allowed(const std::string &name)
{
}
	
void AccessControlList::remove_group_denied(const std::string &name)
{
}

/////////////////////////////////////////////////////////////////////////////
// AccessControlList Implementation:

}
