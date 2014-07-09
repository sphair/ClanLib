
#pragma once

#include <memory>

namespace clan
{

class IODevice;
class DataBuffer;
class WebResponse;
class WebRequest_Impl;

/// \brief HTTP web request class.
class WebRequest
{
public:
	WebRequest();

	static WebRequest options(const std::string &url);
	static WebRequest get(const std::string &url);
	static WebRequest head(const std::string &url);
	static WebRequest post(const std::string &url);
	static WebRequest put(const std::string &url);
	static WebRequest http_delete(const std::string &url);
	static WebRequest trace(const std::string &url);

	void set_header_field(const std::string &name, const std::string &value);
	void set_content_type(const std::string &content_type);

	void set_body(const DataBuffer &data);
	void set_body(const std::string &data);
	void set_body(IODevice data);
	void set_body(const void *data, int size);
	//IODevice get_body_writer();

	WebResponse get_response();

private:
	std::shared_ptr<WebRequest_Impl> impl;
};

}
