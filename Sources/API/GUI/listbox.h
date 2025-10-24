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

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_listbox
#define header_listbox

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"
#include "listitem.h"

class CL_ListBox_Generic;

//: Listbox component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_ListBox : public CL_Component
{
//! Construction:
public:
	//: Listbox constructor
	CL_ListBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: Listbox constructor
	CL_ListBox(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Listbox destructor
	virtual ~CL_ListBox();

//! Attributes:
public:
	//: Returns the number of items in the list box. 
	int get_count() const;

	//: Returns items in the list.
	std::vector<CL_ListItem *> &get_items() const;

	//: Returns selected items as strings.
 	std::vector<std::string> get_selected_items() const;

	//: Returns the text of the current item.
	//- <p>Returns "" if item not found. Best to use on single-selection lists, as it will only
	//- return the first selected item.</p>
	const std::string &get_current_text() const;

	//: Returns an item.
	//- <p>Returns NULL if item was not found.</p>S
	CL_ListItem *get_item(int index) const;

	//: Returns an item index by screen coordinates, or -1 for if no item is found
	int get_item(const CL_Point &pt);

	//: Returns the text for an item.
	const std::string &get_text(int index) const;
	
	//: Returns the index of the current item of the listbox, or -1 if no item has been selected. 
	int get_current_item() const;

	//: Returns true if item index is selected. Returns false if it is not selected or if there is an error.
	bool is_selected(int index) const;

	//: Returns true if the listbox is in multiselection mode.
	bool is_multi_selection() const;

	//: Returns the height of one item.
	int get_item_height() const;

	//: Returns the index of the item at the top of the screen.
	int get_top_item() const;

//! Operations:
public:
	//: Inserts a listitem into the list at index. 
	//- <p>If index is negative, text is inserted at the end of the list.
	//- Returns the index of the text</p>
	int insert_item(CL_ListItem *item, int index = -1, bool delete_item = false);

	//: Inserts text into the list at index. 
	//- <p>If index is negative, text is inserted at the end of the list.
	//- Returns the index of the text</p>
	int insert_item(const std::string &text, int index = -1);

	//: Removes and deletes the item at position index.
	//- <p>If index is equal to get_current_item(), a new item gets highlighted and the sig_highlighted() signal is emitted.</p>
	void remove_item(int index);

	//: Replaces the item at position index with new item. 
	void change_item(CL_ListItem *item, int index);

	//: Replaces the item at position index with text. 
	void change_item(const std::string &text, int index);

	//: Sets the highlighted item to the item index.
	//- <p>The highlighting is moved and the listbox scrolled as necessary. </p>
	void set_current_item(int index);

	//: Selects item at index if select is true, or unselects it if select is false.
	//- <p>If the listbox is a single-selection listbox and and select is true, set_current_item() is called. 
	//- If the listbox is a single-selection listbox and and select is false, clear_selection()
	//- is called if index is the currently selected item. </p>
	void set_selected(int index, bool select);

	//: Deselects current selected item.
	void clear_selection();

	//: If select is true, all items get selected, else all get unselected.
	//: This works only in Multi selection mode.
	void select_all(bool select = true);

	//: Inverts the selection.
	//: Works only in Multi selection mode.
	void invert_selection();

	//: Sets the listbox to multiselection if enable is true,
	//: single selection otherwise.
	void set_multi_selection(bool enable = true);

	//: Sets the height of each item.
	void set_item_height(int height);

	//: Scrolls the list box so the item at position index in the list is displayed in the top row of the listbox. 
	void set_top_item(int index);

	//: Sorts the items in ascending order, if ascending is true, or descending otherwise. 
	void sort(bool ascending = true);

	//: Deletes all items in the list. 
	void clear();

//! Signals:
public:
	//: This signal is emitted when the user highlights a new current item.
	//- <p>The argument is the index of the new item, which is already current.</p>
	CL_Signal_v1<int> &sig_highlighted();

	//: This signal is emitted when the user double clicks, presses space or return on an item.
	//- <p>The argument is the index of the item.</p>
	CL_Signal_v1<int> &sig_activated();

	//: This signal is emitted when the selection set of a listbox changes.
	CL_Signal_v0 &sig_selection_changed();

	//: This signal is emitted when a new item is added to the listbox.
	CL_Signal_v1<int> &sig_item_added();

	//: This signal is emitted when an item is removed from the listbox.
	CL_Signal_v1<int> &sig_item_removed();
	
	//: This signal is emitted when the listbox is cleared.
	CL_Signal_v0 &sig_clear();

//! Implementation:
private:
	CL_ListBox(const CL_ListBox &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.

	CL_ListBox_Generic *impl;
};

#endif
