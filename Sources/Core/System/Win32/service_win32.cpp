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
#include "API/Core/Text/console.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/thread.h"
#include "service_win32.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Win32 Construction:

CL_Service_Win32::CL_Service_Win32(CL_Service *service, const CL_String &service_name)
: CL_Service_Impl(service, service_name), debug_mode(false), check_point(1)
{
}

CL_Service_Win32::~CL_Service_Win32()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Win32 Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Win32 Operations:

int CL_Service_Win32::main(int argc, char **argv)
{
	std::vector<CL_String> args;
	for (int i=0; i<argc; i++)
		args.push_back(CL_StringHelp::local8_to_text(argv[i]));

	if (argc == 2 && args[1] == "-debug")
	{
		return run_debug(args);
	}
	else if (argc == 2 && args[1] == "-install")
	{
		return run_install();
	}
	else if (argc == 2 && args[1] == "-uninstall")
	{
		return run_uninstall();
	}
	else
	{
		print_help();
		return run_service();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Service_Win32 Implementation:

int CL_Service_Win32::run_debug(std::vector<CL_String> args)
{
	debug_mode = true;
	SetConsoleCtrlHandler(&CL_Service_Win32::control_handler, TRUE);

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
	return 0;
}

int CL_Service_Win32::run_service()
{
	SERVICE_TABLE_ENTRY dispatch_table[] =
	{
		{ (LPTSTR) service_name.c_str(), (LPSERVICE_MAIN_FUNCTION) &CL_Service_Win32::service_thread_main },
		{ 0, 0 }
	};

	BOOL start_service = StartServiceCtrlDispatcher(dispatch_table);
	if (start_service == FALSE)
		return 0;

	return 0;
}

void CL_Service_Win32::service_thread_main(DWORD argc, LPTSTR *argv)
{
	if (instance == 0)
		return;

	CL_Service_Win32 *instance_win32 = (CL_Service_Win32 *) instance;

	memset(&instance_win32->service_status, 0, sizeof(SERVICE_STATUS));
	instance_win32->service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	instance_win32->service_status.dwServiceSpecificExitCode = 0;
	if (!instance_win32->debug_mode)
		instance_win32->handle_service_status = RegisterServiceCtrlHandler(CL_StringHelp::utf8_to_ucs2(instance_win32->service_name).c_str(), service_ctrl);

	instance_win32->report_status(SERVICE_START_PENDING, NO_ERROR, 2000);

	std::vector<CL_String> args;
	for (DWORD i=0; i<argc; i++)
		args.push_back(argv[i]);

	instance_win32->report_status(SERVICE_RUNNING, NO_ERROR, 0);

	instance_win32->service_start(args);
	while (true)
	{
		int wakeup_reason = CL_Event::wait(instance_win32->stop_event, instance_win32->reload_event);
		if (wakeup_reason == 1)
			instance_win32->service_reload();
		else
			break;
	}
	instance_win32->service_stop();

	instance_win32->report_status(SERVICE_STOPPED, NO_ERROR, 2000);
}

BOOL CL_Service_Win32::control_handler(DWORD ctrl_type)
{
	if (instance == 0)
		return FALSE;

	CL_Service_Win32 *instance_win32 = (CL_Service_Win32 *) instance;

	switch(ctrl_type)
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
		instance_win32->report_status(SERVICE_STOP_PENDING, NO_ERROR, 15000);
		instance_win32->stop_event.set();
		return TRUE;
	}
	return FALSE;
}

BOOL CL_Service_Win32::report_status(
	DWORD current_state,
	DWORD win32_exitcode,
	DWORD wait_hint)
{
	if (debug_mode)
		return TRUE;

	if (current_state != SERVICE_START_PENDING)
		service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	service_status.dwCurrentState = current_state;
	service_status.dwWin32ExitCode = win32_exitcode;
	service_status.dwWaitHint = wait_hint;

	switch (current_state)
	{
	case SERVICE_RUNNING:
	case SERVICE_STOPPED:
		break;
	default:
		service_status.dwCheckPoint = check_point++;
	}

	return SetServiceStatus(handle_service_status, &service_status);
}

VOID WINAPI CL_Service_Win32::service_ctrl(DWORD ctrl_code)
{
	if (instance == 0)
		return;

	CL_Service_Win32 *instance_win32 = (CL_Service_Win32 *) instance;

	if (ctrl_code == SERVICE_CONTROL_STOP)
	{
		instance_win32->report_status(SERVICE_STOP_PENDING, NO_ERROR, 1000);
		instance_win32->stop_event.set();
	}
	else
	{
		instance_win32->report_status(instance_win32->service_status.dwCurrentState, NO_ERROR, 0);
	}
}

int CL_Service_Win32::run_install()
{
	TCHAR exe_filename[MAX_PATH];
	BOOL result = GetModuleFileName(0, exe_filename, 1024);
	if (result == FALSE)
	{
		CL_Console::write_line("Could not install service. Unable to retrieve executable path.");
		return 1;
	}

	SC_HANDLE handle_manager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (handle_manager == 0)
	{
		CL_Console::write_line("Could not install service. Failed to open service control manager.");
		return 2;
	}

	SC_HANDLE handle_service = CreateService(
		handle_manager, CL_StringHelp::utf8_to_ucs2(service_name).c_str(), CL_StringHelp::utf8_to_ucs2(service_name).c_str(),
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, exe_filename, 0, 0, 0, 0, 0);
	if (handle_service == 0)
	{
		CloseServiceHandle(handle_manager);
		CL_Console::write_line("Could not create service in service control manager.");
		return 3;
	}

	CloseServiceHandle(handle_service);
	CloseServiceHandle(handle_manager);

	CL_Console::write_line("%1 installed.", service_name);
	return 0;
}

int CL_Service_Win32::run_uninstall()
{
	SC_HANDLE handle_manager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (handle_manager == 0)
	{
		CL_Console::write_line("Could not uninstall service. Failed to open service control manager.");
		return 1;
	}

	SC_HANDLE handle_service = OpenService(
		handle_manager, CL_StringHelp::utf8_to_ucs2(service_name).c_str(), SERVICE_ALL_ACCESS);
	if (handle_service == 0)
	{
		CloseServiceHandle(handle_manager);
		CL_Console::write_line("Could not open service in service control manager.");
		return 2;
	}

	SERVICE_STATUS service_status;
	memset(&service_status, 0, sizeof(SERVICE_STATUS));
	if (ControlService(handle_service, SERVICE_CONTROL_STOP, &service_status))
	{
		CL_Console::write("Stopping %1..", service_name);
		Sleep(1000);
		while (QueryServiceStatus(handle_service, &service_status))
		{
			if (service_status.dwCurrentState == SERVICE_STOP_PENDING)
			{
				CL_Console::write(".");
				Sleep(1000);
			}
			else
			{
				break;
			}
		}
		CL_Console::write_line("");
		if (service_status.dwCurrentState == SERVICE_STOPPED)
			CL_Console::write_line("%1 stopped.", service_name);
		else
			CL_Console::write_line("%1 failed to stop.", service_name);
	}

	if (DeleteService(handle_service))
		CL_Console::write_line("%1 removed.", service_name);
	else
		CL_Console::write_line("Unable to uninstall service. DeleteService failed.");

	CloseServiceHandle(handle_service);
	CloseServiceHandle(handle_manager);
	return 0;
}

void CL_Service_Win32::print_help()
{
	CL_Console::write_line("Service parameters:");
	CL_Console::write_line("  -install            - Install service");
	CL_Console::write_line("  -uninstall          - Remove the service");
	CL_Console::write_line("  -debug              - Run service as a console application");
	CL_Console::write_line("");
	CL_Console::write_line("StartServiceCtrlDispatcher being called.");
	CL_Console::write_line("This may take several seconds. Please wait.");
}
