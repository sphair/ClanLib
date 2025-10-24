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
**    Mark Page
**	  Harry Storbacka
*/

#include "SDL/precomp.h"
#include "sdl_display_window_provider.h"
#include "sdl_target_provider.h"
#include "sdl_graphic_context_provider.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "timer_provider_sdl.h"
#include "cursor_provider_sdl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SDLDisplayWindowProvider Construction:

CL_SDLDisplayWindowProvider::CL_SDLDisplayWindowProvider()
: window(&CL_SDLTargetProvider::message_queue), site(0)
{
	window.func_on_resized().set(this, &CL_SDLDisplayWindowProvider::on_window_resized);
}

CL_SDLDisplayWindowProvider::~CL_SDLDisplayWindowProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLDisplayWindowProvider Attributes:

CL_Rect CL_SDLDisplayWindowProvider::get_geometry() const
{
	return window.get_geometry();
}

CL_Rect CL_SDLDisplayWindowProvider::get_viewport() const
{
	return window.get_viewport();
}

bool CL_SDLDisplayWindowProvider::has_focus() const
{
	return window.has_focus();
}

bool CL_SDLDisplayWindowProvider::is_minimized() const
{
	return window.is_minimized();
}

bool CL_SDLDisplayWindowProvider::is_maximized() const
{
	return window.is_maximized();
}

bool CL_SDLDisplayWindowProvider::is_visible() const
{
	return window.is_visible();
}

CL_GraphicContext CL_SDLDisplayWindowProvider::get_gc() const
{
	return gc;
}

CL_InputContext CL_SDLDisplayWindowProvider::get_ic() const
{
	return window.get_ic();
}

#ifdef WIN32
HWND CL_SDLDisplayWindowProvider::get_hwnd() const
{
	throw CL_Exception(cl_text("Fixme"));
}
#endif

bool CL_SDLDisplayWindowProvider::is_clipboard_text_available() const
{
	return window.is_clipboard_text_available();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLDisplayWindowProvider Operations:

CL_TimerProvider *CL_SDLDisplayWindowProvider::alloc_timer(CL_DisplayWindow &disp_window)
{
	return new CL_TimerProvider_SDL(disp_window);
}

CL_Point CL_SDLDisplayWindowProvider::client_to_screen(const CL_Point &client)
{
	return window.client_to_screen(client);
}

CL_Point CL_SDLDisplayWindowProvider::screen_to_client(const CL_Point &screen)
{
	return window.screen_to_client(screen);
}

void CL_SDLDisplayWindowProvider::capture_mouse(bool capture)
{
	window.capture_mouse(capture);
}

void CL_SDLDisplayWindowProvider::destroy()
{
	delete this;
}

void CL_SDLDisplayWindowProvider::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
	site = new_site;
	flip_timer_set = false;
	refresh_rate = description.get_refresh_rate();
	if (!refresh_rate)	// Default the refresh rate to 60 if not defined
		refresh_rate = 60;

	window.create(site, description);
	gc = CL_GraphicContext(new CL_SDLGraphicContextProvider(this));
}

void CL_SDLDisplayWindowProvider::show_system_cursor()
{
	window.show_system_cursor();
}

CL_CursorProvider *CL_SDLDisplayWindowProvider::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	return new CL_CursorProvider_SDL(sprite_description, hotspot);
}

void CL_SDLDisplayWindowProvider::set_cursor(CL_CursorProvider *cursor)
{
	window.set_cursor(static_cast<CL_CursorProvider_SDL *>(cursor));
}

void CL_SDLDisplayWindowProvider::set_cursor(CL_StandardCursor type)
{
	window.set_cursor(type);
}

void CL_SDLDisplayWindowProvider::hide_system_cursor()
{
	window.hide_system_cursor();
}

void CL_SDLDisplayWindowProvider::set_title(const CL_StringRef &new_title)
{
	window.set_title(new_title);
}

void CL_SDLDisplayWindowProvider::set_position(const CL_Rect &pos, bool client_area)
{
	window.set_position(pos, client_area);
}

void CL_SDLDisplayWindowProvider::set_size(int width, int height, bool client_area)
{
	window.set_size(width, height, client_area);
}

void CL_SDLDisplayWindowProvider::set_minimum_size( int width, int height, bool client_area )
{
	window.set_minimum_size(width,height,client_area);
}

void CL_SDLDisplayWindowProvider::set_maximum_size( int width, int height, bool client_area )
{
	window.set_maximum_size(width,height,client_area);
}
void CL_SDLDisplayWindowProvider::set_enabled(bool enable)
{
	window.set_enabled(enable);
}

void CL_SDLDisplayWindowProvider::minimize()
{
	window.minimize();
}

void CL_SDLDisplayWindowProvider::restore()
{
	window.restore();
}

void CL_SDLDisplayWindowProvider::maximize()
{
	window.maximize();
}

void CL_SDLDisplayWindowProvider::show(bool activate)
{
	window.show(activate);
}

void CL_SDLDisplayWindowProvider::hide()
{
	window.hide();
}

void CL_SDLDisplayWindowProvider::bring_to_front()
{
	window.bring_to_front();
}

void CL_SDLDisplayWindowProvider::flip(int interval)
{
	window.flip();

	if (interval<=0)
	{
		flip_timer_set = false;
	}
	else
	{
		if (!flip_timer_set)
		{
			flip_last_time = CL_System::get_time();
			flip_timer_set = true;
		}
		else
		{
			unsigned int current_time = CL_System::get_time();

			int time_diff = current_time - flip_last_time;

			interval *= 1000 / refresh_rate;
			int time_wait = interval - time_diff;

			if ( (time_wait > 0) && (time_wait < interval) )
			{
				SDL_Delay(time_wait);
				flip_last_time = current_time + time_wait;
			}
			else
			{
				flip_last_time = current_time;
			}
		}
	}

}

void CL_SDLDisplayWindowProvider::update(const CL_Rect &rect)
{

}

void CL_SDLDisplayWindowProvider::set_timer(CL_TimerProvider *timer)
{
	window.set_timer(timer);
}

void CL_SDLDisplayWindowProvider::kill_timer(CL_TimerProvider *timer)
{
	window.kill_timer(timer);
}

void CL_SDLDisplayWindowProvider::set_clipboard_text(const CL_StringRef &text)
{
	window.set_clipboard_text(text);
}

CL_String CL_SDLDisplayWindowProvider::get_clipboard_text() const
{
	return window.get_clipboard_text();
}

void CL_SDLDisplayWindowProvider::invalidate_rect(const CL_Rect &rect)
{
	window.invalidate_rect(rect);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLDisplayWindowProvider Implementation:

void CL_SDLDisplayWindowProvider::on_window_resized()
{
	if (gc.get_provider())
		((CL_SDLGraphicContextProvider *) gc.get_provider())->on_window_resized();
}


