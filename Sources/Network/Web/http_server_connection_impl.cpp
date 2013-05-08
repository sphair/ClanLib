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

#include "Network/precomp.h"
#include "http_server_connection_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// HTTPServerConnection_Impl Construction:

HTTPServerConnection_Impl::HTTPServerConnection_Impl()
: request_read(false), performed_read(false), performed_write(false),
  writing_header(false), written_content_length(-1)
{
}

HTTPServerConnection_Impl::~HTTPServerConnection_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// HTTPServerConnection_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// HTTPServerConnection_Impl Operations:

std::string HTTPServerConnection_Impl::get_header_value(
	const std::string &name,
	const std::string &header_lines)
{
	std::string::size_type start, end;
	start = 0;
	while (true)
	{
		end = header_lines.find("\r\n", start);
		if (end == std::string::npos)
			end = header_lines.length();
		if (start == end)
			break;

		std::string line = header_lines.substr(start, end-start);
		if (line.substr(0, name.length()) == name)
		{
			std::string::size_type colon_pos = line.find_first_not_of(" \t", name.length());
			if (colon_pos != std::string::npos && line[colon_pos] == ':')
			{
				std::string::size_type value_pos = line.find_first_not_of(" \t", colon_pos+1);
				return line.substr(value_pos);
			}
		}

		if (end == header_lines.length())
			break;
		start = end + 2;
	}

	return std::string();
}

/////////////////////////////////////////////////////////////////////////////
// HTTPServerConnection_Impl Implementation:

}
