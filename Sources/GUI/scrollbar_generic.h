/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_scrollbar_generic
#define header_scrollbar_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/System/timer.h"
#include "API/Core/Math/rect.h"
#include "API/Display/input_event.h"
#include "API/signals.h"

class CL_ScrollBar;
class CL_Button;
class CL_InputDevice;
class CL_Component;
class CL_DomElement;
class CL_StyleManager;

class CL_ScrollBar_Generic
{
//! Construction:
public:
	CL_ScrollBar_Generic(
		CL_ScrollBar *self,
		int min,
		int max,
		int value,
		bool orientation,
		bool tracking);

	~CL_ScrollBar_Generic();

//! Attributes:
public:
	int get_range() const;

	int min_value;
	int max_value;
	int cur_value;

	bool tracking;
	bool vertical;
	bool fixed_length;

	bool dragging;

	CL_Rect rect_slider;
	int min_slider_length;
	int fixed_slider_length;
			
//! Operations:
public:
	void set_vertical(bool enable);
	void set_range(int min_value, int max_value);
	void set_min_value(int value);
	void set_max_value(int value);
	void set_value(int value, bool using_slider = false);

//! Signals:
public:
	CL_Signal_v1<int> sig_value_changed;
	CL_Signal_v0 sig_slider_pressed;
	CL_Signal_v1<int> sig_slider_moved;
	CL_Signal_v0 sig_slider_released;

//! Slots:
private:
	CL_SlotContainer slots;

//! Callbacks:
private:
	void on_set_options(const CL_DomElement &options);
	void on_child_add(CL_Component *child);
	void on_child_remove(CL_Component *child);
	void on_mouse_down(const CL_InputEvent &key);
	void on_mouse_up(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);
	void on_client_resize(int old_width, int old_height);
	void on_timer_scroll();

//! Implementation:
private:
	void calculate_slider();

	CL_ScrollBar *scrollbar;

	bool mouse_captured;
	CL_Point capture_pos;
	CL_Rect capture_slider;

	CL_Timer timer_scroll;
	int scroll_delta;
};

#endif
