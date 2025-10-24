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
#include "API/Network/Web/http_client_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/System/weakptr.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/logger.h"
#include <vector>

#define cl_min(a, b) ((a) < (b) ? (a) : (b))

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection_Impl Class:

class CL_HTTPClientConnection_Impl
{
//! Construction:
public:
	CL_HTTPClientConnection_Impl(const CL_SocketName &remote)
	: remote(remote)
	{
		worker_thread.start(this, &CL_HTTPClientConnection_Impl::worker_thread_main);
	}

	CL_HTTPClientConnection_Impl(const CL_SocketName &remote, const CL_SocketName &local)
	: remote(remote), local(local)
	{
		worker_thread.start(this, &CL_HTTPClientConnection_Impl::worker_thread_main);
	}

	~CL_HTTPClientConnection_Impl()
	{
		stop_event.set();
		worker_thread.join();
	}

//! Attributes:
public:
	CL_SocketName remote, local;

	CL_Thread worker_thread;

	CL_Event stop_event;

	CL_Event update_event;

	CL_Event created_event;

	CL_Mutex mutex;

	std::vector<CL_DataBuffer> send_messages;

	CL_TCPConnection connection;

//! Operations:
public:
	void worker_thread_main();

	int read_status(CL_String8 &out_status_text);

	bool read_lines(CL_String8 &out_header_lines);

	bool read_line(CL_String8 &out_line);
};

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection_IODeviceProvider Class:

class CL_HTTPClientConnection_IODeviceProvider : public CL_IODeviceProvider
{
//! Construction:
public:
	CL_HTTPClientConnection_IODeviceProvider(
		CL_WeakPtr<CL_HTTPClientConnection_Impl> impl,
		CL_StringRef8 content_length,
		CL_StringRef8 transfer_encoding)
	: impl(impl), chunked_encoding(false), http10(false), bytes_left(0)
	{
		if (transfer_encoding == "chunked")
		{
			chunked_encoding = true;
		}
		else if (transfer_encoding.empty())
		{
			if (content_length.empty())
				http10 = true;
			else
				bytes_left = CL_StringHelp::local8_to_int(content_length);
		}
		else
		{
			throw CL_Exception(cl_format("Unknown transfer encoding: %1", CL_StringHelp::local8_to_text(transfer_encoding)));
		}
	}

//! Operations:
public:
	virtual int send(const void *data, int len, bool send_all)
	{
		return -1;
	}

	virtual int receive(void *data, int len, bool receive_all)
	{
		if (http10)
			return impl->connection.receive(data, len, receive_all);

		char *_data = (char *) data;
		int pos = 0;
		if (bytes_left > 0)
		{
			int bytes_read = impl->connection.receive(_data, cl_min(bytes_left, len), receive_all);
			if (bytes_read <= 0)
				return bytes_read;
			bytes_left -= bytes_read;
			pos += bytes_read;
		}

		if (bytes_left == 0 && chunked_encoding)
		{
			CL_String8 str_chunk_size;
			if (impl->read_line(str_chunk_size) == false)
				throw CL_Exception("Premature end of HTTP response data");
			CL_String::size_type size_length = str_chunk_size.find(';');
			int chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
			if (chunk_size == 0)
				chunked_encoding = false;
			else
				bytes_left = chunk_size;

			if (bytes_left > 0)
				return pos + receive(_data + pos, len - pos, receive_all);
		}

		return pos;
	}

	virtual int peek(void *data, int len)
	{
		impl->connection.get_read_event().wait(15000);
		if (http10)
			return impl->connection.peek(data, len);

		int pos = 0;
		if (bytes_left > 0)
			return impl->connection.peek(data, cl_min(bytes_left, len));

		if (bytes_left == 0 && chunked_encoding)
		{
			CL_String8 str_chunk_size;
			if (impl->read_line(str_chunk_size) == false)
				throw CL_Exception("Premature end of HTTP response data");
			CL_String::size_type size_length = str_chunk_size.find(';');
			int chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
			if (chunk_size == 0)
				chunked_encoding = false;
			else
				bytes_left = chunk_size;

			if (bytes_left > 0)
				return peek(data, len);
		}

		return pos;
	}

