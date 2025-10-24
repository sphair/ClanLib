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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_layout_text.h"
#include "API/CSSLayout/css_layout_element.h"
#include "API/CSSLayout/css_layout_object.h"
#include "API/CSSLayout/css_hit_test_result.h"
#include "BoxTree/css_box_element.h"
#include "BoxTree/css_box_text.h"
#include "BoxTree/css_box_object.h"
#include "LayoutTree/css_layout_hit_test_result.h"
#include "LayoutTree/css_layout_graphics.h"
#include "css_layout_impl.h"
#include "css_layout_node_impl.h"

CL_CSSLayout::CL_CSSLayout()
: impl(new CL_CSSLayout_Impl())
{
	impl->self = impl;
}

bool CL_CSSLayout::is_null() const
{
	return !impl || impl->box_tree.get_root_element() == 0;
}

void CL_CSSLayout::layout(CL_GraphicContext &gc, const CL_Rect &viewport)
{
	impl->throw_if_disposed();

	CL_CSSLayoutGraphics graphics(gc, &impl->resource_cache, impl->viewport);
	impl->box_tree.prepare(&impl->resource_cache);
	impl->layout_tree.create(impl->box_tree.get_root_element());
	impl->layout_tree.layout(&graphics, &impl->resource_cache, viewport.get_size());
	impl->viewport = viewport;
}

void CL_CSSLayout::render_impl(CL_GraphicContext &gc, CL_UniquePtr<ClipWrapper> wrapper)
{
	impl->throw_if_disposed();

	CL_CSSLayoutGraphics graphics(gc, &impl->resource_cache, impl->viewport, wrapper.get());
	impl->layout_tree.render(&graphics, &impl->resource_cache);
}

void CL_CSSLayout::clear_selection()
{
	impl->throw_if_disposed();
	impl->box_tree.set_selection(0, 0, 0, 0);
}

void CL_CSSLayout::set_selection(CL_CSSLayoutNode start, size_t start_text_offset, CL_CSSLayoutNode end, size_t end_text_offset)
{
	impl->throw_if_disposed();
	if (start.is_null() || end.is_null())
	{
		clear_selection();
	}
	else
	{
		impl->box_tree.set_selection(start.impl->box_node, start_text_offset, end.impl->box_node, end_text_offset);
	}
}

CL_CSSHitTestResult CL_CSSLayout::hit_test(CL_GraphicContext &gc, const CL_Point &pos)
{
	impl->throw_if_disposed();
	CL_CSSLayoutGraphics graphics(gc, &impl->resource_cache, impl->viewport);
	CL_CSSLayoutHitTestResult result = impl->layout_tree.hit_test(&graphics, &impl->resource_cache, pos);
	if (result.node)
	{
		CL_CSSHitTestResult result2;
		switch (result.type)
		{
		case CL_CSSLayoutHitTestResult::no_objects_available:
			result2.type = CL_CSSHitTestResult::no_objects_available;
			break;
		case CL_CSSLayoutHitTestResult::outside_top:
			result2.type = CL_CSSHitTestResult::outside_top;
			break;
		case CL_CSSLayoutHitTestResult::outside_left:
			result2.type = CL_CSSHitTestResult::outside_left;
			break;
		case CL_CSSLayoutHitTestResult::outside_right:
			result2.type = CL_CSSHitTestResult::outside_right;
			break;
		case CL_CSSLayoutHitTestResult::outside_bottom:
			result2.type = CL_CSSHitTestResult::outside_bottom;
			break;
		case CL_CSSLayoutHitTestResult::inside:
			result2.type = CL_CSSHitTestResult::inside;
			break;
		}
		result2.node = impl->get_node(result.node);
		result2.text_offset = result.text_offset;
		return result2;
	}
	else
	{
		return CL_CSSHitTestResult();
	}
}

void CL_CSSLayout::load_xml(const CL_String &filename, const CL_String &style_sheet)
{
	impl->throw_if_disposed();
	impl->clear();
	CL_File file(filename, CL_File::open_existing, CL_File::access_read);
	CL_DomDocument doc(file, false);
	impl->box_tree.css.add_sheet("default.css");
	impl->box_tree.css.add_sheet(style_sheet);
	impl->box_tree.create(doc.get_document_element());
}

void CL_CSSLayout::clear()
{
	impl->throw_if_disposed();
	impl->clear();
}

void CL_CSSLayout::set_root_element(CL_CSSLayoutElement element)
{
	impl->throw_if_disposed();
	impl->clear();
	if (!element.is_null() && element.get_parent().is_null())
		impl->box_tree.set_root_element(static_cast<CL_CSSBoxElement*>(element.impl->box_node));
}

