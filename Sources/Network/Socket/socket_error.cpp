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

/////////////////////////////////////////////////////////////////////////////
// CL_SocketError Attributes:

CL_String CL_SocketError::get_last_error_message()
{
#ifdef WIN32
	int err = WSAGetLastError();
	switch (err)
	{
	case WSAEACCES:
		return cl_text("Permission denied");
	case WSAEADDRINUSE:
		return cl_text("Address already in use");
	case WSAEADDRNOTAVAIL:
		return cl_text("Cannot assign requested address");
	case WSAEAFNOSUPPORT:
		return cl_text("Address family not supported by protocol family");
	case WSAEALREADY:
		return cl_text("Operation already in progress");
	case WSAECONNABORTED:
		return cl_text("Software caused connection abort");
	case WSAECONNREFUSED:
		return cl_text("Connection refused");
	case WSAECONNRESET:
		return cl_text("Connection reset by peer");
	case WSAEDESTADDRREQ:
		return cl_text("Destination address required");
	case WSAEFAULT:
		return cl_text("Bad address");
	case WSAEHOSTDOWN:
		return cl_text("Host is down");
	case WSAEINPROGRESS:
		return cl_text("Operation now in progress");
	case WSAEINTR:
		return cl_text("Interrupted function call");
	case WSAEINVAL:
		return cl_text("Invalid argument");
	case WSAEISCONN:
		return cl_text("Socket is already connected");
	case WSAEMFILE:
		return cl_text("Too many open files");
	case WSAEMSGSIZE:
		return cl_text("Message too long");
	case WSAENETDOWN:
		return cl_text("Network is down");
	case WSAENETRESET:
		return cl_text("Network dropped connection on reset");
	case WSAENETUNREACH:
		return cl_text("Network is unreachable");
	case WSAENOBUFS:
		return cl_text("No buffer space available");
	case WSAENOPROTOOPT:
		return cl_text("Bad protocol option");
	case WSAENOTCONN:
		return cl_text("Socket is not connected");
	case WSAENOTSOCK:
		return cl_text("Socket operation on nonsocket");
	case WSAEOPNOTSUPP:
		return cl_text("Operation not supported");
	case WSAEPFNOSUPPORT:
		return cl_text("Protocol family not supported");
	case WSAEPROCLIM:
		return cl_text("Too many processes");
	case WSAEPROTONOSUPPORT:
		return cl_text("Protocol not supported");
	case WSAEPROTOTYPE:
		return cl_text("Protocol wrong type for socket");
	case WSAESHUTDOWN:
		return cl_text("Cannot send after socket shutdown");
	case WSAESOCKTNOSUPPORT:
		return cl_text("Socket type not supported");
	case WSAETIMEDOUT:
		return cl_text("Connection timed out");
//	case WSATYPE_NOT_FOUND:
//		return cl_text("Class type not found");
	case WSAEWOULDBLOCK:
		return cl_text("Resource temporarily unavailable");
	case WSAHOST_NOT_FOUND:
		return cl_text("Host not found");
//	case WSA_INVALID_HANDLE:
//		return cl_text("Specified event object handle is invalid");
//	case WSA_INVALID_PARAMETER:
//		return cl_text("One or more parameters are invalid");
//	case WSAINVALIDPROCTABLE:
//		return cl_text("Invalid procedure table from service provider");
//	case WSAINVALIDPROVIDER:
//		return cl_text("Invalid service provider version number");
//	case WSA_IO_INCOMPLETE:
//		return cl_text("Overlapped I/O event object not in signaled state");
//	case WSA_IO_PENDING:
//		return cl_text("Overlapped operations will complete later");
//	case WSA_NOT_ENOUGH_MEMORY:
//		return cl_text("Insufficient memory available");
	case WSANOTINITIALISED:
		return cl_text("Successful WSAStartup not yet performed");
	case WSANO_DATA:
		return cl_text("Valid name, no data record of requested type");
	case WSANO_RECOVERY:
		return cl_text("This is a nonrecoverable error");
//	case WSAPROVIDERFAILEDINIT:
//		return cl_text("Unable to initialize a service provider");
//	case WSASYSCALLFAILURE:
//		return cl_text("System call failure");
	case WSASYSNOTREADY:
		return cl_text("Network subsystem is unavailable");
	case WSATRY_AGAIN:
		return cl_text("Nonauthoritative host not found");
	case WSAVERNOTSUPPORTED:
		return cl_text("Winsock.dll version out of range");
	case WSAEDISCON:
		return cl_text("Graceful shutdown in progress");
//	case WSA_OPERATION_ABORTED:
//		return cl_text("Overlapped operation aborted");
	default:
		return cl_format(cl_text("Unknown socket error %1"), err);
	}
#else
	return strerror(errno);
#endif
}

