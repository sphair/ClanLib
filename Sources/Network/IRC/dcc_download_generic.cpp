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

#include "dcc_download_generic.h"
#include "API/Core/IOData/outputsource_provider.h"
#include "API/Core/IOData/outputsource_file.h"
#include "API/Core/System/system.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#ifdef WIN32
#include "winsock.h"
#else
#include <netinet/in.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Construction:

CL_DCCDownload_Generic::CL_DCCDownload_Generic(
	const std::string &server,
	const std::string &port,
	const std::string &filename,
	int total_size,
	CL_OutputSourceProvider *provider,
	bool delete_provider)
	:
	server(server), port(port), filename(filename), total_size(total_size),
	provider(provider), delete_provider(delete_provider), status(CL_DCCDownload::connecting),
	send_signal(no_signal)
{
	thread = CL_Thread(this);
	thread.start();
}

CL_DCCDownload_Generic::~CL_DCCDownload_Generic()
{
	shutdown_trigger.set_flag();
	thread.wait();
	if (delete_provider) delete provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Operations:

void CL_DCCDownload_Generic::add_ref()
{
	ref_count++;
}

void CL_DCCDownload_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload_Generic Implementation:

void CL_DCCDownload_Generic::run()
{
	while (shutdown_trigger.get_flag() == false)
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
			mutex_lock.leave();
		
			CL_Socket sock(CL_Socket::tcp);
			sock.connect(CL_IPAddress(server, port));
			sock.set_nodelay();
			
			mutex_lock.enter();
			status = CL_DCCDownload::downloading;
			mutex_lock.leave();
			
			int total_read = 0;

			CL_OutputSource_File outputfile(filename);
			char buffer[64*1024];
			while (true)
			{
				CL_EventListener listener;
				listener.add_trigger(sock.get_read_trigger());
				listener.add_trigger(&shutdown_trigger);
				listener.add_trigger(&reconnect_trigger);
				listener.wait();
				if (shutdown_trigger.get_flag()) break;
				if (reconnect_trigger.get_flag())
				{
					reconnect_trigger.reset();
					mutex_lock.enter();
					status = CL_DCCDownload::connecting;
					mutex_lock.leave();
					sock = CL_Socket(CL_Socket::tcp);
					sock.connect(CL_IPAddress(server, port));
					continue;
				}

				int data_read = sock.input.read(buffer, 64*1024);
				if (data_read == 0) break;
				total_read += data_read;
				outputfile.write(buffer, data_read);
				unsigned int total_read_network_order = htonl(total_read);
				sock.output.write(&total_read_network_order, 4);
			}
			
			mutex_lock.enter();
			status = CL_DCCDownload::finished;
			send_signal = complete_signal;
			mutex_lock.leave();
			break;
		}
		catch (CL_Error err)
		{
			mutex_lock.enter();
			status = CL_DCCDownload::connection_lost;
			send_signal = lost_signal;
			error = err.message;
			mutex_lock.leave();
		}
	}
}
	
void CL_DCCDownload_Generic::keep_alive()
{
	CL_MutexSection mutex_lock(&mutex);
	if (send_signal == complete_signal)
	{
		send_signal = no_signal;
		mutex_lock.leave();
		sig_download_complete();
	}
	else if (send_signal == lost_signal)
	{
		std::string err = error;
		send_signal = no_signal;
		mutex_lock.leave();
		sig_connection_lost(err);
	}
}
