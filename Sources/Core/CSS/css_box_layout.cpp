/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/CSS/css_box_layout.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/System/weakptr.h"
#include "API/Core/System/block_allocator.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBox_Impl Class:

class CL_CSSBox_Impl : public CL_BlockAllocated
{
public:
	CL_CSSBox_Impl(CL_WeakPtr<CL_CSSBoxLayout_Impl> box_layout)
	: box_layout(box_layout), type(CL_CSSBox::NULL_BOX), anonymous(false)
	{
	}

	CL_WeakPtr<CL_CSSBoxLayout_Impl> box_layout;

	CL_CSSBox::Type type;

	bool anonymous;

	CL_CSSBox parent_box;

	CL_CSSBox first_child;

	CL_CSSBox last_child;

	CL_CSSBox prev_sibling;

	CL_CSSBox next_sibling;
};

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBoxLayout_Impl Class:

class CL_CSSBoxLayout_Impl
{
public:
	CL_CSSBox body_box;

	CL_BlockAllocator block_allocator;

	static CL_CSSBox create_box(CL_SharedPtr<CL_CSSBoxLayout_Impl> sharedptr, CL_CSSBox::Type type)
	{
		CL_SharedPtr<CL_CSSBox_Impl> box_impl(
			new(&sharedptr->block_allocator) CL_CSSBox_Impl(
				CL_WeakPtr<CL_CSSBoxLayout_Impl>(sharedptr)));
		box_impl->type = type;
		return CL_CSSBox(box_impl);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBoxLayout Construction:

CL_CSSBoxLayout::CL_CSSBoxLayout()
: impl(new CL_CSSBoxLayout_Impl)
{
	impl->body_box = CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::BLOCK_BOX);
}

CL_CSSBoxLayout::CL_CSSBoxLayout(CL_SharedPtr<CL_CSSBoxLayout_Impl> impl)
: impl(impl)
{
}

CL_CSSBoxLayout::~CL_CSSBoxLayout()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBoxLayout Attributes:

CL_CSSBox CL_CSSBoxLayout::get_body_box()
{
	return CL_CSSBox(impl->body_box);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBoxLayout Operations:

CL_CSSBox CL_CSSBoxLayout::create_inline_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::INLINE_BOX);
}

CL_CSSInlineTextBox CL_CSSBoxLayout::create_inline_text_box(const CL_StringRef &text)
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::INLINE_TEXT_BOX).to_inline_text_box();
}

CL_CSSInlineObjectBox CL_CSSBoxLayout::create_inline_object_box(const CL_DomElement &element)
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::INLINE_OBJECT_BOX).to_inline_object_box();
}

CL_CSSBox CL_CSSBoxLayout::create_block_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::BLOCK_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_list_item_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::LIST_ITEM_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_run_in_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::RUN_IN_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_compact_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::COMPACT_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_marker_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::MARKER_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_inline_table_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::INLINE_TABLE_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_row_group_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_ROW_GROUP_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_header_group_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_HEADER_GROUP_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_footer_group_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_FOOTER_GROUP_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_row_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_ROW_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_column_group_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_COLUMN_GROUP_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_column_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_COLUMN_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_cell_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_CELL_BOX);
}

CL_CSSBox CL_CSSBoxLayout::create_table_caption_box()
{
	return CL_CSSBoxLayout_Impl::create_box(impl, CL_CSSBox::TABLE_CAPTION_BOX);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBoxLayout Implementation:


/////////////////////////////////////////////////////////////////////////////
// CL_CSSBox Construction:

CL_CSSBox::CL_CSSBox()
{
}

CL_CSSBox::CL_CSSBox(CL_SharedPtr<CL_CSSBox_Impl> impl)
: impl(impl)
{
}

CL_CSSBox::~CL_CSSBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBox Attributes:

CL_CSSBoxLayout CL_CSSBox::get_box_layout()
{
	if (impl.is_null())
		return CL_CSSBoxLayout();
	else
		return CL_CSSBoxLayout(impl->box_layout);
}

CL_CSSBox::Type CL_CSSBox::get_type() const
{
	if (impl.is_null())
		return CL_CSSBox::NULL_BOX;
	else
		return impl->type;
}

CL_CSSBox CL_CSSBox::get_parent_box() const
{
	if (impl.is_null())
		return CL_CSSBox();
	else
		return impl->parent_box;
}

CL_CSSBox CL_CSSBox::get_first_child() const
{
	if (impl.is_null())
		return CL_CSSBox();
	else
		return impl->first_child;
}
	
CL_CSSBox CL_CSSBox::get_last_child() const
{
	if (impl.is_null())
		return CL_CSSBox();
	else
		return impl->last_child;
}
	
CL_CSSBox CL_CSSBox::get_previous_sibling() const
{
	if (impl.is_null())
		return CL_CSSBox();
	else
		return impl->prev_sibling;
}
	
CL_CSSBox CL_CSSBox::get_next_sibling() const
{
	if (impl.is_null())
		return CL_CSSBox();
	else
		return impl->next_sibling;
}

bool CL_CSSBox::is_null() const
{
	return get_type() == NULL_BOX;
}

bool CL_CSSBox::is_anonymous() const
{
	if (impl.is_null())
		return false;
	else
		return impl->anonymous;
}

bool CL_CSSBox::is_inline_box() const
{
	return get_type() == INLINE_BOX;
}

bool CL_CSSBox::is_inline_text_box() const
{
	return get_type() == INLINE_TEXT_BOX;
}

bool CL_CSSBox::is_inline_object_box() const
{
	return get_type() == INLINE_OBJECT_BOX;
}

bool CL_CSSBox::is_block_box() const
{
	return get_type() == BLOCK_BOX;
}

bool CL_CSSBox::is_list_item_box() const
{
	return get_type() == LIST_ITEM_BOX;
}

bool CL_CSSBox::is_run_in_box() const
{
	return get_type() == RUN_IN_BOX;
}

bool CL_CSSBox::is_compact_box() const
{
	return get_type() == COMPACT_BOX;
}

bool CL_CSSBox::is_marker_box() const
{
	return get_type() == MARKER_BOX;
}

bool CL_CSSBox::is_table_box() const
{
	return get_type() == TABLE_BOX;
}

bool CL_CSSBox::is_inline_table_box() const
{
	return get_type() == INLINE_TABLE_BOX;
}

bool CL_CSSBox::is_table_row_group_box() const
{
	return get_type() == TABLE_ROW_GROUP_BOX;
}

bool CL_CSSBox::is_table_header_group_box() const
{
	return get_type() == TABLE_HEADER_GROUP_BOX;
}

bool CL_CSSBox::is_table_footer_group_box() const
{
	return get_type() == TABLE_FOOTER_GROUP_BOX;
}

bool CL_CSSBox::is_table_row_box() const
{
	return get_type() == TABLE_ROW_BOX;
}

bool CL_CSSBox::is_table_column_group_box() const
{
	return get_type() == TABLE_COLUMN_GROUP_BOX;
}

bool CL_CSSBox::is_table_column_box() const
{
	return get_type() == TABLE_COLUMN_BOX;
}

bool CL_CSSBox::is_table_cell_box() const
{
	return get_type() == TABLE_CELL_BOX;
}

bool CL_CSSBox::is_table_caption_box() const
{
	return get_type() == TABLE_CAPTION_BOX;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBox Operations:

CL_CSSInlineTextBox CL_CSSBox::to_inline_text_box() const
{
	if (is_inline_text_box())
		return CL_CSSInlineTextBox(impl);
	else
		return CL_CSSInlineTextBox();
}

CL_CSSInlineObjectBox CL_CSSBox::to_inline_object_box() const
{
	if (is_inline_object_box())
		return CL_CSSInlineObjectBox(impl);
	else
		return CL_CSSInlineObjectBox();
}

CL_CSSBox CL_CSSBox::append_child(CL_CSSBox new_child)
{
	if (new_child.impl.is_null())
		return new_child;
	if (!new_child.impl->parent_box.is_null())
		new_child.impl->parent_box.remove_child(new_child);

	new_child.impl->parent_box = *this;
	if (!impl->last_child.is_null())
	{
		impl->last_child.impl->next_sibling = new_child;
		new_child.impl->prev_sibling = impl->last_child;
	}
	impl->last_child = new_child;
	if (impl->first_child.is_null())
		impl->first_child = new_child;

	return new_child;
}

CL_CSSBox CL_CSSBox::remove_child(CL_CSSBox child)
{
	if (child.impl->parent_box.impl != impl)
		return child;

	if (!child.impl->prev_sibling.is_null())
		child.impl->prev_sibling.impl->next_sibling = child.impl->next_sibling;
	if (!child.impl->next_sibling.is_null())
		child.impl->next_sibling.impl->prev_sibling = child.impl->prev_sibling;
	if (impl->first_child.impl == child.impl)
		impl->first_child = child.impl->next_sibling;
	if (impl->last_child.impl == child.impl)
		impl->last_child = child.impl->prev_sibling;

	child.impl->parent_box = CL_CSSBox();
	child.impl->prev_sibling = CL_CSSBox();
	child.impl->next_sibling = CL_CSSBox();

	return child;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSBox Implementation:


/////////////////////////////////////////////////////////////////////////////
// CL_CSSInlineTextBox Construction:

CL_CSSInlineTextBox::CL_CSSInlineTextBox()
{
}

CL_CSSInlineTextBox::CL_CSSInlineTextBox(CL_SharedPtr<CL_CSSBox_Impl> impl)
{
}

CL_CSSInlineTextBox::~CL_CSSInlineTextBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSInlineTextBox Attributes:

CL_StringRef CL_CSSInlineTextBox::get_text() const
{
	return CL_StringRef();
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSInlineObjectBox Construction:

CL_CSSInlineObjectBox::CL_CSSInlineObjectBox()
{
}

CL_CSSInlineObjectBox::CL_CSSInlineObjectBox(CL_SharedPtr<CL_CSSBox_Impl> impl)
{
}

CL_CSSInlineObjectBox::~CL_CSSInlineObjectBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSInlineObjectBox Attributes:

CL_DomElement CL_CSSInlineObjectBox::get_object_element() const
{
	return CL_DomElement();
}
