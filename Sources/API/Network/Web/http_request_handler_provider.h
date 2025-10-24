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

/// \addtogroup clanNetwork_Web clanNetwork Web
/// \{

#pragma once

#include "../api_network.h"

class CL_HTTPServerConnection;

/// \brief HTTP request handler provider interface.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPRequestHandlerProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_HTTPRequestHandlerProvider() { return; }

	/// \brief Destroy
	virtual void destroy() = 0;

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	virtual bool is_handling_request(
		const CL_StringRef8 &type,
		const CL_StringRef8 &url,
		const CL_StringRef8 &headers) = 0;

	/// \brief Handle request
	///
	/// \param connection = HTTPServer Connection
	virtual void handle_request(CL_HTTPServerConnection &connection) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
