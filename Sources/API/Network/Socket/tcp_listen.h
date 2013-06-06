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

namespace clan
{
/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{

class Event;
class SocketName;
class TCPConnection;
class TCPListen_Impl;

/// \brief TCP listen socket.
class CL_API_NETWORK TCPListen
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a TCPListen
	///
	/// \param name = Socket Name
	/// \param queue_size = value
	/// \param force_bind = bool
	TCPListen(const SocketName &name, int queue_size = 5, bool force_bind = true);

	~TCPListen();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Accept event
	///
	/// \return accept_event
	Event get_accept_event() const;

	/// \brief Get Handle
	///
	/// \return handle
	int get_handle() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Accept
	///
	/// \return TCPConnection
	TCPConnection accept();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<TCPListen_Impl> impl;
/// \}
};

}

/// \}
