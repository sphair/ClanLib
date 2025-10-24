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
#include "../../Core/System/sharedptr.h"

class CL_WebserviceMessage;
class CL_WebserviceClient_Impl;

/// \brief Web service caller class.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_WebserviceClient
{
/// \name Construction
/// \{

public:
	CL_WebserviceClient();

	/// \brief Constructs a WebserviceClient
	///
	/// \param url = String Ref
	CL_WebserviceClient(const CL_StringRef &url);

	~CL_WebserviceClient();

/// \}
/// \name Operations
/// \{

public:
	CL_WebserviceMessage invoke(
		const CL_StringRef &soap_action,
		const CL_WebserviceMessage &input);

	void send_request(
		const CL_StringRef &soap_action,
		const CL_WebserviceMessage &input);

	/// \brief Read response
	///
	/// \return Webservice Message
	CL_WebserviceMessage read_response();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_WebserviceClient_Impl> impl;
/// \}
};

/// \}
