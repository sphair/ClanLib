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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_component_description.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/keys.h"
#include "listview_column_header_impl.h"

#ifdef INCLUDE_COMPONENTS

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewColumnHeader Construction:

ListViewColumnHeader::ListViewColumnHeader()
{
}

ListViewColumnHeader::ListViewColumnHeader(const std::shared_ptr<ListViewColumnHeader_Impl> &impl)
: impl(impl)
{
}

ListViewColumnHeader::~ListViewColumnHeader()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewColumnHeader Attributes:

void ListViewColumnHeader::throw_if_null() const
{
	if (!impl)
		throw Exception("ListViewColumnHeader is null");
}

std::string ListViewColumnHeader::get_column_id() const
{
	return impl->column_id;
}

std::string ListViewColumnHeader::get_caption() const
{
	return impl->caption;
}

int ListViewColumnHeader::get_width()
{
	return impl->width;
}

int ListViewColumnHeader::get_used_width()
{
	return impl->rect.get_width();
}

ListViewColumnHeader::TextAlignment ListViewColumnHeader::get_alignment()
{
	return impl->align;
}

ListViewColumnHeader ListViewColumnHeader::get_next_sibling()
{
	return ListViewColumnHeader(impl->next_sibling);
}

ListViewColumnHeader ListViewColumnHeader::get_prev_sibling()
{
	return ListViewColumnHeader(impl->prev_sibling.lock());
}

/////////////////////////////////////////////////////////////////////////////
// ListViewColumnHeader Operations:

void ListViewColumnHeader::set_width(int width)
{
	impl->width = width;
	if (!impl->func_size_changed.is_null())
		impl->func_size_changed.invoke(*this);
}

void ListViewColumnHeader::set_caption(const std::string &str)
{
	impl->caption = str;
}

void ListViewColumnHeader::set_column_id(const std::string &str)
{
	impl->column_id = str;
}

void ListViewColumnHeader::set_alignment(ListViewColumnHeader::TextAlignment align)
{
	impl->align = align;
}

void ListViewColumnHeader::set_rect(const Rect &rect)
{
	impl->rect = rect;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewColumnHeader Events:

Callback_v1<ListViewColumnHeader> &ListViewColumnHeader::func_size_changed()
{
	return impl->func_size_changed;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewColumnHeader Implementation:

}

#endif
