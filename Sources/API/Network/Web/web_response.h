
#pragma once

#include <memory>

namespace clan
{

class DataBuffer;
class IODevice;
class WebResponse_Impl;

/// \brief HTTP web response class.
class WebResponse
{
public:
	WebResponse();

	int get_status_code();
	std::string get_status_string();

	bool has_header_field(const std::string &name);
	std::string get_header_field(const std::string &name);

	bool has_content_type();
	std::string get_content_type();

	DataBuffer get_body();
	std::string get_body_string();
	// IODevice get_body_reader();

private:
	std::shared_ptr<WebResponse_Impl> impl;
	friend class WebRequest;
};

}
