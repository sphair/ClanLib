/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "Network/precomp.h"
#include "API/Network/Web/url_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_URLHelp Operations:

CL_StringRef CL_URLHelp::get_protocol(const CL_StringRef &url)
{
	CL_StringRef::size_type pos = url.find("://");
	if (pos == CL_StringRef::npos)
		return CL_StringRef();
	else
		return url.substr(0, pos);
}

CL_StringRef CL_URLHelp::get_full_path(const CL_StringRef &url)
{
	CL_StringRef::size_type pos1 = url.find("://");
	CL_StringRef::size_type pos2 = url.find('?');
	if (pos1 == CL_StringRef::npos)
		pos1 = 0;
	else
		pos1++;

	if (pos2 == CL_StringRef::npos)
		return url.substr(pos1);
	else
		return url.substr(pos1, pos2 - pos1);
}

CL_StringRef CL_URLHelp::get_host(const CL_StringRef &url)
{
	CL_StringRef path = get_full_path(url);
	if (path.substr(0, 2) != "//")
		return CL_StringRef();

	CL_StringRef::size_type pos1 = path.find_first_of(":/", 2);
	if (pos1 == CL_StringRef::npos)
		return path.substr(2);
	else
		return path.substr(2, pos1 - 2);
}

CL_StringRef CL_URLHelp::get_port(const CL_StringRef &url)
{
	CL_StringRef path = get_full_path(url);
	if (path.substr(0, 2) != "//")
		return CL_StringRef();

	CL_StringRef::size_type pos1 = path.find_first_of(":/", 2);
	if (pos1 == CL_StringRef::npos)
		return CL_StringRef();
	else if (path[pos1] == '/')
		return CL_StringRef();
	pos1++;

	CL_StringRef::size_type pos2 = path.find('/', pos1);
	if (pos2 == CL_StringRef::npos)
		return path.substr(pos1);
	else
		return path.substr(pos1, pos2 - pos1);
}

CL_StringRef CL_URLHelp::get_local_path(const CL_StringRef &url)
{
	CL_StringRef path = get_full_path(url);
	if (path.substr(0, 2) != "//")
		return path;
	CL_StringRef::size_type pos1 = path.find('/', 2);
	if (pos1 == CL_StringRef::npos)
		return CL_StringRef();
	else
		return path.substr(pos1);
}

CL_StringRef CL_URLHelp::get_query(const CL_StringRef &url)
{
	CL_StringRef::size_type pos1 = url.find('?');
	if (pos1 == CL_StringRef::npos)
		return CL_StringRef();
	else
		return url.substr(pos1+1);
}
