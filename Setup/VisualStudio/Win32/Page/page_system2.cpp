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

#include "..\precomp.h"
#include "page_system2.h"
#include "..\resource.h"
#include <shlobj.h>

typedef HRESULT (WINAPI *FolderPathFunc)(HWND, LPTSTR, int, BOOL);

PageSystem2::PageSystem2()
{
	memset(path_output_include, 0, sizeof(TCHAR) * MAX_PATH);
	memset(path_output_lib, 0, sizeof(TCHAR) * MAX_PATH);

	_tcscpy(path_output_include, TEXT("C:\\Development\\Environment\\ClanLib\\Include"));
	_tcscpy(path_output_lib, TEXT("C:\\Development\\Environment\\ClanLib\\Lib"));

	HKEY hKey = 0;
	LONG result = RegOpenKeyEx(
		HKEY_CURRENT_USER, TEXT("Software\\Clanlib.org\\ClanLib Configure\\InstallLocation"),
		0, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS)
	{
		DWORD type;
		TCHAR buffer[MAX_PATH+1];
		DWORD size;
		
		size = MAX_PATH;
		result = RegQueryValueEx(hKey, TEXT("OutputInclude"), 0, &type, (LPBYTE) buffer, &size);
		if (result == ERROR_SUCCESS && type == REG_SZ)
		{
			buffer[size] = 0;
			_tcscpy(path_output_include, buffer);
		}

		size = MAX_PATH;
		result = RegQueryValueEx(hKey, TEXT("OutputLib"), 0, &type, (LPBYTE) buffer, &size);
		if (result == ERROR_SUCCESS && type == REG_SZ)
		{
			buffer[size] = 0;
			_tcscpy(path_output_lib, buffer);
		}

		RegCloseKey(hKey);
	}

	memset(&propsheetpage, 0, sizeof(PROPSHEETPAGE));
	propsheetpage.dwSize = sizeof(PROPSHEETPAGE);
	propsheetpage.dwFlags = PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	propsheetpage.pszTemplate = MAKEINTRESOURCE(IDD_SYSTEM2_PAGE);
	propsheetpage.pfnDlgProc = &PageSystem2::dialog_proc;
	propsheetpage.lParam = (LPARAM) this;
	propsheetpage.pszHeaderTitle = TEXT("Output Library Setup");
	propsheetpage.pszHeaderSubTitle = TEXT("Installation paths");
	handle_propsheetpage = CreatePropertySheetPage(&propsheetpage);
}

INT_PTR CALLBACK PageSystem2::dialog_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND default_focus = (HWND) wParam;
			PROPSHEETPAGE *propsheetpage = (PROPSHEETPAGE *) lParam;
			PageSystem2 *self = (PageSystem2 *) propsheetpage->lParam;
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) self);

			SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_INCLUDE), WM_SETTEXT, 0, (LPARAM) self->path_output_include);
			SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_LIB), WM_SETTEXT, 0, (LPARAM) self->path_output_lib);

			// return FALSE if we set the focus
			return TRUE;
		}
	case WM_NOTIFY:
		{
			NMHDR *header = (NMHDR *) lParam;
			PageSystem2 *self = (PageSystem2 *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			return self->on_notify(hWnd, header);
		}
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			PageSystem2 *self = (PageSystem2 *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			switch (LOWORD(wParam))
			{
			case IDC_OUTPUT_LIB_BROWSE:
				self->on_output_lib_browse_clicked(hWnd);
				return TRUE;
			case IDC_OUTPUT_INCLUDE_BROWSE:
				self->on_output_include_browse_clicked(hWnd);
				return TRUE;
			}
		}
		return FALSE;
	case WM_DESTROY:
	default:
		return FALSE;
	}
}

INT_PTR PageSystem2::on_notify(HWND hWnd, NMHDR *header)
{
	switch (header->code)
	{
	case PSN_SETACTIVE:
		PostMessage(header->hwndFrom, PSM_SETWIZBUTTONS, 0, PSWIZB_BACK|PSWIZB_NEXT);
		return TRUE;
	case PSN_WIZBACK:
	case PSN_WIZNEXT:
		SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_INCLUDE), WM_GETTEXT, MAX_PATH, (LPARAM) path_output_include);
		SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_LIB), WM_GETTEXT, MAX_PATH, (LPARAM) path_output_lib);
		return TRUE;
	case PSN_WIZFINISH:
	default:
		return FALSE;
	}
}

void PageSystem2::on_output_lib_browse_clicked(HWND hWnd)
{
	TCHAR str[1024];
	str[0] = 0;
	if (BrowseForFolder(hWnd, TEXT("Select output Library directory"), str))
	{
		SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_LIB), WM_SETTEXT, 0, (LPARAM) str);
	}
}

void PageSystem2::on_output_include_browse_clicked(HWND hWnd)
{
	TCHAR str[1024];
	str[0] = 0;
	if (BrowseForFolder(hWnd, TEXT("Select output Include directory"), str))
	{
		SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_INCLUDE), WM_SETTEXT, 0, (LPARAM) str);
	}
}

BOOL PageSystem2::BrowseForFolder(HWND hOwner, TCHAR* szTitle, TCHAR* szRetval)
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
