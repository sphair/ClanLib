
#include "Network/precomp.h"
#include "API/Network/Web/web_response.h"
#include "web_response_impl.h"

CL_WebResponse::CL_WebResponse()
{
}

int CL_WebResponse::get_status_code()
{
	CL_WebResponse_Impl::throw_if_null(impl);
	return impl->status_code;
}

CL_String CL_WebResponse::get_status_string()
{
	CL_WebResponse_Impl::throw_if_null(impl);
	return impl->status_text;
}

bool CL_WebResponse::has_header_field(const CL_String &name)
{
	CL_WebResponse_Impl::throw_if_null(impl);
	return impl->headers.find(name) != impl->headers.end();
}

CL_String CL_WebResponse::get_header_field(const CL_String &name)
{
	CL_WebResponse_Impl::throw_if_null(impl);
	if (!has_header_field(name))
		throw CL_Exception(cl_format("%1 HTTP response header field not found", name));
	return impl->headers[name];
}

bool CL_WebResponse::has_content_type()
{
	return has_header_field("Content-Type");
}

CL_String CL_WebResponse::get_content_type()
{
	return get_header_field("Content-Type");
}

CL_DataBuffer CL_WebResponse::get_body()
{
	CL_WebResponse_Impl::throw_if_null(impl);
	return impl->body;
}

CL_String CL_WebResponse::get_body_string()
{
	CL_WebResponse_Impl::throw_if_null(impl);
	return CL_String(impl->body.get_data(), impl->body.get_size());
}

/*
CL_IODevice CL_WebResponse::get_body_reader()
{

}
*/
