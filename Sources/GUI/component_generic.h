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

#ifndef header_component_generic
#define header_component_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/component.h"
#include "API/GUI/layout.h"

class CL_Component_Generic
{
public:
	CL_Component_Generic(
		CL_Component *self,
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style);

	~CL_Component_Generic();

	void find_preferred_size();

	void remove_focus();

	void begin_paint();
	void paint_children();
	void end_paint();

	CL_GUIManager *get_gui_manager() const;

	void on_set_options(const CL_DomElement &options);
	
	void on_input_down(const CL_InputEvent &key);
	void on_input_up(const CL_InputEvent &key);
	void on_key_down(const CL_InputEvent &key);
	void on_key_up(const CL_InputEvent &key);
	void on_mouse_down(const CL_InputEvent &key);
	void on_mouse_up(const CL_InputEvent &key);
	void on_mouse_dblclk(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);
	void on_close();
	
	CL_Component *owner;
	CL_Component *parent;

	CL_Component *client_area;

	CL_StyleManager *style_manager;

	CL_Rect position;

	CL_Size minimum_size;
	CL_Size maximum_size;
	CL_Size preferred_size;
	CL_LayoutPolicy vertical_policy;
	CL_LayoutPolicy horizontal_policy;
	CL_Layout layout;

	CL_GUIManager *gui;

	bool focusable;
	bool visible;
	bool enabled;
	bool mouse_over;
	bool event_passing;
	bool topmost_flag;
	bool clipping;
	bool paint_clipping;
	
	int tab_id;
	
	std::list<CL_Component *> children;
	std::list<CL_Component *> delete_children;

	CL_ComponentStyle *style;
	bool delete_style;

	CL_Signal_v1<const CL_DomElement &> sig_set_options;
	CL_Signal_v0 sig_begin_paint;
	CL_Signal_v0 sig_paint;
	CL_Signal_v0 sig_end_paint;
	CL_Signal_v0 sig_paint_children;
	CL_Signal_v1<const CL_InputEvent &> sig_input_down;
	CL_Signal_v1<const CL_InputEvent &> sig_input_up;
	CL_Signal_v1<const CL_InputEvent &> sig_key_down;
	CL_Signal_v1<const CL_InputEvent &> sig_key_up;
	CL_Signal_v1<const CL_InputEvent &> sig_mouse_down;
	CL_Signal_v1<const CL_InputEvent &> sig_mouse_up;
	CL_Signal_v1<const CL_InputEvent &> sig_mouse_dblclk;
	CL_Signal_v1<const CL_InputEvent &> sig_mouse_move;
	CL_Signal_v0 sig_mouse_enter;
	CL_Signal_v0 sig_mouse_leave;
	CL_Signal_v0 sig_got_focus;
	CL_Signal_v0 sig_lost_focus;
	CL_Signal_v0 sig_close;
	CL_Signal_v2<int, int> sig_resize;
	CL_Signal_v2<int, int> sig_move;
	CL_Signal_v2<int &, int &> sig_transform_coords;
	CL_Signal_v1<CL_Component *> sig_child_add;
	CL_Signal_v1<CL_Component *> sig_child_remove;
	CL_Signal_v1<CL_Component *> sig_child_before_add;
	CL_Signal_v1<CL_Component *> sig_child_before_remove;
	CL_Signal_v1<CL_Size &> sig_get_minimum_size;
	CL_Signal_v1<CL_Size &> sig_get_maximum_size;
	CL_Signal_v1<CL_Size &> sig_get_preferred_size;

	CL_SlotContainer slots;
};

#endif
