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

#include "socket_generic.h"
#include "API/Network/Socket/socket.h"
#include "API/Network/Socket/inputsource_socket.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket construction:

CL_InputSource_Socket::CL_InputSource_Socket(const CL_Socket &sock)
: socket(sock.impl), ref_count_socket(true)
{
	socket->add_ref();
}

CL_InputSource_Socket::~CL_InputSource_Socket()
{
	if (ref_count_socket) socket->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket attributes:

int CL_InputSource_Socket::tell() const
{
	return 0;
}

int CL_InputSource_Socket::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket operations:

int CL_InputSource_Socket::read(void *data, int size)
{
	char *d = (char*) data;
	int gotten = 0;

	CL_Socket s(socket);
	while (size-gotten > 0)
	{
		int received = s.recv(d+gotten, size-gotten);
		if (received == 0)
			break;
		if (received < 0)
		{
			s.get_read_trigger()->wait();
			continue;
		}

		gotten += received;
	}
	return gotten;
}

void CL_InputSource_Socket::open()
{
}

void CL_InputSource_Socket::close()
{
}

CL_InputSource *CL_InputSource_Socket::clone() const
{
	return new CL_InputSource_Socket(CL_Socket(socket));
}

void CL_InputSource_Socket::seek(int pos, SeekEnum seek_type)
{
}

void CL_InputSource_Socket::push_position()
{
}

void CL_InputSource_Socket::pop_position()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket implementation:
