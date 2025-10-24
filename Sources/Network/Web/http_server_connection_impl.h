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

#pragma once


#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/System/databuffer.h"

class CL_HTTPServerConnection_Impl
{
/// \name Construction
/// \{

public:
	CL_HTTPServerConnection_Impl();

	~CL_HTTPServerConnection_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_TCPConnection connection;

	CL_StringRef8 request_type;

	CL_StringRef8 request_url;

	CL_StringRef8 request_headers;

	CL_DataBuffer request_data;

	bool request_read;

	bool performed_read, performed_write;

	bool writing_header;

	cl_int64 written_content_length;


/// \}
/// \name Operations
/// \{

public:
	static CL_StringRef8 get_header_value(
		const CL_StringRef8 &name,
		const CL_StringRef8 &header_lines);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


