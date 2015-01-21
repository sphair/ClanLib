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

	int retval;

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
	if (clan::Application::main == 0)
	{
		MessageBox(NULL, TEXT("No program instance found"), TEXT("ClanLib/Win32"), 32);
		return 0;
	}

	// Get commandline arguments.
	std::vector<std::string> args;
	calc_commandline(args);

	// Call clanapp main:
	if (clan::Application::enable_catch_exceptions)
	{
		try
		{
			retval = clan::Application::main(args);
		}
		catch(clan::Exception &exception)
		{
			// Create a console window for text-output if not available
			//std::string console_name("Console");
			//if (!args.empty())
			//	console_name = args[0];
			//clan::ConsoleWindow console(console_name, 80, 160);
			//clan::Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			//console.display_close_message();

			ExceptionDialog::show(exception);

			retval = -1;
		}

	}
	else
	{
		retval = clan::Application::main(args);
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

/////////////////////////////////////////////////////////////////////////////
// clan::Application:

clan::Application::MainFunction *clan::Application::main = 0;
bool clan::Application::enable_catch_exceptions = true;

