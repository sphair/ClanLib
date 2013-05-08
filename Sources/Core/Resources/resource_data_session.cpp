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
#include "API/Core/Resources/resource_data_session.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ResourceDataSession Construction:

ResourceDataSession::ResourceDataSession()
{
}

ResourceDataSession::ResourceDataSession(
	const std::string &data_name, const Resource &data_resource)
: name(data_name), resource(data_resource)
{
	if (!name.empty())
		resource.add_data_session(name);
}

ResourceDataSession::ResourceDataSession(const ResourceDataSession &copy)
: name(copy.name), resource(copy.resource)
{
	if (!name.empty())
		resource.add_data_session(name);
}

ResourceDataSession::~ResourceDataSession()
{
	if (!name.empty())
	{
		int count = resource.remove_data_session(name);
		if (count == 0)
			resource.clear_data(name);
	}
}

/////////////////////////////////////////////////////////////////////////////
// ResourceDataSession Attributes:

/////////////////////////////////////////////////////////////////////////////
// ResourceDataSession Operations:

ResourceDataSession &ResourceDataSession::operator =(const ResourceDataSession &copy)
{
	if (name == copy.name && resource == copy.resource)
		return *this;

	if (!name.empty())
	{
		int count = resource.remove_data_session(name);
		if (count == 0)
			resource.clear_data(name);
	}
	name = copy.name;
	resource = copy.resource;
	if (!name.empty())
		resource.add_data_session(name);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// ResourceDataSession Implementation:

}
