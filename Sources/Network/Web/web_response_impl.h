
#pragma once

#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/System/event.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "web_request_impl.h"
#include "ring_buffer.h"

class CL_WebResponse_Impl
{
public:
	void retrieve();
	void send_request(CL_TCPConnection &connection);
	void read_response(CL_TCPConnection &connection);
	void read_header(CL_TCPConnection &connection, CL_RingBuffer &buffer);
	CL_String get_request_keyword();
	void write_to_buffer(CL_TCPConnection &connection, CL_RingBuffer &buffer);

	static void throw_if_null(const CL_SharedPtr<CL_WebResponse_Impl> &impl)
	{
		if (!impl)
			throw CL_Exception("CL_WebResponse object is null");
	}

	CL_SharedPtr<CL_WebRequest_Impl> request;
	int status_code;
	CL_String status_text;
	std::map<CL_String, CL_String> headers;
	CL_DataBuffer body;
};

inline void CL_WebResponse_Impl::send_request(CL_TCPConnection &connection)
{
	request->headers["Host"] = request->url.host;
	request->headers["Connection"] = "close";
	if (request->type == CL_WebRequest_Impl::type_post || request->type == CL_WebRequest_Impl::type_trace)
	{
		if (request->body.is_null())
			request->headers["Content-Length"] = "0";
		else
			request->headers["Content-Length"] = cl_format("%1", request->body.get_size());
	}

	CL_String request_header = cl_format("%1 %2%3 HTTP/1.1\r\n", get_request_keyword(), request->url.path, request->url.query);
	for (std::map<CL_String, CL_String>::iterator it = request->headers.begin(); it != request->headers.end(); ++it)
		request_header += cl_format("%1: %2\r\n", it->first, it->second);
	request_header += "\r\n";

	connection.send(request_header.data(), request_header.length(), true);
	connection.set_nodelay(true);
	if (!request->body.is_null())
		connection.send(request->body.get_data(), request->body.get_size(), true);
	else
		connection.send(0, 0);
}

inline void CL_WebResponse_Impl::read_response(CL_TCPConnection &connection)
{
	CL_RingBuffer buffer(16*1024);
	read_header(connection, buffer);

	if (request->type != CL_WebRequest_Impl::type_head)
	{
		if (headers.find("Transfer-Encoding") != headers.end())
		{
			if (headers["Transfer-Encoding"] != "chunked")
				throw CL_Exception("Unsupported transfer encoding");

			CL_IODevice_Memory memory_device;
			while (true)
			{
				size_t header_length = buffer.find("\r\n", 2);
				while (header_length == CL_RingBuffer::npos)
				{
					write_to_buffer(connection, buffer);
					header_length = buffer.find("\r\n", 2);
				}

				CL_String chunk_header = buffer.read_to_string(header_length);
				buffer.read(2);
				size_t size_end_pos = chunk_header.find_first_of("; \t\r\n");
				size_t length = CL_StringHelp::text_to_uint(chunk_header.substr(0, size_end_pos), 16);
				if (length == 0)
					break;

				size_t bytes_read = 0;
				while (bytes_read < length)
				{
					const char *src = buffer.get_read_pos();
					size_t src_available = buffer.get_read_size();
					if (src_available == 0)
					{
						write_to_buffer(connection, buffer);
						src_available = buffer.get_read_size();
					}
					src_available = cl_min(src_available, length - bytes_read);
					memory_device.write(src, src_available);
					buffer.read(src_available);
					bytes_read += src_available;
				}

				size_t crlf = buffer.find("\r\n", 2);
				while (crlf == CL_RingBuffer::npos)
				{
					write_to_buffer(connection, buffer);
					crlf = buffer.find("\r\n", 2);
				}
				if (crlf != 0)
					throw CL_Exception("Missing CRLF in HTTP chunked encoded data");
				buffer.read(2);
			}

			// Read trailer
			while (true)
			{
				size_t line_length = buffer.find("\r\n", 2);
				while (line_length == CL_RingBuffer::npos)
				{
					write_to_buffer(connection, buffer);
					line_length = buffer.find("\r\n", 2);
				}
				buffer.read(line_length+2);
				if (line_length == 0)
					break;
			}

			body = memory_device.get_data();
		}
		else if (headers.find("Content-Length") != headers.end())
		{
			CL_IODevice_Memory memory_device;
			size_t length = CL_StringHelp::text_to_uint(headers["Content-Length"]);
			size_t bytes_read = 0;
			while (bytes_read < length)
			{
				const char *src = buffer.get_read_pos();
				size_t src_available = buffer.get_read_size();
				if (src_available == 0)
				{
					write_to_buffer(connection, buffer);
					src_available = buffer.get_read_size();
				}
				src_available = cl_min(src_available, length - bytes_read);
				memory_device.write(src, src_available);
				buffer.read(src_available);
				bytes_read += src_available;
			}
			body = memory_device.get_data();
		}
		else
		{
			CL_IODevice_Memory memory_device;
			while (true)
			{
				const char *src = buffer.get_read_pos();
				size_t src_available = buffer.get_read_size();
				if (src_available == 0)
				{
					if (!connection.get_read_event().wait(15000))
						throw CL_Exception("Connection timed out");
					int received = connection.read(buffer.get_write_pos(), buffer.get_write_size(), false);
					if (received == 0)
						break;
					src_available = buffer.get_read_size();
				}
				memory_device.write(src, src_available);
				buffer.read(src_available);
			}
			body = memory_device.get_data();
		}
	}
}

