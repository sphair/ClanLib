/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/Layout/css_layout.h"
#include "API/CSSLayout/Layout/css_layout_text.h"
#include "API/CSSLayout/Layout/css_layout_element.h"
#include "API/CSSLayout/Layout/css_layout_object.h"
#include "API/CSSLayout/Layout/css_hit_test_result.h"
#include "BoxTree/css_box_element.h"
#include "BoxTree/css_box_text.h"
#include "BoxTree/css_box_object.h"
#include "LayoutTree/css_layout_hit_test_result.h"
#include "LayoutTree/css_layout_graphics.h"
#include "css_layout_impl.h"
#include "css_layout_node_impl.h"

namespace clan
{

CSSLayout::CSSLayout()
: impl(new CSSLayout_Impl())
{
	impl->self = impl;
}

bool CSSLayout::is_null() const
{
	return !impl || impl->box_tree.get_root_element() == 0;
}

void CSSLayout::set_dpi(float new_dpi)
{
	impl->throw_if_disposed();
	impl->resource_cache.set_dpi(new_dpi);
}

void CSSLayout::set_css_document(const CSSDocument &doc)
{
	impl->box_tree.css = doc;
}

void CSSLayout::layout(Canvas &canvas, const Rect &viewport)
{
	impl->throw_if_disposed();

	CSSLayoutGraphics graphics(canvas, &impl->resource_cache, impl->viewport);
	impl->box_tree.prepare(&impl->resource_cache);
	impl->layout_tree.create(impl->box_tree.get_root_element());
	impl->layout_tree.layout(&graphics, &impl->resource_cache, viewport.get_size());
	impl->viewport = viewport;
}

void CSSLayout::render_impl(Canvas &canvas, std::unique_ptr<ClipWrapper> wrapper)
{
	impl->throw_if_disposed();

	CSSLayoutGraphics graphics(canvas, &impl->resource_cache, impl->viewport, wrapper.get());
	impl->layout_tree.render(&graphics, &impl->resource_cache);
}

CSSHitTestResult CSSLayout::hit_test(Canvas &canvas, const Point &pos)
{
	impl->throw_if_disposed();

	CSSLayoutGraphics graphics(canvas, &impl->resource_cache, impl->viewport);
	CSSLayoutHitTestResult result = impl->layout_tree.hit_test(&graphics, &impl->resource_cache, pos);
	if (result.node)
	{
		CSSHitTestResult result2;
		switch (result.type)
		{
		case CSSLayoutHitTestResult::no_objects_available:
			result2.type = CSSHitTestResult::no_objects_available;
			break;
		case CSSLayoutHitTestResult::outside_top:
			result2.type = CSSHitTestResult::outside_top;
			break;
		case CSSLayoutHitTestResult::outside_left:
			result2.type = CSSHitTestResult::outside_left;
			break;
		case CSSLayoutHitTestResult::outside_right:
			result2.type = CSSHitTestResult::outside_right;
			break;
		case CSSLayoutHitTestResult::outside_bottom:
			result2.type = CSSHitTestResult::outside_bottom;
			break;
		case CSSLayoutHitTestResult::inside:
			result2.type = CSSHitTestResult::inside;
			break;
		}
		result2.node = impl->get_node(result.node);
		result2.text_offset = result.text_offset;
		return result2;
	}
	else
	{
		return CSSHitTestResult();
	}
}

void CSSLayout::set_document_element(CSSLayoutElement element)
{
	impl->throw_if_disposed();
	impl->clear();
	if (!element.is_null() && element.get_parent().is_null())
		impl->box_tree.set_root_element(static_cast<CSSBoxElement*>(element.impl->box_node));
	else
		impl->box_tree.set_root_element(0);
}

CSSLayoutElement CSSLayout::get_document_element()
{
	impl->throw_if_disposed();
	CSSBoxElement *root = impl->box_tree.get_root_element();
	if (root)
	{
		std::shared_ptr<CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
		node_impl->box_node = root;
		return CSSLayoutNode(node_impl).to_element();
	}
	else
	{
		return CSSLayoutElement();
	}
}

CSSLayoutObject CSSLayout::create_object()
{
	impl->throw_if_disposed();
	std::shared_ptr<CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
	node_impl->box_node = new CSSBoxObject();
	return CSSLayoutNode(node_impl).to_object();
}

CSSLayoutElement CSSLayout::create_element(const std::string &name)
{
	impl->throw_if_disposed();
	std::shared_ptr<CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
	node_impl->box_node = new CSSBoxElement();
	CSSLayoutElement e = CSSLayoutNode(node_impl).to_element();
	e.set_name(name);
	return e;
}

CSSLayoutText CSSLayout::create_text(const std::string &text)
{
	impl->throw_if_disposed();
	std::shared_ptr<CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
	node_impl->box_node = new CSSBoxText();
	CSSLayoutText node = CSSLayoutNode(node_impl).to_text();
	node.set_text(text);
	return node;
}

CSSLayoutElement CSSLayout::find_element(const std::string &name)
{
	if (!get_document_element().is_null())
	{
		std::vector<CSSLayoutNode> stack;
		stack.push_back(get_document_element());
		while (!stack.empty())
		{
			if (stack.back().is_element() && stack.back().to_element().get_name() == name)
				return stack.back().to_element();

			CSSLayoutNode next = stack.back().get_first_child();
			if (!next.is_null())
			{
				stack.push_back(next);
			}
			else
			{
				while (!stack.empty())
				{
					next = stack.back().get_next_sibling();
					if (next.is_null())
					{
						stack.pop_back();
					}
					else
					{
						stack.back() = next;
						break;
					}
				}
			}
		}
		return CSSLayoutElement();
	}
	else
	{
		return CSSLayoutElement();
	}
}

Callback<Image(Canvas &, const std::string &)> &CSSLayout::func_get_image()
{
	return impl->resource_cache.cb_get_image;
}

CSSLayout::CSSLayout(std::shared_ptr<CSSLayout_Impl> impl)
: impl(impl)
{
}

/////////////////////////////////////////////////////////////////////////////

CSSLayout_Impl::~CSSLayout_Impl()
{
	dispose();
}

std::shared_ptr<CSSLayoutNode_Impl> CSSLayout_Impl::alloc_node_impl() const
{
	CSSLayoutNode_Impl *node_impl = 0;
	if (free_node_impls.empty())
	{
		node_impl = new CSSLayoutNode_Impl();
		node_impl->layout_impl = self;
	}
	else
	{
		node_impl = free_node_impls.back();
		free_node_impls.pop_back();
	}
	return std::shared_ptr<CSSLayoutNode_Impl>(node_impl, NodeImplDeleter(this));
}

void CSSLayout_Impl::free_node_impl(CSSLayoutNode_Impl *node_impl)
{
	if (!node_impl->layout_impl.expired())
		free_node_impls.push_back(node_impl);
	else
		delete node_impl;
}

void CSSLayout_Impl::on_dispose()
{
	clear();
}

void CSSLayout_Impl::clear()
{
	layout_tree.clear();
	box_tree.clear();
}

CSSLayoutNode CSSLayout_Impl::get_node(CSSBoxNode *box_node) const
{
	if (box_node)
	{
		std::shared_ptr<CSSLayoutNode_Impl> node_impl = alloc_node_impl();
		node_impl->box_node = box_node;
		return CSSLayoutNode(node_impl);
	}
	else
	{
		return CSSLayoutNode();
	}
}

CSSLayout CSSLayout_Impl::get_layout()
{
	throw_if_disposed();
	return CSSLayout(std::shared_ptr<CSSLayout_Impl>(self));
}

}
