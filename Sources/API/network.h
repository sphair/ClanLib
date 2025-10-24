/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//: <p>Enables network communication.  Includes low-lever socket access as well
//: as higher-level network objects.</p>
//! Global=Network

#ifndef header_network_api
#define header_network_api

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Network/IRC/irc_connection.h"
#include "Network/IRC/irc_numerics.h"
#include "Network/IRC/dcc_download.h"

#include "Network/NetVariables/netvariables.h"

#include "Network/NetObjects/netobject_client.h"
#include "Network/NetObjects/netobject_controller.h"
#include "Network/NetObjects/netobject_server.h"

#include "Network/NetSession/inputsource_netpacket.h"
#include "Network/NetSession/inputsource_netstream.h"
#include "Network/NetSession/netcomputer.h"
#include "Network/NetSession/netgroup.h"
#include "Network/NetSession/netpacket.h"
#include "Network/NetSession/netsession.h"
#include "Network/NetSession/netstream.h"
#include "Network/NetSession/outputsource_netpacket.h"
#include "Network/NetSession/outputsource_netstream.h"

#include "Network/Socket/inputsource_socket.h"
#include "Network/Socket/ip_address.h"
#include "Network/Socket/outputsource_socket.h"
#include "Network/Socket/socket.h"

#include "Network/setupnetwork.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanNetwork-static-mt.lib")
#else
#pragma comment(lib, "clanNetwork-static-mt-debug.lib")
#endif
#pragma comment(lib, "ws2_32.lib")
#endif

#endif
