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

/// \addtogroup clanNetwork_NetGame clanNetwork NetGame
/// \{

#pragma once

#include "event.h"
#include "../Socket/tcp_connection.h" // TODO: Remove
#include "../../Core/System/thread.h" // TODO: Remove
#include "../../Core/System/event.h"	// TODO: Remove

class CL_NetGameConnectionSite;

/// \brief CL_NetGameConnection
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_NetGameConnection
{
public:
	CL_NetGameConnection(CL_NetGameConnectionSite *site, const CL_TCPConnection &connection);
	~CL_NetGameConnection();

	void set_data(const CL_StringRef &name, void *data);
	void *get_data(const CL_StringRef &name) const;

	void send_event(const CL_NetGameEvent &game_event);

private:
	void connection_main();

	CL_NetGameConnectionSite *site;
	CL_TCPConnection connection;
	CL_Thread thread;
	CL_Event stop_event, queue_event;
	CL_Mutex mutex;
	std::vector<CL_NetGameEvent> send_queue;
	struct AttachedData
	{
		CL_String name;
		void *data;
	};
	std::vector<AttachedData> data;
};

/// \}

