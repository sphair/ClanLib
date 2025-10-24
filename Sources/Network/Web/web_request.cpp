
#include "Network/precomp.h"
#include "API/Network/Web/web_request.h"
#include "API/Network/Web/web_response.h"
#include "web_request_impl.h"
#include "web_response_impl.h"

CL_WebRequest::CL_WebRequest()
{
}

CL_WebRequest CL_WebRequest::options(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_options, url));
	return request;
}

CL_WebRequest CL_WebRequest::get(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_get, url));
	return request;
}

CL_WebRequest CL_WebRequest::head(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_head, url));
	return request;
}

CL_WebRequest CL_WebRequest::post(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_post, url));
	return request;
}

CL_WebRequest CL_WebRequest::put(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_put, url));
	return request;
}

CL_WebRequest CL_WebRequest::http_delete(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_delete, url));
	return request;
}

CL_WebRequest CL_WebRequest::trace(const CL_String &url)
{
	CL_WebRequest request;
	request.impl = CL_SharedPtr<CL_WebRequest_Impl>(new CL_WebRequest_Impl(CL_WebRequest_Impl::type_trace, url));
	return request;
}

void CL_WebRequest::set_header_field(const CL_String &name, const CL_String &value)
{
	CL_WebRequest_Impl::throw_if_null(impl);
	impl->headers[name] = value;
}

void CL_WebRequest::set_content_type(const CL_String &content_type)
{
	set_header_field("Content-Type", content_type);
}

void CL_WebRequest::set_body(const CL_DataBuffer &data)
{
	set_body(data.get_data(), data.get_size());
}

void CL_WebRequest::set_body(const CL_String &data)
{
	set_body(data.data(), data.length());
}

void CL_WebRequest::set_body(CL_IODevice data)
{
	CL_WebRequest_Impl::throw_if_null(impl);
	if (impl->type != CL_WebRequest_Impl::type_post && impl->type != CL_WebRequest_Impl::type_trace)
		throw CL_Exception("HTTP request type does not support body data");

	CL_IODevice_Memory memory_device;
	while (true)
	{
		char buffer[16*1024];
		int length = data.read(buffer, 16*1024, false);
		if (length == 0)
			break;
		memory_device.write(buffer, length);
	}
	impl->body = memory_device.get_data();
}

void CL_WebRequest::set_body(const void *data, int size)
{
	CL_WebRequest_Impl::throw_if_null(impl);
	impl->body = CL_DataBuffer(data, size);
}

/*
CL_IODevice CL_WebRequest::get_body_writer()
{
}
*/

CL_WebResponse CL_WebRequest::get_response()
{
	CL_WebRequest_Impl::throw_if_null(impl);
	CL_WebResponse response;
	response.impl = CL_SharedPtr<CL_WebResponse_Impl>(new CL_WebResponse_Impl());
	response.impl->request = impl;
	response.impl->retrieve();
	return response;
}
