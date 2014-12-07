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
**    Chu Chin Kuan
*/

#pragma once

#include "API/Display/Window/display_window_description.h"
#include <X11/Xlib.h>

#if defined(WIN32) || defined(__APPLE__)
#error This file should only be included on Linux builds.
#else

namespace clan
{

/** Platform-specific display window handle container.
 *  This is the implementation for X11 display windows.
 */
class DisplayWindowHandle
{
public:
	::Display* display;
	::Window window;

	DisplayWindowHandle() = delete;
	DisplayWindowHandle(DisplayWindowHandle const &other) : DisplayWindowHandle(other.get_display(), other.get_window()) { }
	DisplayWindowHandle(::Display* _display, ::Window _window) : display(_display), window(_window) { }

	void set_display(::Display* _display) { display = _display; }
	void set_window(::Window _window) { window = _window; }

	::Display* get_display() const { return display; }
	::Window get_window() const { return window; }
};

}

#endif
