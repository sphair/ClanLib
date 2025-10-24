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
**    Harry Storbacka
**    Kenneth Gangstoe
*/
	
#include "Display/precomp.h"
#include "API/Display/Window/display_window_description.h"
#include "display_window_description_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription construction:

CL_DisplayWindowDescription::CL_DisplayWindowDescription() 
: impl(new CL_DisplayWindowDescription_Impl)
{
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(const CL_String &title) 
: impl(new CL_DisplayWindowDescription_Impl)
{
	impl->title = title;
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(const CL_String &title, const CL_Rect &position, bool client_area)
: impl(new CL_DisplayWindowDescription_Impl)
{
	impl->title = title;
	impl->position = position;
	impl->position_client_area = client_area;
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(const CL_String &title, const CL_Size &size, bool client_area)
	: impl(new CL_DisplayWindowDescription_Impl)
{
	impl->title = title;
	impl->position.set_size(size);
	impl->position_client_area = client_area;
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(const CL_Rect &position, bool client_area)
: impl(new CL_DisplayWindowDescription_Impl)
{
	impl->position = position;
	impl->position_client_area = client_area;
}

CL_DisplayWindowDescription::CL_DisplayWindowDescription(const CL_DisplayWindowDescription &copy) : impl(copy.impl)
{
}

CL_DisplayWindowDescription::~CL_DisplayWindowDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription attributes:

const CL_String &CL_DisplayWindowDescription::get_title() const
{
	return impl->title;
}

bool CL_DisplayWindowDescription::get_decorations() const
{
	return impl->decorations;
}

CL_Size CL_DisplayWindowDescription::get_size() const
{
	return impl->position.get_size();
}

CL_Rect CL_DisplayWindowDescription::get_position() const
{
	return impl->position;
}

bool CL_DisplayWindowDescription::get_position_client_area() const
{
	return impl->position_client_area;
}

bool CL_DisplayWindowDescription::is_fullscreen() const
{
	return impl->fullscreen;
}

int CL_DisplayWindowDescription::get_flipping_buffers() const
{
	return impl->flipping_buffers;
}

bool CL_DisplayWindowDescription::get_allow_resize() const
{
	return impl->allow_resize;
}

CL_DisplayWindow CL_DisplayWindowDescription::get_owner() const
{
	return impl->owner;
}

bool CL_DisplayWindowDescription::is_layered() const
{
	return impl->layered;
}

#ifdef WIN32
HWND CL_DisplayWindowDescription::get_handle() const
{
	return impl->handle;
}
#endif

int CL_DisplayWindowDescription::get_bpp() const
{
	return impl->bpp;
}

int CL_DisplayWindowDescription::get_refresh_rate() const
{
	return impl->refresh_rate;
}

bool CL_DisplayWindowDescription::has_caption() const
{
	return impl->caption;
}

bool CL_DisplayWindowDescription::has_sysmenu() const
{
	return impl->sysmenu;
}

bool CL_DisplayWindowDescription::has_minimize_button() const
{
	return impl->minimize_button;
}

bool CL_DisplayWindowDescription::has_maximize_button() const
{
	return impl->maximize_button;
}

bool CL_DisplayWindowDescription::has_border() const
{
	return impl->border;
}

bool CL_DisplayWindowDescription::is_tool_window() const
{
	return impl->tool_window;
}

bool CL_DisplayWindowDescription::is_topmost() const
{
	return impl->topmost;
}

bool CL_DisplayWindowDescription::is_visible() const
{
	return impl->visible;
}

bool CL_DisplayWindowDescription::get_using_gui_window_cache() const
{
	return impl->use_cached_window;
}

bool CL_DisplayWindowDescription::has_drop_shadow() const
{
	return impl->drop_shadow;
}

bool CL_DisplayWindowDescription::get_tablet_context() const
{
	return impl->create_tablet_context;
}

CL_UnknownSharedPtr CL_DisplayWindowDescription::get_data(const CL_String &name) const
{
	std::map<CL_String, CL_UnknownSharedPtr>::const_iterator it;
	it = impl->data_objects.find(name);
	if (it != impl->data_objects.end())
		return it->second;
	else
		return CL_UnknownSharedPtr();
}

int CL_DisplayWindowDescription::get_depth_size() const
{
	return impl->depth_size;
}

int CL_DisplayWindowDescription::get_stencil_size() const
{
	return impl->stencil_size;
}

int CL_DisplayWindowDescription::get_fullscreen_monitor() const
{
	return impl->fullscreen_monitor;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription operations:

CL_DisplayWindowDescription &CL_DisplayWindowDescription::operator =(const CL_DisplayWindowDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_DisplayWindowDescription::show_caption(bool value)
{
	impl->caption = value;
}

void CL_DisplayWindowDescription::show_sysmenu(bool value)
{
	impl->sysmenu = value;
}

void CL_DisplayWindowDescription::show_minimize_button(bool value)
{
	impl->minimize_button = value;
}

void CL_DisplayWindowDescription::show_maximize_button(bool value)
{
	impl->maximize_button = value;
}

void CL_DisplayWindowDescription::show_border(bool value)
{
	impl->border = value;
}

void CL_DisplayWindowDescription::set_decorations(bool value)
{
	impl->decorations = value;
}

void CL_DisplayWindowDescription::set_topmost(bool value)
{
	impl->topmost = value;
}

void CL_DisplayWindowDescription::set_visible(bool value)
{
	impl->visible = value;
}

void CL_DisplayWindowDescription::set_tool_window(bool value)
{
	impl->tool_window = value;
}

void CL_DisplayWindowDescription::set_drop_shadow(bool value)
{
	impl->drop_shadow = value;
}

void CL_DisplayWindowDescription::set_owner_window(const CL_DisplayWindow &owner)
{
	impl->owner = owner;
}

void CL_DisplayWindowDescription::set_title(const CL_String &title)
{
	impl->title = title;
}

void CL_DisplayWindowDescription::set_size(const CL_Size &size, bool client_area)
{
	impl->position.set_size(size);
	impl->position_client_area = client_area;
}

void CL_DisplayWindowDescription::set_position(const CL_Rect &position, bool client_area)
{
	impl->position = position;
	impl->position_client_area = client_area;
}

void CL_DisplayWindowDescription::set_fullscreen(bool fullscreen, int monitor)
{
	impl->fullscreen = fullscreen;
	impl->fullscreen_monitor = monitor;
}

void CL_DisplayWindowDescription::set_flipping_buffers(int num_buffers)
{
	impl->flipping_buffers = num_buffers;
}

void CL_DisplayWindowDescription::set_allow_resize(bool allow_resize)
{
	impl->allow_resize = allow_resize;
}

void CL_DisplayWindowDescription::set_bpp(int bytes)
{
	impl->bpp = bytes;
}

void CL_DisplayWindowDescription::set_refresh_rate(int refresh_rate)
{
	impl->refresh_rate = refresh_rate;
}

void CL_DisplayWindowDescription::set_tablet_context(bool create)
{
	impl->create_tablet_context = create;
}

void CL_DisplayWindowDescription::set_layered(bool layered)
{
	impl->layered = layered;
}

#ifdef WIN32
void CL_DisplayWindowDescription::set_handle(HWND handle)
{
	impl->handle = handle;
}
#endif

void CL_DisplayWindowDescription::set_data(const CL_String &name, const CL_UnknownSharedPtr &ptr)
{
	impl->data_objects[name] = ptr;
}

void CL_DisplayWindowDescription::set_depth_size(int value)
{
	impl->depth_size = value;
}

void CL_DisplayWindowDescription::set_stencil_size(int value)
{
	impl->stencil_size = value;
}

void CL_DisplayWindowDescription::set_using_gui_window_cache(bool value)
{
	impl->use_cached_window = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowDescription implementation:
