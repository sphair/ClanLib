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

#include <ctype.h>
#include <windows.h>
#include <vector>
#include "API/Core/System/setupcore.h"
#include "API/Core/System/crash_reporter.h"
#include "API/Application/clanapp.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

static void calc_commandline(int *argc, char ***argv);

/////////////////////////////////////////////////////////////////////////////
// WinMain:

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int retval;
	int argc;
	char **argv;

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
	if (CL_ClanApplication::app == NULL)
	{
		MessageBox(NULL, "No program instance found", "ClanLib/Win32", 32);
		return 0;
	}

#ifndef _DEBUG
	// Create minidumps when not in debug mode.
	CL_CrashReporter crash_reporter;
#endif

	// Get commandline arguments.
	calc_commandline(&argc, &argv);

	// Initialize clanlib and call clanapp main:
	CL_SetupCore::set_instance(hInstance);
	CL_SetupCore::init();
	retval = CL_ClanApplication::app->main(argc, argv);
	CL_SetupCore::deinit();

	// calc_commandline() doesn't clean up after itself. tsk tsk:
	delete[] argv;

	return retval;
}

/////////////////////////////////////////////////////////////////////////////
// calc_commandline:

static void calc_commandline(int *argc, char ***argv)
{
	char *command_line = GetCommandLine();
	static std::vector<char *> pos;
	bool new_arg = true;

	for (;*command_line;command_line++) 
	{
		if (*command_line == '"')
		{
			new_arg = true;
			command_line++;
			char *start_arg = command_line;
			for (;*command_line && *command_line!='"';command_line++)
			{
				if (!isspace(*command_line)) new_arg = false;
			}
			if (new_arg == false) pos.push_back(start_arg);
			if (*command_line == '"') *command_line = 0;
			else if (*command_line == 0) break;
			command_line++;
			if (*command_line == 0) break;
			new_arg = true;
		}
		
		if (new_arg && !isspace(*command_line))
		{
			new_arg = false;
			pos.push_back(command_line);
		}
		else if (!new_arg && isspace(*command_line))
		{
			new_arg = true;
			*command_line = 0;
		}
	}
	int num_words = pos.size();

	char **words = new char*[num_words + 1];

	int i;
	for (i=0; i<num_words; i++)
	{
		words[i] = pos[i];
	}
	words[i] = NULL;

	*argc = num_words;
	*argv = words;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ClanApplication:

CL_ClanApplication *CL_ClanApplication::app = NULL;

CL_ClanApplication::CL_ClanApplication()
{
	CL_ClanApplication::app = this;
}

CL_ClanApplication::~CL_ClanApplication()
{
	CL_ClanApplication::app = NULL;
}
