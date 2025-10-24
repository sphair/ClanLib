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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
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


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnHeader Construction:

CL_ListViewColumnHeader::CL_ListViewColumnHeader()
{
}

CL_ListViewColumnHeader::CL_ListViewColumnHeader(const CL_SharedPtr<CL_ListViewColumnHeader_Impl> &impl)
: impl(impl)
{
}

CL_ListViewColumnHeader::~CL_ListViewColumnHeader()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnHeader Attributes:

void CL_ListViewColumnHeader::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_ListViewColumnHeader is null");
}

CL_StringRef CL_ListViewColumnHeader::get_column_id() const
{
	return impl->column_id;
}

CL_StringRef CL_ListViewColumnHeader::get_caption() const
{
	return impl->caption;
}

int CL_ListViewColumnHeader::get_width()
{
	return impl->width;
}

CL_ListViewColumnHeader::TextAlignment CL_ListViewColumnHeader::get_alignment()
{
	return impl->align;
}

CL_ListViewColumnHeader CL_ListViewColumnHeader::get_next_sibling()
{
	return CL_ListViewColumnHeader(impl->next_sibling);
}

CL_ListViewColumnHeader CL_ListViewColumnHeader::get_prev_sibling()
{
	return CL_ListViewColumnHeader(impl->prev_sibling);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnHeader Operations:

void CL_ListViewColumnHeader::set_width(int width)
{
	impl->width = width;
	if (!impl->func_size_changed.is_null())
		impl->func_size_changed.invoke(*this);
}

void CL_ListViewColumnHeader::set_caption(const CL_StringRef &str)
{
	impl->caption = str;
}

void CL_ListViewColumnHeader::set_column_id(const CL_StringRef &str)
{
	impl->column_id = str;
}

void CL_ListViewColumnHeader::set_alignment(CL_ListViewColumnHeader::TextAlignment align)
{
	impl->align = align;
}

void CL_ListViewColumnHeader::set_rect(const CL_Rect &rect)
{
	impl->rect = rect;
}

void CL_ListViewColumnHeader::set_gui_theme_part(const CL_GUIThemePart &part)
{
	impl->part = part;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnHeader Events:

CL_Callback_v1<CL_ListViewColumnHeader> &CL_ListViewColumnHeader::func_size_changed()
{
	return impl->func_size_changed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnHeader Implementation:

