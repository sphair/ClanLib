/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "D3D9/precomp.h"
#include "d3d9_display_window_provider.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "d3d9_target_provider.h"
#include "d3d9_graphic_context_provider.h"
#include "Display/Win32/cursor_provider_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9DisplayWindowProvider Construction:

CL_D3D9DisplayWindowProvider::CL_D3D9DisplayWindowProvider()
: win32_window(), site(0)
{
	win32_window.func_on_resized().set(this, &CL_D3D9DisplayWindowProvider::on_window_resized);
}

CL_D3D9DisplayWindowProvider::~CL_D3D9DisplayWindowProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9DisplayWindowProvider Attributes:

CL_Rect CL_D3D9DisplayWindowProvider::get_geometry() const
{
	return win32_window.get_geometry();
}

CL_Rect CL_D3D9DisplayWindowProvider::get_viewport() const
{
	return win32_window.get_viewport();
}

bool CL_D3D9DisplayWindowProvider::has_focus() const
{
	return win32_window.has_focus();
}

bool CL_D3D9DisplayWindowProvider::is_minimized() const
{
	return win32_window.is_minimized();
}

bool CL_D3D9DisplayWindowProvider::is_maximized() const
{
	return win32_window.is_maximized();
}

bool CL_D3D9DisplayWindowProvider::is_visible() const
{
	return win32_window.is_visible();
}

CL_GraphicContext& CL_D3D9DisplayWindowProvider::get_gc()
{
	return gc;
}

CL_InputContext& CL_D3D9DisplayWindowProvider::get_ic()
{
	return win32_window.get_ic();
}


CL_Size CL_D3D9DisplayWindowProvider::get_minimum_size(bool client_area) const
{
	return CL_Size();
}

CL_Size CL_D3D9DisplayWindowProvider::get_maximum_size(bool client_area) const
{
	return CL_Size();
}

CL_String CL_D3D9DisplayWindowProvider::get_title() const
{
	return CL_String();
}

HWND CL_D3D9DisplayWindowProvider::get_hwnd() const
{
	return win32_window.get_hwnd();
}

bool CL_D3D9DisplayWindowProvider::is_clipboard_text_available() const
{
	return win32_window.is_clipboard_text_available();
}

bool CL_D3D9DisplayWindowProvider::is_clipboard_image_available() const
{
	return win32_window.is_clipboard_image_available();
}

CL_String CL_D3D9DisplayWindowProvider::get_clipboard_text() const
{
	return win32_window.get_clipboard_text();
}

CL_PixelBuffer CL_D3D9DisplayWindowProvider::get_clipboard_image() const
{
	return win32_window.get_clipboard_image();
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9DisplayWindowProvider Operations:

CL_Point CL_D3D9DisplayWindowProvider::client_to_screen(const CL_Point &client)
{
	return win32_window.client_to_screen(client);
}

CL_Point CL_D3D9DisplayWindowProvider::screen_to_client(const CL_Point &screen)
{
	return win32_window.screen_to_client(screen);
}

void CL_D3D9DisplayWindowProvider::capture_mouse(bool capture)
{
	win32_window.capture_mouse(capture);
}

void CL_D3D9DisplayWindowProvider::destroy()
{
	delete this;
}

void CL_D3D9DisplayWindowProvider::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
	site = new_site;
	win32_window.create(site, description);
	if (!direct3d.is_null())
		return;
	direct3d = CL_ComPtr<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION));
	if (direct3d.is_null())
		throw CL_Exception("Failed to create Direct3D 9 interface");

	D3DPRESENT_PARAMETERS present_parameters;
	memset(&present_parameters, 0, sizeof(D3DPRESENT_PARAMETERS));
	present_parameters.BackBufferWidth = win32_window.get_viewport().get_width();
	present_parameters.BackBufferHeight = win32_window.get_viewport().get_height();
	present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	present_parameters.SwapEffect = D3DSWAPEFFECT_COPY; // We have to use Copy instead of Discard here because otherwise we can't do partial updates.
	present_parameters.Windowed = TRUE;
	HRESULT result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win32_window.get_hwnd(), D3DCREATE_MIXED_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &present_parameters, d3ddevice.output_variable());
	if (FAILED(result))
	{
		direct3d.clear();
		throw CL_Exception("Failed to create Direct3D 9 device");
	}

	gc = CL_GraphicContext(new CL_D3D9GraphicContextProvider(this));
}

