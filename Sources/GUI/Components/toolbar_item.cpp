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
#include "API/GUI/gui_manager.h"
#include "API/GUI/Components/toolbar_item.h"
#include "toolbar_item_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ToolBarItem Construction:

ToolBarItem::ToolBarItem()
{
}

ToolBarItem::ToolBarItem(int id)
: impl(new ToolBarItem_Impl)
{
	impl->id = id;
}

ToolBarItem::ToolBarItem(const std::shared_ptr<ToolBarItem_Impl> &impl)
: impl(impl)
{
}

ToolBarItem::~ToolBarItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// ToolBarItem Attributes:

std::string ToolBarItem::get_text() const
{
	return impl->text;
}

int ToolBarItem::get_id() const
{
	return impl->id;
}

void ToolBarItem::throw_if_null() const
{
	if (!impl)
		throw Exception("ToolBarItem is null");
}

bool ToolBarItem::is_disabled() const
{
	return impl->disabled;
}

bool ToolBarItem::is_toggling() const
{
	return impl->toggling;
}

bool ToolBarItem::is_pressed() const
{
	return impl->pressed;
}

/////////////////////////////////////////////////////////////////////////////
// ToolBarItem Events:

/////////////////////////////////////////////////////////////////////////////
// ToolBarItem Operations:

void ToolBarItem::set_id(int id)
{
	impl->id = id;
}

void ToolBarItem::set_text(const std::string &text)
{
	impl->text = text;
}

void ToolBarItem::set_icon(const Sprite &image, int frame)
{
	impl->icon = image;
	impl->frame = frame;
}

void ToolBarItem::set_toggling(bool toggling)
{
	impl->toggling = toggling;
}

void ToolBarItem::set_pressed( bool value )
{
	impl->pressed = value;
}

/////////////////////////////////////////////////////////////////////////////
// ToolBarItem Implementation:

}

