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

#include "precomp.h"
#include "wizard.h"
#include "resource.h"
#include "workspace_generator_msvc8.h"
#include "workspace.h"

Wizard::Wizard()
{
	// Upscale bitmaps to correct DPI:
	// (too bad this was all wasted effort as Microsoft didn't test WIZARD97 with other settings than 96 DPI!)

	HBITMAP logo = (HBITMAP)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDB_LOGO1), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	HBITMAP banner = (HBITMAP)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BANNER), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

	BITMAP logo_info, banner_info;
	GetObject(logo, sizeof(BITMAP), &logo_info);
	GetObject(banner, sizeof(BITMAP), &banner_info);

	SIZE logo_size, banner_size;
	logo_size.cx = logo_info.bmWidth;
	logo_size.cy = logo_info.bmHeight;
	banner_size.cx = banner_info.bmWidth;
	banner_size.cy = banner_info.bmHeight;

	HDC dc = GetDC(0);
	int dpi = GetDeviceCaps(dc, LOGPIXELSX);
	float scale = dpi / 96.0f;

	SIZE new_logo_size, new_banner_size;
	new_logo_size.cx = (int)std::round(logo_size.cx * scale);
	new_logo_size.cy = (int)std::round(logo_size.cy * scale);
	new_banner_size.cx = (int)std::round(banner_size.cx * scale);
	new_banner_size.cy = (int)std::round(banner_size.cy * scale);

	HBITMAP new_logo = CreateCompatibleBitmap(dc, new_logo_size.cx, new_logo_size.cy);
	HBITMAP new_banner = CreateCompatibleBitmap(dc, new_banner_size.cx, new_banner_size.cy);

	HDC dest_dc = CreateCompatibleDC(dc);
	HDC src_dc = CreateCompatibleDC(dc);
	HGDIOBJ old_dest_bitmap;
	HGDIOBJ old_src_bitmap;

	int old_stretch_mode = SetStretchBltMode(dest_dc, HALFTONE);
	SetBrushOrgEx(dest_dc, 0, 0, 0);

	old_dest_bitmap = SelectObject(dest_dc, new_logo);
	old_src_bitmap = SelectObject(src_dc, logo);
	StretchBlt(dest_dc, 0, 0, new_logo_size.cx, new_logo_size.cy, src_dc, 0, 0, logo_size.cx, logo_size.cy, SRCCOPY);

	SelectObject(dest_dc, new_banner);
	SelectObject(src_dc, banner);
	StretchBlt(dest_dc, 0, 0, new_banner_size.cx, new_banner_size.cy, src_dc, 0, 0, banner_size.cx, banner_size.cy, SRCCOPY);

	SelectObject(dest_dc, old_dest_bitmap);
	SelectObject(src_dc, old_src_bitmap);

	SetStretchBltMode(dest_dc, old_stretch_mode);

	DeleteDC(dest_dc);
	DeleteDC(src_dc);

	ReleaseDC(0, dc);

	memset(&propsheetheader, 0, sizeof(PROPSHEETHEADER));
	propsheetheader.dwSize = sizeof(PROPSHEETHEADER);
	propsheetheader.dwFlags = PSH_WIZARD97 | PSH_WATERMARK | PSH_HEADER | PSH_USEHBMWATERMARK | PSH_USEHBMHEADER;
	propsheetheader.hInstance = GetModuleHandle(0);
	propsheetheader.pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
	propsheetheader.pszCaption = MAKEINTRESOURCE(IDS_PROPSHT_CAPTION);
	propsheetheader.nPages = 5;
	propsheetheader.phpage = pages;
	propsheetheader.hbmWatermark = new_logo;
	propsheetheader.hbmHeader = new_banner;
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

		DWORD include_x64 = (page_target.include_x64 ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("IncludeX64"), 0, REG_DWORD,
			(LPBYTE) &include_x64, sizeof(DWORD));

		DWORD target_android = (page_target.target_android ? 1 : 0);
		RegSetValueEx(
			hKey, TEXT("TargetAndroid"), 0, REG_DWORD,
			(LPBYTE)&target_android, sizeof(DWORD));

		RegCloseKey(hKey);
	}

	Workspace workspace = create_workspace(page_target.target_android);

	WorkspaceGenerator_MSVC8 generator;
	generator.set_target_version(page_target.target_version);

	if (page_target.target_android)
	{
		generator.set_platforms(false, false, page_target.include_sse2, page_target.include_intrinsics, page_target.enable_debug_optimize, page_target.enable_whole_program_optimize);
		generator.set_android(true);
	}
	else
	{
		generator.set_platforms(true, page_target.include_x64, page_target.include_sse2, page_target.include_intrinsics, page_target.enable_debug_optimize, page_target.enable_whole_program_optimize);
		generator.set_android(false);
	}

	generator.enable_configurations(page_target.include_mtdll);
	generator.write(workspace);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Workspace creation:

Workspace Wizard::create_workspace(bool target_android)
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

	std::list<std::string> ignore_list;
	ignore_list.push_back(".");
	ignore_list.push_back("..");

	if (target_android)
	{
		ignore_list.push_back("Win32");
		ignore_list.push_back("WGL");
		ignore_list.push_back("setupnetwork_win32.cpp");
		ignore_list.push_back("font_engine_win32.h");
		ignore_list.push_back("font_engine_win32.cpp");
	}
	else
	{
		ignore_list.push_back("Unix");
		ignore_list.push_back("Android");
		ignore_list.push_back("setupnetwork_unix.cpp");
		ignore_list.push_back("font_engine_freetype.h");
		ignore_list.push_back("font_engine_freetype.cpp");
	}

	ignore_list.push_back("X11");
	ignore_list.push_back("Makefile.am");
	ignore_list.push_back("GLX");
	ignore_list.push_back("MacOS");
	ignore_list.push_back("MacOSX");
	ignore_list.push_back("Linux");
	ignore_list.push_back("OSX");
	ignore_list.push_back("Cocoa");
	ignore_list.push_back("AGL");
	ignore_list.push_back("font_engine_cocoa.h");
	ignore_list.push_back("font_engine_cocoa.mm");

	Project clanCore(
		"Core",
		"clanCore",
		"core.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanApp(
		"App",
		"clanApp",
		"application.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanNetwork(
		"Network",
		"clanNetwork",
		"network.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanDisplay(
		"Display",
		"clanDisplay",
		"display.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanSound(
		"Sound",
		"clanSound",
		"sound.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanGL(
		"GL",
		"clanGL",
		"gl.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanUI(
		"UI",
		"clanUI",
		"ui.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);

	Project clanXML(
		"XML",
		"clanXML",
		"xml.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug, ignore_list);


	// Add projects to workspace:
	workspace.projects.push_back(clanCore);
	workspace.projects.push_back(clanApp);
	workspace.projects.push_back(clanNetwork);
	workspace.projects.push_back(clanDisplay);
	workspace.projects.push_back(clanSound);
	workspace.projects.push_back(clanGL);
	workspace.projects.push_back(clanUI);
	workspace.projects.push_back(clanXML);

	if (!target_android)
	{
		Project clanD3D(
			"D3D",
			"clanD3D",
			"d3d.h",
			libs_list_shared,
			libs_list_release,
			libs_list_debug, ignore_list);

		workspace.projects.push_back(clanD3D);
	}


	return workspace;
}

std::string Wizard::text_to_local8(const std::wstring &text)
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

std::wstring Wizard::local8_to_text(const std::string &local8)
{
	int text_length = MultiByteToWideChar(CP_ACP, 0, local8.data(), int(local8.length()), 0, 0);
	if (text_length <= 0)
		return std::wstring();
	WCHAR *buffer = new WCHAR[text_length];
	if (buffer == 0)
		return std::wstring();
	text_length = MultiByteToWideChar(CP_ACP, 0, local8.data(), int(local8.length()), buffer, text_length);
	if (text_length <= 0)
	{
		delete[] buffer;
		return std::wstring();
	}
	std::wstring s(buffer, text_length);
	delete[] buffer;
	return s;
}

