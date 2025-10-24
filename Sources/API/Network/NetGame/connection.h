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

/// \addtogroup clanNetwork_NetGame clanNetwork NetGame
/// \{

#pragma once

#include "../api_network.h"

#include "event.h"
#include "../Socket/tcp_connection.h" // TODO: Remove
#include "../Socket/socket_name.h" // TODO: Remove
#include "../../Core/System/thread.h" // TODO: Remove
#include "../../Core/System/event.h"	// TODO: Remove

class CL_NetGameConnectionSite;
class CL_NetGameConnection_Impl;

/// \brief CL_NetGameConnection
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetGameConnection
{
public:

	/// \brief Constructs a NetGameConnection
	///
	/// \param site = Net Game Connection Site
	/// \param connection = TCPConnection
	CL_NetGameConnection(CL_NetGameConnectionSite *site, const CL_TCPConnection &connection);
	CL_NetGameConnection(CL_NetGameConnectionSite *site, const CL_SocketName &socket_name);

	~CL_NetGameConnection();

	/// \brief Set data
	///
	/// \param name = String Ref
	/// \param data = void
	void set_data(const CL_StringRef &name, void *data);

	/// \brief Get data
	///
	/// \param name = String Ref
	///
	/// \return void
	void *get_data(const CL_StringRef &name) const;

	/// \brief Send event
	///
	/// \param game_event = Net Game Event
	void send_event(const CL_NetGameEvent &game_event);

	/// \brief Disconnects a client
	void disconnect();

private:
	/// \brief Disallow copy constructors
	CL_NetGameConnection(CL_NetGameConnection &other);
	CL_NetGameConnection &operator =(const CL_NetGameConnection &other);

	CL_NetGameConnection_Impl *impl;
};

/// \}

