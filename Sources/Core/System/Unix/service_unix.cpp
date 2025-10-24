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

#include "Core/precomp.h"
#include "API/Core/System/service.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/thread.h"
#include "service_unix.h"
#include <iostream>
#include <signal.h>

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Unix Construction:

CL_Service_Unix::CL_Service_Unix(CL_Service *service, const CL_String &service_name)
: CL_Service_Impl(service, service_name)
{
}

CL_Service_Unix::~CL_Service_Unix()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Unix Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Unix Operations:

int CL_Service_Unix::main(int argc, char **argv)
{
	std::vector<CL_String> args;
	for (int i=0; i<argc; i++)
		args.push_back(argv[i]);

	if (argc >= 3 && args[1] == "-daemon")
	{
		return run_daemon(args);
	}
	else if (argc == 2 && args[1] == "-debug")
	{
		return run_debug(args);
	}
	else
	{
		std::cout << "Service parameters:" << std::endl;
		std::cout << "  -daemon <pidfile>   - Start service as a unix daemon" << std::endl;
		std::cout << "  -debug              - Debug service" << std::endl;
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Unix Implementation:

void CL_Service_Unix::service_thread_main(std::vector<CL_String> args)
{
	service_start(args);
	while (true)
	{
		int wakeup_reason = CL_Event::wait(stop_event, reload_event);
		if (wakeup_reason == 1)
			service_reload();
		else
			break;
	}
	service_stop();
}

void CL_Service_Unix::sig_term(int signal_code)
{
	if (instance)
	{
		CL_Service_Unix *instance_unix = (CL_Service_Unix *) instance;
		instance_unix->stop_event.set();
	}
}

void CL_Service_Unix::sig_hup(int signal_code)
{
	if (instance)
	{
		CL_Service_Unix *instance_unix = (CL_Service_Unix *) instance;
		instance_unix->reload_event.set();
	}
}

int CL_Service_Unix::run_daemon(std::vector<CL_String> args)
{
	try
	{
		CL_File file(args[2], CL_File::create_always, CL_File::access_read_write);

		struct sigaction action;
		memset(&action, 0, sizeof(struct sigaction));
		action.sa_handler = &CL_Service_Unix::sig_term;
		sigaction(SIGTERM, &action, 0);
		memset(&action, 0, sizeof(struct sigaction));
		action.sa_handler = &CL_Service_Unix::sig_hup;
		sigaction(SIGHUP, &action, 0);

		int pid = fork();
		if (pid)
		{
			CL_String8 str_pid = CL_StringHelp::int_to_local8(pid);
			file.write(str_pid.c_str(), str_pid.length());
			file.write("\n", 1);
			return pid;
		}
		else
		{
			file.close();
			close(0);
			close(1);
			close(2);

			// Starting service in seperate thread to avoid
			// signals sent to this pid causing EINTR errors
			// randomly in the service itself.
			CL_Thread thread;
			thread.start(this, &CL_Service_Unix::service_thread_main, args);
			while (true)
			{
				try
				{
					if (stop_event.wait())
						break;
				}
				catch (CL_Exception e)
				{
				}
			}
			thread.join();
			return 0;
		}
	}
	catch (CL_Exception e)
	{
		std::cout << "Unhandled exception: " << e.message.c_str() << std::endl;
	}
	return 1;
}

int CL_Service_Unix::run_debug(std::vector<CL_String> args)
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &CL_Service_Unix::sig_term;
	sigaction(SIGTERM, &action, 0);
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &CL_Service_Unix::sig_hup;
	sigaction(SIGHUP, &action, 0);
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &CL_Service_Unix::sig_term;
	sigaction(SIGINT, &action, 0);

	// Starting service in separate thread to avoid
	// signals sent to this pid causing EINTR errors
	// randomly in the service itself.
	CL_Thread thread;
	thread.start(this, &CL_Service_Unix::service_thread_main, args);
	while (true)
	{
		try
		{
			if (stop_event.wait())
				break;
		}
		catch (CL_Exception e)
		{
		}
	}
	thread.join();
	return 0;
}
