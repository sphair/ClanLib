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

#include "connection_site.h"	// TODO: Remove
#include "../../Core/System/event.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"

class CL_NetGameEvent;
class CL_NetGameConnection;
class CL_NetGameClient_Impl;

/// \brief CL_NetGameClient
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetGameClient : CL_NetGameConnectionSite
{
public:
	CL_NetGameClient();
	~CL_NetGameClient();

	/// \brief Connect
	///
	/// \param server = String
	/// \param port = String
	void connect(const CL_String &server, const CL_String &port);

	/// \brief Disconnect
	void disconnect();

	/// \brief Process events
	void process_events();

	/// \brief Get Event arrived
	///
	/// \return event_arrived
	CL_Event &get_event_arrived();

	/// \brief Send event
	///
	/// \param game_event = Net Game Event
	void send_event(const CL_NetGameEvent &game_event);
	CL_Signal_v1<const CL_NetGameEvent &> &sig_event_received();

	/// \brief Sig connected
	///
	/// \return Signal_v0
	CL_Signal_v0 &sig_connected();

	/// \brief Sig disconnected
	///
	/// \return Signal_v0
	CL_Signal_v0 &sig_disconnected();

private:

	/// \brief Add network event
	///
	/// \param e = Net Game Network Event
	void add_network_event(const CL_NetGameNetworkEvent &e);

	CL_SharedPtr<CL_NetGameClient_Impl> impl;
};

/// \}

