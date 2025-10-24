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

/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{

#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_Event;
class CL_SocketName;
class CL_UDPSocket_Impl;

/// \brief UDP socket.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_UDPSocket
{
/// \name Construction
/// \{

public:
	CL_UDPSocket();

	/// \brief Constructs a UDPSocket
	///
	/// \param local_name = Socket Name
	/// \param force_bind = bool
	CL_UDPSocket(const CL_SocketName &local_name, bool force_bind = true);

	/// \brief Constructs a UDPSocket
	///
	/// \param socket = value
	/// \param close_socket = bool
	CL_UDPSocket(int socket, bool close_socket);

	~CL_UDPSocket();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Handle
	///
	/// \return handle
	int get_handle() const;

	/// \brief Get Local name
	///
	/// \return local_name
	CL_SocketName get_local_name() const;

	/// \brief Get Read event
	///
	/// \return read_event
	CL_Event get_read_event();

	/// \brief Get Write event
	///
	/// \return write_event
	CL_Event get_write_event();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Bind
	///
	/// \param local_name = Socket Name
	/// \param force_bind = bool
	void bind(const CL_SocketName &local_name, bool force_bind = true);

	/// \brief Set handle
	///
	/// \param socket = value
	/// \param close_socket = bool
	void set_handle(int socket, bool close_socket);

	/// \brief Send
	///
	/// \param data = void
	/// \param len = value
	/// \param to = Socket Name
	///
	/// \return int
	int send(const void *data, int len, const CL_SocketName &to);

	/// \brief Receive
	///
	/// \param data = void
	/// \param len = value
	/// \param out_from = Socket Name
	///
	/// \return int
	int receive(void *data, int len, CL_SocketName &out_from);

	/// \brief Peek
	///
	/// \param data = void
	/// \param len = value
	/// \param out_from = Socket Name
	///
	/// \return int
	int peek(void *data, int len, CL_SocketName &out_from);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_UDPSocket_Impl> impl;
/// \}
};

/// \}
