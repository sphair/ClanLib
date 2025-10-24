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

#ifndef header_treenode_generic
#define header_treenode_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include "API/Signals/slot_container.h"
#include "API/Core/Math/point.h"
#include "API/Display/input_event.h"

class CL_TreeNode;

//: TreeView node
class CL_TreeNode_Generic
{
public:
	CL_TreeNode_Generic(CL_TreeNode *self, CL_TreeNode *parent, CL_TreeView *root);
	~CL_TreeNode_Generic();
	
	void set_collapsed(bool new_collapsed);
	void show_nodes(bool show);
	void set_component(CL_Component *new_component);
	void clear(bool do_signal);

	CL_TreeView *root;
	CL_TreeNode *parent;
	CL_TreeNode *treenode;

	CL_Component *component;

	std::list<CL_TreeNode *> children;

	CL_SlotContainer slots;
	CL_Slot slot_mouse_up;

	void *userdata;

	bool collapsed;
	bool selected;
	int selectable;	 // 0 - not selectable, 1 - selectable, 2 - use parent setting
	int collapsable; // 0 - not collapsable, 1 - collapsable, 2 - use parent setting
	bool root_node;

	int placement_offset;

	void on_child_click(const CL_InputEvent &key);

	CL_Signal_v1<CL_Point &> sig_paint_node;

private:

	void destroy(); //removes all children and kills its component
};

#endif
