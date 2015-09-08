/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Kenneth Gangstoe
**    Magnus Norddahl
*/

#include "Network/precomp.h"
#include "API/Core/Text/string_format.h"
#include "socket_error.h"

#ifndef WIN32
#include <errno.h>
#include <cstring>
#endif

namespace clan
{
	std::string SocketError::get_last_error_message()
	{
#ifdef WIN32
		int err = WSAGetLastError();
		switch (err)
		{
		case WSAEACCES:
			return "Permission denied";
		case WSAEADDRINUSE:
			return "Address already in use";
		case WSAEADDRNOTAVAIL:
			return "Cannot assign requested address";
		case WSAEAFNOSUPPORT:
			return "Address family not supported by protocol family";
		case WSAEALREADY:
			return "Operation already in progress";
		case WSAECONNABORTED:
			return "Software caused connection abort";
		case WSAECONNREFUSED:
			return "Connection refused";
		case WSAECONNRESET:
			return "Connection reset by peer";
		case WSAEDESTADDRREQ:
			return "Destination address required";
		case WSAEFAULT:
			return "Bad address";
		case WSAEHOSTDOWN:
			return "Host is down";
		case WSAEINPROGRESS:
			return "Operation now in progress";
		case WSAEINTR:
			return "Interrupted function call";
		case WSAEINVAL:
			return "Invalid argument";
		case WSAEISCONN:
			return "Socket is already connected";
		case WSAEMFILE:
			return "Too many open files";
		case WSAEMSGSIZE:
			return "Message too long";
		case WSAENETDOWN:
			return "Network is down";
		case WSAENETRESET:
			return "Network dropped connection on reset";
		case WSAENETUNREACH:
			return "Network is unreachable";
		case WSAENOBUFS:
			return "No buffer space available";
		case WSAENOPROTOOPT:
			return "Bad protocol option";
		case WSAENOTCONN:
			return "Socket is not connected";
		case WSAENOTSOCK:
			return "Socket operation on nonsocket";
		case WSAEOPNOTSUPP:
			return "Operation not supported";
		case WSAEPFNOSUPPORT:
			return "Protocol family not supported";
		case WSAEPROCLIM:
			return "Too many processes";
		case WSAEPROTONOSUPPORT:
			return "Protocol not supported";
		case WSAEPROTOTYPE:
			return "Protocol wrong type for socket";
		case WSAESHUTDOWN:
			return "Cannot send after socket shutdown";
		case WSAESOCKTNOSUPPORT:
			return "Socket type not supported";
		case WSAETIMEDOUT:
			return "Connection timed out";
			//	case WSATYPE_NOT_FOUND:
			//		return "Class type not found";
		case WSAEWOULDBLOCK:
			return "Resource temporarily unavailable";
		case WSAHOST_NOT_FOUND:
			return "Host not found";
			//	case WSA_INVALID_HANDLE:
			//		return "Specified event object handle is invalid";
			//	case WSA_INVALID_PARAMETER:
			//		return "One or more parameters are invalid";
			//	case WSAINVALIDPROCTABLE:
			//		return "Invalid procedure table from service provider";
			//	case WSAINVALIDPROVIDER:
			//		return "Invalid service provider version number";
			//	case WSA_IO_INCOMPLETE:
			//		return "Overlapped I/O event object not in signaled state";
			//	case WSA_IO_PENDING:
			//		return "Overlapped operations will complete later";
			//	case WSA_NOT_ENOUGH_MEMORY:
			//		return "Insufficient memory available";
		case WSANOTINITIALISED:
			return "Successful WSAStartup not yet performed";
		case WSANO_DATA:
			return "Valid name, no data record of requested type";
		case WSANO_RECOVERY:
			return "This is a nonrecoverable error";
			//	case WSAPROVIDERFAILEDINIT:
			//		return "Unable to initialize a service provider";
			//	case WSASYSCALLFAILURE:
			//		return "System call failure";
		case WSASYSNOTREADY:
			return "Network subsystem is unavailable";
		case WSATRY_AGAIN:
			return "Nonauthoritative host not found";
		case WSAVERNOTSUPPORTED:
			return "Winsock.dll version out of range";
		case WSAEDISCON:
			return "Graceful shutdown in progress";
			//	case WSA_OPERATION_ABORTED:
			//		return "Overlapped operation aborted";
		default:
			return string_format("Unknown socket error %1", err);
		}
#else
		return strerror(errno);
#endif
	}
}
