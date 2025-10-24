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

#pragma once


#include "API/Core/System/event.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice_provider.h"

class CL_Event;

class CL_IODeviceProvider_PipeConnection : public CL_IODeviceProvider
{
/// \name Construction
/// \{

public:
	CL_IODeviceProvider_PipeConnection(const CL_StringRef &pipe_name);
#ifdef WIN32
	CL_IODeviceProvider_PipeConnection(HANDLE pipe_handle);
#else
	CL_IODeviceProvider_PipeConnection(int pipe_handle);
#endif

	~CL_IODeviceProvider_PipeConnection();


/// \}
/// \name Attributes
/// \{

public:
#ifdef WIN32
	HANDLE get_handle() const { return handle; }
#else
	int get_handle() const { return handle; }
#endif


/// \}
/// \name Operations
/// \{

public:
	int send(const void *data, int len, bool send_all);

	int receive(void *data, int len, bool receive_all);

	int peek(void *data, int len);

	void disconnect();

	CL_IODeviceProvider *duplicate();


/// \}
/// \name Implementation
/// \{

private:
	int lowlevel_read(void *buffer, int size, bool read_all);

#ifdef WIN32
	HANDLE handle;
#else
	int handle;
#endif

	CL_DataBuffer peeked_data;
/// \}
};


