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

#include "Network/precomp.h"
#include "iodevice_provider_tls_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/file.h"

#ifdef min
#undef min
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Construction:

IODeviceProvider_TLSConnection::IODeviceProvider_TLSConnection()
	: read_buffer(16*1024), read_buffer_pos(0)
{
}
	
IODeviceProvider_TLSConnection::~IODeviceProvider_TLSConnection()
{
	disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Attributes:

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Operations:

void IODeviceProvider_TLSConnection::connect(TCPConnection &device)
{
	connected_device = device;
	read_buffer_pos = 0;
}

void IODeviceProvider_TLSConnection::disconnect()
{
	connected_device = TCPConnection();
	tls_client = TLSClient();
}

int IODeviceProvider_TLSConnection::send(const void *data, int len, bool send_all)
{
	int pos = 0;
	do
	{
		update_io_buffers(send_all);

		pos += tls_client.encrypt(static_cast<const char*>(data) + pos, len - pos);

		update_io_buffers(send_all);

	} while (send_all && pos != len);
	return pos;
}

int IODeviceProvider_TLSConnection::receive(void *data, int len, bool receive_all)
{
	int pos = 0;
	do
	{
		update_io_buffers(receive_all);

		int bytes_available = std::min(tls_client.get_decrypted_data_available(), len);
		memcpy(static_cast<char*>(data) + pos, tls_client.get_decrypted_data(), bytes_available);

		update_io_buffers(receive_all);

	} while (receive_all && pos != len);
	return pos;
}

int IODeviceProvider_TLSConnection::peek(void *data, int len)
{
	throw Exception("Not implemented");
}

IODeviceProvider *IODeviceProvider_TLSConnection::duplicate()
{
	throw Exception("IODeviceProvider_TLSConnection::duplicate() - duplicate not supported for TLS connections.");
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Implementation:

void IODeviceProvider_TLSConnection::update_io_buffers(bool wait)
{
	// Pass on any encrypted data ready to be sent:
	if (tls_client.get_encrypted_data_available() != 0)
	{
		connected_device.write(tls_client.get_encrypted_data(), tls_client.get_encrypted_data_available(), wait);
	}

	// Read incoming data:
	if (read_buffer_pos != read_buffer.get_size())
	{
		read_buffer_pos += connected_device.read(read_buffer.get_data() + read_buffer_pos, read_buffer.get_size() - read_buffer_pos, false);
	}

	// Pass any available incoming data to TLSClient for decryption:
	if (read_buffer_pos != 0)
	{
		int tcp_bytes_read = tls_client.decrypt(read_buffer.get_data(), read_buffer_pos);
		memmove(read_buffer.get_data(), read_buffer.get_data() + tcp_bytes_read, read_buffer_pos - tcp_bytes_read);
		read_buffer_pos = 0;
	}
}

}
