
#include <ClanLib/core.h>
#include <ClanLib/network.h>

class HttpPostFileUpload
{
// Construction
public:
	HttpPostFileUpload();
	~HttpPostFileUpload();

// Attributes
public:

// Operations
public:
	void add_form_field(const CL_String &name, const CL_String &data);

	void add_file(const CL_String &filename);

	void send(const CL_String &server, const CL_String &port, const CL_String &file_on_server);

// Implementation:
private:
	CL_DataBuffer create_post_data();
	CL_InterlockedVariable progress;
	const CL_String form_boundary;
	std::vector<CL_DataBuffer> form_elements;
	std::vector<CL_DataBuffer> data_buffers;
};
