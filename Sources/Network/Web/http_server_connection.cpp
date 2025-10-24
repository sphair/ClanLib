/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Network/precomp.h"
#include "API/Network/Web/http_server_connection.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/weakptr.h"
#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "http_server_connection_impl.h"
#include "http_server_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_HTTPServerConnection class:

class CL_IODeviceProvider_HTTPServerConnection : public CL_IODeviceProvider
{
//! Construction:
public:
	CL_IODeviceProvider_HTTPServerConnection(const CL_SharedPtr<CL_HTTPServerConnection_Impl> &impl)
	: impl(impl)
	{
	}

//! Attributes:
public:
	int get_size() const { return impl.lock()->connection.get_size(); }
	
	int get_position() const { return impl.lock()->connection.get_position(); }

//! Operations:
public:
	int send(const void *data, int len, bool send_all)
	{
		impl.lock()->performed_write = true;
		return impl.lock()->connection.send(data, len, send_all);
	}

	int receive(void *data, int len, bool receive_all)
	{
		impl.lock()->performed_read = true;
		return impl.lock()->connection.receive(data, len, receive_all);
	}

	int peek(void *data, int len)
	{
		return impl.lock()->connection.peek(data, len);
	}

	bool seek(int position, CL_IODevice::SeekMode mode)
	{
		return impl.lock()->connection.seek(position, mode);
	}

	CL_IODeviceProvider *duplicate()
	{
		return 0;
	}

//! Implementation:
private:
	CL_WeakPtr<CL_HTTPServerConnection_Impl> impl;
};

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServerConnection Construction:

CL_HTTPServerConnection::CL_HTTPServerConnection()
{
}

CL_HTTPServerConnection::CL_HTTPServerConnection(const CL_SharedPtr<CL_HTTPServerConnection_Impl> &impl)
: CL_IODevice(new CL_IODeviceProvider_HTTPServerConnection(impl)), impl(impl)
{
}

CL_HTTPServerConnection::~CL_HTTPServerConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServerConnection Attributes:


void CL_HTTPServerConnection::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_HTTPServerConnection is null");
}

CL_StringRef8 CL_HTTPServerConnection::get_request_type()
{
	return impl->request_type;
}

CL_StringRef8 CL_HTTPServerConnection::get_request_url()
{
	return impl->request_url;
}

