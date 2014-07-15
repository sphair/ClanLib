/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// HTTPServer_Impl Construction:

HTTPServer_Impl::HTTPServer_Impl()
{
	accept_thread.start(this, &HTTPServer_Impl::accept_thread_main);
}

HTTPServer_Impl::~HTTPServer_Impl()
{
	stop_event.set();
	accept_thread.join();
}

/////////////////////////////////////////////////////////////////////////////
// HTTPServer_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// HTTPServer_Impl Operations:

void HTTPServer_Impl::write_line(TCPConnection &connection, const std::string &line)
{
	connection.send(line.data(), line.length(), true);
	connection.send("\r\n", 2, true);
}

bool HTTPServer_Impl::read_line(TCPConnection &connection, std::string &out_line)
{
	out_line.clear();
	while (out_line.length() < 1024)
	{
		char buffer[1024];
		if (connection.get_read_event().wait(15000) == false)
			throw Exception("Read timed out");
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		std::string str(buffer, bytes_read);
		std::string::size_type pos = str.find("\r\n");
		if (pos == std::string::npos)
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

bool HTTPServer_Impl::read_lines(TCPConnection &connection, std::string &out_header_lines)
{
	out_header_lines.clear();
	while (out_header_lines.length() < 32*1024)
	{
		char buffer[1024];
		if (connection.get_read_event().wait(15000) == false)
			throw Exception("Read timed out");
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		std::string str(buffer, bytes_read);
		std::string::size_type start_pos = out_header_lines.length();
		out_header_lines += str;
		std::string::size_type pos = out_header_lines.find("\r\n\r\n");
		if (pos == std::string::npos)
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
// HTTPServer_Impl Implementation:

void HTTPServer_Impl::accept_thread_main()
{
	while (true)
	{
		MutexSection mutex_lock(&mutex);
		std::vector<Event> events;
		events.push_back(stop_event);
		events.push_back(update_event);
		std::vector<TCPListen>::size_type i;
		for (i = 0; i < listen_ports.size(); i++)
			events.push_back(listen_ports[i].get_accept_event());

		mutex_lock.unlock();
		int result = Event::wait(events);
		if (result <= 0)
			break;
		mutex_lock.lock();
		if (update_event.wait(0))
		{
			update_event.reset();
			continue;
		}

		Thread connection_thread;
		connection_thread.start(
			this,
			&HTTPServer_Impl::connection_thread_main,
			listen_ports[result-2].accept());
	}
}

void HTTPServer_Impl::connection_thread_main(TCPConnection connection)
{
	try
	{
		std::string request;
		bool bool_result = read_line(connection, request);
		if (bool_result == false)
		{
			connection.disconnect_abortive();
			return;
		}

		std::string headers;
		bool_result = read_lines(connection, headers);
		if (bool_result == false)
		{
			connection.disconnect_abortive();
			return;
		}

		// Extract request command, url and version:

		std::string command, url, version;
		std::string::size_type pos1 = request.find(' ');
		if (pos1 == std::string::npos)
			throw Exception("Bad request");
		command = request.substr(0, pos1);
		if (command != "POST" && command != "GET")
			throw Exception("Unsupported");
		std::string::size_type pos2 = request.find(' ', pos1 + 1);
		if (pos2 == std::string::npos)
			throw Exception("Bad request");
		url = request.substr(pos1+1, pos2-pos1-1);
		std::string::size_type pos3 = request.find(' ', pos2 + 1);
		if (pos3 != std::string::npos)
			throw Exception("Bad request");
		version = request.substr(pos2 + 1);

		DataBuffer request_data;

		// Handle request:

		// Look for a request handler that will deal with the HTTP request:
		MutexSection mutex_lock(&mutex);
		std::vector<HTTPRequestHandler>::size_type index, size;
		size = handlers.size();
		bool handled = false;
		for (index = 0; index < size; index++)
		{
			if (handlers[index].is_handling_request(command, url, headers))
			{
				HTTPRequestHandler handler = handlers[index];
				mutex_lock.unlock();

				if (command == "POST")
				{
					write_line(connection, "HTTP/1.1 100 Continue");
					// write_line(connection, "Content-Length: 0");
					write_line(connection, "");
				}

				std::shared_ptr<HTTPServerConnection_Impl> connection_impl(std::make_shared<HTTPServerConnection_Impl>());
				connection_impl->connection = connection;
				connection_impl->request_type = command;
				connection_impl->request_url = url;
				connection_impl->request_headers = headers;
				HTTPServerConnection http_connection(connection_impl);
				handler.handle_request(http_connection);
				handled = true;
				break;
			}
		}
		mutex_lock.unlock();

		if (!handled)
		{
			// No handler wants it.  Reply with 404 Not Found:
			std::string error_msg("404 Not Found");
			write_line(connection, "HTTP/1.1 404 Not Found");
			write_line(connection, "Server: ClanLib HTTP Server");
			write_line(connection, "Connection: close");
			write_line(connection, "Vary: *");
			write_line(connection, "Content-Type: text/plain");
			write_line(connection, "Content-Length: " + StringHelp::int_to_local8(error_msg.length()+2));
			write_line(connection, "");
			write_line(connection, error_msg);
		}

		connection.disconnect_graceful();

/*
		if (url == "/")
		{
			File file("Sources/test.html", File::open_existing);
			DataBuffer response(file.get_size());
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
			write_line(connection, "Content-Length: " + StringHelp::int_to_local8(response.get_size()));
			write_line(connection, "");
			connection.send(response.get_data(), response.get_size(), true);
		}
		else
		{
			DataBuffer response;
			bool error = false;
			try
			{
				response = handle_request(url, headers, request_data);
			}
			catch (const Exception &e)
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
				write_line(connection, "Content-Length: " + StringHelp::int_to_local8(response.get_size()));
				write_line(connection, "");
				connection.send(response.get_data(), response.get_size(), true);
			}
		}
*/
	}
	catch (const Exception& e)
	{
		log_event("error", e.message);
	}
}

}
