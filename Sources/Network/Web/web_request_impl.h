
#pragma once

#include "API/Core/IOData/html_url.h"
#include "API/Core/System/databuffer.h"
#include <map>

namespace clan
{

class WebRequest_Impl
{
public:
	enum Type
	{
		type_options,
		type_get,
		type_head,
		type_post,
		type_put,
		type_delete,
		type_trace
	};

	WebRequest_Impl(Type type, const std::string &url) : type(type), url(url) { }

	static void throw_if_null(const std::shared_ptr<WebRequest_Impl> &impl)
	{
		if (!impl)
			throw Exception("WebRequest object is null");
	}

	Type type;
	std::map<std::string, std::string> headers;
	DataBuffer body;
	HTMLUrl url;
};

}
