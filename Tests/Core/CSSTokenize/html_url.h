
#pragma once

class HTMLUrl
{
public:
	HTMLUrl();
	HTMLUrl(const CL_String &url, const HTMLUrl &base = HTMLUrl());
	CL_String to_string() const;

	CL_String scheme;
	CL_String host;
	CL_String port;
	CL_String path;
	CL_String query;

private:
	CL_String read_scheme();

	CL_String input;
	CL_String::size_type pos;
};