	CL_IODeviceProvider *duplicate()
	{
		throw CL_Exception("CL_HTTPClientConnection_IODeviceProvider::duplicate() - duplicate() not supported.");
		return 0;
	}



//! Implementation:
private:
	CL_WeakPtr<CL_HTTPClientConnection_Impl> impl;

	bool chunked_encoding;

	bool http10;

	int bytes_left;
};

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection Construction:

CL_HTTPClientConnection::CL_HTTPClientConnection()
{
}

CL_HTTPClientConnection::CL_HTTPClientConnection(const CL_SocketName &remote)
: impl(new CL_HTTPClientConnection_Impl(remote))
{
}

CL_HTTPClientConnection::CL_HTTPClientConnection(const CL_SocketName &remote, const CL_SocketName &local)
: impl(new CL_HTTPClientConnection_Impl(remote, local))
{
}

CL_HTTPClientConnection::~CL_HTTPClientConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection Attributes:
void CL_HTTPClientConnection::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_HTTPClientConnection is null");
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection Operations:

void CL_HTTPClientConnection::send_post(
	const CL_StringRef8 &url,
	const CL_StringRef8 &header_lines_str,
	const CL_DataBuffer &post_data,
	bool use_http_10)
{
	static CL_StringRef8 text_post = "POST ";
	static CL_StringRef8 text_protocol10 = " HTTP/1.0\r\n";
	static CL_StringRef8 text_protocol11 = " HTTP/1.1\r\n";
	static CL_StringRef8 text_end_header = "\r\n\r\n";

	CL_StringRef8 text_protocol = (use_http_10 ? text_protocol10 : text_protocol11);

	// Remove any last newlines from header, if present
	CL_StringRef8 header_lines = header_lines_str;
	while (header_lines.length() >= 2 && header_lines.substr(header_lines.length()-2, 2) == "\r\n")
		header_lines = header_lines.substr(0, header_lines.length()-2);

	CL_DataBuffer message(
		text_post.length() + url.length() + text_protocol.length() +
		header_lines.length() + text_end_header.length() + post_data.get_size());

	int pos = 0;
	char *data = message.get_data();

	memcpy(data + pos, text_post.data(), text_post.length());
	pos += text_post.length();

	memcpy(data + pos, url.data(), url.length());
	pos += url.length();

	memcpy(data + pos, text_protocol.data(), text_protocol.length());
	pos += text_protocol.length();

	memcpy(data + pos, header_lines.data(), header_lines.length());
	pos += header_lines.length();

	memcpy(data + pos, text_end_header.data(), text_end_header.length());
	pos += text_end_header.length();

	memcpy(data + pos, post_data.get_data(), post_data.get_size());
	pos += post_data.get_size();

	if (pos != message.get_size())
		throw CL_Exception("Buffer overrun in CL_HTTPClientConnection::send_post!");

	CL_MutexSection mutex_lock(&impl->mutex);
	impl->send_messages.push_back(message);
	impl->update_event.set();
}