inline void CL_WebResponse_Impl::write_to_buffer(CL_TCPConnection &connection, CL_RingBuffer &buffer)
{
	if (buffer.get_write_size() == 0)
		throw CL_Exception("Ring buffer is full");
	if (!connection.get_read_event().wait(15000))
		throw CL_Exception("Connection timed out");
	int received = connection.read(buffer.get_write_pos(), buffer.get_write_size(), false);
	if (received == 0)
		throw CL_Exception("Invalid HTTP response header");
	buffer.write(received);
}

inline void CL_WebResponse_Impl::read_header(CL_TCPConnection &connection, CL_RingBuffer &buffer)
{
	size_t header_length = CL_RingBuffer::npos;
	while (header_length == CL_RingBuffer::npos)
	{
		write_to_buffer(connection, buffer);
		header_length = buffer.find("\r\n\r\n", 4);
	}

	CL_String header = buffer.read_to_string(header_length);
	buffer.read(4);
	std::vector<CL_String> lines = CL_StringHelp::split_text(header, "\n");
	for (size_t i = 0; i < lines.size(); i++)
		lines[i] = CL_StringHelp::trim(lines[i]);
	if (lines.size() < 1)
		throw CL_Exception("Invalid HTTP response header");

	CL_String::size_type index_space1 = lines[0].find_first_of(' ');
	CL_String::size_type index_space2 = lines[0].find_first_of(' ', index_space1 == CL_String::npos ? index_space1 : index_space1 + 1);

	CL_String protocol = lines[0].substr(0, index_space1);
	if (index_space1 == CL_String::npos || index_space2 == CL_String::npos)
		throw CL_Exception("Invalid HTTP 1.1 response status line");

	status_code = CL_StringHelp::text_to_int(lines[0].substr(index_space1+1, index_space2-index_space1-1));
	status_text = lines[0].substr(index_space2+1);

	for (size_t i = 1; i < lines.size(); i++)
	{
		CL_String::size_type index_colon = lines[i].find_first_of(':');
		if (index_colon == CL_String::npos)
			throw CL_Exception("Invalid HTTP response header line");
		CL_String field_name = CL_StringHelp::trim(lines[i].substr(0, index_colon));
		CL_String field_value = CL_StringHelp::trim(lines[i].substr(index_colon+1));

		std::map<CL_String, CL_String>::iterator it = headers.find(field_name);
		if (it == headers.end())
		{
			headers[field_name] = field_value;
		}
		else
		{
			it->second = it->second + "," + field_value;
		}
	}
}

inline void CL_WebResponse_Impl::retrieve()
{
	CL_TCPConnection connection(CL_SocketName(request->url.host, request->url.port));
	send_request(connection);
	read_response(connection);
	if (status_code == 100) // Skip 100 Continue responses
		read_response(connection);

/*
	request = cl_format("GET %1 HTTP/1.1\r\n", url.path+url.query);
	request += cl_format("Host: %1\r\nConnection: close\r\nReferer: %2\r\nAccept: image/png, image/jpeg\r\nUser-Agent: CSSTokenize/0.1\r\n\r\n", url.host, pageurl.to_string());

	CL_TCPConnection connection(CL_SocketName(url.host, url.port));
	connection.set_nodelay(true);
	connection.send(request.data(), request.length(), true);

	CL_String response;
	while (connection.get_read_event().wait(15000))
	{
		char buffer[16*1024];
		int received = connection.read(buffer, 16*1024, false);
		if (received == 0)
			break;
		response.append(buffer, received);
	}
	connection.disconnect_graceful();
*/
}

inline CL_String CL_WebResponse_Impl::get_request_keyword()
{
	switch (request->type)
	{
	case CL_WebRequest_Impl::type_options:
		return "OPTIONS";
	case CL_WebRequest_Impl::type_get:
		return "GET";
	case CL_WebRequest_Impl::type_head:
		return "HEAD";
	case CL_WebRequest_Impl::type_post:
		return "POST";
	case CL_WebRequest_Impl::type_put:
		return "PUT";
	case CL_WebRequest_Impl::type_delete:
		return "DELETE";
	case CL_WebRequest_Impl::type_trace:
		return "TRACE";
	default:
		throw CL_Exception("Unknown request type");
	}
}
