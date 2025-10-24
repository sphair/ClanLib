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

/// \addtogroup clanNetwork_Web clanNetwork Web
/// \{

#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_SocketName;
class CL_HTTPRequestHandler;
class CL_HTTPServer_Impl;

/// \brief HTTP server.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPServer
{
/// \name Construction
/// \{

public:
	CL_HTTPServer();

	~CL_HTTPServer();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

	/// \brief Bind
	///
	/// \param name = Socket Name
	void bind(const CL_SocketName &name);

	/// \brief Add handler
	///
	/// \param handler = HTTPRequest Handler
	void add_handler(const CL_HTTPRequestHandler &handler);

	/// \brief Remove handler
	///
	/// \param handler = HTTPRequest Handler
	void remove_handler(const CL_HTTPRequestHandler &handler);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_HTTPServer_Impl> impl;
/// \}
};

/// \}