void CL_D3D9DisplayWindowProvider::show_system_cursor()
{
	win32_window.show_system_cursor();
	d3ddevice->ShowCursor(TRUE);
}

CL_CursorProvider *CL_D3D9DisplayWindowProvider::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	return new CL_CursorProvider_Win32(sprite_description, hotspot);
}

void CL_D3D9DisplayWindowProvider::set_cursor(CL_CursorProvider *cursor)
{
	win32_window.set_cursor(static_cast<CL_CursorProvider_Win32 *>(cursor));
}

void CL_D3D9DisplayWindowProvider::set_cursor(CL_StandardCursor type)
{
	win32_window.set_cursor(type);
}

void CL_D3D9DisplayWindowProvider::hide_system_cursor()
{
	win32_window.hide_system_cursor();
	d3ddevice->ShowCursor(FALSE);
}

void CL_D3D9DisplayWindowProvider::set_title(const CL_StringRef &new_title)
{
	win32_window.set_title(new_title);
}

void CL_D3D9DisplayWindowProvider::set_position(const CL_Rect &pos, bool client_area)
{
	win32_window.set_position(pos, client_area);
}

void CL_D3D9DisplayWindowProvider::set_size(int width, int height, bool client_area)
{
	win32_window.set_size(width, height, client_area);
}

void CL_D3D9DisplayWindowProvider::set_minimum_size(int width, int height, bool client_area)
{
	win32_window.set_minimum_size(width,height,client_area);
}

void CL_D3D9DisplayWindowProvider::set_maximum_size(int width, int height, bool client_area)
{
	win32_window.set_maximum_size(width,height,client_area);
}

void CL_D3D9DisplayWindowProvider::set_enabled(bool enable)
{
	win32_window.set_enabled(enable);
}

void CL_D3D9DisplayWindowProvider::minimize()
{
	win32_window.minimize();
}

void CL_D3D9DisplayWindowProvider::restore()
{
	win32_window.restore();
}

void CL_D3D9DisplayWindowProvider::maximize()
{
	win32_window.maximize();
}

void CL_D3D9DisplayWindowProvider::show(bool activate)
{
	win32_window.show(activate);
}

void CL_D3D9DisplayWindowProvider::hide()
{
	win32_window.hide();
}

void CL_D3D9DisplayWindowProvider::bring_to_front()
{
	win32_window.bring_to_front();
}

void CL_D3D9DisplayWindowProvider::flip(int interval)
{
	d3ddevice->Present(0, 0, 0, 0);
}

void CL_D3D9DisplayWindowProvider::update(const CL_Rect &rect)
{
	throw CL_Exception("Not implemented yet.");
}

void CL_D3D9DisplayWindowProvider::set_clipboard_text(const CL_StringRef &text)
{
	win32_window.set_clipboard_text(text);
}

void CL_D3D9DisplayWindowProvider::request_repaint(const CL_Rect &rect)
{
	win32_window.request_repaint(rect);
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9DisplayWindowProvider Implementation:

void CL_D3D9DisplayWindowProvider::on_window_resized()
{
	if (gc.get_provider())
		((CL_D3D9GraphicContextProvider *) gc.get_provider())->on_window_resized();
}

void CL_D3D9DisplayWindowProvider::set_clipboard_image( const CL_PixelBuffer &buf )
{
	win32_window.set_clipboard_image(buf);
}

void CL_D3D9DisplayWindowProvider::set_large_icon(const CL_PixelBuffer &image)
{
	win32_window.set_large_icon(image);
}

void CL_D3D9DisplayWindowProvider::set_small_icon(const CL_PixelBuffer &image)
{
	win32_window.set_small_icon(image);
}
