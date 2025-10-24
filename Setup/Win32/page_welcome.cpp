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
#include "page_welcome.h"
#include "resource.h"

PageWelcome::PageWelcome()
{
	memset(&propsheetpage, 0, sizeof(PROPSHEETPAGE));
	propsheetpage.dwSize = sizeof(PROPSHEETPAGE);
	propsheetpage.dwFlags = PSP_HIDEHEADER;
	propsheetpage.pszTemplate = MAKEINTRESOURCE(IDD_WELCOME_PAGE);
	propsheetpage.pfnDlgProc = (DLGPROC) &PageWelcome::dialog_proc;
	propsheetpage.lParam = (LPARAM) this;

	handle_propsheetpage = CreatePropertySheetPage(&propsheetpage);
}

LRESULT CALLBACK PageWelcome::dialog_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND default_focus = (HWND) wParam;
			PROPSHEETPAGE *propsheetpage = (PROPSHEETPAGE *) lParam;
			SetWindowLongPtr(hWnd, GWL_USERDATA, propsheetpage->lParam);

			LPCTSTR welcome_text =
				TEXT("Welcome!\n")
				TEXT("\n")
				TEXT("This is the ClanLib configuration wizard. It will help you set up ")
				TEXT("a sane ClanLib build environment and customize the build process ")
				TEXT("for your needs.\n")
				TEXT("\n")
				TEXT("Enjoy.\n")
				TEXT("The ClanLib Development Team.");
			SendMessage(GetDlgItem(hWnd, IDC_WELCOME_TEXT), WM_SETTEXT, 0, (LPARAM) welcome_text);

			// return FALSE if we set the focus
			return TRUE;
		}
	case WM_NOTIFY:
		{
			NMHDR *header = (NMHDR *) lParam;
			PageWelcome *self = (PageWelcome *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			return self->on_notify(hWnd, header);
		}
	case WM_DESTROY:
	default:
		return FALSE;
	}
}

LRESULT PageWelcome::on_notify(HWND hWnd, NMHDR *header)
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
		PostMessage(header->hwndFrom, PSM_SETWIZBUTTONS, 0, PSWIZB_NEXT);
		return TRUE;
	case PSN_WIZBACK:
	case PSN_WIZNEXT:
	case PSN_WIZFINISH:
	default:
		return FALSE;
	}
}
