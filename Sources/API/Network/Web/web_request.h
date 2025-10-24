
#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_IODevice;
class CL_DataBuffer;
class CL_WebResponse;
class CL_WebRequest_Impl;

/// \brief HTTP web request class.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_WebRequest
{
public:
	CL_WebRequest();

	static CL_WebRequest options(const CL_String &url);
	static CL_WebRequest get(const CL_String &url);
	static CL_WebRequest head(const CL_String &url);
	static CL_WebRequest post(const CL_String &url);
	static CL_WebRequest put(const CL_String &url);
	static CL_WebRequest http_delete(const CL_String &url);
	static CL_WebRequest trace(const CL_String &url);

	void set_header_field(const CL_String &name, const CL_String &value);
	void set_content_type(const CL_String &content_type);

	void set_body(const CL_DataBuffer &data);
	void set_body(const CL_String &data);
	void set_body(CL_IODevice data);
	void set_body(const void *data, int size);
	//CL_IODevice get_body_writer();

	CL_WebResponse get_response();

private:
	CL_SharedPtr<CL_WebRequest_Impl> impl;
};