void CL_HTTPClientConnection::send_get(
	const CL_StringRef8 &url,
	const CL_StringRef8 &header_lines_str,
	bool use_http_10)
{
	static CL_StringRef8 text_get = "GET ";
	static CL_StringRef8 text_protocol10 = " HTTP/1.0\r\n";
	static CL_StringRef8 text_protocol11 = " HTTP/1.1\r\n";
	static CL_StringRef8 text_end_header = "\r\n\r\n";

	CL_StringRef8 text_protocol = (use_http_10 ? text_protocol10 : text_protocol11);

	// Remove any last newlines from header, if present
	CL_StringRef8 header_lines = header_lines_str;
	while (header_lines.length() >= 2 && header_lines.substr(header_lines.length()-2, 2) == "\r\n")
		header_lines = header_lines.substr(0, header_lines.length()-2);

	CL_DataBuffer message(
		text_get.length() + url.length() + text_protocol.length() +
		header_lines.length() + text_end_header.length());

	int pos = 0;
	char *data = message.get_data();

	memcpy(data + pos, text_get.data(), text_get.length());
	pos += text_get.length();

	memcpy(data + pos, url.data(), url.length());
	pos += url.length();

	memcpy(data + pos, text_protocol.data(), text_protocol.length());
	pos += text_protocol.length();

	memcpy(data + pos, header_lines.data(), header_lines.length());
	pos += header_lines.length();

	memcpy(data + pos, text_end_header.data(), text_end_header.length());
	pos += text_end_header.length();

	if (pos != message.get_size())
		throw CL_Exception("Buffer overrun in CL_HTTPClientConnection::send_get!");

	CL_MutexSection mutex_lock(&impl->mutex);
	impl->send_messages.push_back(message);
	impl->update_event.set();
}

int CL_HTTPClientConnection::receive_response(
	CL_String8 &out_status_text,
	CL_String8 &out_header_lines,
	CL_DataBuffer &out_data)
{
	out_status_text.clear();
	out_header_lines.clear();
	out_data.set_size(0);

	CL_Event::wait(impl->stop_event, impl->created_event);
	if (impl->created_event.wait(0) == false)
		return 0;

	int status = impl->read_status(out_status_text);
	if (status == 0)
		return 0;

	if (impl->read_lines(out_header_lines) == false)
		return 0;

	// todo: do not read response data for HEAD

	CL_StringRef8 content_length = get_header_value("Content-Length", out_header_lines);
	CL_StringRef8 transfer_encoding = get_header_value("Transfer-Encoding", out_header_lines);
	CL_StringRef8::size_type extension_pos = transfer_encoding.find_first_of(" \t\r\n;");
	if (extension_pos != CL_StringRef8::npos)
		transfer_encoding = transfer_encoding.substr(0, extension_pos);

	if (transfer_encoding == "chunked")
	{
		out_data.set_size(0);
		CL_String8 str_chunk_size;
		if (impl->read_line(str_chunk_size) == false)
			throw CL_Exception("Premature end of HTTP response data");
		CL_String8::size_type size_length = str_chunk_size.find(';');
		int chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
		while (chunk_size > 0)
		{
			int pos = out_data.get_size();
			out_data.set_size(pos + chunk_size);
			int bytes_read = impl->connection.receive(out_data.get_data()+pos, chunk_size, true);
			if (bytes_read != chunk_size)
			{
				out_data.set_size(0);
				throw CL_Exception("Premature end of HTTP response data");
			}
			char crlf[2];
			bytes_read = impl->connection.receive(crlf, 2, true);
			if (bytes_read != 2)
				throw CL_Exception("Premature end of HTTP response data");
			if (crlf[0] != '\r' || crlf[1] != '\n')
				throw CL_Exception("Expected CRLF after chunk in chunked encoding");

			if (impl->read_line(str_chunk_size) == false)
				throw CL_Exception("Premature end of HTTP response data");
			CL_String8::size_type size_length = str_chunk_size.find(';');
			chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
		}

		CL_String8 trailer;
		if (impl->read_lines(trailer) == false)
			throw CL_Exception("Premature end of HTTP response data");
	}
	else if (transfer_encoding.empty())
	{
		int length = CL_StringHelp::local8_to_int(content_length);
		out_data.set_size(length);
		int bytes_read = impl->connection.receive(out_data.get_data(), out_data.get_size(), true);
		if (bytes_read != length)
		{
			out_data.set_size(0);
			throw CL_Exception("Premature end of HTTP response data");
		}
	}
	else
	{
		throw CL_Exception(cl_format("Unknown transfer encoding: %1", CL_StringHelp::local8_to_text(transfer_encoding)));
	}

	return status;
}

int CL_HTTPClientConnection::receive_response(
	CL_String8 &out_status_text,
	CL_String8 &out_header_lines,
	CL_IODevice &out_data)
{
	out_status_text.clear();
	out_header_lines.clear();
	out_data = CL_IODevice();

	CL_Event::wait(impl->stop_event, impl->created_event);
	if (impl->created_event.wait(0) == false)
		return 0;

	int status = impl->read_status(out_status_text);
	if (status == 0)
		return 0;

	if (impl->read_lines(out_header_lines) == false)
		return 0;

	CL_StringRef8 content_length = get_header_value("Content-Length", out_header_lines);
	CL_StringRef8 transfer_encoding = get_header_value("Transfer-Encoding", out_header_lines);
	CL_StringRef8::size_type extension_pos = transfer_encoding.find_first_of(" \t\r\n;");
	if (extension_pos != CL_StringRef8::npos)
		transfer_encoding = transfer_encoding.substr(0, extension_pos);

	CL_IODevice device(
		new CL_HTTPClientConnection_IODeviceProvider(
			CL_WeakPtr<CL_HTTPClientConnection_Impl>(impl), content_length, transfer_encoding));
	out_data = device;
	return status;

/*
	if (transfer_encoding == "chunked")
	{
		out_data.set_size(0);
		CL_String8 str_chunk_size;
		if (impl->read_line(str_chunk_size) == false)
			throw CL_Exception("Premature end of HTTP response data");
		CL_String8::size_type size_length = str_chunk_size.find(';');
		int chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
		while (chunk_size > 0)
		{
			int pos = out_data.get_size();
			out_data.set_size(pos + chunk_size);
			int bytes_read = impl->connection.receive(out_data.get_data()+pos, chunk_size, true);
			if (bytes_read != chunk_size)
			{
				out_data.set_size(0);
				throw CL_Exception("Premature end of HTTP response data");
			}
			char crlf[2];
			bytes_read = impl->connection.receive(crlf, 2, true);
			if (bytes_read != 2)
				throw CL_Exception("Premature end of HTTP response data");
			if (crlf[0] != '\r' || crlf[1] != '\n')
				throw CL_Exception("Expected CRLF after chunk in chunked encoding");

			if (impl->read_line(str_chunk_size) == false)
				throw CL_Exception("Premature end of HTTP response data");
			CL_String8::size_type size_length = str_chunk_size.find(';');
			chunk_size = CL_StringHelp::local8_to_int(str_chunk_size.substr(0, size_length), 16);
		}

		CL_String8 trailer;
		if (impl->read_lines(trailer) == false)
			throw CL_Exception("Premature end of HTTP response data");
	}
	else if (transfer_encoding.empty())
	{
		int length = CL_StringHelp::local8_to_int(content_length);
		out_data.set_size(length);
		int bytes_read = impl->connection.receive(out_data.get_data(), out_data.get_size(), true);
		if (bytes_read != length)
		{
			out_data.set_size(0);
			throw CL_Exception("Premature end of HTTP response data");
		}
	}
	else
	{
		throw CL_Exception(cl_format("Unknown transfer encoding: %1", transfer_encoding));
	}
*/
}