CL_CSSLayoutElement CL_CSSLayout::get_root_element()
{
	impl->throw_if_disposed();
	CL_CSSBoxElement *root = impl->box_tree.get_root_element();
	if (root)
	{
		CL_SharedPtr<CL_CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
		node_impl->box_node = root;
		return CL_CSSLayoutNode(node_impl).to_element();
	}
	else
	{
		return CL_CSSLayoutElement();
	}
}

void CL_CSSLayout::set_html_body_element(CL_CSSLayoutElement element)
{
	impl->throw_if_disposed();
	impl->clear();
	if (!element.is_null() && element.get_parent().is_null())
		impl->box_tree.set_html_body_element(static_cast<CL_CSSBoxElement*>(element.impl->box_node));
}

CL_CSSLayoutElement CL_CSSLayout::get_html_body_element()
{
	impl->throw_if_disposed();
	CL_CSSBoxElement *html_body = impl->box_tree.get_html_body_element();
	if (html_body)
	{
		CL_SharedPtr<CL_CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
		node_impl->box_node = html_body;
		return CL_CSSLayoutNode(node_impl).to_element();
	}
	else
	{
		return CL_CSSLayoutElement();
	}
}

CL_CSSLayoutObject CL_CSSLayout::create_object()
{
	impl->throw_if_disposed();
	CL_SharedPtr<CL_CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
	node_impl->box_node = new CL_CSSBoxObject();
	return CL_CSSLayoutNode(node_impl).to_object();
}

CL_CSSLayoutElement CL_CSSLayout::create_element(const CL_String &name)
{
	impl->throw_if_disposed();
	CL_SharedPtr<CL_CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
	node_impl->box_node = new CL_CSSBoxElement();
	CL_CSSLayoutElement e = CL_CSSLayoutNode(node_impl).to_element();
	e.set_name(name);
	return e;
}

CL_CSSLayoutText CL_CSSLayout::create_text(const CL_String &text)
{
	impl->throw_if_disposed();
	CL_SharedPtr<CL_CSSLayoutNode_Impl> node_impl = impl->alloc_node_impl();
	node_impl->box_node = new CL_CSSBoxText();
	CL_CSSLayoutText node = CL_CSSLayoutNode(node_impl).to_text();
	node.set_text(text);
	return node;
}

CL_CSSLayoutElement CL_CSSLayout::find_element(const CL_String &name)
{
	if (!get_root_element().is_null())
	{
		std::vector<CL_CSSLayoutNode> stack;
		stack.push_back(get_root_element());
		while (!stack.empty())
		{
			if (stack.back().is_element() && stack.back().to_element().get_name() == name)
				return stack.back().to_element();

			CL_CSSLayoutNode next = stack.back().get_first_child();
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
		return CL_CSSLayoutElement();
	}
	else
	{
		return CL_CSSLayoutElement();
	}
}

CL_Callback_2<CL_Image, CL_GraphicContext &, const CL_String &> &CL_CSSLayout::func_get_image()
{
	return impl->resource_cache.cb_get_image;
}

CL_CSSLayout::CL_CSSLayout(CL_SharedPtr<CL_CSSLayout_Impl> impl)
: impl(impl)
{
}

/////////////////////////////////////////////////////////////////////////////

CL_CSSLayout_Impl::~CL_CSSLayout_Impl()
{
	dispose();
}

CL_SharedPtr<CL_CSSLayoutNode_Impl> CL_CSSLayout_Impl::alloc_node_impl() const
{
	CL_CSSLayoutNode_Impl *node_impl = 0;
	if (free_node_impls.empty())
	{
		node_impl = new CL_CSSLayoutNode_Impl();
		node_impl->layout_impl = self;
	}
	else
	{
		node_impl = free_node_impls.back();
		free_node_impls.pop_back();
	}
	return CL_SharedPtr<CL_CSSLayoutNode_Impl>(node_impl, NodeImplDeleter(this));
}

void CL_CSSLayout_Impl::free_node_impl(CL_CSSLayoutNode_Impl *node_impl)
{
	if (!node_impl->layout_impl.expired())
		free_node_impls.push_back(node_impl);
	else
		delete node_impl;
}

void CL_CSSLayout_Impl::on_dispose()
{
	clear();
}

void CL_CSSLayout_Impl::clear()
{
	layout_tree.clear();
	box_tree.clear();
}

CL_CSSLayoutNode CL_CSSLayout_Impl::get_node(CL_CSSBoxNode *box_node) const
{
	if (box_node)
	{
		CL_SharedPtr<CL_CSSLayoutNode_Impl> node_impl = alloc_node_impl();
		node_impl->box_node = box_node;
		return CL_CSSLayoutNode(node_impl);
	}
	else
	{
		return CL_CSSLayoutNode();
	}
}

CL_CSSLayout CL_CSSLayout_Impl::get_layout()
{
	throw_if_disposed();
	return CL_CSSLayout(CL_SharedPtr<CL_CSSLayout_Impl>(self));
}
