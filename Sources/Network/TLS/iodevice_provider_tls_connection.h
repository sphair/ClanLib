/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#pragma once


#include "API/Core/System/event.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/Crypto/tls_client.h"

namespace clan
{

class IODeviceProvider_TLSConnection : public IODeviceProvider
{
/// \name Construction
/// \{
public:
	IODeviceProvider_TLSConnection();
	~IODeviceProvider_TLSConnection();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	void connect(TCPConnection &device);
	void disconnect();
	int send(const void *data, int len, bool send_all);
	int receive(void *data, int len, bool receive_all);
	int peek(void *data, int len);
	IODeviceProvider *duplicate();
/// \}

/// \name Implementation
/// \{
private:
	void update_io_buffers(bool wait);

	TCPConnection connected_device;
	TLSClient tls_client;

	DataBuffer read_buffer;
	int read_buffer_pos;
/// \}
};

}
