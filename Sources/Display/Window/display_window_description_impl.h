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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "API/Display/Window/display_window.h"
#include <map>
#include "API/Core/Math/rect.h"

namespace clan
{

class DisplayWindowDescription_Impl
{
/// \name Construction
/// \{
public:
	DisplayWindowDescription_Impl()
	{
		title = "Untitled Clanlib Window";
		position = Rect(-1, -1, 639, 479);
		position_client_area = false;
		caption = true;
		topmost = false;
		visible = true;
		minimize_button = true;
		maximize_button = true;
		sysmenu = true;
		drop_shadow = false;
		fullscreen = false;
		fullscreen_monitor = 0;
		flipping_buffers = 2;
		swap_interval = -1;
		allow_resize = false;
		refresh_rate = 0;
		bpp = 0;
		create_tablet_context = false;
		layered = false;
		handle = 0;
		depth_size = 0;
		stencil_size = 0;
		use_cached_window = false;
		allow_screensaver = true;
		update_supported = false;
		multisampling = 0;
	}

	virtual ~DisplayWindowDescription_Impl() { return; }

/// \}
/// \name Attributes
/// \{
public:
	std::string title;
	Rect position;
	bool position_client_area;
	bool caption;
	bool minimize_button;
	bool maximize_button;
	bool sysmenu;
	bool topmost;
	bool visible;
	bool drop_shadow;
	bool fullscreen;
	int fullscreen_monitor;
	bool allow_resize;
	int flipping_buffers;
	int swap_interval;
	int bpp;
	int refresh_rate;
	DisplayWindow owner;
	bool create_tablet_context;
	bool use_cached_window;
	bool layered;

	DisplayWindowHandle *handle;

	int depth_size;
	int stencil_size;
	bool allow_screensaver;
	bool update_supported;
	int multisampling;

	float extend_frame_left = 0.0f;
	float extend_frame_top = 0.0f;
	float extend_frame_right = 0.0f;
	float extend_frame_bottom = 0.0f;

	WindowType type = WindowType::normal;

/// \}
};

}