CL_StringRef8 CL_HTTPClientConnection::get_header_value(
	const CL_StringRef8 &name,
	const CL_StringRef8 &header_lines)
{
	CL_StringRef8::size_type start, end;
	start = 0;
	while (true)
	{
		end = header_lines.find("\r\n", start);
		if (end == CL_StringRef8::npos)
			end = header_lines.length();
		if (start == end)
			break;

		CL_StringRef8 line = header_lines.substr(start, end-start);
		if (line.substr(0, name.length()) == name)
		{
			CL_StringRef8::size_type colon_pos = line.find_first_not_of(" \t", name.length());
			if (colon_pos != CL_StringRef8::npos && line[colon_pos] == ':')
			{
				CL_StringRef8::size_type value_pos = line.find_first_not_of(" \t", colon_pos+1);
				return line.substr(value_pos);
			}
		}

		if (end == header_lines.length())
			break;
		start = end + 2;
	}

	return CL_StringRef8();
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection Implementation:

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPClientConnection_Impl Operations:

void CL_HTTPClientConnection_Impl::worker_thread_main()
{
	try
	{
		connection.connect(remote, local);
		connection.set_nodelay(true);
		created_event.set();

		while (true)
		{
			int result = CL_Event::wait(stop_event, update_event);
			if (result <= 0)
				return;

			update_event.reset();
			while (true)
			{
				CL_MutexSection mutex_lock(&mutex);
				if (send_messages.empty())
					break;
				CL_DataBuffer message = send_messages.front();
				send_messages.erase(send_messages.begin());
				mutex_lock.unlock();

				int send_pos = 0;
				CL_Event write_event = connection.get_write_event();
				while (true)
				{
					result = CL_Event::wait(stop_event, write_event);
					if (result == 0)
						return;
					else if (result < 0 || result > 1)
						throw CL_Exception("Unexpected return value from CL_Event::wait");

					int bytes_sent = connection.send(
						message.get_data() + send_pos,
						message.get_size() - send_pos,
						false);
					if (bytes_sent <= 0)
					{
						stop_event.set();
						return;
					}

					send_pos += bytes_sent;
					if (send_pos == message.get_size())
						break;
				}
			}
		}
	}
	catch (const CL_Exception&)
	{
		stop_event.set();
	}
}

int CL_HTTPClientConnection_Impl::read_status(CL_String8 &out_status_text)
{
	out_status_text.clear();
	while (out_status_text.length() < 1024)
	{
		connection.get_read_event().wait(15000);

		char buffer[1024];
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		CL_StringRef8 str(buffer, bytes_read, false);
		CL_StringRef8::size_type pos = str.find("\r\n");
		if (pos == CL_StringRef8::npos)
		{
			connection.receive(buffer, 1024);
			out_status_text += str;
		}
		else
		{
			connection.receive(buffer, pos + 2);
			out_status_text += str.substr(0, pos);
			CL_StringRef8::size_type space_pos = out_status_text.find(' ');
			if (space_pos == CL_StringRef8::npos)
				return 0;
			CL_StringRef8 version = out_status_text.substr(0, space_pos);
//			if (version != "HTTP/1.1")
//				return 0;
			int status = CL_StringHelp::local8_to_int(out_status_text.substr(space_pos+1, 3));
			out_status_text = out_status_text.substr(space_pos+5);
			return status;
		}
	}
	return 0;
}

bool CL_HTTPClientConnection_Impl::read_line(CL_String8 &out_line)
{
	out_line.clear();
	while (out_line.length() < 1024)
	{
		connection.get_read_event().wait(15000);

		char buffer[1024];
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		CL_StringRef8 str(buffer, bytes_read, false);
		CL_StringRef8::size_type pos = str.find("\r\n");
		if (pos == CL_StringRef8::npos)
		{
			connection.receive(buffer, bytes_read);
			out_line += str;
		}
		else
		{
			connection.receive(buffer, pos + 2);
			out_line += str.substr(0, pos);
			return true;
		}
	}
	return false;
}

bool CL_HTTPClientConnection_Impl::read_lines(CL_String8 &out_header_lines)
{
	out_header_lines.clear();
	while (out_header_lines.length() < 32*1024)
	{
		connection.get_read_event().wait(15000);

		char buffer[1024];
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		CL_StringRef8 str(buffer, bytes_read, false);
		CL_StringRef8::size_type start_pos = out_header_lines.length();
		out_header_lines += str;
		CL_StringRef8::size_type pos = out_header_lines.find("\r\n\r\n");
		if (pos == CL_StringRef8::npos)
		{
			connection.receive(buffer, bytes_read);
		}
		else
		{
			connection.receive(buffer, pos+4-start_pos);
			out_header_lines.resize(pos+4);
			return true;
		}
	}
	return false;
}
