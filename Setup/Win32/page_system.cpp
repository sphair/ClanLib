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
#include "page_system.h"
#include "resource.h"
#include <shlobj.h>

typedef HRESULT (WINAPI *FolderPathFunc)(HWND, LPTSTR, int, BOOL);

PageSystem::PageSystem()
{
	memset(path_input_include, 0, sizeof(TCHAR) * MAX_PATH);
	memset(path_input_lib, 0, sizeof(TCHAR) * MAX_PATH);

	HINSTANCE dll = LoadLibrary(TEXT("shell32.dll"));
	if (dll != NULL)
	{
#ifdef UNICODE
		FARPROC func = GetProcAddress(dll, "SHGetSpecialFolderPathW");
#else
		FARPROC func = GetProcAddress(dll, "SHGetSpecialFolderPathA");
#endif
		if (func != NULL)
		{
			FolderPathFunc GetSpecialFolderPath = (FolderPathFunc) func;

			TCHAR path[MAX_PATH];
			GetSpecialFolderPath(
				NULL,
				path,
				CSIDL_PERSONAL,
				FALSE);

			_tcscpy(path_input_include, path);
			_tcscat(path_input_include, TEXT("\\include"));
			_tcscpy(path_input_lib, path);
			_tcscat(path_input_lib, TEXT("\\lib"));
		}

		FreeLibrary(dll);
	}

	HKEY hKey = 0;
	LONG result = RegOpenKeyEx(
		HKEY_CURRENT_USER, TEXT("Software\\ClanSoft\\ClanLib Configure\\InstallLocation"),
		0, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS)
	{
		DWORD type;
		TCHAR buffer[MAX_PATH+1];
		DWORD size;
		
		size = MAX_PATH;
		result = RegQueryValueEx(hKey, TEXT("InputInclude"), 0, &type, (LPBYTE) buffer, &size);
		if (result == ERROR_SUCCESS && type == REG_SZ)
		{
			buffer[size] = 0;
			_tcscpy(path_input_include, buffer);
		}

		size = MAX_PATH;
		result = RegQueryValueEx(hKey, TEXT("InputLib"), 0, &type, (LPBYTE) buffer, &size);
		if (result == ERROR_SUCCESS && type == REG_SZ)
		{
			buffer[size] = 0;
			_tcscpy(path_input_lib, buffer);
		}

		RegCloseKey(hKey);
	}

	memset(&propsheetpage, 0, sizeof(PROPSHEETPAGE));
	propsheetpage.dwSize = sizeof(PROPSHEETPAGE);
	propsheetpage.dwFlags = PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	propsheetpage.pszTemplate = MAKEINTRESOURCE(IDD_SYSTEM_PAGE);
	propsheetpage.pfnDlgProc = (DLGPROC) &PageSystem::dialog_proc;
	propsheetpage.lParam = (LPARAM) this;
	propsheetpage.pszHeaderTitle = TEXT("Input Library Setup");
	propsheetpage.pszHeaderSubTitle = TEXT("Search paths for headers and libraries");
	handle_propsheetpage = CreatePropertySheetPage(&propsheetpage);
}

LRESULT CALLBACK PageSystem::dialog_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND default_focus = (HWND) wParam;
			PROPSHEETPAGE *propsheetpage = (PROPSHEETPAGE *) lParam;
			PageSystem *self = (PageSystem *) propsheetpage->lParam;
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) self);

			SendMessage(GetDlgItem(hWnd, IDC_INPUT_INCLUDE), WM_SETTEXT, 0, (LPARAM) self->path_input_include);
			SendMessage(GetDlgItem(hWnd, IDC_INPUT_LIB), WM_SETTEXT, 0, (LPARAM) self->path_input_lib);

			// return FALSE if we set the focus
			return TRUE;
		}
	case WM_NOTIFY:
		{
			NMHDR *header = (NMHDR *) lParam;
			PageSystem *self = (PageSystem *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			return self->on_notify(hWnd, header);
		}
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			PageSystem *self = (PageSystem *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			switch (LOWORD(wParam))
			{
			case IDC_INPUT_LIB_BROWSE:
				self->on_input_lib_browse_clicked(hWnd);
				return TRUE;
			case IDC_INPUT_INCLUDE_BROWSE:
				self->on_input_include_browse_clicked(hWnd);
				return TRUE;
			}
		}
		return FALSE;
	case WM_DESTROY:
	default:
		return FALSE;
	}
}

LRESULT PageSystem::on_notify(HWND hWnd, NMHDR *header)
{
	// Don't go to next page yet:
	// SetWindowLong(hwnd, DWL_MSGRESULT, -1);
	// return TRUE;

	// Go straight to the completion page:
	// SetWindowLong(hwnd, DWL_MSGRESULT, IDD_FINISH);
	// return TRUE;

	// Not finished yet:
	// SetWindowLong(hwnd, DWL_MSGRESULT, TRUE);
	// return TRUE;

	switch (header->code)
	{
	case PSN_SETACTIVE:
		PostMessage(header->hwndFrom, PSM_SETWIZBUTTONS, 0, PSWIZB_BACK|PSWIZB_NEXT);
		return TRUE;
	case PSN_WIZBACK:
	case PSN_WIZNEXT:
		SendMessage(GetDlgItem(hWnd, IDC_INPUT_INCLUDE), WM_GETTEXT, MAX_PATH, (LPARAM) path_input_include);
		SendMessage(GetDlgItem(hWnd, IDC_INPUT_LIB), WM_GETTEXT, MAX_PATH, (LPARAM) path_input_lib);
		return TRUE;
	case PSN_WIZFINISH:
	default:
		return FALSE;
	}
}

void PageSystem::on_input_lib_browse_clicked(HWND hWnd)
{
	TCHAR str[1024];
	str[0] = 0;
	if (BrowseForFolder(hWnd, TEXT("Select input Library directory"), str))
	{
		SendMessage(GetDlgItem(hWnd, IDC_INPUT_LIB), WM_SETTEXT, 0, (LPARAM) str);
	}
}

void PageSystem::on_input_include_browse_clicked(HWND hWnd)
{
	TCHAR str[1024];
	str[0] = 0;
	if (BrowseForFolder(hWnd, TEXT("Select input Include directory"), str))
	{
		SendMessage(GetDlgItem(hWnd, IDC_INPUT_INCLUDE), WM_SETTEXT, 0, (LPARAM) str);
	}
}

BOOL PageSystem::BrowseForFolder(HWND hOwner, TCHAR* szTitle, TCHAR* szRetval)
{
	BROWSEINFO info;
	LPITEMIDLIST itemidlist;
	TCHAR szDirectory[_MAX_PATH];
	LPMALLOC pMalloc;
	memset(szDirectory, 0, _MAX_PATH * sizeof(TCHAR));

	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		info.hwndOwner = hOwner;
		info.pidlRoot = NULL;
		info.pszDisplayName = szDirectory;
		info.lpszTitle = szTitle;
		info.ulFlags = 0;
		info.lpfn = NULL;

		itemidlist = SHBrowseForFolder(&info);
		if (itemidlist != NULL)
		{
			SHGetPathFromIDList(itemidlist, szRetval);
			pMalloc->Free(itemidlist);
			pMalloc->Release();
			return TRUE;
		}
		else // User clicked Cancel
		{
			pMalloc->Release();
			return FALSE;
		}
	}
	else
		return FALSE;
}
