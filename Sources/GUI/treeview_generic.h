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

#ifndef header_treeview_generic
#define header_treeview_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/treeview.h"

class CL_TreeView_Generic
{
//! Construction:
public:
	CL_TreeView_Generic(CL_TreeView *self);
	~CL_TreeView_Generic();

//! Attributes:
public:
	int get_column_count() const;
	int get_column_width(int index) const;
	const std::string &get_column_name(int index) const;

	bool show_root_decoration;
	bool show_header;

//! Operations:
public:
	int add_column(const std::string &name, int width);
		
//! Signals:
public:
	CL_Signal_v2<CL_TreeNode *, CL_Point &> sig_paint_node;
	CL_Signal_v1<const CL_TreeNode &> sig_selection_changed;
	CL_Signal_v1<const CL_TreeNode &> sig_item_clicked;
	CL_Signal_v1<const CL_TreeNode &> sig_item_added;
	CL_Signal_v1<const CL_TreeNode &> sig_item_removed;
	CL_Signal_v1<const CL_TreeNode &> sig_item_expanded;
	CL_Signal_v1<const CL_TreeNode &> sig_item_collapsed;
	CL_Signal_v1<int> sig_column_added;
	CL_Signal_v1<int> sig_column_removed;
	CL_Signal_v0 sig_clear;

//! Callbacks:
private:
	void on_paint_children(CL_SlotParent_v0 &super);

//! Implementation:
private:
	struct Column
	{
		std::string name;
		int original_width;
		int width;
	};
	std::vector<Column> columns;

	CL_Slot slot_paint_children;

	CL_TreeView *treeview;
};

#endif
