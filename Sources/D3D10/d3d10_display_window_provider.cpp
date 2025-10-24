/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "D3D10/precomp.h"
#include "d3d10_display_window_provider.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "Display/Win32/timer_provider_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10DisplayWindowProvider Construction:

CL_D3D10DisplayWindowProvider::CL_D3D10DisplayWindowProvider()
{
}

CL_D3D10DisplayWindowProvider::~CL_D3D10DisplayWindowProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10DisplayWindowProvider Attributes:

CL_Rect CL_D3D10DisplayWindowProvider::get_geometry() const
{
	return CL_Rect();
}

CL_Rect CL_D3D10DisplayWindowProvider::get_viewport() const
{
	return CL_Rect();
}

bool CL_D3D10DisplayWindowProvider::has_focus() const
{
	return false;
}

bool CL_D3D10DisplayWindowProvider::is_minimized() const
{
	return false;
}

bool CL_D3D10DisplayWindowProvider::is_maximized() const
{
	return false;
}

bool CL_D3D10DisplayWindowProvider::is_visible() const
{
	return true;
}

CL_GraphicContext CL_D3D10DisplayWindowProvider::get_gc() const
{
	return CL_GraphicContext();
}

CL_InputContext CL_D3D10DisplayWindowProvider::get_ic() const
{
	return CL_InputContext();
}

HWND CL_D3D10DisplayWindowProvider::get_hwnd() const
{
	return 0;
}

bool CL_D3D10DisplayWindowProvider::is_clipboard_text_available() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10DisplayWindowProvider Operations:

CL_TimerProvider *CL_D3D10DisplayWindowProvider::alloc_timer(CL_DisplayWindow &disp_window)
{
	return new CL_TimerProvider_Win32(disp_window);
}

CL_Point CL_D3D10DisplayWindowProvider::client_to_screen(const CL_Point &client)
{
	return CL_Point();
}

CL_Point CL_D3D10DisplayWindowProvider::screen_to_client(const CL_Point &screen)
{
	return CL_Point();
}

void CL_D3D10DisplayWindowProvider::capture_mouse(bool capture)
{
}

void CL_D3D10DisplayWindowProvider::destroy()
{
	delete this;
}

void CL_D3D10DisplayWindowProvider::create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description)
{
}

void CL_D3D10DisplayWindowProvider::show_system_cursor()
{
}

CL_CursorProvider *CL_D3D10DisplayWindowProvider::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	return 0;
}

void CL_D3D10DisplayWindowProvider::set_cursor(CL_CursorProvider *cursor)
{
}

void CL_D3D10DisplayWindowProvider::set_cursor(CL_StandardCursor type)
{
}

void CL_D3D10DisplayWindowProvider::hide_system_cursor()
{
}

void CL_D3D10DisplayWindowProvider::set_title(const CL_StringRef &new_title)
{
}

void CL_D3D10DisplayWindowProvider::set_position(const CL_Rect &pos, bool client_area)
{
}

void CL_D3D10DisplayWindowProvider::set_size(int width, int height, bool client_area)
{
}

void CL_D3D10DisplayWindowProvider::set_minimum_size(int width, int height, bool client_area)
{
}

void CL_D3D10DisplayWindowProvider::set_maximum_size(int width, int height, bool client_area)
{
}

void CL_D3D10DisplayWindowProvider::set_enabled(bool enable)
{
}

void CL_D3D10DisplayWindowProvider::minimize()
{
}

void CL_D3D10DisplayWindowProvider::restore()
{
}

void CL_D3D10DisplayWindowProvider::maximize()
{
}

void CL_D3D10DisplayWindowProvider::show(bool activate)
{
}

void CL_D3D10DisplayWindowProvider::hide()
{
}

void CL_D3D10DisplayWindowProvider::bring_to_front()
{
}

void CL_D3D10DisplayWindowProvider::flip(int interval)
{
}

void CL_D3D10DisplayWindowProvider::update(const CL_Rect &rect)
{
}

void CL_D3D10DisplayWindowProvider::set_timer(CL_TimerProvider *timer)
{
}

void CL_D3D10DisplayWindowProvider::kill_timer(CL_TimerProvider *timer)
{
}

void CL_D3D10DisplayWindowProvider::set_clipboard_text(const CL_StringRef &text)
{
}

CL_String CL_D3D10DisplayWindowProvider::get_clipboard_text() const
{
	return CL_String();
}

void CL_D3D10DisplayWindowProvider::invalidate_rect(const CL_Rect &rect)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10DisplayWindowProvider Implementation:
