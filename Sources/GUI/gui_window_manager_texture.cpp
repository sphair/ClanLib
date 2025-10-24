/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "GUI/precomp.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/gradient.h"
#include "API/Display/Window/display_message_queue.h"
#include "API/Display/Window/display_window_message.h"
#include "API/GUI/gui_window_manager_texture.h"
#include "API/Display/Render/blend_mode.h"
#include "gui_window_manager_texture_impl.h"
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Construction:

CL_GUIWindowManagerTexture::CL_GUIWindowManagerTexture(CL_DisplayWindow display_window)
: impl(new CL_GUIWindowManagerTexture_Impl(display_window))
{
}

CL_GUIWindowManagerTexture::~CL_GUIWindowManagerTexture()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Attributes:

CL_GUIWindowManager::CL_WindowManagerType CL_GUIWindowManagerTexture::get_window_manager_type() const
{
	return cl_wm_type_texture;
}

std::vector<CL_GUIWindowManagerTextureWindow> CL_GUIWindowManagerTexture::get_windows(bool only_visible) const
{
	std::vector<CL_GUIWindowManagerTextureWindow> windows;

	std::vector<CL_GUITopLevelWindowTexture *>::size_type index, size;
	size = impl->z_order.size();
	for (index = size; index > 0; index--)
	{
		CL_GUITopLevelWindowTexture *toplevel_window = impl->z_order[index-1];
		if ( (only_visible == false) || (toplevel_window->visible) )
			windows.push_back(CL_GUIWindowManagerTextureWindow(toplevel_window->window, toplevel_window->subtexture, toplevel_window->geometry));
	}

	return windows;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Events:

CL_Callback_v0 &CL_GUIWindowManagerTexture::func_repaint()
{
	return impl->func_repaint;
}

CL_Callback_v1<CL_FrameBuffer &> &CL_GUIWindowManagerTexture::func_setup_framebuffer()
{
	return impl->func_setup_framebuffer;
}


CL_Callback_v2<CL_InputEvent &, CL_InputState &> &CL_GUIWindowManagerTexture::func_input_intercept()
{
	return impl->func_input_intercept;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Operations:

void CL_GUIWindowManagerTexture::set_site(CL_GUIWindowManagerSite *site)
{
	impl->site = site;
}

void CL_GUIWindowManagerTexture::create_window(
	CL_GUITopLevelWindow *handle,
	CL_GUITopLevelWindow *owner,
	CL_GUITopLevelDescription description,
	bool temporary)
{
	// Create the texture group, if it has not already been set
	CL_GraphicContext gc = impl->display_window.get_gc();
	if (impl->texture_group.is_null())
	{
		//TODO: This should be set to the maximum texture size that is <= 2048
		CL_Size initial_texture_size = CL_Size(512, 512);
		impl->texture_group = CL_TextureGroup(gc, initial_texture_size);
	}

	//Note: destroy_window() deletes this window
	CL_GUITopLevelWindowTexture *toplevel_window = new CL_GUITopLevelWindowTexture(handle);
	CL_Rect geometry = description.get_position();
	toplevel_window->geometry = geometry;
	toplevel_window->enabled = true;
	toplevel_window->visible = true;
	toplevel_window->subtexture = impl->texture_group.add(gc, geometry.get_size());

	CL_Texture frame_texture = toplevel_window->subtexture.get_texture();
	if (description.get_depth_size() > 0)
	{
		toplevel_window->depth_buffer = CL_RenderBuffer(gc, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), CL_DEPTH_COMPONENT);
	}
	if (description.get_stencil_size() > 0)
	{
		toplevel_window->stencil_buffer = CL_RenderBuffer(gc, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), CL_STENCIL);
	}

	impl->window_map[handle] = toplevel_window;
	impl->z_order.insert(impl->z_order.begin(), toplevel_window);
	impl->activated_window = handle;

	impl->clear_frame_buffer(toplevel_window);

	if (!impl->frame_buffer_initial_setup)
	{
		impl->frame_buffer_initial_setup = true;
		if (!impl->func_setup_framebuffer.is_null())
			impl->func_setup_framebuffer.invoke(impl->frame_buffer);
	}

	// to-to: report focus change.
}

void CL_GUIWindowManagerTexture::destroy_window(CL_GUITopLevelWindow *handle)
{
	CL_GUITopLevelWindowTexture *toplevel_window = impl->window_map[handle];
	impl->texture_group.remove(toplevel_window->subtexture);

	if (handle == impl->activated_window)	// Ensure that the destroyed window is not in focus
	{
		impl->activated_window = NULL;
	}

	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	impl->z_order.erase(std::find(impl->z_order.begin(), impl->z_order.end(), toplevel_window));
	delete toplevel_window;
	impl->window_map.erase(impl->window_map.find(handle));
}

void CL_GUIWindowManagerTexture::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	impl->window_map[handle]->enabled = enable;
}

bool CL_GUIWindowManagerTexture::has_focus(CL_GUITopLevelWindow *handle) const
{
	return impl->activated_window == handle;
}

void CL_GUIWindowManagerTexture::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate)
{
	CL_GUITopLevelWindowTexture *toplevel_window = impl->window_map[handle];
	toplevel_window->visible = visible;
	if (activate)
		impl->activated_window = handle;
}

void CL_GUIWindowManagerTexture::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	// to-do: convert client area rect to window area rect, if needed.

	CL_GraphicContext gc = impl->display_window.get_gc();
	CL_GUITopLevelWindowTexture *toplevel_window = impl->window_map[handle];

	if ((toplevel_window->geometry.get_width() == geometry.get_width()) &&
		(toplevel_window->geometry.get_height() == geometry.get_height()) )
	{
		// Do not need to rebuild texture if the width and height has not changed
		toplevel_window->geometry = geometry;
		return;
	}

	toplevel_window->geometry = geometry;

	impl->texture_group.remove(toplevel_window->subtexture);
	toplevel_window->subtexture = impl->texture_group.add(gc, geometry.get_size());

	if (!toplevel_window->depth_buffer.is_null())
	{
		toplevel_window->depth_buffer = CL_RenderBuffer(gc, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), CL_DEPTH_COMPONENT);
	}

	if (!toplevel_window->stencil_buffer.is_null())
	{
		toplevel_window->stencil_buffer = CL_RenderBuffer(gc, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), CL_STENCIL);
	}
	impl->clear_frame_buffer(toplevel_window);
}

CL_Rect CL_GUIWindowManagerTexture::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowTexture *>::const_iterator it = impl->window_map.find(handle);
	if (it != impl->window_map.end())
	{
		if (client_area)
			return CL_Rect(CL_Point(0, 0), it->second->geometry.get_size());
		else
			return it->second->geometry;
	}
	else
		return CL_Rect();
}

CL_Point CL_GUIWindowManagerTexture::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	CL_Rect geometry = get_geometry(handle, false);
	return CL_Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
}

CL_Point CL_GUIWindowManagerTexture::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	CL_Rect geometry = get_geometry(handle, false);
	return CL_Point(window_point.x + geometry.left, window_point.y + geometry.top);
}

CL_GraphicContext CL_GUIWindowManagerTexture::get_gc(CL_GUITopLevelWindow *handle)
{
	return impl->display_window.get_gc();
}

CL_InputContext CL_GUIWindowManagerTexture::get_ic(CL_GUITopLevelWindow *handle)
{
	return impl->display_window.get_ic();
}

CL_GraphicContext CL_GUIWindowManagerTexture::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext gc = impl->display_window.get_gc();

	CL_GUITopLevelWindowTexture *toplevel_window = impl->window_map[handle];

	impl->setup_frame_buffer(toplevel_window);

	if ( (impl->painting_set == false) || (impl->setup_painting_called == false) )
	{
		impl->painting_set = true;
		gc.set_frame_buffer(impl->frame_buffer);

		CL_BlendMode blendmode;
		blendmode.enable_blending(true);
		gc.set_blend_mode(blendmode);
	}

	gc.set_map_mode(cl_map_2d_upper_left);

	CL_Rect geometry = toplevel_window->subtexture.get_geometry();

	// TODO: Is gc.set_cliprect() required - Should the components clip instead?
	gc.set_cliprect(CL_Rect(update_region.left + geometry.left, update_region.top + geometry.top, update_region.get_size() ));

	// Translate model view matrix to the texture position
	gc.push_modelview();
	gc.mult_translate(geometry.left, geometry.top, 0);
	return gc;
}

void CL_GUIWindowManagerTexture::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext gc = impl->display_window.get_gc();
	gc.flush_batcher();

	CL_GUITopLevelWindowTexture *toplevel_window = impl->window_map[handle];

	// Restore the model view matrix
	gc.pop_modelview();

	// Only reset the frame buffer at this point when setup_painting() was not used
	if (!impl->setup_painting_called)
	{
		gc.reset_cliprect();
		gc.reset_blend_mode();
		gc.reset_frame_buffer();
	}
}

CL_Timer CL_GUIWindowManagerTexture::create_timer(CL_GUITopLevelWindow *handle)
{
	CL_Timer timer(impl->display_window);
	return timer;
}

void CL_GUIWindowManagerTexture::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
{
	if (state)
	{
		impl->display_window.capture_mouse(true);
		impl->capture_mouse_window = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (impl->capture_mouse_window == handle)
		{
			impl->display_window.capture_mouse(false);
			impl->capture_mouse_window = NULL;
		}
	}
}

CL_DisplayWindow CL_GUIWindowManagerTexture::get_display_window(CL_GUITopLevelWindow *handle) const
{
	return impl->display_window;
}

void CL_GUIWindowManagerTexture::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
}

void CL_GUIWindowManagerTexture::set_cursor(CL_GUITopLevelWindow *handle, CL_StandardCursor type)
{
}

void CL_GUIWindowManagerTexture::bring_to_front(CL_GUITopLevelWindow *handle)
{
	impl->bring_to_front(handle);
}

bool CL_GUIWindowManagerTexture::is_minimized(CL_GUITopLevelWindow *handle)
{
	return false;
}

bool CL_GUIWindowManagerTexture::is_maximized(CL_GUITopLevelWindow *handle)
{
	return false;
}

bool CL_GUIWindowManagerTexture::has_message()
{
	return CL_DisplayMessageQueue::wait(0);
}

void CL_GUIWindowManagerTexture::process_message()
{
	CL_DisplayWindowMessage message = CL_DisplayMessageQueue::get_message();
	CL_DisplayMessageQueue::dispatch_message(message);
}

void CL_GUIWindowManagerTexture::wait_for_message()
{
	CL_DisplayMessageQueue::wait();
}

void CL_GUIWindowManagerTexture::invalidate_rect(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	// Only invalidate when a valid rect was given
	if ((update_region.left >= update_region.right) || (update_region.top >= update_region.bottom))
	{
		return;
	}

	CL_GUITopLevelWindowTexture *wptr = impl->window_map[handle];
	if (wptr->dirty)
	{
		// If the update region is already set, then check to see if it is a seperate rect or if it
		// should be extended to include the new update region

		bool updated = false;
		std::vector<CL_Rect>::size_type size = wptr->update_region_list.size();
		for (int i = 0; i < size; i++)
		{
			if (wptr->update_region_list[i].is_overlapped(update_region))
			{
				// Found an overlapping section
				wptr->update_region_list[i].bounding_rect(update_region);
				updated = true;
				break;
			}
		}

		if (!updated)
		{
			wptr->update_region_list.push_back(update_region);
		}
		else
		{
			//TODO: Check for overlapping rects in the wptr->update_region_list
		}
	}
	else
	{
		wptr->dirty = true;
		wptr->update_region_list.clear();
		wptr->update_region_list.push_back(update_region);
	}
}

void CL_GUIWindowManagerTexture::update()
{
	impl->update_paint();
}

void CL_GUIWindowManagerTexture::set_texture_group(CL_TextureGroup &new_texture_group)
{
	if (new_texture_group.is_null())
	{
		throw CL_Exception(cl_text("Specified texture group is not valid"));
	}

	if (!impl->window_map.empty())
	{
		throw CL_Exception(cl_text("Cannot specify a new texture group when gui windows exist"));
	}

	impl->texture_group = new_texture_group;
}

void CL_GUIWindowManagerTexture::setup_painting()
{
	impl->setup_painting_called = true;
	impl->painting_set = false;
}

void CL_GUIWindowManagerTexture::complete_painting()
{
	impl->setup_painting_called = false;

	if ( impl->painting_set )
	{
		impl->painting_set = false;
		CL_GraphicContext gc = impl->display_window.get_gc();
		gc.reset_blend_mode();
		gc.reset_cliprect();
		gc.reset_frame_buffer();
	}

	if (!impl->func_repaint.is_null())
	{
		impl->func_repaint.invoke();
	}
	else
	{
		impl->default_repaint();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Implementation:
