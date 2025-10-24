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
	/// \brief Construct a null instance
	CL_HTTPServerConnection();

	/// \brief Constructs a HTTPServerConnection
	///
	/// \param impl = Shared Ptr
	CL_HTTPServerConnection(const CL_SharedPtr<CL_HTTPServerConnection_Impl> &impl);

	~CL_HTTPServerConnection();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Request type
	///
	/// \return request_type
	CL_StringRef8 get_request_type();

	/// \brief Get Request url
	///
	/// \return request_url
	CL_StringRef8 get_request_url();

	/// \brief Get Request headers
	///
	/// \return request_headers
	CL_StringRef8 get_request_headers();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Read request data
	///
	/// \return Data Buffer
	CL_DataBuffer read_request_data();

	/// \brief Write response status
	///
	/// \param status_code = value
	/// \param status_text = String Ref8
	void write_response_status(int status_code, const CL_StringRef8 &status_text = CL_StringRef8());

	/// \brief Write response headers
	///
	/// \param headers = String Ref8
	void write_response_headers(const CL_StringRef8 &headers);

	/// \brief Write response data
	///
	/// \param data = Data Buffer
	void write_response_data(const CL_DataBuffer &data);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_HTTPServerConnection_Impl> impl;
/// \}
};

/// \}
