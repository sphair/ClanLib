/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DisplayWindowDescription construction:

DisplayWindowDescription::DisplayWindowDescription()
: impl(std::make_shared<DisplayWindowDescription_Impl>())
{
}

DisplayWindowDescription::DisplayWindowDescription(const std::string &title)
: impl(std::make_shared<DisplayWindowDescription_Impl>())
{
	impl->title = title;
}

DisplayWindowDescription::DisplayWindowDescription(const std::string &title, const Rectf &position, bool client_area)
: impl(std::make_shared<DisplayWindowDescription_Impl>())
{
	impl->title = title;
	impl->position = position;
	impl->position_client_area = client_area;
}

DisplayWindowDescription::DisplayWindowDescription(const std::string &title, const Sizef &size, bool client_area)
	: impl(std::make_shared<DisplayWindowDescription_Impl>())
{
	impl->title = title;
	impl->position.set_size(size);
	impl->position_client_area = client_area;
}

DisplayWindowDescription::DisplayWindowDescription(const Rectf &position, bool client_area)
: impl(std::make_shared<DisplayWindowDescription_Impl>())
{
	impl->position = position;
	impl->position_client_area = client_area;
}

DisplayWindowDescription::DisplayWindowDescription(const DisplayWindowDescription &copy) : impl(copy.impl)
{
}

DisplayWindowDescription::~DisplayWindowDescription()
{
}

DisplayWindowDescription DisplayWindowDescription::clone() const
{
	DisplayWindowDescription copy;
	*copy.impl = *impl;
	return copy;
}

/////////////////////////////////////////////////////////////////////////////
// DisplayWindowDescription attributes:

const std::string &DisplayWindowDescription::get_title() const
{
	return impl->title;
}

Sizef DisplayWindowDescription::get_size() const
{
	return impl->position.get_size();
}

Rectf DisplayWindowDescription::get_position() const
{
	return impl->position;
}

bool DisplayWindowDescription::get_position_client_area() const
{
	return impl->position_client_area;
}

bool DisplayWindowDescription::is_fullscreen() const
{
	return impl->fullscreen;
}

int DisplayWindowDescription::get_flipping_buffers() const
{
	return impl->flipping_buffers;
}

int DisplayWindowDescription::get_swap_interval() const
{
	return impl->swap_interval;
}

bool DisplayWindowDescription::get_allow_resize() const
{
	return impl->allow_resize;
}

DisplayWindow DisplayWindowDescription::get_owner() const
{
	return impl->owner;
}

bool DisplayWindowDescription::is_layered() const
{
	return impl->layered;
}

DisplayWindowHandle DisplayWindowDescription::get_handle() const
{
	return impl->handle;
}

int DisplayWindowDescription::get_bpp() const
{
	return impl->bpp;
}

int DisplayWindowDescription::get_refresh_rate() const
{
	return impl->refresh_rate;
}

bool DisplayWindowDescription::has_caption() const
{
	return impl->caption;
}

bool DisplayWindowDescription::has_sysmenu() const
{
	return impl->sysmenu;
}

bool DisplayWindowDescription::has_minimize_button() const
{
	return impl->minimize_button;
}

bool DisplayWindowDescription::has_maximize_button() const
{
	return impl->maximize_button;
}

bool DisplayWindowDescription::has_no_activate() const
{
	return impl->no_activate;
}

bool DisplayWindowDescription::is_topmost() const
{
	return impl->topmost;
}

bool DisplayWindowDescription::is_visible() const
{
	return impl->visible;
}

bool DisplayWindowDescription::has_drop_shadow() const
{
	return impl->drop_shadow;
}

bool DisplayWindowDescription::get_tablet_context() const
{
	return impl->create_tablet_context;
}

int DisplayWindowDescription::get_depth_size() const
{
	return impl->depth_size;
}

int DisplayWindowDescription::get_stencil_size() const
{
	return impl->stencil_size;
}

int DisplayWindowDescription::get_fullscreen_monitor() const
{
	return impl->fullscreen_monitor;
}

bool DisplayWindowDescription::get_allow_screensaver() const
{
	return impl->allow_screensaver;
}

bool DisplayWindowDescription::is_update_supported() const
{
	return impl->update_supported;
}

int DisplayWindowDescription::get_multisampling() const
{
	return impl->multisampling;
}

/////////////////////////////////////////////////////////////////////////////
// DisplayWindowDescription operations:

DisplayWindowDescription &DisplayWindowDescription::operator =(const DisplayWindowDescription &copy)
{
	impl = copy.impl;
	return *this;
}

bool DisplayWindowDescription::is_main() const
{
	return impl->type == WindowType::main;
}

bool DisplayWindowDescription::is_dialog() const
{
	return impl->type == WindowType::dialog;
}

bool DisplayWindowDescription::is_popup() const
{
	return impl->type == WindowType::popup;
}

void DisplayWindowDescription::set_main_window()
{
	impl->type = WindowType::main;
}

void DisplayWindowDescription::set_dialog_window()
{
	impl->type = WindowType::dialog;
}

void DisplayWindowDescription::set_popup_window()
{
	impl->type = WindowType::popup;
}

void DisplayWindowDescription::show_caption(bool value)
{
	impl->caption = value;
}

void DisplayWindowDescription::show_sysmenu(bool value)
{
	impl->sysmenu = value;
}

void DisplayWindowDescription::show_minimize_button(bool value)
{
	impl->minimize_button = value;
}

void DisplayWindowDescription::show_maximize_button(bool value)
{
	impl->maximize_button = value;
}

void DisplayWindowDescription::set_topmost(bool value)
{
	impl->topmost = value;
}

void DisplayWindowDescription::set_no_activate(bool value)
{
	impl->no_activate = value;
}

void DisplayWindowDescription::set_visible(bool value)
{
	impl->visible = value;
}

void DisplayWindowDescription::set_drop_shadow(bool value)
{
	impl->drop_shadow = value;
}

void DisplayWindowDescription::set_owner_window(const DisplayWindow &owner)
{
	impl->owner = owner;
}

void DisplayWindowDescription::set_title(const std::string &title)
{
	impl->title = title;
}

void DisplayWindowDescription::set_size(const Sizef &size, bool client_area)
{
	impl->position.set_size(size);
	impl->position_client_area = client_area;
}

void DisplayWindowDescription::set_position(const Rectf &position, bool client_area)
{
	impl->position = position;
	impl->position_client_area = client_area;
}

void DisplayWindowDescription::set_fullscreen(bool fullscreen, int monitor)
{
	impl->fullscreen = fullscreen;
	impl->fullscreen_monitor = monitor;
}

void DisplayWindowDescription::set_flipping_buffers(int num_buffers)
{
	impl->flipping_buffers = num_buffers;
}

void DisplayWindowDescription::set_swap_interval(int interval)
{
	impl->swap_interval = interval;
}

void DisplayWindowDescription::set_allow_resize(bool allow_resize)
{
	impl->allow_resize = allow_resize;
}

void DisplayWindowDescription::set_bpp(int bytes)
{
	impl->bpp = bytes;
}

void DisplayWindowDescription::set_refresh_rate(int refresh_rate)
{
	impl->refresh_rate = refresh_rate;
}

void DisplayWindowDescription::set_tablet_context(bool create)
{
	impl->create_tablet_context = create;
}

void DisplayWindowDescription::set_layered(bool layered)
{
	impl->layered = layered;
}

void DisplayWindowDescription::set_handle(DisplayWindowHandle handle)
{
	impl->handle = handle;
}

void DisplayWindowDescription::set_depth_size(int value)
{
	impl->depth_size = value;
}

void DisplayWindowDescription::set_stencil_size(int value)
{
	impl->stencil_size = value;
}

void DisplayWindowDescription::set_allow_screensaver(bool allow_screensaver)
{
	impl->allow_screensaver = allow_screensaver;
}

void DisplayWindowDescription::set_update_supported(bool value) const
{
	impl->update_supported = true;
}

void DisplayWindowDescription::set_multisampling(int value)
{
	impl->multisampling = value;
}

/////////////////////////////////////////////////////////////////////////////
// DisplayWindowDescription implementation:

}
