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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/Components/popupmenu_item.h"
#include "popupmenu_impl.h"
#include "popupmenu_window.h"
#include "menu_modal_loop.h"

namespace clan
{

class PopupMenuItem_Impl
{
public:
	PopupMenuItem_Impl() :
	  id(-1),
	  submenu(PopupMenu::create_null_object()),
	  disabled(false),
	  separator(false),
	  checkable(false),
	  is_checked(false)
	{
	}

	PopupMenu submenu;
	std::string text;
	std::string accelerator_text;
	PixelBuffer image;
	int id;
	bool disabled;
	bool separator;
	bool checkable;
	bool is_checked;
	Callback<void()> func_clicked;
};


/////////////////////////////////////////////////////////////////////////////
// PopupMenuItem Construction:

PopupMenuItem::PopupMenuItem()
{
}

PopupMenuItem::PopupMenuItem(int id)
: impl(new PopupMenuItem_Impl)
{
	impl->id = id;
}

PopupMenuItem::PopupMenuItem(const std::shared_ptr<PopupMenuItem_Impl> &impl)
: impl(impl)
{
}

PopupMenuItem::~PopupMenuItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenuItem Attributes:

std::string PopupMenuItem::get_text() const
{
	return impl->text;
}

int PopupMenuItem::get_id() const
{
	return impl->id;
}

void PopupMenuItem::throw_if_null() const
{
	if (!impl)
		throw Exception("PopupMenuItem is null");
}

bool PopupMenuItem::is_separator() const
{
	return impl->separator;
}

bool PopupMenuItem::has_submenu() const
{
	return !impl->submenu.is_null();
}

PopupMenu PopupMenuItem::get_submenu()
{
	return impl->submenu;
}

std::string PopupMenuItem::get_accelerator_text() const
{
	return impl->accelerator_text;
}

bool PopupMenuItem::is_checkable() const
{
	return impl->checkable;
}

bool PopupMenuItem::is_checked() const
{
	return impl->is_checked;
}

PixelBuffer PopupMenuItem::get_icon() const
{
	return impl->image;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenuItem Events:

Callback<void()> & PopupMenuItem::func_clicked()
{	
	return impl->func_clicked;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenuItem Operations:

void PopupMenuItem::set_id(int id)
{
	impl->id = id;
}

void PopupMenuItem::set_text(const std::string &text)
{
	impl->text = text;
}

void PopupMenuItem::set_icon(const PixelBuffer &image)
{
	impl->image = image;
}

void PopupMenuItem::set_submenu(PopupMenu &submenu)
{
	impl->submenu = submenu;
}

bool PopupMenuItem::is_disabled() const
{
	return impl->disabled;
}

void PopupMenuItem::set_accelerator_text(const std::string &str)
{
	impl->accelerator_text = str;
}

void PopupMenuItem::set_enabled(bool enabled)
{
	impl->disabled = !enabled;
}

void PopupMenuItem::set_separator(bool separator)
{
	impl->separator = separator;
}

void PopupMenuItem::set_checkable( bool checkable )
{
	impl->checkable = checkable;
}

void PopupMenuItem::set_checked( bool checked )
{
	impl->is_checked = checked;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenuItem Implementation:

}
