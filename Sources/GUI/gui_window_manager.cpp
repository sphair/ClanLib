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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Render/graphic_context.h"

class CL_GUIWindowManager_Impl
{
public:
	CL_GUIWindowManager_Impl()
	: provider(0)
	{
	}

	~CL_GUIWindowManager_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_GUIWindowManagerProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManager Construction:

CL_GUIWindowManager::CL_GUIWindowManager(CL_GUIWindowManagerProvider *provider)
: impl(new CL_GUIWindowManager_Impl)
{
	impl->provider = provider;
}

CL_GUIWindowManager::~CL_GUIWindowManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManager Attributes:

CL_GUIWindowManager::CL_WindowManagerType CL_GUIWindowManager::get_window_manager_type() const
{
	return impl->provider->get_window_manager_type();
}

bool CL_GUIWindowManager::is_null()
{
	return impl.is_null();
}

CL_GUIWindowManagerProvider *CL_GUIWindowManager::get_provider() const
{
	if (impl.is_null())
		return 0;
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManager Operations:

void CL_GUIWindowManager::set_site(CL_GUIWindowManagerSite *site)
{
	impl->provider->set_site(site);
}

void CL_GUIWindowManager::create_window(
	CL_GUITopLevelWindow *handle,
	CL_GUITopLevelWindow *owner,
	CL_GUITopLevelDescription description)
{
	impl->provider->create_window(handle, owner, description);
}

void CL_GUIWindowManager::destroy_window(CL_GUITopLevelWindow *handle)
{
	impl->provider->destroy_window(handle);
}

void CL_GUIWindowManager::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	impl->provider->enable_window(handle, enable);
}

bool CL_GUIWindowManager::has_focus(CL_GUITopLevelWindow *handle) const
{
	return impl->provider->has_focus(handle);
}

void CL_GUIWindowManager::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate)
{
	impl->provider->set_visible(handle, visible, activate);
}

void CL_GUIWindowManager::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	impl->provider->set_geometry(handle, geometry, client_area);
}

CL_Rect CL_GUIWindowManager::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	return impl->provider->get_geometry(handle, client_area);
}

CL_Point CL_GUIWindowManager::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	return impl->provider->screen_to_window(handle, screen_point, client_area);
}

CL_Point CL_GUIWindowManager::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	return impl->provider->window_to_screen(handle, window_point, client_area);
}

CL_GraphicContext& CL_GUIWindowManager::get_gc(CL_GUITopLevelWindow *handle) const
{
	return impl->provider->get_gc(handle);
}

CL_InputContext& CL_GUIWindowManager::get_ic(CL_GUITopLevelWindow *handle) const
{
	return impl->provider->get_ic(handle);
}

CL_GraphicContext CL_GUIWindowManager::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	return impl->provider->begin_paint(handle, update_region);
}

void CL_GUIWindowManager::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	impl->provider->end_paint(handle, update_region);
}

void CL_GUIWindowManager::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
{
	impl->provider->capture_mouse(handle, state);
}

CL_DisplayWindow CL_GUIWindowManager::get_display_window(CL_GUITopLevelWindow *handle) const
{
	return impl->provider->get_display_window(handle);
}

void CL_GUIWindowManager::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
	impl->provider->set_cursor(handle, cursor);
}

void CL_GUIWindowManager::set_cursor(CL_GUITopLevelWindow *handle, CL_StandardCursor type)
{
	impl->provider->set_cursor(handle, type);
}

void CL_GUIWindowManager::bring_to_front(CL_GUITopLevelWindow *handle)
{
	impl->provider->bring_to_front(handle);
}

bool CL_GUIWindowManager::is_minimized(CL_GUITopLevelWindow *handle) const
{
	return impl->provider->is_minimized(handle);
}

bool CL_GUIWindowManager::is_maximized(CL_GUITopLevelWindow *handle) const
{
	return impl->provider->is_maximized(handle);
}

void CL_GUIWindowManager::request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	impl->provider->request_repaint(handle, update_region);
}

void CL_GUIWindowManager::update()
{
	impl->provider->update();
}

void CL_GUIWindowManager::setup_painting()
{
	impl->provider->setup_painting();
}

void CL_GUIWindowManager::complete_painting()
{
	impl->provider->complete_painting();
}

void CL_GUIWindowManager::set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	impl->provider->set_cliprect(handle, gc, rect);
}

void CL_GUIWindowManager::reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	impl->provider->reset_cliprect(handle, gc);
}

void CL_GUIWindowManager::push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	impl->provider->push_cliprect(handle, gc, rect);
}

void CL_GUIWindowManager::pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	impl->provider->pop_cliprect(handle, gc);
}



/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManager Implementation:
