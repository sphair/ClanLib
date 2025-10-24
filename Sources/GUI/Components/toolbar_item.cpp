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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/Components/toolbar_item.h"
#include "toolbar_item_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBarItem Construction:

CL_ToolBarItem::CL_ToolBarItem()
{
}

CL_ToolBarItem::CL_ToolBarItem(int id)
: impl(new CL_ToolBarItem_Impl)
{
	impl->id = id;
}

CL_ToolBarItem::CL_ToolBarItem(const CL_SharedPtr<CL_ToolBarItem_Impl> &impl)
: impl(impl)
{
}

CL_ToolBarItem::~CL_ToolBarItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBarItem Attributes:

CL_StringRef CL_ToolBarItem::get_text() const
{
	return impl->text;
}

int CL_ToolBarItem::get_id() const
{
	return impl->id;
}

void CL_ToolBarItem::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_ToolBarItem is null");
}

bool CL_ToolBarItem::is_disabled() const
{
	return impl->disabled;
}

bool CL_ToolBarItem::is_toggling() const
{
	return impl->toggling;
}

bool CL_ToolBarItem::is_pressed() const
{
	return impl->pressed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBarItem Events:

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBarItem Operations:

void CL_ToolBarItem::set_id(int id)
{
	impl->id = id;
}

void CL_ToolBarItem::set_text(const CL_StringRef &text)
{
	impl->text = text;
}

void CL_ToolBarItem::set_icon(const CL_Sprite &image, int frame)
{
	impl->icon = image;
	impl->frame = frame;
}

void CL_ToolBarItem::set_toggling(bool toggling)
{
	impl->toggling = toggling;
}

void CL_ToolBarItem::set_pressed( bool value )
{
	impl->pressed = value;
}
/////////////////////////////////////////////////////////////////////////////
// CL_ToolBarItem Implementation:
