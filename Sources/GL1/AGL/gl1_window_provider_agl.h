/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Sean Heber
*/

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include <list>
#include <map>

class InputDeviceProvider_MacKeyboard;
class InputDeviceProvider_MacMouse;

class CL_GL1WindowProvider_AGL : public DisplayWindowProvider
{
/// \name Construction
/// \{

public:
	CL_GL1WindowProvider_AGL();

	~CL_GL1WindowProvider_AGL();


/// \}
/// \name Attributes
/// \{

public:
	Rect get_geometry() const;

	Rect get_viewport() const;

	bool has_focus() const;

	/** bool is_fullscreen() const;
	    static std::list<HGLRC> opengl_contexts;
	    static std::map<HWND, CL_GL1WindowProvider_WGL *> window_map;*/


/// \}
/// \name Operations
/// \{

public:
	void destroy() { delete this; }
	void create(DisplayWindowSite *site, const DisplayWindowDescription &description);

	void show_system_cursor();

	void hide_system_cursor();

	void set_title(const std::string &new_title);

	void set_position(const Rect &pos);

	void set_size(int width, int height);

	/// \brief Flip opengl buffers.
	void flip(int interval);

	/// \brief Copy a region of the backbuffer to the frontbuffer.
	void update(const Rect &rect);

	void process_messages();


/// \}
/// \name Implementation
/// \{

private:
	InputDeviceProvider_Win32Keyboard *keyboard;

	InputDeviceProvider_Win32Mouse *mouse;

	InputDeviceProvider_Win32Tablet *tablet;

	DisplayWindowSite *site;

	friend class InputDeviceProvider_MacKeyboard;

	friend class InputDeviceProvider_MacMouse;
/// \}
};


