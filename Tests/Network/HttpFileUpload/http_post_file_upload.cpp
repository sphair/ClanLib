
#include <ClanLib/core.h>
#include <ClanLib/network.h>

#include "http_post_file_upload.h"

//////////////////////////////////////////////////////////////////////////
// Construction

HttpPostFileUpload::HttpPostFileUpload() 
: form_boundary("39f2057baece2")
{
}

HttpPostFileUpload::~HttpPostFileUpload()
{
}

//////////////////////////////////////////////////////////////////////////
// Attributes


//////////////////////////////////////////////////////////////////////////
// Operations

void HttpPostFileUpload::add_form_field(const CL_String &name, const CL_String &data)
{
	CL_String str = cl_format("\r\n--%1\r\n", form_boundary);
	str += cl_format("content-disposition: form-data; name=\"%1\"\r\n\r\n", name);
	str += data + "\r\n";

	CL_String8 str8 = CL_StringHelp::text_to_utf8(str);
	CL_DataBuffer buf(str8.size());
	void *buf_ptr = (void*)buf.get_data();
	memcpy(buf_ptr, str8.data(), str8.size());

	form_elements.push_back(buf);
}

void HttpPostFileUpload::add_file(const CL_String &filename)
{
	CL_String8 header = CL_StringHelp::text_to_utf8( 	
		cl_format(
		"\r\n"
		"--%1\r\n"
		"Content-disposition: attachment; name=\"Files[]\"; filename=\"%2\"\r\n"
		"Content-type: application/octet-stream\r\n"
		"Content-Transfer-Encoding: binary\r\n"
		"\r\n",
		form_boundary, filename));

	CL_File file(filename);
	CL_DataBuffer buf(header.size() + file.get_size());
	char *buf_ptr = buf.get_data();
	memcpy(buf_ptr, header.data(), header.size());
	file.read(buf_ptr + header.size(), file.get_size());
	data_buffers.push_back(buf);
}

CL_DataBuffer HttpPostFileUpload::create_post_data()
{
	int total_size = 0;
	std::vector<CL_DataBuffer>::size_type i;
	for (i=0;i<form_elements.size();++i)
		total_size += form_elements[i].get_size();
	for (i=0;i<data_buffers.size();++i)
		total_size += data_buffers[i].get_size();

	CL_String8 f_boundary = CL_StringHelp::text_to_utf8(cl_format("\r\n--%1", form_boundary));
	CL_String8 f_boundary_end = CL_StringHelp::text_to_utf8(cl_format("\r\n--%1--", form_boundary));

	total_size += f_boundary_end.size();

	CL_DataBuffer post_data(total_size);
	char *post_ptr = post_data.get_data();

	int pos = 0;
	for (i=0;i<form_elements.size();++i)
	{
		CL_DataBuffer &buf = form_elements[i];
		const char *ptr = buf.get_data(); 

		memcpy(post_ptr+pos, ptr, buf.get_size());
		pos += buf.get_size();
	}

	for (i=0;i<data_buffers.size();++i)
	{
		CL_DataBuffer &buf = data_buffers[i];
		const char *ptr = buf.get_data(); 

		memcpy(post_ptr+pos, ptr, buf.get_size());
		pos += buf.get_size();
	}

	memcpy(post_ptr+pos, f_boundary_end.data(), f_boundary_end.size());
	pos += f_boundary_end.size();

	if (pos > post_data.get_size())
		throw CL_Exception("Buffer overflow while creating post data.");

	return post_data;
}

void HttpPostFileUpload::send(const CL_String &server, const CL_String &port, const CL_String &file_on_server)
{
	progress.set(0);

	CL_DataBuffer post_data = create_post_data();
	CL_String post_header = 
		cl_format (
			"POST %4 HTTP/1.1\r\n"
			"Host: %1\r\n"
			"User-Agent: UploadTest\r\n"
			"Content-Length: %2\r\n"
			"Connection: close\r\n"
			"Content-Type: multipart/form-data, boundary=%3\r\n\r\n",
			server,
			post_data.get_size(),
			form_boundary,
			file_on_server);
	CL_String8 post_header8 = CL_StringHelp::text_to_utf8(post_header);

	CL_DataBuffer request(post_header8.length()+post_data.get_size());
	memcpy(request.get_data(), post_header8.data(), post_header8.length());
	memcpy(request.get_data()+post_header8.length(), post_data.get_data(), post_data.get_size());

	CL_SocketName socket(server, port);
	CL_TCPConnection connection(socket);
	int bytes_written = 0;
	while (bytes_written < request.get_size())
	{
		progress.set(bytes_written*99/request.get_size());
		connection.get_write_event().wait();
		bytes_written += connection.write(request.get_data()+bytes_written, request.get_size()-bytes_written, false);
	}

	CL_IODevice_Memory response_device;
	while (true)
	{
		char buffer[16*1024];
		connection.get_read_event().wait();
		int bytes_read = connection.read(buffer, 16*1024, false);
		if (bytes_read == 0)
			break;
		response_device.write(buffer, bytes_read);
		if (response_device.get_size() > 1024*1024)
			throw CL_Exception("Web server response was greater than 1 MB!!");
	}

	progress.set(100);

	CL_DataBuffer response = response_device.get_data();
	// to do: verify that response was a 200-299 response code
}

