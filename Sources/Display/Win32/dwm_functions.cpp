
#include "Display/precomp.h"
#include "dwm_functions.h"

namespace clan
{

bool DwmFunctions::is_composition_enabled()
{
	open_dll();
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

void DwmFunctions::enable_alpha_channel(HWND hwnd)
{
	open_dll();
	if (EnableBlurBehindWindow)
	{
		DWM_BLURBEHIND blur_behind = { 0 };
		blur_behind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
		blur_behind.fEnable = TRUE;
		blur_behind.hRgnBlur = CreateRectRgn(0, 0, 0, 0);
		EnableBlurBehindWindow(hwnd, &blur_behind);

		//SetLayeredWindowAttributes(hwnd, 0, 255, LWA_COLORKEY);
		//HRGN region = CreateRectRgn(0, 0, 1024, 1024);
		//SetWindowRgn(hwnd, region, FALSE);

		/*BLENDFUNCTION blend;
		memset(&blend, 0, sizeof(BLENDFUNCTION));
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = (BYTE) 255;
		blend.AlphaFormat = AC_SRC_ALPHA;
		UpdateLayeredWindow(hwnd, 0, 0, 0, 0, 0, 0, &blend, ULW_ALPHA);*/
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
		}
	}
}

HMODULE DwmFunctions::dll_handle = 0;
DwmFunctions::FuncDwmIsCompositionEnabled *DwmFunctions::IsCompositionEnabled = 0;
DwmFunctions::FuncDwmEnableBlurBehindWindow *DwmFunctions::EnableBlurBehindWindow = 0;

}
