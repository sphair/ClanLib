/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

#pragma once

#include "BoxTree/css_box_tree.h"
#include "LayoutTree/css_layout_tree.h"
#include "css_resource_cache.h"
#include "API/Core/System/disposable_object.h"
class CL_CSSLayout;

class CL_CSSLayout_Impl : public CL_DisposableObject
{
public:
	~CL_CSSLayout_Impl();
	CL_CSSLayout get_layout();
	void clear();
	CL_CSSBoxElement *get_root_element() { return box_tree.get_root_element(); }

	CL_CSSLayoutNode get_node(CL_CSSBoxNode *box_node) const;
	CL_SharedPtr<CL_CSSLayoutNode_Impl> alloc_node_impl() const;
	void free_node_impl(CL_CSSLayoutNode_Impl *node_impl);

	CL_CSSBoxTree box_tree;
	CL_CSSLayoutTree layout_tree;
	CL_CSSResourceCache resource_cache;
	mutable std::vector<CL_CSSLayoutNode_Impl *> free_node_impls;
	CL_WeakPtr<CL_CSSLayout_Impl> self;

private:
	void on_dispose();
};
