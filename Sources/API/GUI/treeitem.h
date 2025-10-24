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

#ifndef header_treeitem
#define header_treeitem

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

class CL_TreeItem_Generic;
class CL_TreeNode;
class CL_Surface;
class CL_CheckBox;

//: TreeView_Item component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_TreeItem : public CL_Component
{
//! Construction:
public:
	//: TreeView Item Constructor
	CL_TreeItem(
		CL_TreeNode *node,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: TreeView Item Destructor
	virtual ~CL_TreeItem();

//! Attributes:
public:
	//: Returns the item text for a specified column.
	//return: The item text.
	const std::string &get_text(int column = 0) const;

	//: Returns the item component for a specified column.
	//return: The item component.
	CL_Component *get_component(int column = 0) const;

	//: Returns the TreeView node this component is used in.
	CL_TreeNode *get_node() const;

	//: Return the icon.
	CL_Surface *get_icon() const;

	//: Get the margin between the start of the item and start of the text.
	int get_text_margin(void) const;

	//: Return the height.
	int get_custom_height() const;

//! Operations:
public:
	//: Sets the text for a specified column.
	//param text: The column text.
	void set_text(int column, const std::string &text);

	//: Adds a checkbox to a specified column.
	CL_CheckBox *set_checkbox(int column, const std::string &text = std::string());

	//: Adds a component for a specified column.
	void set_component(int column, CL_Component *component);

	//: Sets the items icon.
	//- <p>If 'delete_surface' is true, the surface will be deleted when the component is deleted.</p>
	void set_icon(CL_Surface *surface, bool delete_surface = false);

	//: Set the margin between the start of the item and start of the text.
	//: Use this to set a margin if you have no icon on this item, but on others.
	void set_text_margin(int margin);

	//: Sets the height.
	void set_custom_height(int height);

//! Signals:
public:
	//: This signal is emitted when the item is clicked on.
	CL_Signal_v0 &sig_clicked();

//! Implementation:
private:
	CL_TreeItem(const CL_TreeItem &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_TreeItem_Generic *impl;
};

#endif
