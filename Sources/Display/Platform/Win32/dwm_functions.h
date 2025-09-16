/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#pragma once

#include <dwmapi.h>

namespace clan
{
	class DwmFunctions
	{
	public:
		static void open_dll();

		static bool is_composition_enabled();
		static void enable_alpha_channel(HWND hwnd, HRGN rgn);

		static void extend_frame_into_client_area(HWND hwnd, int left, int top, int right, int bottom);

	private:
		typedef HRESULT(WINAPI FuncDwmIsCompositionEnabled)(BOOL *pfEnabled);
		typedef HRESULT(WINAPI FuncDwmEnableBlurBehindWindow)(HWND hwnd, const DWM_BLURBEHIND *pBlurBehind);
		typedef HRESULT(WINAPI FuncDwmExtendFrameIntoClientArea)(HWND hwnd, const MARGINS *pMarInset);
		typedef HRESULT(WINAPI FuncDwmDefWindowProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);

		static HMODULE dll_handle;
		static FuncDwmIsCompositionEnabled *IsCompositionEnabled;
		static FuncDwmEnableBlurBehindWindow *EnableBlurBehindWindow;
		static FuncDwmExtendFrameIntoClientArea *ExtendFrameIntoClientArea;

	public:
		static FuncDwmDefWindowProc *Dwm_DefWindowProc;
	};
}
