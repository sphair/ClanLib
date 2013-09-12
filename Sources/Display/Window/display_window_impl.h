/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "API/Display/Window/display_window.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/cursor.h"
#include "API/Display/display.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{

class DisplayWindow_Impl
{
public:
	DisplayWindow_Impl()
	: provider(0)
	{
		SharedGCData::add_ref();
		site.sig_lost_focus = &sig_lost_focus;
		site.sig_got_focus = &sig_got_focus;
		site.sig_resize = &sig_resize;
		site.sig_paint = &sig_paint;
		site.sig_window_close = &sig_window_close;
		site.sig_window_destroy = &sig_window_destroy;
		site.sig_window_minimized = &sig_window_minimized;
		site.sig_window_maximized = &sig_window_maximized;
		site.sig_window_restored = &sig_window_restored;
		site.func_window_resize = &func_window_resize;
		site.func_minimize_clicked = &func_minimize_clicked;
		site.sig_window_moved = &sig_window_moved;
#ifdef WIN32
		site.func_window_message = &func_window_message;
#endif
	}

	~DisplayWindow_Impl()
	{
        // TODO:
        // DON'T PULL THIS INTO THE MAIN CODE BASE!
        // This is an OSX hack to get the NSApplication to fully shutdown.
        // I am going to guess this issue is caused by the temporary OSX threading hack.
        // So, hopefully this problem goes away once that is resolved.
		//if (provider)
		// 	delete provider;
		provider = 0;
		SharedGCData::release_ref();
	}

	DisplayWindowProvider *provider;
	Signal_v0 sig_lost_focus;
	Signal_v0 sig_got_focus;
	Signal_v2<int, int> sig_resize;
	Signal_v1<const Rect &> sig_paint;
	Signal_v0 sig_window_close;
	Signal_v0 sig_window_destroy;
	Signal_v0 sig_window_minimized;
	Signal_v0 sig_window_maximized;
	Signal_v0 sig_window_restored;
	Signal_v0 sig_window_moved;
	Signal_v0 sig_window_flip;
	Callback_v1<Rect &> func_window_resize;
	Callback_0<bool> func_minimize_clicked;
	DisplayWindowSite site;
	Cursor current_cursor;

#ifdef WIN32
	Callback_4<bool, HWND, UINT, WPARAM, LPARAM> func_window_message;
#endif

};

}
