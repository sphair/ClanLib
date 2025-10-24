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

class CL_DataBuffer;
class CL_IODevice;
class CL_SocketName;
class CL_HTTPClientConnection_Impl;

/// \brief HTTP client connection.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPClientConnection
{
/// \name Construction
/// \{

public:
	/// \brief Construct a null instance
	CL_HTTPClientConnection();

	/// \brief Constructs a HTTPClientConnection
	///
	/// \param remote = Socket Name
	CL_HTTPClientConnection(const CL_SocketName &remote);

	/// \brief Constructs a HTTPClientConnection
	///
	/// \param remote = Socket Name
	/// \param local = Socket Name
	CL_HTTPClientConnection(const CL_SocketName &remote, const CL_SocketName &local);

	~CL_HTTPClientConnection();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

/// \}
/// \name Operations
/// \{

public:
	void send_post(
		const CL_StringRef8 &url,
		const CL_StringRef8 &header_lines,
		const CL_DataBuffer &post_data,
		bool use_http_10 = false);

	void send_get(
		const CL_StringRef8 &url,
		const CL_StringRef8 &header_lines,
		bool use_http_10 = false);

	int receive_response(
		CL_String8 &out_status_text,
		CL_String8 &out_header_lines,
		CL_DataBuffer &out_data);

	int receive_response(
		CL_String8 &out_status_text,
		CL_String8 &out_header_lines,
		CL_IODevice &out_data);

	static CL_StringRef8 get_header_value(
		const CL_StringRef8 &name,
		const CL_StringRef8 &header_lines);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_HTTPClientConnection_Impl> impl;
/// \}
};

/// \}
