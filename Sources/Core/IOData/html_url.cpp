/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Core/IOData/html_url.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	HTMLUrl::HTMLUrl()
	{
		scheme = "http";
		port = "80";
		path = "/";
	}

	HTMLUrl::HTMLUrl(const std::string &url, const HTMLUrl &base)
	{
		std::string input = url;
		std::string::size_type pos = 0;

		// http://tools.ietf.org/html/rfc3987
		// tl;dr, haha

		std::string s = read_scheme(input, pos);
		if (input.substr(pos, 1) == ":")
		{
			scheme = s;
			pos += 1;

			if (StringHelp::compare(scheme, "data", true) == 0)
			{
				std::string::size_type comma = input.find_first_of(',', pos);
				if (comma == std::string::npos)
				{
					data = input.substr(pos);
				}
				else
				{
					std::string::size_type semicolon = input.find_first_of(';', pos);
					if (semicolon < comma)
					{
						content_type = input.substr(pos, semicolon - pos);
						encoding = input.substr(semicolon + 1, comma - semicolon - 1);
						data = input.substr(comma + 1);
					}
					else
					{
						content_type = input.substr(pos, comma - pos);
						data = input.substr(comma + 1);
					}
				}
				return;
			}
		}
		else
		{
			scheme = base.scheme;
			pos = 0;
		}

		if (input.substr(pos, 2) == "//")
		{
			pos += 2;
			std::string::size_type colon = input.find_first_of(':', pos);
			std::string::size_type slash = input.find_first_of('/', pos);
			if (colon < slash)
			{
				host = input.substr(pos, colon - pos);
				port = input.substr(colon + 1, slash - colon - 1);
			}
			else
			{
				host = input.substr(pos, slash - pos);
				port = base.port;
			}
			pos = slash;
		}
		else
		{
			host = base.host;
			port = base.port;
		}

		std::string::size_type query_pos = input.find_first_of('?', pos);
		if (query_pos == std::string::npos)
		{
			path = input.substr(pos);
			query.clear();
		}
		else
		{
			path = input.substr(pos, query_pos - pos);
			query = input.substr(query_pos);
		}

		if (path.substr(0, 1) != "/")
		{
			std::string base_directory;
			std::string::size_type last_slash = base.path.find_last_of('/');
			if (last_slash == std::string::npos)
			{
				base_directory = "/";
			}
			else
			{
				base_directory = base.path.substr(0, last_slash + 1);
			}
			path = base_directory + path;
		}

		while (true)
		{
			std::string::size_type p = path.find("/../");
			if (p == 0 || p == std::string::npos)
				break;
			std::string::size_type p2 = path.rfind('/', p - 1);
			if (p2 == std::string::npos)
				break;
			path = path.substr(0, p2) + path.substr(p + 3);
		}
	}

	std::string HTMLUrl::read_scheme(std::string &input, std::string::size_type &pos)
	{
		if (pos < input.length() && ((input[pos] >= 'A' && input[pos] <= 'Z') || (input[pos] >= 'a' && input[pos] <= 'z')))
		{
			std::string::size_type pos2;
			for (pos2 = pos + 1; pos2 != input.length(); pos2++)
			{
				if ((input[pos2] >= 'A' && input[pos2] <= 'Z') ||
					(input[pos2] >= 'a' && input[pos2] <= 'z') ||
					(input[pos2] >= '0' && input[pos2] <= '9') ||
					input[pos2] == '.' || input[pos2] == '+' || input[pos2] == '-')
				{
				}
				else
				{
					break;
				}
			}
			std::string s = input.substr(pos, pos2 - pos);
			pos = pos2;
			return s;
		}
		else
		{
			return std::string();
		}
	}

	std::string HTMLUrl::to_string() const
	{
		if (StringHelp::compare(scheme, "data", true) == 0)
		{
			if (!content_type.empty() && !encoding.empty())
				return scheme + ":" + content_type + ";" + encoding + "," + data;
			else
				return scheme + ":" + data;
		}
		else
		{
			if (host.empty())
				return path.substr(1) + query;
			else
				return scheme + "://" + host + ":" + port + path + query;
		}
	}
}
