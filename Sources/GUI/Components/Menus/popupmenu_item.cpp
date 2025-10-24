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
#include "API/GUI/Components/popupmenu_item.h"
#include "popupmenu_impl.h"
#include "popupmenu_window.h"
#include "menu_modal_loop.h"


class CL_PopupMenuItem_Impl
{
public:
	CL_PopupMenuItem_Impl() :
	  id(-1),
	  submenu(CL_PopupMenu::create_null_object()),
	  disabled(false),
	  separator(false),
	  checkable(false),
	  is_checked(false)
	{
	}

	CL_PopupMenu submenu;
	CL_String text;
	CL_String accelerator_text;
	CL_PixelBuffer image;
	int id;
	bool disabled;
	bool separator;
	bool checkable;
	bool is_checked;
	CL_Callback_v0 func_clicked;
};


/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenuItem Construction:

CL_PopupMenuItem::CL_PopupMenuItem()
{
}

CL_PopupMenuItem::CL_PopupMenuItem(int id)
: impl(new CL_PopupMenuItem_Impl)
{
	impl->id = id;
}

CL_PopupMenuItem::CL_PopupMenuItem(const CL_SharedPtr<CL_PopupMenuItem_Impl> &impl)
: impl(impl)
{
}

CL_PopupMenuItem::~CL_PopupMenuItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenuItem Attributes:

CL_StringRef CL_PopupMenuItem::get_text() const
{
	return impl->text;
}

int CL_PopupMenuItem::get_id() const
{
	return impl->id;
}

void CL_PopupMenuItem::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_PopupMenuItem is null");
}

bool CL_PopupMenuItem::is_separator() const
{
	return impl->separator;
}

bool CL_PopupMenuItem::has_submenu() const
{
	return !impl->submenu.is_null();
}

CL_PopupMenu CL_PopupMenuItem::get_submenu()
{
	return impl->submenu;
}

CL_StringRef CL_PopupMenuItem::get_accelerator_text() const
{
	return impl->accelerator_text;
}

bool CL_PopupMenuItem::is_checkable() const
{
	return impl->checkable;
}

bool CL_PopupMenuItem::is_checked() const
{
	return impl->is_checked;
}

CL_PixelBuffer CL_PopupMenuItem::get_icon() const
{
	return impl->image;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenuItem Events:

CL_Callback_v0 & CL_PopupMenuItem::func_clicked()
{	
	return impl->func_clicked;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenuItem Operations:

void CL_PopupMenuItem::set_id(int id)
{
	impl->id = id;
}

void CL_PopupMenuItem::set_text(const CL_StringRef &text)
{
	impl->text = text;
}

void CL_PopupMenuItem::set_icon(const CL_PixelBuffer &image)
{
	impl->image = image;
}

void CL_PopupMenuItem::set_submenu(CL_PopupMenu &submenu)
{
	impl->submenu = submenu;
}

bool CL_PopupMenuItem::is_disabled() const
{
	return impl->disabled;
}

void CL_PopupMenuItem::set_accelerator_text(const CL_StringRef &str)
{
	impl->accelerator_text = str;
}

void CL_PopupMenuItem::set_enabled(bool enabled)
{
	impl->disabled = !enabled;
}

void CL_PopupMenuItem::set_separator(bool separator)
{
	impl->separator = separator;
}

void CL_PopupMenuItem::set_checkable( bool checkable )
{
	impl->checkable = checkable;
}

void CL_PopupMenuItem::set_checked( bool checked )
{
	impl->is_checked = checked;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenuItem Implementation:
