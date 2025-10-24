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

	CL_TCPConnection(const CL_SocketName &remote);

	CL_TCPConnection(const CL_SocketName &remote, const CL_SocketName &local);

	CL_TCPConnection(int socket, bool close_socket);

	~CL_TCPConnection();


/// \}
/// \name Attributes
/// \{

public:
	int get_handle() const;

	CL_SocketName get_local_name() const;

	CL_SocketName get_remote_name() const;

	CL_Event get_read_event();

	CL_Event get_write_event();


/// \}
/// \name Operations
/// \{

public:
	bool connect(const CL_SocketName &remote);

	bool connect(const CL_SocketName &remote, const CL_SocketName &local);

	void set_handle(int socket, bool close_socket);

	/// \brief Performs a graceful shutdown, ensuring all data is sent before the connection is closed
	void disconnect_graceful();

	/// \brief Performs an abortive shutdown, closing the connection immidiately
	void disconnect_abortive();

	/// \brief Disables or enables buffering data before it is sent
	void set_nodelay(bool enable = true);

	void flush();


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
