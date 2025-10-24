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
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/logger.h"
#include "API/Network/Web/http_server_connection.h"
#include "http_server_impl.h"
#include "http_server_connection_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer_Impl Construction:

CL_HTTPServer_Impl::CL_HTTPServer_Impl()
{
	accept_thread.start(this, &CL_HTTPServer_Impl::accept_thread_main);
}

CL_HTTPServer_Impl::~CL_HTTPServer_Impl()
{
	stop_event.set();
	accept_thread.join();
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer_Impl Operations:

void CL_HTTPServer_Impl::write_line(CL_TCPConnection &connection, const CL_String8 &line)
{
	connection.send(line.data(), line.length(), true);
	connection.send("\r\n", 2, true);
}

bool CL_HTTPServer_Impl::read_line(CL_TCPConnection &connection, CL_String8 &out_line)
{
	out_line.clear();
	while (out_line.length() < 1024)
	{
		char buffer[1024];
		if (connection.get_read_event().wait(15000) == false)
			throw CL_Exception("Read timed out");
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

bool CL_HTTPServer_Impl::read_lines(CL_TCPConnection &connection, CL_String8 &out_header_lines)
{
	out_header_lines.clear();
	while (out_header_lines.length() < 32*1024)
	{
		char buffer[1024];
		if (connection.get_read_event().wait(15000) == false)
			throw CL_Exception("Read timed out");
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

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer_Impl Implementation:

void CL_HTTPServer_Impl::accept_thread_main()
{
	while (true)
	{
		CL_MutexSection mutex_lock(&mutex);
		std::vector<CL_Event> events;
		events.push_back(stop_event);
		events.push_back(update_event);
		std::vector<CL_TCPListen>::size_type i;
		for (i = 0; i < listen_ports.size(); i++)
			events.push_back(listen_ports[i].get_accept_event());

		mutex_lock.unlock();
		int result = CL_Event::wait(events);
		if (result <= 0)
			break;
		mutex_lock.lock();
		if (update_event.wait(0))
		{
			update_event.reset();
			continue;
		}

		CL_Thread connection_thread;
		connection_thread.start(
			this,
			&CL_HTTPServer_Impl::connection_thread_main,
			listen_ports[result-2].accept());
	}
}

void CL_HTTPServer_Impl::connection_thread_main(CL_TCPConnection connection)
{
	try
	{
		CL_String8 request;
		bool bool_result = read_line(connection, request);
		if (bool_result == false)
		{
			connection.disconnect_abortive();
			return;
		}

		CL_String8 headers;
		bool_result = read_lines(connection, headers);
		if (bool_result == false)
		{
			connection.disconnect_abortive();
			return;
		}

		// Extract request command, url and version:

		CL_StringRef8 command, url, version;
		CL_String8::size_type pos1 = request.find(' ');
		if (pos1 == CL_String8::npos)
			throw CL_Exception("Bad request");
		command = request.substr(0, pos1);
		if (command != "POST" && command != "GET")
			throw CL_Exception("Unsupported");
		CL_String8::size_type pos2 = request.find(' ', pos1 + 1);
		if (pos2 == CL_String8::npos)
			throw CL_Exception("Bad request");
		url = request.substr(pos1+1, pos2-pos1-1);
		CL_String8::size_type pos3 = request.find(' ', pos2 + 1);
		if (pos3 != CL_String8::npos)
			throw CL_Exception("Bad request");
		version = request.substr(pos2 + 1);

		CL_DataBuffer request_data;

		// Handle request:

		// Look for a request handler that will deal with the HTTP request:
		CL_MutexSection mutex_lock(&mutex);
		std::vector<CL_HTTPRequestHandler>::size_type index, size;
		size = handlers.size();
		bool handled = false;
		for (index = 0; index < size; index++)
		{
			if (handlers[index].is_handling_request(command, url, headers))
			{
				CL_HTTPRequestHandler handler = handlers[index];
				mutex_lock.unlock();

				if (command == "POST")
				{
					write_line(connection, "HTTP/1.1 100 Continue");
					// write_line(connection, "Content-Length: 0");
					write_line(connection, "");
				}

				CL_SharedPtr<CL_HTTPServerConnection_Impl> connection_impl(new CL_HTTPServerConnection_Impl);
				connection_impl->connection = connection;
				connection_impl->request_type = command;
				connection_impl->request_url = url;
				connection_impl->request_headers = headers;
				CL_HTTPServerConnection http_connection(connection_impl);
				handler.handle_request(http_connection);
				handled = true;
				break;
			}
		}
		mutex_lock.unlock();

		if (!handled)
		{
			// No handler wants it.  Reply with 404 Not Found:
			CL_StringRef8 error_msg("404 Not Found");
			write_line(connection, "HTTP/1.1 404 Not Found");
			write_line(connection, "Server: ClanLib HTTP Server");
			write_line(connection, "Connection: close");
			write_line(connection, "Vary: *");
			write_line(connection, "Content-Type: text/plain");
			write_line(connection, "Content-Length: " + CL_StringHelp::int_to_local8(error_msg.length()+2));
			write_line(connection, "");
			write_line(connection, error_msg);
		}

		connection.disconnect_graceful();

/*
		if (url == "/")
		{
			CL_File file("Sources/test.html", CL_File::open_existing);
			CL_DataBuffer response(file.get_size());
			file.read(response.get_data(), response.get_size(), true);
			file.close();

			// Send back response.

			write_line(connection, "HTTP/1.1 200 OK");
			write_line(connection, "Server: ClanLib HTTP Server");
			write_line(connection, "Connection: close");
//			write_line(connection, "Date: Sun, 16 Oct 2005 20:13:00 GMT");
//			write_line(connection, "Expires: Sun, 16 Oct 2005 20:13:00 GMT");
			write_line(connection, "Vary: *");
//			write_line(connection, "ETag: \"foobar\"");
			write_line(connection, "Content-Type: text/html");
			write_line(connection, "Content-Length: " + CL_StringHelp::int_to_local8(response.get_size()));
			write_line(connection, "");
			connection.send(response.get_data(), response.get_size(), true);
		}
		else
		{
			CL_DataBuffer response;
			bool error = false;
			try
			{
				response = handle_request(url, headers, request_data);
			}
			catch (const CL_Exception &e)
			{
//					write_line(connection, "HTTP/1.1 404 Not Found");
				write_line(connection, "HTTP/1.1 404 Not Found");
				write_line(connection, "Server: ClanLib HTTP Server");
				write_line(connection, "Connection: close");
//				write_line(connection, "Date: Sun, 16 Oct 2005 20:13:00 GMT");
//				write_line(connection, "Expires: Sun, 16 Oct 2005 20:13:00 GMT");
				write_line(connection, "Vary: *");
//				write_line(connection, "ETag: \"foobar\"");
				write_line(connection, "Content-Type: text/plain");
				write_line(connection, "Content-Length: 0");
				write_line(connection, "");
				error = true;
			}

			// Send back response.

			if (!error)
			{
				write_line(connection, "HTTP/1.1 200 OK");
				write_line(connection, "Server: ClanLib HTTP Server");
				write_line(connection, "Connection: close");
//				write_line(connection, "Date: Sun, 16 Oct 2005 20:13:00 GMT");
//				write_line(connection, "Expires: Sun, 16 Oct 2005 20:13:00 GMT");
				write_line(connection, "Vary: *");
//				write_line(connection, "ETag: \"foobar\"");
				write_line(connection, "Content-Type: text/xml");
				write_line(connection, "Content-Length: " + CL_StringHelp::int_to_local8(response.get_size()));
				write_line(connection, "");
				connection.send(response.get_data(), response.get_size(), true);
			}
		}
*/
	}
	catch (const CL_Exception& e)
	{
		cl_log_event("error", e.message);
	}
}
