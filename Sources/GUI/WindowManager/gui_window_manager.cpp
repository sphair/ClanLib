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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

class GUIWindowManager_Impl
{
public:
	GUIWindowManager_Impl()
	: provider(0)
	{
	}

	~GUIWindowManager_Impl()
	{
		if (provider)
			delete provider;
	}

	GUIWindowManagerProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManager Construction:

GUIWindowManager::GUIWindowManager()
{
}

GUIWindowManager::GUIWindowManager(GUIWindowManagerProvider *provider)
: impl(std::make_shared<GUIWindowManager_Impl>())
{
	impl->provider = provider;
}

GUIWindowManager::GUIWindowManager(const GUIWindowManager &src)
: impl(src.impl)
{
}

GUIWindowManager::~GUIWindowManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManager Attributes:

GUIWindowManager::WindowManagerType GUIWindowManager::get_window_manager_type() const
{
	return impl->provider->get_window_manager_type();
}

bool GUIWindowManager::is_null() const
{
	return !impl;
}

GUIWindowManagerProvider *GUIWindowManager::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManager Operations:

void GUIWindowManager::set_site(GUIWindowManagerSite *site)
{
	impl->provider->set_site(site);
}

void GUIWindowManager::create_window(
	GUITopLevelWindow *handle,
	GUITopLevelWindow *owner,
	GUIComponent *component,
	GUITopLevelDescription description)
{
	impl->provider->create_window(handle, owner, component, description);
}

void GUIWindowManager::destroy_window(GUITopLevelWindow *handle)
{
	impl->provider->destroy_window(handle);
}

void GUIWindowManager::enable_window(GUITopLevelWindow *handle, bool enable)
{
	impl->provider->enable_window(handle, enable);
}

bool GUIWindowManager::has_focus(GUITopLevelWindow *handle) const
{
	return impl->provider->has_focus(handle);
}

void GUIWindowManager::set_visible(GUITopLevelWindow *handle, bool visible, bool activate)
{
	impl->provider->set_visible(handle, visible, activate);
}

void GUIWindowManager::set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area)
{
	impl->provider->set_geometry(handle, geometry, client_area);
}

Rect GUIWindowManager::get_geometry(GUITopLevelWindow *handle, bool client_area) const
{
	return impl->provider->get_geometry(handle, client_area);
}

Point GUIWindowManager::screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const
{
	return impl->provider->screen_to_window(handle, screen_point, client_area);
}

Point GUIWindowManager::window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const
{
	return impl->provider->window_to_screen(handle, window_point, client_area);
}

Canvas& GUIWindowManager::get_canvas(GUITopLevelWindow *handle) const
{
	return impl->provider->get_canvas(handle);
}

InputContext GUIWindowManager::get_ic(GUITopLevelWindow *handle) const
{
	return impl->provider->get_ic(handle);
}

Canvas GUIWindowManager::begin_paint(GUITopLevelWindow *handle, const Rect &update_region)
{
	return impl->provider->begin_paint(handle, update_region);
}

void GUIWindowManager::end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region)
{
	impl->provider->end_paint(canvas, handle, update_region);
}

void GUIWindowManager::capture_mouse(GUITopLevelWindow *handle, bool state)
{
	impl->provider->capture_mouse(handle, state);
}

DisplayWindow GUIWindowManager::get_display_window(GUITopLevelWindow *handle) const
{
	return impl->provider->get_display_window(handle);
}

void GUIWindowManager::set_cursor(GUITopLevelWindow *handle, const Cursor &cursor)
{
	impl->provider->set_cursor(handle, cursor);
}

void GUIWindowManager::set_cursor(GUITopLevelWindow *handle, StandardCursor type)
{
	impl->provider->set_cursor(handle, type);
}

void GUIWindowManager::bring_to_front(GUITopLevelWindow *handle)
{
	impl->provider->bring_to_front(handle);
}

bool GUIWindowManager::is_minimized(GUITopLevelWindow *handle) const
{
	return impl->provider->is_minimized(handle);
}

bool GUIWindowManager::is_maximized(GUITopLevelWindow *handle) const
{
	return impl->provider->is_maximized(handle);
}

void GUIWindowManager::request_repaint(GUITopLevelWindow *handle, const Rect &update_region)
{
	impl->provider->request_repaint(handle, update_region);
}

void GUIWindowManager::update()
{
	impl->provider->update();
}

void GUIWindowManager::set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	impl->provider->set_cliprect(handle, canvas, rect);
}

void GUIWindowManager::reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	impl->provider->reset_cliprect(handle, canvas);
}

void GUIWindowManager::push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	impl->provider->push_cliprect(handle, canvas, rect);
}

void GUIWindowManager::pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	impl->provider->pop_cliprect(handle, canvas);
}



/////////////////////////////////////////////////////////////////////////////
// GUIWindowManager Implementation:

}
