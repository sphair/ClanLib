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
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/IOData/pipe_connection.h"
#include "iodevice_provider_pipe_connection.h"
#include "Core/IOData/iodevice_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PipeConnection Construction:

PipeConnection::PipeConnection(const std::string &pipe_name)
: IODevice(new IODeviceProvider_PipeConnection(pipe_name))
{
}

#ifdef WIN32
PipeConnection::PipeConnection(HANDLE pipe_handle, bool dummy)
: IODevice(new IODeviceProvider_PipeConnection(pipe_handle))
{
}
#else
PipeConnection::PipeConnection(int pipe_handle, bool dummy)
: IODevice(new IODeviceProvider_PipeConnection(pipe_handle))
{
}
#endif
	
PipeConnection::~PipeConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// PipeConnection Attributes:

/////////////////////////////////////////////////////////////////////////////
// PipeConnection Operations:

void PipeConnection::disconnect()
{
	IODeviceProvider_PipeConnection *provider = dynamic_cast<IODeviceProvider_PipeConnection*>(impl->provider);
	provider->disconnect();
}


/////////////////////////////////////////////////////////////////////////////
// PipeConnection Implementation:

}
