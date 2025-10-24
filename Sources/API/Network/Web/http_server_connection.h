/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "../../Core/IOData/iodevice.h"

class CL_DataBuffer;
class CL_HTTPServerConnection_Impl;

/// \brief HTTP server connection.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPServerConnection : public CL_IODevice
{
/// \name Construction
/// \{

public:
	CL_HTTPServerConnection();

	CL_HTTPServerConnection(const CL_SharedPtr<CL_HTTPServerConnection_Impl> &impl);

	~CL_HTTPServerConnection();


/// \}
/// \name Attributes
/// \{

public:
	CL_StringRef8 get_request_type();

	CL_StringRef8 get_request_url();

	CL_StringRef8 get_request_headers();


/// \}
/// \name Operations
/// \{

public:
	CL_DataBuffer read_request_data();

	void write_response_status(int status_code, const CL_StringRef8 &status_text = CL_StringRef8());

	void write_response_headers(const CL_StringRef8 &headers);

	void write_response_data(const CL_DataBuffer &data);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_HTTPServerConnection_Impl> impl;
/// \}
};


/// \}
