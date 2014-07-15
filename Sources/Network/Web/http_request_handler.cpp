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
#include "API/Network/Web/http_request_handler.h"
#include "API/Network/Web/http_request_handler_provider.h"
#include "http_request_handler_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// HTTPRequestHandler Construction:

HTTPRequestHandler::HTTPRequestHandler()
{
}

HTTPRequestHandler::HTTPRequestHandler(HTTPRequestHandlerProvider *provider)
: impl(std::make_shared<HTTPRequestHandler_Impl>())
{
	impl->provider = provider;
}

HTTPRequestHandler::~HTTPRequestHandler()
{
}

/////////////////////////////////////////////////////////////////////////////
// HTTPRequestHandler Attributes:

HTTPRequestHandlerProvider *HTTPRequestHandler::get_provider() const
{
	return impl->provider;
}

void HTTPRequestHandler::throw_if_null() const
{
	if (!impl)
		throw Exception("HTTPRequestHandler is null");
}

/////////////////////////////////////////////////////////////////////////////
// HTTPRequestHandler Operations:

bool HTTPRequestHandler::is_handling_request(
	const std::string &type,
	const std::string &url,
	const std::string &headers)
{
	if (impl && impl->provider)
		return impl->provider->is_handling_request(type, url, headers);
	return false;
}

void HTTPRequestHandler::handle_request(HTTPServerConnection &connection)
{
	if (impl && impl->provider)
		return impl->provider->handle_request(connection);
}

/////////////////////////////////////////////////////////////////////////////
// HTTPRequestHandler Implementation:

}
