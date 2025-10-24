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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#define snprintf _snprintf
#endif

#ifndef WIN32
#include <errno.h>
#endif

#include "socket_generic.h"
#include <cstdio>
#include <cstring>
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_KeepAliveHelper operations:

void CL_Socket_KeepAliveHelper::keep_alive()
{
	// performance note: it would be much more efficient if we maintained a global
	// list of all sockets instanciated, and then do one select for them all.
	//
	// I'm too lazy to do that - at least until it becomes a real performance problem.
	// -- mbn 2. marts 2001
	
	if (impl->valid == false)
		return;

	fd_set rfds, wfds, efds;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	FD_SET(impl->sock, &rfds);
	FD_SET(impl->sock, &wfds);
	FD_SET(impl->sock, &efds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int nread = 0;
	char buf = 0;

	int result = select(impl->sock+1, &rfds, &wfds, &efds, &tv);
	if (result > 0)
	{
		if (FD_ISSET(impl->sock, &rfds))
		{

			// Peek and see if there is anydata on the socket, if not 
			// the remote side has disconnected.
			nread = recv(impl->sock, &buf, 1, MSG_PEEK);

			if(nread == 0)
			{
				sig_disconnected();
				impl->disconnect();
				return;
			}
			else
				sig_read_triggered();
		}
		if (FD_ISSET(impl->sock, &wfds)) sig_write_triggered();
		if (FD_ISSET(impl->sock, &efds)) sig_exception_triggered();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic static operations:

CL_IPAddress CL_Socket_Generic::create_ip_address(const sockaddr_in &addr_in)
{
	char port[32];
	memset(port, 0, 32);
	
	snprintf(port,6,"%d",ntohs(addr_in.sin_port));
	
	char *addr = inet_ntoa(addr_in.sin_addr);
	
	return CL_IPAddress((std::string)addr, (std::string)port);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic construction:

CL_Socket_Generic::CL_Socket_Generic()
: sock(-1), read(NULL), write(NULL), exception(NULL), keep_alive_helper(0), ref_count(0), valid(true)
{
}

CL_Socket_Generic::~CL_Socket_Generic()
{
	delete read;
	delete write;
	delete exception;
	disconnect();
	delete keep_alive_helper;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic operations:

void CL_Socket_Generic::add_ref()
{
	ref_count++;
}

void CL_Socket_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

std::string CL_Socket_Generic::get_error_message()
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
		return CL_String::format("Unknown socket error %1", err);
	}
#else
	return std::string(strerror(errno));
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic overridables:

CL_EventTrigger_Socket *CL_Socket_Generic::create_read_trigger()
{
	return new CL_EventTrigger_Socket(this, CL_EventTrigger_Socket::read);
}

CL_EventTrigger_Socket *CL_Socket_Generic::create_write_trigger()
{
	return new CL_EventTrigger_Socket(this, CL_EventTrigger_Socket::write);
}

CL_EventTrigger_Socket *CL_Socket_Generic::create_exception_trigger()
{
	return new CL_EventTrigger_Socket(this, CL_EventTrigger_Socket::exception);
}

void CL_Socket_Generic::disconnect()
{
	if(sock != -1)
	{
		#ifdef WIN32
		
		//the code below causes a one minute deadlock when closing a socket - (might be only sockets 
		//that are not bound yet, this happens with the copy constructor when initializing
		//in CTalk.  Maybe we should set a flag for when a socket is actually bound?  And even in that case,
		//build a blocking thread to do this so we can return right away?
		//Let's keep it disabled for now, Magnus has said he has a fix coming in 0.9 that we can backport.
		//-mrfun Sept 22 2006
		
		/*
		// Must stop and wait for all send data to be sent (thanks Microsoft):
		HANDLE event_object = WSACreateEvent();
		if (event_object != WSA_INVALID_EVENT)
		{
			if (WSAEventSelect(sock, event_object, FD_CLOSE) != SOCKET_ERROR)
				WSAWaitForMultipleEvents(1, &event_object, FALSE, 60*1000, FALSE);
			WSACloseEvent(event_object);
		}
		*/
		closesocket(sock);
		#else
		close(sock);
		#endif
	}
	
	sock = -1;
	valid = false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic implementation:
