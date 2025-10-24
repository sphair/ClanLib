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
*/

#include "precomp.h"
#include "wizard.h"
#include "resource.h"
#include "workspace_generator_msvc.h"
#include "../Generic/workspace.h"

Wizard::Wizard()
{
	memset(&propsheetheader, 0, sizeof(PROPSHEETHEADER));
	propsheetheader.dwSize = sizeof(PROPSHEETHEADER);
	propsheetheader.dwFlags = PSH_WIZARD97|PSH_WATERMARK|PSH_HEADER;
	propsheetheader.hInstance = GetModuleHandle(0);
	propsheetheader.pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
	propsheetheader.pszCaption = MAKEINTRESOURCE(IDS_PROPSHT_CAPTION);
	propsheetheader.nPages = 5;
	propsheetheader.phpage = pages;
	propsheetheader.pszbmWatermark = MAKEINTRESOURCE(IDB_LOGO1);
	propsheetheader.pszbmHeader = MAKEINTRESOURCE(IDB_BANNER);
	pages[0] = page_welcome.handle_propsheetpage;
	pages[1] = page_target.handle_propsheetpage;
	pages[2] = page_system.handle_propsheetpage;
	pages[3] = page_system2.handle_propsheetpage;
	pages[4] = page_finished.handle_propsheetpage;
	page_finished.wizard = this;
}

INT_PTR Wizard::exec()
{
	return PropertySheet(&propsheetheader);
}

BOOL Wizard::finish()
{
	HKEY hKey = 0;
	LONG result = RegCreateKeyEx(
		HKEY_CURRENT_USER, TEXT("Software\\ClanSoft\\ClanLib Configure\\InstallLocation"),
		0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
	if (result == ERROR_SUCCESS)
	{
		RegSetValueEx(
			hKey, TEXT("InputInclude"), 0, REG_SZ,
			(LPBYTE) page_system.path_input_include,
			(_tcslen(page_system.path_input_include) + 1) * sizeof(TCHAR));

		RegSetValueEx(
			hKey, TEXT("InputLib"), 0, REG_SZ,
			(LPBYTE) page_system.path_input_lib,
			(_tcslen(page_system.path_input_lib) + 1) * sizeof(TCHAR));

		RegSetValueEx(
			hKey, TEXT("OutputInclude"), 0, REG_SZ,
			(LPBYTE) page_system2.path_output_include,
			(_tcslen(page_system2.path_output_include) + 1) * sizeof(TCHAR));

		RegSetValueEx(
			hKey, TEXT("OutputLib"), 0, REG_SZ,
			(LPBYTE) page_system2.path_output_lib,
			(_tcslen(page_system2.path_output_lib) + 1) * sizeof(TCHAR));

		DWORD target_version = page_target.target_version;
		RegSetValueEx(
			hKey, TEXT("TargetVersion"), 0, REG_DWORD,
			(LPBYTE) &target_version, sizeof(DWORD));

		RegCloseKey(hKey);
	}


	Workspace workspace = create_workspace();

	if (page_target.target_version == 600)
	{
		WorkspaceGenerator_MSVC6 generator6;
		generator6.write(workspace);
	}
	else
	{
		WorkspaceGenerator_MSVC7 generator7;
		generator7.target_version = page_target.target_version;
		generator7.write(workspace);
	}

	/*
	\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Include Dirs
	\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Library Dirs
	\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Path Dirs
	\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Source Dirs
	*/

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Workspace creation:

Workspace Wizard::create_workspace()
{
	Workspace workspace;
	workspace.input_lib_dir = text_to_local8(page_system.path_input_lib);
	workspace.input_include_dir = text_to_local8(page_system.path_input_include);
	workspace.output_include_dir = text_to_local8(page_system2.path_output_include);
	workspace.output_lib_dir = text_to_local8(page_system2.path_output_lib);

	// Write all library project files:
	std::list<std::string> libs_list_shared;
	std::list<std::string> libs_list_release;
	std::list<std::string> libs_list_debug;
	std::list<std::string> defines_list;

	defines_list.push_back("USE_OPENGL");
	defines_list.push_back("USE_NETWORK");
	defines_list.push_back("USE_CLANSOUND");
	
	defines_list.push_back("DIRECTINPUT_VERSION=0x0800");
	
	Project clanCore(
		"Core",
		"clanCore",
		"core.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanSignals(
		"Signals",
		"clanSignals",
		"signals.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanApp(
		"Application",
		"clanApp",
		"application.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanNetwork(
		"Network",
		"clanNetwork",
		"network.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanDisplay(
		"Display",
		"clanDisplay",
		"display.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanSound(
		"Sound",
		"clanSound",
		"sound.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanGL(
		"GL",
		"clanGL",
		"gl.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanSDL(
		"SDL",
		"clanSDL",
		"sdl.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanGUI(
		"GUI",
		"clanGUI",
		"gui.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanGUIStyleSilver(
		"GUIStyleSilver",
		"clanGUIStyleSilver",
		"guistylesilver.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanVorbis(
		"Vorbis",
		"clanVorbis",
		"vorbis.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanMikMod(
		"MikMod",
		"clanMikMod",
		"mikmod.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	// Add projects to workspace:
	workspace.projects.push_back(clanSignals);
	workspace.projects.push_back(clanCore);
	workspace.projects.push_back(clanApp);
	workspace.projects.push_back(clanNetwork);
	workspace.projects.push_back(clanDisplay);
	workspace.projects.push_back(clanSound);
	workspace.projects.push_back(clanGL);
	workspace.projects.push_back(clanSDL);
	workspace.projects.push_back(clanGUI);
	workspace.projects.push_back(clanGUIStyleSilver);
	workspace.projects.push_back(clanVorbis);
	workspace.projects.push_back(clanMikMod);

	return workspace;
}