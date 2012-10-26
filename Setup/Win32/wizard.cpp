/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "workspace_generator_msvc8.h"
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
	// Simple check to see if we are in ClanLib root directory - check if we can locate the Sources directory
	DWORD file_attributes = GetFileAttributes(TEXT("Sources"));
	if (file_attributes != INVALID_FILE_ATTRIBUTES && (file_attributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return PropertySheet(&propsheetheader);
	}
	else
	{
		MessageBox(0, TEXT("Unable to locate ClanLib directory.\n\nPlease start this application from the ClanLib root directory\n(where the Sources and Projects directories are located),\nor start the application directly from Visual Studio."), TEXT("Unable to locate ClanLib"), MB_ICONERROR|MB_OK);
		return 0;
	}
}

BOOL Wizard::finish()
{
	HKEY hKey = 0;
	LONG result = RegCreateKeyEx(
		HKEY_CURRENT_USER, TEXT("Software\\Clanlib.org\\ClanLib Configure\\InstallLocation"),
		0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
	if (result == ERROR_SUCCESS)
	{
		RegSetValueEx(
			hKey, TEXT("InputInclude"), 0, REG_SZ,
			(LPBYTE) page_system.path_input_include,
			(DWORD) (_tcslen(page_system.path_input_include) + 1) * sizeof(TCHAR));

		RegSetValueEx(
			hKey, TEXT("InputLib"), 0, REG_SZ,
			(LPBYTE) page_system.path_input_lib,
			(DWORD) (_tcslen(page_system.path_input_lib) + 1) * sizeof(TCHAR));

		RegSetValueEx(
			hKey, TEXT("OutputInclude"), 0, REG_SZ,
			(LPBYTE) page_system2.path_output_include,
			(DWORD) (_tcslen(page_system2.path_output_include) + 1) * sizeof(TCHAR));

		RegSetValueEx(
			hKey, TEXT("OutputLib"), 0, REG_SZ,
			(LPBYTE) page_system2.path_output_lib,
			(DWORD) (_tcslen(page_system2.path_output_lib) + 1) * sizeof(TCHAR));

		DWORD target_version = page_target.target_version;
		RegSetValueEx(
			hKey, TEXT("TargetVersion"), 0, REG_DWORD,
			(LPBYTE) &target_version, sizeof(DWORD));

		DWORD include_unicode = (page_target.include_unicode ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeUnicode"), 0, REG_DWORD,
			(LPBYTE) &include_unicode, sizeof(DWORD));

		DWORD include_mtdll = (page_target.include_mtdll ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeMTDLL"), 0, REG_DWORD,
			(LPBYTE) &include_mtdll, sizeof(DWORD));

		DWORD include_sse2 = (page_target.include_sse2 ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeSSE2"), 0, REG_DWORD,
			(LPBYTE) &include_sse2, sizeof(DWORD));

		DWORD enable_debug_optimize = (page_target.enable_debug_optimize ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("DebugOptimize"), 0, REG_DWORD,
			(LPBYTE) &enable_debug_optimize, sizeof(DWORD));

		DWORD enable_whole_program_optimize = (page_target.enable_whole_program_optimize ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("WholeProgramOptimize"), 0, REG_DWORD,
			(LPBYTE) &enable_whole_program_optimize, sizeof(DWORD));

		DWORD include_intrinsics = (page_target.include_intrinsics ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeIntrinsics"), 0, REG_DWORD,
			(LPBYTE) &include_intrinsics, sizeof(DWORD));

		DWORD include_dll = (page_target.include_dll ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeDLL"), 0, REG_DWORD,
			(LPBYTE) &include_dll, sizeof(DWORD));

		DWORD include_x64 = (page_target.include_x64 ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeX64"), 0, REG_DWORD,
			(LPBYTE) &include_x64, sizeof(DWORD));

		RegCloseKey(hKey);
	}

	Workspace workspace = create_workspace();

	if(page_target.target_version == 900)
	{
		WorkspaceGenerator_MSVC8 generator8;
		generator8.set_target_version(page_target.target_version);
		generator8.set_platforms(true, page_target.include_x64, page_target.include_sse2, page_target.include_intrinsics, page_target.enable_debug_optimize, page_target.enable_whole_program_optimize);
		generator8.enable_configurations(page_target.include_mtdll, page_target.include_dll);
		generator8.write(workspace);
	}
	else if(page_target.target_version == 1000)
	{
		WorkspaceGenerator_MSVC8 generator10;
		generator10.set_target_version(page_target.target_version);
		generator10.set_platforms(true, page_target.include_x64, page_target.include_sse2, page_target.include_intrinsics, page_target.enable_debug_optimize, page_target.enable_whole_program_optimize);
		generator10.enable_configurations(page_target.include_mtdll, page_target.include_dll);
		generator10.write(workspace);
	}

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
	
	Project clanCore(
		"Core",
		"clanCore",
		"core.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanDatabase(
		"Database",
		"clanDatabase",
		"database.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanSqlite(
		"Sqlite",
		"clanSqlite",
		"sqlite.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanApp(
		"App",
		"clanApp",
		"application.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanNetwork(
		"Network",
		"clanNetwork",
		"network.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanDisplay(
		"Display",
		"clanDisplay",
		"display.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanSound(
		"Sound",
		"clanSound",
		"sound.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanGL(
		"GL",
		"clanGL",
		"gl.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanGL1(
		"GL1",
		"clanGL1",
		"gl1.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanD3D(
		"D3D",
		"clanD3D",
		"d3d.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	//Project clanSWRender(
	//	"SWRender",
	//	"clanSWRender",
	//	"swrender.h",
	//	libs_list_shared,
	//	libs_list_release,
	//	libs_list_debug);

	Project clanGUI(
		"GUI",
		"clanGUI",
		"gui.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanVorbis(
		"Vorbis",
		"clanVorbis",
		"vorbis.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanCSSLayout(
		"CSSLayout",
		"clanCSSLayout",
		"csslayout.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	Project clanCompute(
		"Compute",
		"clanCompute",
		"compute.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug);

	// Add projects to workspace:
	workspace.projects.push_back(clanCore);
	workspace.projects.push_back(clanDatabase);
	workspace.projects.push_back(clanSqlite);
	workspace.projects.push_back(clanApp);
	workspace.projects.push_back(clanNetwork);
	workspace.projects.push_back(clanDisplay);
	workspace.projects.push_back(clanSound);
	workspace.projects.push_back(clanGL);
	workspace.projects.push_back(clanD3D);

	//if (page_target.include_intrinsics)
	//	workspace.projects.push_back(clanSWRender);

	workspace.projects.push_back(clanCSSLayout);
	workspace.projects.push_back(clanGL1);

	workspace.projects.push_back(clanGUI);
	workspace.projects.push_back(clanVorbis);

	workspace.projects.push_back(clanCompute);

	return workspace;
}

#ifdef UNICODE
std::string Wizard::text_to_local8(const tstring &text)
{
	int local8_length = WideCharToMultiByte(CP_ACP, 0, text.data(), int(text.length()), 0, 0, 0, 0);
	if (local8_length <= 0)
		return std::string();
	char *buffer = new char[local8_length];
	if (buffer == 0)
		return std::string();
	local8_length = WideCharToMultiByte(CP_ACP, 0, text.data(), int(text.length()), buffer, local8_length, 0, 0);
	if (local8_length <= 0)
	{
		delete[] buffer;
		return std::string();
	}
	std::string s(buffer, local8_length);
	delete[] buffer;
	return s;
}

Wizard::tstring Wizard::local8_to_text(const std::string &local8)
{
	int text_length = MultiByteToWideChar(CP_ACP, 0, local8.data(), int(local8.length()), 0, 0);
	if (text_length <= 0)
		return tstring();
	WCHAR *buffer = new WCHAR[text_length];
	if (buffer == 0)
		return tstring();
	text_length = MultiByteToWideChar(CP_ACP, 0, local8.data(), int(local8.length()), buffer, text_length);
	if (text_length <= 0)
	{
		delete[] buffer;
		return tstring();
	}
	tstring s(buffer, text_length);
	delete[] buffer;
	return s;
}
#else
std::string Wizard::text_to_local8(const tstring &text)
{
	return text;
}

tstring Wizard::local8_to_text(const std::string &local8)
{
	return local8;
}
#endif
