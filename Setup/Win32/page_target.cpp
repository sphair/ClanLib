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
#include "page_target.h"
#include "resource.h"
#include <shlobj.h>

typedef HRESULT (WINAPI *FolderPathFunc)(HWND, LPTSTR, int, BOOL);

PageTarget::PageTarget()
{
	target_version = 710;

	HKEY hKey = 0;
	LONG result = RegOpenKeyEx(
		HKEY_CURRENT_USER, TEXT("Software\\ClanSoft\\ClanLib Configure\\InstallLocation"),
		0, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS)
	{
		DWORD type;
		DWORD value;
		DWORD size;
		
		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("TargetVersion"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			target_version = value;
		}

		RegCloseKey(hKey);
	}

	memset(&propsheetpage, 0, sizeof(PROPSHEETPAGE));
	propsheetpage.dwSize = sizeof(PROPSHEETPAGE);
	propsheetpage.dwFlags = PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	propsheetpage.pszTemplate = MAKEINTRESOURCE(IDD_TARGET_PAGE);
	propsheetpage.pfnDlgProc = (DLGPROC) &PageTarget::dialog_proc;
	propsheetpage.lParam = (LPARAM) this;
	propsheetpage.pszHeaderTitle = TEXT("Project Setup");
	propsheetpage.pszHeaderSubTitle = TEXT("Development environment");
	handle_propsheetpage = CreatePropertySheetPage(&propsheetpage);
}

LRESULT CALLBACK PageTarget::dialog_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND default_focus = (HWND) wParam;
			PROPSHEETPAGE *propsheetpage = (PROPSHEETPAGE *) lParam;
			PageTarget *self = (PageTarget *) propsheetpage->lParam;
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) self);

			if (self->target_version == 600)
				CheckRadioButton(hWnd, IDC_RADIO_VC60, IDC_RADIO_VC80, IDC_RADIO_VC60);
			else if (self->target_version == 700)
				CheckRadioButton(hWnd, IDC_RADIO_VC60, IDC_RADIO_VC80, IDC_RADIO_VC70);
			else if (self->target_version == 710)
				CheckRadioButton(hWnd, IDC_RADIO_VC60, IDC_RADIO_VC80, IDC_RADIO_VC71);
			else if (self->target_version == 800)
				CheckRadioButton(hWnd, IDC_RADIO_VC60, IDC_RADIO_VC80, IDC_RADIO_VC80);

			// return FALSE if we set the focus
			return TRUE;
		}
	case WM_NOTIFY:
		{
			NMHDR *header = (NMHDR *) lParam;
			PageTarget *self = (PageTarget *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			return self->on_notify(hWnd, header);
		}
	case WM_DESTROY:
	default:
		return FALSE;
	}
}

LRESULT PageTarget::on_notify(HWND hWnd, NMHDR *header)
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
		if (IsDlgButtonChecked(hWnd, IDC_RADIO_VC60) == BST_CHECKED)
			target_version = 600;
		else if (IsDlgButtonChecked(hWnd, IDC_RADIO_VC70) == BST_CHECKED)
			target_version = 700;
		else if (IsDlgButtonChecked(hWnd, IDC_RADIO_VC71) == BST_CHECKED)
			target_version = 710;
		else if (IsDlgButtonChecked(hWnd, IDC_RADIO_VC80) == BST_CHECKED)
			target_version = 800;
		return TRUE;
	case PSN_WIZFINISH:
	default:
		return FALSE;
	}
}
