
#include "Network/precomp.h"
#include "API/Network/Web/web_response.h"
#include "web_response_impl.h"

namespace clan
{

WebResponse::WebResponse()
{
}

int WebResponse::get_status_code()
{
	WebResponse_Impl::throw_if_null(impl);
	return impl->status_code;
}

std::string WebResponse::get_status_string()
{
	WebResponse_Impl::throw_if_null(impl);
	return impl->status_text;
}

bool WebResponse::has_header_field(const std::string &name)
{
	WebResponse_Impl::throw_if_null(impl);
	return impl->headers.find(name) != impl->headers.end();
}

std::string WebResponse::get_header_field(const std::string &name)
{
	WebResponse_Impl::throw_if_null(impl);
	if (!has_header_field(name))
		throw Exception(string_format("%1 HTTP response header field not found", name));
	return impl->headers[name];
}

bool WebResponse::has_content_type()
{
	return has_header_field("Content-Type");
}

std::string WebResponse::get_content_type()
{
	return get_header_field("Content-Type");
}

DataBuffer WebResponse::get_body()
{
	WebResponse_Impl::throw_if_null(impl);
	return impl->body;
}

std::string WebResponse::get_body_string()
{
	WebResponse_Impl::throw_if_null(impl);
	return std::string(impl->body.get_data(), impl->body.get_size());
}

/*
IODevice WebResponse::get_body_reader()
{

}
*/

}
