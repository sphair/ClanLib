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

/// \brief <p>Enables network communication.  Includes low-level socket access as well
/// \brief as higher-level network objects.</p>
//! Global=Network

#pragma once

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "Network/setupnetwork.h"

#include "Network/Socket/dns_resolver.h"
#include "Network/Socket/dns_packet.h"
#include "Network/Socket/dns_resource_record.h"
#include "Network/Socket/socket_name.h"
#include "Network/Socket/tcp_connection.h"
#include "Network/Socket/tcp_listen.h"
#include "Network/Socket/udp_socket.h"

#include "Network/Web/http_client_connection.h"
#include "Network/Web/http_header_help.h"
#include "Network/Web/http_request_handler.h"
#include "Network/Web/http_request_handler_provider.h"
#include "Network/Web/http_server.h"
#include "Network/Web/http_server_connection.h"
#include "Network/Web/soap_help.h"
#include "Network/Web/url_help.h"
#include "Network/Web/webservice_client.h"
#include "Network/Web/webservice_message.h"
#include "Network/Web/webservice_part.h"
#include "Network/Web/webservice_exception.h"

#include "Network/IRC/irc_connection.h"
#include "Network/IRC/irc_numerics.h"
#include "Network/IRC/dcc_download.h"

#include "Network/NetGame/client.h"
#include "Network/NetGame/connection.h"
#include "Network/NetGame/event.h"
#include "Network/NetGame/event_dispatcher_v0.h"
#include "Network/NetGame/event_dispatcher_v1.h"
#include "Network/NetGame/event_dispatcher_v2.h"
#include "Network/NetGame/event_dispatcher_v3.h"
#include "Network/NetGame/event_value.h"
#include "Network/NetGame/server.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined (_MSC_VER)
	#if !defined (UNICODE)
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-dll.lib")
				#else
					#pragma comment(lib, "clanNetwork-dll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-dll-debug.lib")
				#else
					#pragma comment(lib, "clanNetwork-dll-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mtdll.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mtdll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mtdll-debug.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mtdll-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "ws2_32.lib")
			#pragma comment(lib, "iphlpapi.lib")
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mt.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mt.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mt-debug.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mt-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "ws2_32.lib")
			#pragma comment(lib, "iphlpapi.lib")
		#endif
	#else
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-dll-uc.lib")
				#else
					#pragma comment(lib, "clanNetwork-dll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-dll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanNetwork-dll-uc-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mtdll-uc.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mtdll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mtdll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mtdll-uc-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "ws2_32.lib")
			#pragma comment(lib, "iphlpapi.lib")
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mt-uc.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mt-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanNetwork-x64-static-mt-uc-debug.lib")
				#else
					#pragma comment(lib, "clanNetwork-static-mt-uc-debug.lib")
				#endif
			#endif
			#pragma comment(lib, "ws2_32.lib")
			#pragma comment(lib, "iphlpapi.lib")
		#endif
	#endif
#endif

