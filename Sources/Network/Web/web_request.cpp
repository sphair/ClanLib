
#include "Network/precomp.h"
#include "API/Network/Web/web_request.h"
#include "API/Network/Web/web_response.h"
#include "web_request_impl.h"
#include "web_response_impl.h"

namespace clan
{

WebRequest::WebRequest()
{
}

WebRequest WebRequest::options(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_options, url));
	return request;
}

WebRequest WebRequest::get(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_get, url));
	return request;
}

WebRequest WebRequest::head(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_head, url));
	return request;
}

WebRequest WebRequest::post(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_post, url));
	return request;
}

WebRequest WebRequest::put(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_put, url));
	return request;
}

WebRequest WebRequest::http_delete(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_delete, url));
	return request;
}

WebRequest WebRequest::trace(const std::string &url)
{
	WebRequest request;
	request.impl = std::shared_ptr<WebRequest_Impl>(new WebRequest_Impl(WebRequest_Impl::type_trace, url));
	return request;
}

void WebRequest::set_header_field(const std::string &name, const std::string &value)
{
	WebRequest_Impl::throw_if_null(impl);
	impl->headers[name] = value;
}

void WebRequest::set_content_type(const std::string &content_type)
{
	set_header_field("Content-Type", content_type);
}

void WebRequest::set_body(const DataBuffer &data)
{
	set_body(data.get_data(), data.get_size());
}

void WebRequest::set_body(const std::string &data)
{
	set_body(data.data(), data.length());
}

void WebRequest::set_body(IODevice data)
{
	WebRequest_Impl::throw_if_null(impl);
	if (impl->type != WebRequest_Impl::type_post && impl->type != WebRequest_Impl::type_trace)
		throw Exception("HTTP request type does not support body data");

	IODevice_Memory memory_device;
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

void WebRequest::set_body(const void *data, int size)
{
	WebRequest_Impl::throw_if_null(impl);
	impl->body = DataBuffer(data, size);
}

/*
IODevice WebRequest::get_body_writer()
{
}
*/

WebResponse WebRequest::get_response()
{
	WebRequest_Impl::throw_if_null(impl);
	WebResponse response;
	response.impl = std::shared_ptr<WebResponse_Impl>(new WebResponse_Impl());
	response.impl->request = impl;
	response.impl->retrieve();
	return response;
}

}
