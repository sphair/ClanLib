
#pragma once

#include "API/Core/IOData/html_url.h"
#include "API/Core/System/databuffer.h"
#include <map>

class CL_WebRequest_Impl
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

	CL_WebRequest_Impl(Type type, const CL_String &url) : type(type), url(url) { }

	static void throw_if_null(const CL_SharedPtr<CL_WebRequest_Impl> &impl)
	{
		if (!impl)
			throw CL_Exception("WebRequest object is null");
	}

	Type type;
	std::map<CL_String, CL_String> headers;
	CL_DataBuffer body;
	CL_HTMLUrl url;
};
