
#include "precomp.h"
#include "html_url.h"

HTMLUrl::HTMLUrl()
{
	scheme = "http";
	port = "80";
	path = "/";
}

HTMLUrl::HTMLUrl(const CL_String &url, const HTMLUrl &base)
: input(url), pos(0)
{
	// http://tools.ietf.org/html/rfc3987

	CL_String s = read_scheme();
	if (input.substr(pos, 1) == ":")
	{
		scheme = s;
		pos += 1;
	}
	else
	{
		scheme = base.scheme;
		pos = 0;
	}

	if (input.substr(pos, 2) == "//")
	{
		pos += 2;
		CL_String::size_type colon = input.find_first_of(':', pos);
		CL_String::size_type slash = input.find_first_of('/', pos);
		if (colon < slash)
		{
			host = input.substr(pos, colon-pos);
			port = input.substr(colon+1, slash-colon-1);
		}
		else
		{
			host = input.substr(pos, slash-pos);
			port = base.port;
		}
		pos = slash;
	}
	else
	{
		host = base.host;
		port = base.port;
	}

	CL_String::size_type query_pos = input.find_first_of('?');
	path = input.substr(pos, query_pos);
	query = input.substr(query_pos);

	if (path.substr(0, 1) != "/")
	{
		CL_String base_directory;
		CL_String::size_type last_slash = base.path.find_last_of('/');
		if (last_slash == CL_String::npos)
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
		CL_String::size_type p = path.find("/../");
		if (p == 0 || p == CL_String::npos)
			break;
		CL_String::size_type p2 = path.rfind('/', p-1);
		if (p2 == CL_String::npos)
			break;
		path = path.substr(0, p2) + path.substr(p+3);
	}
}

CL_String HTMLUrl::read_scheme()
{
	if (pos < input.length() && ((input[pos] >= 'A' && input[pos] <= 'Z') || (input[pos] >= 'a' && input[pos] <= 'z')))
	{
		CL_String::size_type pos2;
		for (pos2 = pos+1; pos2 != input.length(); pos2++)
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
		CL_String s = input.substr(pos, pos2-pos);
		pos = pos2;
		return s;
	}
	else
	{
		return CL_String();
	}
}

CL_String HTMLUrl::to_string() const
{
	return scheme + "://" + host + ":" + port + path + query;
}
