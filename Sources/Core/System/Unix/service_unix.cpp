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
#include "API/Core/System/service.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/exception.h"
#include "service_unix.h"
#include <iostream>
#include <thread>
#include <signal.h>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Service_Unix Construction:

Service_Unix::Service_Unix(Service *service, const std::string &service_name)
: Service_Impl(service, service_name)
{
}

Service_Unix::~Service_Unix()
{
}

/////////////////////////////////////////////////////////////////////////////
// Service_Unix Attributes:

/////////////////////////////////////////////////////////////////////////////
// Service_Unix Operations:

int Service_Unix::main(int argc, char **argv)
{
	std::vector<std::string> args;
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
// Service_Unix Implementation:

void Service_Unix::service_thread_main(std::vector<std::string> args)
{
	service_start(args);
	while (true)
	{
		std::unique_lock<std::mutex> lock(mutex);
		event_condition.wait(lock, [&]() { return stop_flag || reload_flag; });

		if (stop_flag)
			break;

		reload_flag = false;
		service_reload();
	}
	service_stop();
}

void Service_Unix::sig_term(int signal_code)
{
	if (instance)
	{
		Service_Unix *instance_unix = static_cast<Service_Unix *> instance;

		{
			std::unique_lock<std::mutex> lock(instance_unix->mutex);
			instance_unix->stop_flag = true;
		}
		instance_unix->event_condition.notify_all();

	}
}

void Service_Unix::sig_hup(int signal_code)
{
	if (instance)
	{
		Service_Unix *instance_unix = static_cast<Service_Unix *> instance;
		{
			std::unique_lock<std::mutex> lock(instance_unix->mutex);
			instance_unix->reload_flag = true;
		}
		instance_unix->event_condition.notify_all();
	}
}

int Service_Unix::run_daemon(std::vector<std::string> args)
{
	try
	{
		File file(args[2], File::create_always, File::access_read_write);

		struct sigaction action;
		memset(&action, 0, sizeof(struct sigaction));
		action.sa_handler = &Service_Unix::sig_term;
		sigaction(SIGTERM, &action, nullptr);
		memset(&action, 0, sizeof(struct sigaction));
		action.sa_handler = &Service_Unix::sig_hup;
		sigaction(SIGHUP, &action, nullptr);

		int pid = fork();
		if (pid)
		{
			std::string str_pid = StringHelp::int_to_local8(pid);
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
			std::thread thread([=](){service_thread_main(args); });
			while (true)
			{
				std::unique_lock<std::mutex> lock(mutex);
				event_condition.wait(lock, [&]() { return stop_flag || reload_flag; });

				if (stop_flag)
					break;
	
			}
			thread.join();
			return 0;
		}
	}
	catch (Exception &e)
	{
		std::cout << "Unhandled exception: " << e.message.c_str() << std::endl;
	}
	return 1;
}

int Service_Unix::run_debug(std::vector<std::string> args)
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &Service_Unix::sig_term;
	sigaction(SIGTERM, &action, nullptr);
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &Service_Unix::sig_hup;
	sigaction(SIGHUP, &action, nullptr);
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &Service_Unix::sig_term;
	sigaction(SIGINT, &action, nullptr);

	// Starting service in separate thread to avoid
	// signals sent to this pid causing EINTR errors
	// randomly in the service itself.
	std::thread thread([=](){service_thread_main(args); });

	while (true)
	{
		std::unique_lock<std::mutex> lock(mutex);
		event_condition.wait(lock, [&]() { return stop_flag || reload_flag; });

		if (stop_flag)
			break;
	}
	thread.join();
	return 0;
}

}

