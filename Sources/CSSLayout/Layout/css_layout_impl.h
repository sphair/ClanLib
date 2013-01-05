/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "CSSLayout/css_resource_cache.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{

class CSSLayout;

class CSSLayout_Impl : public DisposableObject
{
public:
	~CSSLayout_Impl();
	CSSLayout get_layout();
	void clear();
	CSSBoxElement *get_root_element() { return box_tree.get_root_element(); }

	CSSLayoutNode get_node(CSSBoxNode *box_node) const;
	std::shared_ptr<CSSLayoutNode_Impl> alloc_node_impl() const;
	void free_node_impl(CSSLayoutNode_Impl *node_impl);

	Rect viewport;
	CSSBoxTree box_tree;
	CSSLayoutTree layout_tree;
	CSSResourceCache resource_cache;
	mutable std::vector<CSSLayoutNode_Impl *> free_node_impls;
	std::weak_ptr<CSSLayout_Impl> self;

	struct NodeImplDeleter
	{
		NodeImplDeleter(const CSSLayout_Impl *impl) : impl(const_cast<CSSLayout_Impl*>(impl)) { }
		CSSLayout_Impl *impl;
		void operator()(CSSLayoutNode_Impl *node) { impl->free_node_impl(node); }
	};

private:
	void on_dispose();
};

}
