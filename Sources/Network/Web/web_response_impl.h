
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

namespace clan
{

class WebResponse_Impl
{
public:
	void retrieve();
	void send_request(TCPConnection &connection);
	void read_response(TCPConnection &connection);
	void read_header(TCPConnection &connection, RingBuffer &buffer);
	std::string get_request_keyword();
	void write_to_buffer(TCPConnection &connection, RingBuffer &buffer);

	static void throw_if_null(const std::shared_ptr<WebResponse_Impl> &impl)
	{
		if (!impl)
			throw Exception("WebResponse object is null");
	}

	std::shared_ptr<WebRequest_Impl> request;
	int status_code;
	std::string status_text;
	std::map<std::string, std::string> headers;
	DataBuffer body;
};

inline void WebResponse_Impl::send_request(TCPConnection &connection)
{
	request->headers["Host"] = request->url.host;
	request->headers["Connection"] = "close";
	if (request->type == WebRequest_Impl::type_post || request->type == WebRequest_Impl::type_trace)
	{
		if (request->body.is_null())
			request->headers["Content-Length"] = "0";
		else
			request->headers["Content-Length"] = string_format("%1", request->body.get_size());
	}

	std::string request_header = string_format("%1 %2%3 HTTP/1.1\r\n", get_request_keyword(), request->url.path, request->url.query);
	for (std::map<std::string, std::string>::iterator it = request->headers.begin(); it != request->headers.end(); ++it)
		request_header += string_format("%1: %2\r\n", it->first, it->second);
	request_header += "\r\n";

	connection.send(request_header.data(), request_header.length(), true);
	connection.set_nodelay(true);
	if (!request->body.is_null())
		connection.send(request->body.get_data(), request->body.get_size(), true);
	else
		connection.send(nullptr, 0);
}

inline void WebResponse_Impl::read_response(TCPConnection &connection)
{
	RingBuffer buffer(16*1024);
	read_header(connection, buffer);

	if (request->type != WebRequest_Impl::type_head)
	{
		if (headers.find("Transfer-Encoding") != headers.end())
		{
			if (headers["Transfer-Encoding"] != "chunked")
				throw Exception("Unsupported transfer encoding");

			IODevice_Memory memory_device;
			while (true)
			{
				size_t header_length = buffer.find("\r\n", 2);
				while (header_length == RingBuffer::npos)
				{
					write_to_buffer(connection, buffer);
					header_length = buffer.find("\r\n", 2);
				}

				std::string chunk_header = buffer.read_to_string(header_length);
				buffer.read(2);
				size_t size_end_pos = chunk_header.find_first_of("; \t\r\n");
				size_t length = StringHelp::text_to_uint(chunk_header.substr(0, size_end_pos), 16);
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
					src_available = min(src_available, length - bytes_read);
					memory_device.write(src, src_available);
					buffer.read(src_available);
					bytes_read += src_available;
				}

				size_t crlf = buffer.find("\r\n", 2);
				while (crlf == RingBuffer::npos)
				{
					write_to_buffer(connection, buffer);
					crlf = buffer.find("\r\n", 2);
				}
				if (crlf != 0)
					throw Exception("Missing CRLF in HTTP chunked encoded data");
				buffer.read(2);
			}

			// Read trailer
			while (true)
			{
				size_t line_length = buffer.find("\r\n", 2);
				while (line_length == RingBuffer::npos)
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
			IODevice_Memory memory_device;
			size_t length = StringHelp::text_to_uint(headers["Content-Length"]);
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
				src_available = min(src_available, length - bytes_read);
				memory_device.write(src, src_available);
				buffer.read(src_available);
				bytes_read += src_available;
			}
			body = memory_device.get_data();
		}
		else
		{
			IODevice_Memory memory_device;
			while (true)
			{
				const char *src = buffer.get_read_pos();
				size_t src_available = buffer.get_read_size();
				if (src_available == 0)
				{
					if (!connection.get_read_event().wait(15000))
						throw Exception("Connection timed out");
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

inline void WebResponse_Impl::write_to_buffer(TCPConnection &connection, RingBuffer &buffer)
{
	if (buffer.get_write_size() == 0)
		throw Exception("Ring buffer is full");
	if (!connection.get_read_event().wait(15000))
		throw Exception("Connection timed out");
	int received = connection.read(buffer.get_write_pos(), buffer.get_write_size(), false);
	if (received == 0)
		throw Exception("Invalid HTTP response header");
	buffer.write(received);
}

inline void WebResponse_Impl::read_header(TCPConnection &connection, RingBuffer &buffer)
{
	size_t header_length = RingBuffer::npos;
	while (header_length == RingBuffer::npos)
	{
		write_to_buffer(connection, buffer);
		header_length = buffer.find("\r\n\r\n", 4);
	}

	std::string header = buffer.read_to_string(header_length);
	buffer.read(4);
	std::vector<std::string> lines = StringHelp::split_text(header, "\n");
	for (size_t i = 0; i < lines.size(); i++)
		lines[i] = StringHelp::trim(lines[i]);
	if (lines.size() < 1)
		throw Exception("Invalid HTTP response header");

	std::string::size_type index_space1 = lines[0].find_first_of(' ');
	std::string::size_type index_space2 = lines[0].find_first_of(' ', index_space1 == std::string::npos ? index_space1 : index_space1 + 1);

	std::string protocol = lines[0].substr(0, index_space1);
	if (index_space1 == std::string::npos || index_space2 == std::string::npos)
		throw Exception("Invalid HTTP 1.1 response status line");

	status_code = StringHelp::text_to_int(lines[0].substr(index_space1+1, index_space2-index_space1-1));
	status_text = lines[0].substr(index_space2+1);

	for (size_t i = 1; i < lines.size(); i++)
	{
		std::string::size_type index_colon = lines[i].find_first_of(':');
		if (index_colon == std::string::npos)
			throw Exception("Invalid HTTP response header line");
		std::string field_name = StringHelp::trim(lines[i].substr(0, index_colon));
		std::string field_value = StringHelp::trim(lines[i].substr(index_colon+1));

		std::map<std::string, std::string>::iterator it = headers.find(field_name);
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

inline void WebResponse_Impl::retrieve()
{
	TCPConnection connection(SocketName(request->url.host, request->url.port));
	send_request(connection);
	read_response(connection);
	if (status_code == 100) // Skip 100 Continue responses
		read_response(connection);

/*
	request = string_format("GET %1 HTTP/1.1\r\n", url.path+url.query);
	request += string_format("Host: %1\r\nConnection: close\r\nReferer: %2\r\nAccept: image/png, image/jpeg\r\nUser-Agent: CSSTokenize/0.1\r\n\r\n", url.host, pageurl.to_string());

	TCPConnection connection(SocketName(url.host, url.port));
	connection.set_nodelay(true);
	connection.send(request.data(), request.length(), true);

	std::string response;
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

inline std::string WebResponse_Impl::get_request_keyword()
{
	switch (request->type)
	{
	case WebRequest_Impl::type_options:
		return "OPTIONS";
	case WebRequest_Impl::type_get:
		return "GET";
	case WebRequest_Impl::type_head:
		return "HEAD";
	case WebRequest_Impl::type_post:
		return "POST";
	case WebRequest_Impl::type_put:
		return "PUT";
	case WebRequest_Impl::type_delete:
		return "DELETE";
	case WebRequest_Impl::type_trace:
		return "TRACE";
	default:
		throw Exception("Unknown request type");
	}
}

}
