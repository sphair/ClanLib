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
#include "dcc_download_generic.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/file.h"
#include "API/Core/System/system.h"
#include "API/Core/System/event.h"
#include "API/Core/System/exception.h"
#include "API/Network/Socket/socket_name.h"
#ifndef WIN32
#include <netinet/in.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Construction:

CL_DCCDownload_Generic::CL_DCCDownload_Generic(
	const CL_String &server,
	const CL_String &port,
	const CL_String &filename,
	int total_size,
	CL_VirtualDirectory directory)
: server(server), port(port), filename(filename), total_size(total_size),
	directory(directory), status(CL_DCCDownload::connecting),
	send_signal(no_signal)
{
	thread.start(this, &CL_DCCDownload_Generic::thread_main);
}

CL_DCCDownload_Generic::~CL_DCCDownload_Generic()
{
	event_shutdown.set();
	thread.join();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Implementation:

void CL_DCCDownload_Generic::thread_main()
{
	while (event_shutdown.wait(0) == false)
	{
		CL_MutexSection mutex_lock(&mutex);
		if (status != CL_DCCDownload::connecting)
		{
			CL_System::sleep(100);
			continue;
		}

		try
		{
			status = CL_DCCDownload::connecting;
			mutex_lock.unlock();
		
			CL_TCPConnection connection(CL_SocketName(server, port));
			connection.set_nodelay();
			
			mutex_lock.lock();
			status = CL_DCCDownload::downloading;
			mutex_lock.unlock();
			
			int total_read = 0;

			CL_File outputfile(filename, CL_File::create_always, CL_File::access_read_write);
			char buffer[64*1024];
			while (true)
			{
				CL_Event read_event = connection.get_read_event();
				int result = CL_Event::wait(read_event, event_shutdown, event_reconnect);
				if (result == 0)
				{
					int data_read = connection.read(buffer, 64*1024);
					if (data_read == 0) break;
					total_read += data_read;
					outputfile.write(buffer, data_read);
					unsigned int total_read_network_order = htonl(total_read);
					connection.write(&total_read_network_order, 4);
				}
				else if (result == 2)
				{
					event_reconnect.reset();
					mutex_lock.lock();
					status = CL_DCCDownload::connecting;
					mutex_lock.unlock();
					connection = CL_TCPConnection(CL_SocketName(server, port));
					continue;
				}
				else
				{
					break;
				}
			}
			
			mutex_lock.lock();
			status = CL_DCCDownload::finished;
			send_signal = complete_signal;
			mutex_lock.unlock();
			break;
		}
		catch (CL_Exception err)
		{
			mutex_lock.lock();
			status = CL_DCCDownload::connection_lost;
			send_signal = lost_signal;
			error = err.message;
			mutex_lock.unlock();
		}
	}
}
	
void CL_DCCDownload_Generic::update()
{
	CL_MutexSection mutex_lock(&mutex);
	if (send_signal == complete_signal)
	{
		send_signal = no_signal;
		mutex_lock.unlock();
		sig_download_complete.invoke();
	}
	else if (send_signal == lost_signal)
	{
		CL_String err = error;
		send_signal = no_signal;
		mutex_lock.unlock();
		sig_connection_lost.invoke(err);
	}
}
