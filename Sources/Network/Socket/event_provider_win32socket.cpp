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

#include "Network/precomp.h"
#include "event_provider_win32socket.h"

/////////////////////////////////////////////////////////////////////////////
// CL_WSAEventSelectHandler Construction:

CL_WSAEventSelectHandler::CL_WSAEventSelectHandler()
: socket_handle(INVALID_SOCKET), wsa_handle(0), read_handle(0),
	write_handle(0), exception_handle(0)
{
	wsa_handle = CreateEvent(0, TRUE, FALSE, 0);
	read_handle = CreateEvent(0, TRUE, FALSE, 0);
	write_handle = CreateEvent(0, TRUE, FALSE, 0);
	exception_handle = CreateEvent(0, TRUE, FALSE, 0);

	if (wsa_handle == 0 ||
		read_handle == 0 ||
		write_handle == 0 ||
		exception_handle == 0)
	{
		if (wsa_handle != 0)
			CloseHandle(wsa_handle);
		if (read_handle != 0)
			CloseHandle(read_handle);
		if (write_handle != 0)
			CloseHandle(write_handle);
		if (exception_handle != 0)
			CloseHandle(exception_handle);

		throw CL_Exception(cl_text("CreateEvent failed in CL_WSAEventSelectHandler constructor"));
	}
}

CL_WSAEventSelectHandler::~CL_WSAEventSelectHandler()
{
	stop_select();

	if (wsa_handle != 0)
		CloseHandle(wsa_handle);
	if (read_handle != 0)
		CloseHandle(read_handle);
	if (write_handle != 0)
		CloseHandle(write_handle);
	if (exception_handle != 0)
		CloseHandle(exception_handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_WSAEventSelectHandler Attributes:

CL_Mutex CL_WSAEventSelectHandler::mutex;

/////////////////////////////////////////////////////////////////////////////
// CL_WSAEventSelectHandler Operations:

void CL_WSAEventSelectHandler::start_select(SOCKET new_socket_handle)
{
	CL_MutexSection mutex_lock(&mutex);
	stop_select();
	socket_handle = new_socket_handle;
	if (socket_handle == INVALID_SOCKET)
		throw CL_Exception(cl_text("Invalid handle passed to start_select"));
	int result = WSAEventSelect(
		socket_handle,
		wsa_handle,
		FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE);
	if (result == SOCKET_ERROR)
		throw CL_Exception(cl_text("WSAEventSelect failed"));
}

void CL_WSAEventSelectHandler::stop_select()
{
	CL_MutexSection mutex_lock(&mutex);
	if (socket_handle == INVALID_SOCKET)
		return;
	WSAEventSelect(socket_handle, wsa_handle, 0);
	ResetEvent(wsa_handle);
	ResetEvent(read_handle);
	ResetEvent(write_handle);
	ResetEvent(exception_handle);
	socket_handle = INVALID_SOCKET;
}

void CL_WSAEventSelectHandler::update_events()
{
	CL_MutexSection mutex_lock(&mutex);

	ResetEvent(wsa_handle);

	fd_set readfds, writefds, exceptfds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(socket_handle, &readfds);
	FD_SET(socket_handle, &writefds);
	FD_SET(socket_handle, &exceptfds);
	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int result = select(
		socket_handle + 1,
		&readfds,
		&writefds,
		&exceptfds,
		&tv);
		if (result < 0)
			throw CL_Exception(cl_text("select failed in CL_WSAEventSelectHandler::update_events()"));

	if (FD_ISSET(socket_handle, &readfds))
		SetEvent(read_handle);
	else
		ResetEvent(read_handle);

	if (FD_ISSET(socket_handle, &writefds))
		SetEvent(write_handle);
	else
		ResetEvent(write_handle);

	if (FD_ISSET(socket_handle, &exceptfds))
		SetEvent(exception_handle);
	else
		ResetEvent(exception_handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32Socket Construction:

CL_EventProvider_Win32Socket::CL_EventProvider_Win32Socket(
	CL_WSAEventSelectHandler *handler,
	SocketEventType type)
: handler(handler), type(type)
{
}

CL_EventProvider_Win32Socket::~CL_EventProvider_Win32Socket()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32Socket Attributes:

HANDLE CL_EventProvider_Win32Socket::get_event_handle(int index)
{
	if (index == 0)
	{
		return handler->wsa_handle;
	}
	else if (index == 1)
	{
		switch (type)
		{
		case socket_event_read:
			return handler->read_handle;
		case socket_event_write:
			return handler->write_handle;
		case socket_event_exception:
			return handler->exception_handle;
		default:
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int CL_EventProvider_Win32Socket::get_num_event_handles()
{
	return 2;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32Socket Operations:

bool CL_EventProvider_Win32Socket::check_before_wait()
{
	handler->update_events();
	return false;
}

bool CL_EventProvider_Win32Socket::check_after_wait(int index)
{
	if (index == 0)
	{
		handler->update_events();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32Socket Implementation:
