/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Network/Web/http_server.h"
#include "http_server_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer Construction:

CL_HTTPServer::CL_HTTPServer()
: impl(new CL_HTTPServer_Impl)
{
}

CL_HTTPServer::~CL_HTTPServer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer Operations:

void CL_HTTPServer::bind(const CL_SocketName &name)
{
	CL_TCPListen tcp_listen(name);
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->listen_ports.push_back(tcp_listen);
	impl->update_event.set();
}

void CL_HTTPServer::add_handler(const CL_HTTPRequestHandler &handler)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->handlers.push_back(handler);
}

void CL_HTTPServer::remove_handler(const CL_HTTPRequestHandler &handler)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	std::vector<CL_HTTPRequestHandler>::size_type index, size;
	size = impl->handlers.size();
	for (index = 0; index < size; index++)
	{
		impl->handlers.erase(impl->handlers.begin() + index);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_HTTPServer Implementation:
