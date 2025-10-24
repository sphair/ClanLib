/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_DisplayWindow_Impl
{
public:
	CL_DisplayWindow_Impl()
	: provider(0)
	{
		CL_SharedGCData::add_ref();
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
	}

	~CL_DisplayWindow_Impl()
	{
		CL_SharedGCData::release_ref();
		if (provider)
			provider->destroy();
		provider = 0;
	}

	CL_DisplayWindowProvider *provider;
	CL_Signal_v0 sig_lost_focus;
	CL_Signal_v0 sig_got_focus;
	CL_Signal_v2<int, int> sig_resize;
	CL_Signal_v1<const CL_Rect &> sig_paint;
	CL_Signal_v0 sig_window_close;
	CL_Signal_v0 sig_window_destroy;
	CL_Signal_v0 sig_window_minimized;
	CL_Signal_v0 sig_window_maximized;
	CL_Signal_v0 sig_window_restored;
	CL_Signal_v0 sig_window_moved;
	CL_Callback_v1<CL_Rect &> func_window_resize;
	CL_Callback_0<bool> func_minimize_clicked;
	CL_DisplayWindowSite site;
	CL_Cursor current_cursor;
};
