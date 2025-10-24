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

#ifndef header_guimanager_generic
#define header_guimanager_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/gui_manager.h"
#include "API/signals.h"
#include "API/Core/System/event_trigger.h"
#include <stack>

class CL_InputDevice;
class CL_InputEvent;

class CL_GUIManager_Generic
{
//! Construction:
public:
	CL_GUIManager_Generic(CL_GUIManager *owner);
	~CL_GUIManager_Generic();

//! Attributes:
public:
	CL_Component *get_focus();

	CL_Component *get_capture();

	bool is_input_enabled();

	CL_EventTrigger update_trigger;

//! Operations:
public:
	void run(CL_Component *modal_level);
	void show();
	void quit();

	void enable_input();
	void disable_input();
	
	void gui_capture_mouse(CL_Component *component);
	void gui_release_mouse();

	void set_focus(CL_Component *component);

	void component_deleted(CL_Component *component);
	CL_Component *get_modal_component() const;
	
//! Signals:
public:
	CL_Signal_v1 <CL_Component *> sig_focus_changed;
	
//! Implementation:
private:
	void on_input_up(const CL_InputEvent &key);
	void on_input_down(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);
	void on_mouse_dblclk(const CL_InputEvent &key);
	void on_resize(int width, int height);
	void on_paint(const CL_Rect &rect);
	CL_Component *find_focus_component(const CL_InputEvent &key);
	void do_mouse_enter_leave(int x, int y, CL_Component *new_focus);
	CL_Component *tab_next();
	CL_Component *tab_previous();
	int find_highest_tab_id(const CL_Component *component);

	CL_SlotContainer slots;

	volatile bool quit_run;
	CL_GUIManager *owner;
	bool input_enabled;

	int current_tab_id;
	CL_Component *comp_focus;
	CL_Component *comp_capture_mouse;
	CL_Component *comp_mouse_over;

	//: This is a temporary worker variable for event handlers.
	//- <p>Only reason this is in the header is because it must be set to null when component_deleted
	//- is called with the component.</p>
	CL_Component *focus_component;

	std::stack<CL_Component *> modal_stack;
};

#endif
