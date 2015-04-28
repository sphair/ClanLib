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

#include <ctype.h>
#include <windows.h>
#include <vector>
#if defined UNICODE && !defined _UNICODE
#define _UNICODE
#endif
#include <tchar.h>
#include "API/App/clanapp.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/console_window.h"
#include "API/Core/Text/console.h"
#include "API/Core/ErrorReporting/exception_dialog.h"
#include "API/display.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#if defined(_MSC_VER)
#pragma comment(lib, "user32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

namespace clan
{
	static void calc_commandline(std::vector<std::string> &out_args);

	static ApplicationInstancePrivate *app_instance = 0;
	static bool enable_catch_exceptions = false;
	static int timing_timeout = 0;

	static std::vector<std::string> command_line_args;

	ApplicationInstancePrivate::ApplicationInstancePrivate(bool catch_exceptions)
	{
		app_instance = this;
		enable_catch_exceptions = catch_exceptions;
	}

	const std::vector<std::string> &Application::main_args()
	{
		return command_line_args;
	}

	void Application::use_timeout_timing(int timeout)
	{
		timing_timeout = timeout;
	}
}

/////////////////////////////////////////////////////////////////////////////
// WinMain:

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	using namespace clan;

#if defined(_DEBUG) && defined(_MSC_VER)
	// Visual C++ memoryleak debugging. By setting the _CRTDBG_LEAK_CHECK_DF
	// flag, we produce a memory leak dump at exit in the visual c++ debug output
	// window.
//	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//	flag |= _CRTDBG_LEAK_CHECK_DF;
//	_CrtSetDbgFlag(flag);
#endif

	// Did the game developer remember to create one global application
	// interface?
	if (clan::app_instance == 0)
	{
		MessageBox(NULL, TEXT("No program instance found"), TEXT("ClanLib/Win32"), 32);
		return 0;
	}

	calc_commandline(command_line_args);

	int retval = 0;

	if (clan::enable_catch_exceptions)
	{
		try
		{
			std::unique_ptr<Application> app = app_instance->create();
			while (true)
			{
				try
				{
					if (!app->update())
						break;

					if (!clan::RunLoop::process(timing_timeout))
						break;
				}
				catch (...)
				{
					std::exception_ptr exception = std::current_exception();
					ExceptionDialog::show(exception);
					retval = -1;
					break;
				}
			}
		}
		catch (...)
		{
			std::exception_ptr exception = std::current_exception();
			ExceptionDialog::show(exception);
			retval = -1;
		}
	}
	else
	{
		std::unique_ptr<Application> app = app_instance->create();
		while (true)
		{
			if (!app->update())
				break;

			if (!clan::RunLoop::process(timing_timeout))
				break;
		}
	}

	return retval;
}

/////////////////////////////////////////////////////////////////////////////
// calc_commandline:

namespace clan
{
	static void calc_commandline(std::vector<std::string> &out_args)
	{
		TCHAR *command_line = GetCommandLine();
		static std::vector<TCHAR *> pos;
		bool new_arg = true;

		for (;*command_line;command_line++) 
		{
			if (*command_line == TEXT('"'))
			{
				new_arg = true;
				command_line++;
				TCHAR *start_arg = command_line;
				for (;*command_line && *command_line!='"';command_line++)
				{
					if (!_istspace(*command_line)) new_arg = false;
				}
				if (new_arg == false) pos.push_back(start_arg);
				if (*command_line == TEXT('"')) *command_line = 0;
				else if (*command_line == 0) break;
				command_line++;
				if (*command_line == 0) break;
				new_arg = true;
			}
		
			if (new_arg && !_istspace(*command_line))
			{
				new_arg = false;
				pos.push_back(command_line);
			}
			else if (!new_arg && _istspace(*command_line))
			{
				new_arg = true;
				*command_line = 0;
			}
		}
		out_args.reserve(pos.size());
		for (std::vector<TCHAR *>::size_type cnt=0; cnt<pos.size(); cnt++)
		{
			out_args.push_back(StringHelp::ucs2_to_text(pos[cnt]));
		}
	}
}
