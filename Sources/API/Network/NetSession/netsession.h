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

/// \addtogroup clanNetwork_NetSessions clanNetwork NetSessions
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/Signals/signal_v2.h"
#include "../../Core/Text/string_types.h"

class CL_SocketName;
class CL_NetStream;
class CL_DataBuffer;
class CL_NetComputer;
class CL_NetGroup;
class CL_NetSession_Impl;

/// \brief Net session class.
///
/// \xmlonly !group=Network/NetSessions! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetSession
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a netsession.
	///
	/// \param app_id Application identifier. This must match any remote
	/// \param app_id netsession connecting to/from this netsession.
	CL_NetSession(const CL_String &app_id);

	CL_NetSession(const CL_NetSession &copy);


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a list of all computers connected.
	CL_NetGroup &get_all();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Start listening for connections.
	void start_listen(const CL_SocketName &local);

	/// \brief Stop listening for connections and disconnect everyone.
	void stop_listen();

	/// \brief Connect to remote netsession.
	CL_NetComputer connect(const CL_SocketName &remote);

	/// \brief Connect to remote netsession asyncronously.
	/** <p>The DNS lookup and connect is done in a worker thread, thus not
	    blocking the calling thread. If the connection fails,
	    CL_NetSession::sig_disconnected() is emitted.</p>*/
	CL_NetComputer connect_async(const CL_SocketName &remote);

	/// \brief Enable or disable debug output.
	/** <p>This will output the netchannels each netpacket arrive in.</p>*/
	void show_debug(bool enable = true);


/// \}
/// \name Signals
/// \{

public:
	/// \brief Signal invoked when a computer connects to the netsession server.
	CL_Signal_v1<CL_NetComputer &> &sig_computer_connected();

	/// \brief Signal invoked when a computer re-connects to the netsession server.
	/** <p>This happens when a CL_NetComputer instance has still not been destroyed
	    since its last connect (allows you to recognize players dropping out, so
	    they dont loose their score when they reconnect).</p>*/
	CL_Signal_v1<CL_NetComputer &> &sig_computer_reconnected();

	/// \brief Signal invoked when a computer disconnects from the netsession server.
	CL_Signal_v1<CL_NetComputer &> &sig_computer_disconnected();

	/// \brief Signal invoked when someone makes a connection to the specified
	/// \brief netstream.
	CL_Signal_v1<CL_NetStream &> &sig_netstream_connect(const CL_String &netstream);

	/// \brief Signal invoked when someone sends a netpacket to the specified packet channel.
	CL_Signal_v2<CL_DataBuffer &, CL_NetComputer &> &sig_netpacket_receive(const CL_String &packet_channel);


/// \}
/// \name Implementation
/// \{

public:
	CL_NetSession(CL_SharedPtr<CL_NetSession_Impl> impl);

private:
	CL_SharedPtr<CL_NetSession_Impl> impl;
/// \}
};


/// \}
