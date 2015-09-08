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

#include "Display/precomp.h"
#include "dwm_functions.h"

namespace clan
{
	bool DwmFunctions::is_composition_enabled()
	{
		if (IsCompositionEnabled)
		{
			BOOL is_enabled = FALSE;
			HRESULT result = IsCompositionEnabled(&is_enabled);
			return SUCCEEDED(result) && is_enabled == TRUE;
		}
		else
		{
			return false;
		}
	}

	void DwmFunctions::enable_alpha_channel(HWND hwnd, HRGN rgn)
	{
		if (EnableBlurBehindWindow)
		{
			DWM_BLURBEHIND blur_behind = { 0 };
			blur_behind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED;
			blur_behind.fEnable = TRUE;
			blur_behind.fTransitionOnMaximized = TRUE;
			blur_behind.hRgnBlur = rgn;

			EnableBlurBehindWindow(hwnd, &blur_behind);
		}
	}

	void DwmFunctions::extend_frame_into_client_area(HWND hwnd, int left, int top, int right, int bottom)
	{
		if (ExtendFrameIntoClientArea)
		{
			MARGINS margins = { 0 };
			margins.cxLeftWidth = left;
			margins.cxRightWidth = right;
			margins.cyBottomHeight = bottom;
			margins.cyTopHeight = top;
			ExtendFrameIntoClientArea(hwnd, &margins);
		}
	}

	bool DwmFunctions::is_vista_or_later()
	{
		OSVERSIONINFO info = { 0 };
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&info);
		return info.dwMajorVersion >= 6;
	}

	void DwmFunctions::open_dll()
	{
		if (dll_handle == 0 && is_vista_or_later())
		{
			dll_handle = LoadLibrary(L"dwmapi.dll");
			if (dll_handle)
			{
				IsCompositionEnabled = (FuncDwmIsCompositionEnabled *)GetProcAddress(dll_handle, "DwmIsCompositionEnabled");
				EnableBlurBehindWindow = (FuncDwmEnableBlurBehindWindow *)GetProcAddress(dll_handle, "DwmEnableBlurBehindWindow");
				ExtendFrameIntoClientArea = (FuncDwmExtendFrameIntoClientArea *)GetProcAddress(dll_handle, "DwmExtendFrameIntoClientArea");
				ExtendFrameIntoClientArea = (FuncDwmExtendFrameIntoClientArea *)GetProcAddress(dll_handle, "DwmExtendFrameIntoClientArea");
				Dwm_DefWindowProc = (FuncDwmDefWindowProc *)GetProcAddress(dll_handle, "DwmDefWindowProc");
			}
		}
	}

	HMODULE DwmFunctions::dll_handle = 0;
	DwmFunctions::FuncDwmIsCompositionEnabled *DwmFunctions::IsCompositionEnabled = 0;
	DwmFunctions::FuncDwmEnableBlurBehindWindow *DwmFunctions::EnableBlurBehindWindow = 0;
	DwmFunctions::FuncDwmExtendFrameIntoClientArea *DwmFunctions::ExtendFrameIntoClientArea = 0;
	DwmFunctions::FuncDwmDefWindowProc *DwmFunctions::Dwm_DefWindowProc = 0;
}
