/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#pragma once

#include "API/Network/Web/http_request_handler.h"
#include "API/Network/Socket/tcp_listen.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/System/event.h"
#include <vector>
#include <mutex>
#include <thread>

namespace clan
{

class HTTPServer_Impl
{
/// \name Construction
/// \{

public:
	HTTPServer_Impl();

	~HTTPServer_Impl();


/// \}
/// \name Attributes
/// \{

public:
	std::recursive_mutex mutex;

	std::thread accept_thread;

	Event stop_event, update_event;

	std::vector<HTTPRequestHandler> handlers;

	std::vector<TCPListen> listen_ports;


/// \}
/// \name Operations
/// \{

public:
	static void write_line(TCPConnection &connection, const std::string &line);

	static bool read_line(TCPConnection &connection, std::string &out_line);

	static bool read_lines(TCPConnection &connection, std::string &out_header_lines);


/// \}
/// \name Implementation
/// \{

private:
	void accept_thread_main();

	void connection_thread_main(TCPConnection connection);
/// \}
};

}