CL_StringRef8 CL_HTTPServerConnection::get_request_headers()
{
	return impl->request_headers;
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServerConnection Operations:

CL_DataBuffer CL_HTTPServerConnection::read_request_data()
{
	if (impl->request_read)
		return impl->request_data;
	if (impl->performed_read)
		throw CL_Exception("Cannot read request data if manual reading has been performed first.");

	impl->request_read = true;

	if (impl->request_type == "POST")
	{
		CL_StringRef8 content_length = impl->get_header_value("Content-Length", impl->request_headers);
		CL_StringRef8 transfer_encoding = impl->get_header_value("Transfer-Encoding", impl->request_headers);
		CL_StringRef8::size_type extension_pos = transfer_encoding.find_first_of(" \t\r\n;");
		if (extension_pos != CL_StringRef8::npos)
			transfer_encoding = transfer_encoding.substr(0, extension_pos);

		if (transfer_encoding == "chunked")
		{
			impl->request_data.set_size(0);
			CL_String8 str_chunk_size;
			if (CL_HTTPServer_Impl::read_line(impl->connection, str_chunk_size) == false)
				throw CL_Exception("Premature end of HTTP response data");
			CL_String8::size_type size_length = str_chunk_size.find(';');
			int chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
			while (chunk_size > 0)
			{
				int pos = impl->request_data.get_size();
				impl->request_data.set_size(pos + chunk_size);
				int bytes_read = impl->connection.receive(impl->request_data.get_data()+pos, chunk_size, true);
				if (bytes_read != chunk_size)
				{
					impl->request_data.set_size(0);
					throw CL_Exception("Premature end of HTTP response data");
				}
				char crlf[2];
				bytes_read = impl->connection.receive(crlf, 2, true);
				if (bytes_read != 2)
					throw CL_Exception("Premature end of HTTP response data");
				if (crlf[0] != '\r' || crlf[1] != '\n')
					throw CL_Exception("Expected CRLF after chunk in chunked encoding");

				if (CL_HTTPServer_Impl::read_line(impl->connection, str_chunk_size) == false)
					throw CL_Exception("Premature end of HTTP response data");
				CL_String8::size_type size_length = str_chunk_size.find(';');
				chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
			}

			CL_String8 trailer;
			if (CL_HTTPServer_Impl::read_lines(impl->connection, trailer) == false)
				throw CL_Exception("Premature end of HTTP response data");
		}
		else if (transfer_encoding.empty())
		{
			int length = CL_StringHelp::local8_to_int(content_length);
			impl->request_data.set_size(length);
			int bytes_read = impl->connection.receive(impl->request_data.get_data(), impl->request_data.get_size(), true);
			if (bytes_read != length)
			{
				impl->request_data.set_size(0);
				throw CL_Exception("Premature end of HTTP response data");
			}
		}
		else
		{
			throw CL_Exception(cl_format("Unknown transfer encoding: %1", CL_StringHelp::local8_to_text(transfer_encoding)));
		}
	}

	return impl->request_data;
}

void CL_HTTPServerConnection::write_response_status(int status_code, const CL_StringRef8 &status_text)
{
	if (impl->performed_write)
		throw CL_Exception("Cannot write reponse status if manual writing has been performed first.");

	CL_String8 status_line;
	status_line.append("HTTP/1.1 ");
	status_line.append(CL_StringHelp::int_to_local8(status_code));
	status_line.append(" ");
	status_line.append(status_text);
	status_line.append("\r\n");
	impl->connection.write(status_line.data(), status_line.length(), true);
}

void CL_HTTPServerConnection::write_response_headers(const CL_StringRef8 &headers)
{
	if (impl->performed_write)
		throw CL_Exception("Cannot write reponse headers if manual writing has been performed first.");

	// Analyze and print headers line by line:

	bool server_line = false;
	bool connection_line = false;
	bool date_line = false;
	bool expires_line = false;
	bool vary_line = false;

	CL_StringRef8::size_type start_pos = 0;
	while (start_pos != CL_StringRef8::npos)
	{
		CL_StringRef8::size_type end_pos = headers.find('\n', start_pos);
		CL_StringRef8 line;
		if (end_pos == CL_StringRef8::npos)
		{
			line = headers.substr(start_pos);
			start_pos = CL_StringRef8::npos;
		}
		else
		{
			if (end_pos > start_pos && headers[end_pos - 1] == '\r')
				line = headers.substr(start_pos, end_pos - start_pos - 1);
			else
				line = headers.substr(start_pos, end_pos - start_pos);
			start_pos = end_pos + 1;
		}
		if (line.length() > 0)
		{
			CL_StringRef8 name;
			CL_StringRef8::size_type pos = line.find(':');
			if (pos != CL_StringRef::npos)
				name = line.substr(0, pos);

			if (name == "Server")
				server_line = true;
			else if (name == "Connection")
				connection_line = true;
			else if (name == "Date")
				date_line = true;
			else if (name == "Expires")
				expires_line = true;
			else if (name == "Vary")
				vary_line = true;

			impl->connection.write(line.data(), line.length(), true);
			impl->connection.write("\r\n", 2, true);
		}
	}

	static CL_StringRef8 str_server_line("Server: ClanLib HTTP Server\r\n");
	static CL_StringRef8 str_connection_line("Connection: close\r\n");
	static CL_StringRef8 str_vary_line("Vary: *\r\n");
	if (!server_line)
		impl->connection.write(str_server_line.data(), str_server_line.length(), true);
	if (!connection_line)
		impl->connection.write(str_connection_line.data(), str_connection_line.length(), true);
	if (!date_line && !expires_line && !vary_line)
		impl->connection.write(str_vary_line.data(), str_vary_line.length(), true);
//	write_line(connection, "Date: Sun, 16 Oct 2005 20:13:00 GMT");
//	write_line(connection, "Expires: Sun, 16 Oct 2005 20:13:00 GMT");

	impl->writing_header = true;
}

void CL_HTTPServerConnection::write_response_data(const CL_DataBuffer &data)
{
	// Make sure HTTP headers are written and sane:
	if (impl->performed_write)
		throw CL_Exception("Cannot write reponse data if manual writing has been performed first.");
	if (!impl->writing_header)
		write_response_headers(CL_StringRef8());
	if (impl->writing_header)
	{
		if (impl->written_content_length == -1)
		{
			CL_String8 length;
			length.append("Content-Length: ");
			length.append(CL_StringHelp::int_to_local8(data.get_size()));
			length.append("\r\n");
			impl->connection.write(length.data(), length.length(), true);
		}
		impl->connection.write("\r\n", 2, true);
	}
	impl->writing_header = false;
	if (impl->written_content_length >= 0 && data.get_size() != impl->written_content_length)
		throw CL_Exception("HTTP Content-Length in header does not match response data size!");

	// Header should be ok.  Write the actual data:
	impl->connection.write(data.get_data(), data.get_size(), true);
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServerConnection Implementation:
