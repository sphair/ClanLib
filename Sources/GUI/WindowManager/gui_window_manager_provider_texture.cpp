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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "GUI/precomp.h"
#include "gui_window_manager_provider_texture.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_window_manager_texture_window.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/image.h"
#include "../gui_manager_impl.h"
#include <algorithm>
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Texture Construction:

GUIWindowManagerProvider_Texture::GUIWindowManagerProvider_Texture(DisplayWindow &display_window)
	: site(0), activated_window(0), capture_mouse_window(NULL), display_window(display_window), canvas_window(display_window),
  frame_buffer_initial_setup(false), frame_buffer_stencil_attached(false), frame_buffer_depth_attached(false)
{
	cc.connect(display_window.sig_window_close(), Callback<void()>(this, &GUIWindowManagerProvider_Texture::on_displaywindow_window_close));

	InputContext ic = display_window.get_ic();
    cc.connect(ic.get_mouse().sig_key_up(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_up));
    cc.connect(ic.get_mouse().sig_key_down(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_down));
    cc.connect(ic.get_mouse().sig_key_dblclk(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_down));
    cc.connect(ic.get_mouse().sig_pointer_move(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_move));

    cc.connect(ic.get_keyboard().sig_key_up(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input));
    cc.connect(ic.get_keyboard().sig_key_down(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input));

	for (int tc = 0; tc < ic.get_tablet_count(); ++tc)
	{
        cc.connect(ic.get_tablet(tc).sig_axis_move(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_move));
        cc.connect(ic.get_tablet(tc).sig_key_down(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_down));
        cc.connect(ic.get_tablet(tc).sig_key_dblclk(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input_mouse_down));
        cc.connect(ic.get_tablet(tc).sig_key_up(), Callback<void(const InputEvent&)>(this, &GUIWindowManagerProvider_Texture::on_input));
	}

	frame_buffer = FrameBuffer(canvas_window);

	BlendStateDescription blend_desc;
	default_blend = BlendState(canvas_window, blend_desc);

	blend_desc.enable_blending(false);
	opaque_blend = BlendState(canvas_window, blend_desc);

	blend_desc.enable_blending(true);
	blend_desc.set_blend_function(blend_one, blend_one_minus_src_alpha, blend_one, blend_one_minus_src_alpha);
	premultiplied_alpha_blend = BlendState(canvas_window, blend_desc);
}

GUIWindowManagerProvider_Texture::~GUIWindowManagerProvider_Texture()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Texture Attributes:

GUIWindowManager::WindowManagerType GUIWindowManagerProvider_Texture::get_window_manager_type() const
{
	return GUIWindowManager::cl_wm_type_texture;
}

std::vector<GUIWindowManagerTextureWindow> GUIWindowManagerProvider_Texture::get_windows(bool only_visible) const
{
	std::vector<GUIWindowManagerTextureWindow> windows;

	get_all_windows_zorder(only_visible, windows, root_window_z_order);
	return windows;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Texture Operations:

void GUIWindowManagerProvider_Texture::update_paint()
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowTexture *>::iterator it;
	for (it = window_map.begin(); it != window_map.end(); ++it)
	{
		if (it->second->dirty)
		{
			it->second->dirty = false;
			// Make a copy, since on_render() may call request_repaint()
			std::vector<Rect> update_region_list = it->second->update_region_list;
			std::vector<Rect>::size_type size = update_region_list.size();
			for (int i = 0; i < size; i++)
			{
				site->func_paint->invoke(it->first, update_region_list[i]);
			}
		}
	}
}

void GUIWindowManagerProvider_Texture::draw_windows(Canvas &canvas)
{
	canvas.set_blend_state(premultiplied_alpha_blend);
	draw_all_windows(canvas, root_window_z_order);
	canvas.reset_blend_state();
}

void GUIWindowManagerProvider_Texture::on_displaywindow_window_close()
{
	if (activated_window == 0)
		return;

	// this isn't really right, but it will allow us to close our test GUI applications.
	site->func_close->invoke(activated_window);
}

void GUIWindowManagerProvider_Texture::on_input(const InputEvent &input_event)
{
	if (activated_window == 0)
		return;

	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	invoke_input_received(activated_window, new_input_event);
}

void GUIWindowManagerProvider_Texture::on_input_mouse_move(const InputEvent &input_event)
{
	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
				func_input_intercept.invoke(new_input_event);

	bool capture_mouse_flag = false;
	if (capture_mouse_window)
	{
		// Only capture when left mouse is pressed
		//(see win32 mouse capture for behaviour http://msdn.microsoft.com/en-us/library/ms646262.aspx )
// ** Disabled - It seems multiple windows in the same app act differently to microsoft docs **
//		if (get_display_window(capture_mouse_window).get_ic().get_mouse().get_keycode(mouse_left))
		{
			capture_mouse_flag = true;
		}
	}


	GUITopLevelWindow *toplevel_window;
	if (capture_mouse_flag)
	{
		// From MSDN:
		//   "Only the foreground window can capture the mouse.
		//    When a background window attempts to do so, the window receives messages
		//    only for mouse events that occur when the cursor hot spot is within
		//    the visible portion of the window."
// ** Disabled - It seems multiple windows in the same app act differently to microsoft docs **
//		toplevel_window = get_captured_window_at_point(new_input_event.mouse_pos, capture_mouse_window);
		toplevel_window = capture_mouse_window;
	}
	else
	{
		toplevel_window = get_window_at_point(new_input_event.mouse_pos);
	}

	if (toplevel_window)
		invoke_input_received(toplevel_window, new_input_event);
}

void GUIWindowManagerProvider_Texture::on_input_mouse_up(const InputEvent &input_event)
{
	// It seems multiple windows in the same app act differently for window SetCapture()
	if (!capture_mouse_window)
	{
		// Process MouseUp as normal input event when capture mouse is not used
		on_input(input_event);
		return;
	}

	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	invoke_input_received(capture_mouse_window, new_input_event);
}

void GUIWindowManagerProvider_Texture::on_input_mouse_down(const InputEvent &input_event)
{
	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	// It seems multiple windows in the same app act differently for window SetCapture()
	if (capture_mouse_window)
	{
		invoke_input_received(capture_mouse_window, new_input_event);
		return;
	}

	GUITopLevelWindow *toplevel_window;
	toplevel_window = get_window_at_point(new_input_event.mouse_pos);

	if (toplevel_window)
		bring_to_front(toplevel_window);

	// Send focus lost events
	if (toplevel_window)
	{
		if (toplevel_window != activated_window)
		{
			// Change of focus required
			// From: http://msdn.microsoft.com/en-us/library/ms646274(VS.85).aspx - deactivation occurs before activation

			if (activated_window)
			{
				GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);

				site->func_focus_lost->invoke(activated_window);

				if (toplevel_window_alive.is_null())
					toplevel_window = get_window_at_point(new_input_event.mouse_pos);
			}
		}
	}

	// Send focus gained events
	if (toplevel_window)
	{
		if (toplevel_window != activated_window)
		{
			activated_window = toplevel_window;
			GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);
			site->func_focus_gained->invoke(activated_window);
			if (toplevel_window_alive.is_null())
				toplevel_window = get_window_at_point(new_input_event.mouse_pos);
		}
	}

	// Send mouse click event to toplevel window
	if (toplevel_window)
	{
		invoke_input_received(toplevel_window, new_input_event);
	}

}

void GUIWindowManagerProvider_Texture::clear_frame_buffer(GUITopLevelWindowTexture *toplevel_window )
{
	setup_frame_buffer(toplevel_window);
	Canvas canvas(canvas_window, frame_buffer);
	canvas.set_blend_state(opaque_blend);
	canvas.fill_rect(toplevel_window->subtexture.get_geometry(), Colorf::transparent);
}

void GUIWindowManagerProvider_Texture::setup_frame_buffer(GUITopLevelWindowTexture *toplevel_window )
{
	Texture2D texture = toplevel_window->subtexture.get_texture();

	// Only attach the color buffer if it has changed
	if ( (frame_buffer_texture_attached.is_null()) || (texture != frame_buffer_texture_attached) )
	{
		frame_buffer.attach_color(0, texture);
		frame_buffer_texture_attached = texture;
	}

	if (toplevel_window->depth_buffer.is_null())
	{
		if (frame_buffer_depth_attached)
		{
			frame_buffer.detach_depth();
			frame_buffer_depth_attached = false;
		}
	}
	else
	{
		frame_buffer_depth_attached = true;
		frame_buffer.attach_depth(toplevel_window->depth_buffer);
	}

	if (toplevel_window->stencil_buffer.is_null())
	{
		if (frame_buffer_stencil_attached)
		{
			frame_buffer.detach_stencil();
			frame_buffer_stencil_attached = false;
		}
	}
	else
	{
		frame_buffer.attach_stencil(toplevel_window->stencil_buffer);
		frame_buffer_stencil_attached = true;
	}
}

void GUIWindowManagerProvider_Texture::bring_to_front(GUITopLevelWindow *handle)
{
	GUITopLevelWindowTexture *texture = get_window_texture(handle);
	if(texture)
	{
		// Get root texture
		GUITopLevelWindowTexture *root_texture = texture;
		while (root_texture->owner_window)
		{
			root_texture = root_texture->owner_window;
		}

		// Check root window order
		if (root_window_z_order[0] != root_texture)
		{
			std::vector<GUITopLevelWindowTexture *>::iterator it;
			it = std::find(root_window_z_order.begin(), root_window_z_order.end(), root_texture);
			root_window_z_order.erase(it);
			root_window_z_order.insert(root_window_z_order.begin(), root_texture);
		}

		// Check owner window order
		if (texture->owner_window)
		{
			std::vector<GUITopLevelWindowTexture *> &z_order = texture->owner_window->child_windows_zorder;
			if (z_order[0] != texture)
			{
				std::vector<GUITopLevelWindowTexture *>::iterator it;
				it = std::find(z_order.begin(), z_order.end(), texture);
				z_order.erase(it);
				z_order.insert(z_order.begin(), texture);
			}
		}
	}
}

void GUIWindowManagerProvider_Texture::set_site(GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void GUIWindowManagerProvider_Texture::create_window(
	GUITopLevelWindow *handle,
	GUITopLevelWindow *owner,
	GUIComponent *component,
	GUITopLevelDescription description)
{
	// Create the texture group, if it has not already been set
	if (texture_group.is_null())
	{
		//Set the texture group size to 0. This will force the texture group to use a single texture
		//for each GUI Window. This was done this way for 3 reasons.
		//1) This class cannot predict the optimum texture size - It may be too small or too large (wasting GPU RAM)
		//2) It ensures the boundary of the window equals the boundary of the texture, so the OpenGL pixel blending on scaled/rotated images work.
		//3) Depth buffer and Stencil buffer size must match the Texture2D size.
		//If you want multiple windows in a single texture - Use set_texture_group()
		Size initial_texture_size = Size(0, 0);
		texture_group = TextureGroup(initial_texture_size);
	}

	//Note: destroy_window() deletes this window
	GUITopLevelWindowTexture *toplevel_window = new GUITopLevelWindowTexture(handle);
	Rect geometry = description.get_position();
	toplevel_window->geometry = geometry;
	toplevel_window->enabled = true;
	toplevel_window->visible = true;
	toplevel_window->subtexture = texture_group.add(canvas_window, geometry.get_size());

	Texture2D frame_texture = toplevel_window->subtexture.get_texture();
	if (description.get_depth_size() > 0)
	{
		toplevel_window->depth_buffer = RenderBuffer(canvas_window, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), tf_depth_component24);
	}
	if (description.get_stencil_size() > 0)
	{
		toplevel_window->stencil_buffer = RenderBuffer(canvas_window, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), tf_stencil_index8);
	}

	window_map[handle] = toplevel_window;

	if (owner)
	{
		// Attach to owner
		toplevel_window->owner_window = get_window_texture(owner);
		toplevel_window->owner_window->child_windows_zorder.insert(toplevel_window->owner_window->child_windows_zorder.begin(), toplevel_window);
	}
	else
	{
		// Attach to root
		root_window_z_order.insert(root_window_z_order.begin(), toplevel_window);
	}

	activated_window = handle;

	clear_frame_buffer(toplevel_window);

	if (!frame_buffer_initial_setup)
	{
		frame_buffer_initial_setup = true;
		if (!func_setup_framebuffer.is_null())
			func_setup_framebuffer.invoke(frame_buffer);
	}

	// to-to: report focus change.
}

void GUIWindowManagerProvider_Texture::destroy_window(GUITopLevelWindow *handle)
{
	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	GUITopLevelWindowTexture *toplevel_window = get_window_texture(handle);

	if (toplevel_window->owner_window)
	{
		// Detach from owner
		toplevel_window->owner_window->child_windows_zorder.erase(std::find(toplevel_window->owner_window->child_windows_zorder.begin(), toplevel_window->owner_window->child_windows_zorder.end(), toplevel_window));
	}
	else
	{
		// Detach from root
		root_window_z_order.erase(std::find(root_window_z_order.begin(), root_window_z_order.end(), toplevel_window));
	}

	texture_group.remove(toplevel_window->subtexture);

	if (handle == activated_window)	// Ensure that the destroyed window is not in focus
	{
		activated_window = NULL;
	}

	delete toplevel_window;
	window_map.erase(window_map.find(handle));
}

void GUIWindowManagerProvider_Texture::enable_window(GUITopLevelWindow *handle, bool enable)
{
	GUITopLevelWindowTexture *window_texture = get_window_texture(handle);
	window_texture->enabled = enable;
}

bool GUIWindowManagerProvider_Texture::has_focus(GUITopLevelWindow *handle) const
{
	return activated_window == handle;
}

void GUIWindowManagerProvider_Texture::set_visible(GUITopLevelWindow *handle, bool visible, bool activate)
{
	GUITopLevelWindowTexture *toplevel_window = get_window_texture(handle);
	toplevel_window->visible = visible;
	if (activate)
		activated_window = handle;
}

void GUIWindowManagerProvider_Texture::set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area)
{
	// to-do: convert client area rect to window area rect, if needed.

	GUITopLevelWindowTexture *toplevel_window = get_window_texture(handle);

	if ((toplevel_window->geometry.get_width() == geometry.get_width()) &&
		(toplevel_window->geometry.get_height() == geometry.get_height()) )
	{
		// Do not need to rebuild texture if the width and height has not changed
		toplevel_window->geometry = geometry;
		return;
	}

	toplevel_window->geometry = geometry;

	texture_group.remove(toplevel_window->subtexture);
	toplevel_window->subtexture = texture_group.add(canvas_window, geometry.get_size());

	if (!toplevel_window->depth_buffer.is_null())
	{
		toplevel_window->depth_buffer = RenderBuffer(canvas_window, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), tf_depth_component24);
	}
	if (!toplevel_window->stencil_buffer.is_null())
	{
		toplevel_window->stencil_buffer = RenderBuffer(canvas_window, toplevel_window->geometry.get_width(), toplevel_window->geometry.get_height(), tf_stencil_index8);
	}

	clear_frame_buffer(toplevel_window);
}

Rect GUIWindowManagerProvider_Texture::get_geometry(GUITopLevelWindow *handle, bool client_area) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowTexture *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		return it->second->geometry;
	}
	else
	{
		return Rect();
	}
}

Point GUIWindowManagerProvider_Texture::screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const
{
	Rect geometry = get_geometry(handle, false);
	return Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
}

Point GUIWindowManagerProvider_Texture::window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const
{
	Rect geometry = get_geometry(handle, false);
	return Point(window_point.x + geometry.left, window_point.y + geometry.top);
}

Canvas& GUIWindowManagerProvider_Texture::get_canvas(GUITopLevelWindow *handle)
{
	return canvas_window;
}

InputContext GUIWindowManagerProvider_Texture::get_ic(GUITopLevelWindow *handle) const
{
	return display_window.get_ic();
}

Canvas GUIWindowManagerProvider_Texture::begin_paint(GUITopLevelWindow *handle, const Rect &update_region)
{
	GUITopLevelWindowTexture *toplevel_window = get_window_texture(handle);

	setup_frame_buffer(toplevel_window);
	Canvas canvas(canvas_window, frame_buffer);
	Rect subtexture_geometry = toplevel_window->subtexture.get_geometry();

	Rect clip_rect = update_region;
	clip_rect.translate(subtexture_geometry.left, subtexture_geometry.top);
	clip_rect.overlap(subtexture_geometry);

	// TODO: Draw::fill() is slightly slower than canvas.clear() - but it works with a cliprect on the D3D target
	canvas.set_blend_state(opaque_blend);
	canvas.fill_rect(clip_rect, Colorf::transparent);

	canvas.set_blend_state(default_blend);
	canvas.set_cliprect(clip_rect);
	//canvas.clear(Colorf::transparent);

	// Translate model view matrix to the texture position
	canvas.mult_translate(subtexture_geometry.left, subtexture_geometry.top, 0);
	return canvas;
}

void GUIWindowManagerProvider_Texture::end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region)
{
}

void GUIWindowManagerProvider_Texture::capture_mouse(GUITopLevelWindow *handle, bool state)
{
	if (state)
	{
		display_window.capture_mouse(true);
		capture_mouse_window = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (capture_mouse_window == handle)
		{
			display_window.capture_mouse(false);
			capture_mouse_window = NULL;
		}
	}
}

DisplayWindow GUIWindowManagerProvider_Texture::get_display_window(GUITopLevelWindow *handle) const
{
	return display_window;
}

void GUIWindowManagerProvider_Texture::set_cursor(GUITopLevelWindow *handle, const Cursor &cursor)
{
}

void GUIWindowManagerProvider_Texture::set_cursor(GUITopLevelWindow *handle, StandardCursor type)
{
}

bool GUIWindowManagerProvider_Texture::is_minimized(GUITopLevelWindow *handle) const
{
	return false;
}

bool GUIWindowManagerProvider_Texture::is_maximized(GUITopLevelWindow *handle) const
{
	return false;
}

void GUIWindowManagerProvider_Texture::request_repaint(GUITopLevelWindow *handle, const Rect &update_region)
{
	// Only invalidate when a valid rect was given
	if ((update_region.left >= update_region.right) || (update_region.top >= update_region.bottom))
	{
		return;
	}

	GUITopLevelWindowTexture *wptr = get_window_texture(handle);
	if (wptr->dirty)
	{
		// If the update region is already set, then check to see if it is a seperate rect or if it
		// should be extended to include the new update region

		bool updated = false;
		std::vector<Rect>::size_type size = wptr->update_region_list.size();
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

void GUIWindowManagerProvider_Texture::update()
{
	update_paint();

	if (!func_repaint.is_null())
	{
		func_repaint.invoke();
	}
	else
	{
		canvas_window.clear(Colorf::black); // Do not change this to Colorf::transparent. It has unintended side effects on windows with an alpha channel.
		draw_windows(canvas_window);
		canvas_window.flush();
		display_window.flip();
	}
}

void GUIWindowManagerProvider_Texture::set_texture_group(TextureGroup &new_texture_group)
{
	if (new_texture_group.is_null())
	{
		throw Exception("Specified texture group is not valid");
	}

	if (!window_map.empty())
	{
		throw Exception("Cannot specify a new texture group when gui windows exist");
	}

	texture_group = new_texture_group;
}

void GUIWindowManagerProvider_Texture::process()
{
	invalidate_constant_repaint_components();
	update_paint();
}

bool GUIWindowManagerProvider_Texture::is_constant_repaint_enabled(GUIComponent *component) const
{
	if (component->get_constant_repaint())
	{
		return true;
	}
	else
	{
		std::vector<GUIComponent *> components = component->get_child_components();
		for (std::vector<GUIComponent *>::size_type i = 0; i < components.size(); i++)
		{
			if (is_constant_repaint_enabled(components[i]))
				return true;
		}
		return false;
	}
}

void GUIWindowManagerProvider_Texture::invalidate_constant_repaint_components()
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowTexture *>::iterator it;
	for (it = window_map.begin(); it != window_map.end(); ++it)
	{
		GUIComponent *component = it->first->component;
		if (component)
		{
			if (is_constant_repaint_enabled(component))
			{
				component->request_repaint();
			}
		}
	}
}

void GUIWindowManagerProvider_Texture::set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	GUITopLevelWindowTexture *toplevel_window = get_window_texture(handle);
	Rect subtexture_geometry = toplevel_window->subtexture.get_geometry();
	Rect cliprect = rect;
	cliprect.translate(subtexture_geometry.left, subtexture_geometry.top);
	cliprect.overlap(subtexture_geometry);
	canvas.set_cliprect(cliprect);
}

void GUIWindowManagerProvider_Texture::reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.pop_cliprect();
}

void GUIWindowManagerProvider_Texture::push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	GUITopLevelWindowTexture *toplevel_window = get_window_texture(handle);
	Rect subtexture_geometry = toplevel_window->subtexture.get_geometry();
	Rect cliprect = rect;
	cliprect.translate(subtexture_geometry.left, subtexture_geometry.top);
	cliprect.overlap(subtexture_geometry);
	canvas.push_cliprect(cliprect);
}

void GUIWindowManagerProvider_Texture::pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.pop_cliprect();
}


/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Texture Implementation:

GUITopLevelWindow *GUIWindowManagerProvider_Texture::get_window_at_point(const Point &point)
{
	return get_window_at_point(point, root_window_z_order);
}

GUITopLevelWindow *GUIWindowManagerProvider_Texture::get_window_at_point(const Point &point, const std::vector<GUITopLevelWindowTexture *> &z_order)
{
	std::vector<GUITopLevelWindowTexture *>::size_type index, size;
	size = z_order.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindowTexture *toplevel_window_texture = z_order[index];
		if (toplevel_window_texture->visible)
		{
			// Check children first
			GUITopLevelWindow *child_window = get_window_at_point(point, toplevel_window_texture->child_windows_zorder);
			if (child_window)
				return child_window;

			if (toplevel_window_texture->geometry.contains(point))
			{
				return toplevel_window_texture->window;
			}
		}
	}
	return 0;
}

void GUIWindowManagerProvider_Texture::invoke_input_received(GUITopLevelWindow *window, const InputEvent &input_event)
{
	GUITopLevelWindowTexture *texture_window = get_window_texture(window);
	InputEvent inp_event = input_event;
	inp_event.mouse_pos.x -= texture_window->geometry.left;
	inp_event.mouse_pos.y -= texture_window->geometry.top;
	site->func_input_received->invoke(window, inp_event);
}

GUITopLevelWindowTexture *GUIWindowManagerProvider_Texture::get_window_texture(GUITopLevelWindow *handle)
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowTexture *>::iterator it;
	it = window_map.find(handle);
	if (it == window_map.end())
		throw Exception("Invalid GUI Top Level Window requested");
	return it->second;
}

// Used by get_windows() to get child windows z order
void GUIWindowManagerProvider_Texture::get_all_windows_zorder(bool only_visible, std::vector<GUIWindowManagerTextureWindow> &windows_dest_list, const std::vector<GUITopLevelWindowTexture *> &z_order) const
{
	std::vector<GUITopLevelWindowTexture *>::size_type index, size;
	size = z_order.size();
	for (index = size; index > 0; index--)
	{
		GUITopLevelWindowTexture *toplevel_window = z_order[index-1];
		if ( (only_visible == false) || (toplevel_window->visible) )
		{
			windows_dest_list.push_back(GUIWindowManagerTextureWindow(toplevel_window->window, toplevel_window->subtexture, toplevel_window->geometry));

			// Get all children
			get_all_windows_zorder(only_visible, windows_dest_list, toplevel_window->child_windows_zorder);
		}
	}
}


void GUIWindowManagerProvider_Texture::draw_all_windows(Canvas &canvas, std::vector<GUITopLevelWindowTexture *> &z_order)
{
	std::vector<GUITopLevelWindowTexture *>::size_type index, size;
	size = z_order.size();
	for (index = size; index > 0; index--)
	{
		GUITopLevelWindowTexture *toplevel_window = z_order[index-1];
		if (toplevel_window->visible)
		{
			Image image(toplevel_window->subtexture);
			image.draw(canvas, toplevel_window->geometry.left,  toplevel_window->geometry.top);

			// Draw all children
			draw_all_windows(canvas, toplevel_window->child_windows_zorder);
		}
	}
}

}
