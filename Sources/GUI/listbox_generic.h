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

#ifndef header_listbox_generic
#define header_listbox_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>

#include "API/signals.h"
#include "API/GUI/listitem.h"

class CL_ListBox;
class CL_Component;
class CL_DomElement;
class CL_StyleManager;
class CL_InputEvent;
class CL_InputDevice;
class CL_Rect;
class CL_ScrollBar;
class CL_Point;

class CL_ListBox_Generic
{
public:
	CL_ListBox_Generic(CL_ListBox *self);

	~CL_ListBox_Generic();

	std::vector<std::string> get_selected_items() const;
	const std::string &get_current_text() const;
	CL_ListItem *get_item(int index) const;
	int get_item(const CL_Point &pt);

	const std::string &get_text(int index) const;
	int get_current_item() const;

	int insert_item(CL_ListItem *item, int index, bool delete_item);
	int insert_item(const std::string &text, int index);
	void remove_item(int index);
	void change_item(CL_ListItem *item, int index);
	void change_item(const std::string &text, int index);
	void set_current_item(int index);
	void clear_selection();
	void set_selected(int index, bool select);
	void select_all(bool select);
	void invert_selection();
	void set_multi_selection(bool enable);
	void set_item_height(int height);
	void set_top_item(int index);
	void sort(bool ascending);
	void clear();

	CL_Signal_v1<int> sig_highlighted;
	CL_Signal_v1<int> sig_activated;
	CL_Signal_v0 sig_selection_changed;
	CL_Signal_v1<int> sig_item_added;
	CL_Signal_v1<int> sig_item_removed;
	CL_Signal_v0 sig_clear;
		
	void on_set_options(const CL_DomElement &options);
	void on_mouse_down(const CL_InputEvent &key);
	void on_key_up(const CL_InputEvent &key);

	int item_height;
	int scroll_offset;
	bool multi_selection;

	CL_ListBox *listbox;
	CL_ScrollBar *scrollbar;

	std::vector<CL_ListItem *> items;

	CL_SlotContainer slots;
};

#endif
