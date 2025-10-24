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

#ifndef header_treeview
#define header_treeview

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
#include "treenode.h"

class CL_TreeView_Generic;

//: TreeView component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_TreeView : public CL_Component, public CL_TreeNode
{
//! Construction:
public:
	//: TreeView Constructor
	CL_TreeView(
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: TreeView Constructor
	CL_TreeView(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: TreeView Destructor
	virtual ~CL_TreeView();

//! Attributes:
public:
	//: Returns the column count.
	int get_column_count() const;

	//: Returns the column width at index.
	//: Index starts at 0.
	int get_column_width(int index) const;

	//: Returns the column name at index.
	//: Index starts at 0.
	const std::string &get_column_name(int index) const;

	//: Returns if root decoration is visible or not.
	bool is_root_decoration_visible() const;

	//: Returns if header is visible or not.
	bool is_header_visible() const;

//! Operations:
public:
	//: Add a column.
	int add_column(const std::string &name, int width);

	//: Show or hide root decoration for items.
	void show_root_decoration(bool enable = true);

	//: Show or hide columns.
	void show_header(bool enable = true);

//! Signals:
public:
	//: This signal is emitted when the selection set of a tree changes.
	CL_Signal_v1<const CL_TreeNode &> &sig_selection_changed();

	//: This signal is emitted when an item is clicked / selected.
	CL_Signal_v1<const CL_TreeNode &> &sig_item_clicked();

	//: This signal is emitted when a new item is added to the treeview.
	CL_Signal_v1<const CL_TreeNode &> &sig_item_added();

	//: This signal is emitted when an item is removed from the treeview.
	CL_Signal_v1<const CL_TreeNode &> &sig_item_removed();
	
	//: This signal is emitted when an item is collapsed.
	CL_Signal_v1<const CL_TreeNode &> &sig_item_collapsed();

	//: This signal is emitted when an item is collapsed.
	CL_Signal_v1<const CL_TreeNode &> &sig_item_expanded();
	
	//: This signal is emitted when a new column is added to the treeview.
	CL_Signal_v1<int> &sig_column_added();

	//: This signal is emitted when an column is removed from the treeview.
	CL_Signal_v1<int> &sig_column_removed();
	
	//: This signal is emitted when the treeview is cleared.
	CL_Signal_v0 &sig_clear();

	//: Draws the node (used by styles).
	CL_Signal_v2<CL_TreeNode *, CL_Point &> &sig_paint_node();

//! Implementation:
protected:
	friend class CL_TreeNode_Generic;
	void add_node_to_delete_list(CL_TreeNode *pNode);
	void clear_delete_list();
	std::list<CL_TreeNode *> node_delete_list;

private:
	
	CL_TreeView(const CL_TreeView &copy) : CL_Component(NULL, NULL), CL_TreeNode(NULL, NULL) { return; } // disallow copy construction.
	CL_TreeView_Generic *impl;
};

#endif
