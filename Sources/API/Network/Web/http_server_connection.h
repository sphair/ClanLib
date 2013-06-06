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


#pragma once

#include "../api_network.h"
#include <memory>
#include "../../Core/IOData/iodevice.h"

namespace clan
{
/// \addtogroup clanNetwork_Web clanNetwork Web
/// \{

class DataBuffer;
class HTTPServerConnection_Impl;

/// \brief HTTP server connection.
class CL_API_NETWORK HTTPServerConnection : public IODevice
{
/// \name Construction
/// \{

public:
	/// \brief Construct a null instance
	HTTPServerConnection();

	/// \brief Constructs a HTTPServerConnection
	///
	/// \param impl = Shared Ptr
	HTTPServerConnection(const std::shared_ptr<HTTPServerConnection_Impl> &impl);

	~HTTPServerConnection();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Request type
	///
	/// \return request_type
	std::string get_request_type();

	/// \brief Get Request url
	///
	/// \return request_url
	std::string get_request_url();

	/// \brief Get Request headers
	///
	/// \return request_headers
	std::string get_request_headers();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Read request data
	///
	/// \return Data Buffer
	DataBuffer read_request_data();

	/// \brief Write response status
	///
	/// \param status_code = value
	/// \param status_text = String Ref8
	void write_response_status(int status_code, const std::string &status_text = std::string());

	/// \brief Write response headers
	///
	/// \param headers = String Ref8
	void write_response_headers(const std::string &headers);

	/// \brief Write response data
	///
	/// \param data = Data Buffer
	void write_response_data(const DataBuffer &data);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<HTTPServerConnection_Impl> impl;
/// \}
};

}

/// \}
