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
#include "../../Core/IOData/iodevice.h"

class CL_SocketName;
class CL_Event;

/// \brief TCP connection socket I/O device.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_TCPConnection : public CL_IODevice
{
/// \name Construction
/// \{

public:
	CL_TCPConnection();

	/// \brief Constructs a TCPConnection
	///
	/// \param remote = Socket Name
	CL_TCPConnection(const CL_SocketName &remote);

	/// \brief Constructs a TCPConnection
	///
	/// \param remote = Socket Name
	/// \param local = Socket Name
	CL_TCPConnection(const CL_SocketName &remote, const CL_SocketName &local);

	/// \brief Constructs a TCPConnection
	///
	/// \param socket = value
	/// \param close_socket = bool
	CL_TCPConnection(int socket, bool close_socket);

	~CL_TCPConnection();

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

	/// \brief Get Remote name
	///
	/// \return remote_name
	CL_SocketName get_remote_name() const;

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

	/// \brief Connect
	///
	/// \param remote = Socket Name
	void connect(const CL_SocketName &remote);

	/// \brief Connect
	///
	/// \param remote = Socket Name
	/// \param local = Socket Name
	void connect(const CL_SocketName &remote, const CL_SocketName &local);

	/// \brief Set handle
	///
	/// \param socket = value
	/// \param close_socket = bool
	void set_handle(int socket, bool close_socket);

	/// \brief Performs a graceful shutdown, ensuring all data is sent before the connection is closed
	void disconnect_graceful();

	/// \brief Performs an abortive shutdown, closing the connection immediately
	void disconnect_abortive();

	/// \brief Disables or enables buffering data before it is sent
	void set_nodelay(bool enable = true);

	/// \brief Enables or disables transmitting keep-alive packets
	///
	/// \param enable = True to enable keep-alive, false to disable
	/// \param timeout = Specifies the timeout, in milliseconds, with no activity until the first keep-alive packet is sent.
	/// \param interval = Specifies the interval, in milliseconds, between when successive keep-alive packets are sent if no acknowledgment is received.
	///
	/// Both timeout and interval must be set to a non-zero value before any of them are used.  They cannot be specified individually.
	void set_keep_alive(bool enable, int timeout = 0, int interval = 0);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
