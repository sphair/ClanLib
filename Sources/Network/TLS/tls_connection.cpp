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
**    Mark Page
*/

#include "Network/precomp.h"
#include "API/Network/TLS/tls_connection.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "Core/IOData/iodevice_impl.h"
#include "iodevice_provider_tls_connection.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// TLSConnection Construction:

TLSConnection::TLSConnection()
: IODevice(new IODeviceProvider_TLSConnection())
{
}

TLSConnection::TLSConnection(TCPConnection &device)
: IODevice(new IODeviceProvider_TLSConnection())
{
	connect(device);
}

TLSConnection::~TLSConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// TLSConnection Attributes:

/////////////////////////////////////////////////////////////////////////////
// TLSConnection Operations:

void TLSConnection::connect(TCPConnection &device)
{
	IODeviceProvider_TLSConnection *provider = dynamic_cast<IODeviceProvider_TLSConnection*>(impl->provider);
	provider->connect(device);
}


void TLSConnection::disconnect()
{
	IODeviceProvider_TLSConnection *provider = dynamic_cast<IODeviceProvider_TLSConnection*>(impl->provider);
	provider->disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// TLSConnection Implementation:

}
