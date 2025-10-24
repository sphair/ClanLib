
#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_DataBuffer;
class CL_IODevice;
class CL_WebResponse_Impl;

/// \brief HTTP web response class.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_WebResponse
{
public:
	CL_WebResponse();

	int get_status_code();
	CL_String get_status_string();

	bool has_header_field(const CL_String &name);
	CL_String get_header_field(const CL_String &name);

	bool has_content_type();
	CL_String get_content_type();

	CL_DataBuffer get_body();
	CL_String get_body_string();
	// CL_IODevice get_body_reader();

private:
	CL_SharedPtr<CL_WebResponse_Impl> impl;
	friend class CL_WebRequest;
};
