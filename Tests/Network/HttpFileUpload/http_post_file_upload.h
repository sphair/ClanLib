
#include <ClanLib/core.h>
#include <ClanLib/network.h>
using namespace clan;

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
	void add_form_field(const std::string &name, const std::string &data);

	void add_file(const std::string &filename);

	void send(const std::string &server, const std::string &port, const std::string &file_on_server);

// Implementation:
private:
	DataBuffer create_post_data();
	InterlockedVariable progress;
	const std::string form_boundary;
	std::vector<DataBuffer> form_elements;
	std::vector<DataBuffer> data_buffers;
};
