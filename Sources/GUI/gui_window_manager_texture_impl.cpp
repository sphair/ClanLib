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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "gui_window_manager_texture_impl.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_state.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/image.h"
#include "gui_manager_impl.h"
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture_Impl Construction:

CL_GUIWindowManagerTexture_Impl::CL_GUIWindowManagerTexture_Impl(CL_DisplayWindow display_window)
: site(0), activated_window(0), capture_mouse_window(NULL), display_window(display_window), 
  setup_painting_called(false), painting_set(false), frame_buffer_initial_setup(false), frame_buffer_stencil_attached(false), frame_buffer_depth_attached(false)
{
//	slots.connect(display_window.sig_paint(), this, &CL_GUIWindowManagerTexture_Impl::on_displaywindow_paint);
	slots.connect(display_window.sig_window_close(), this, &CL_GUIWindowManagerTexture_Impl::on_displaywindow_window_close);

	CL_InputContext ic = display_window.get_ic();
	slots.connect(ic.get_mouse().sig_key_up(), this, &CL_GUIWindowManagerTexture_Impl::on_input);
	slots.connect(ic.get_mouse().sig_key_down(), this, &CL_GUIWindowManagerTexture_Impl::on_input_mouse_down);
	slots.connect(ic.get_mouse().sig_pointer_move(), this, &CL_GUIWindowManagerTexture_Impl::on_input_mouse_move);

	slots.connect(ic.get_keyboard().sig_key_up(), this, &CL_GUIWindowManagerTexture_Impl::on_input);
	slots.connect(ic.get_keyboard().sig_key_down(), this, &CL_GUIWindowManagerTexture_Impl::on_input);

	for (int tc = 0; tc < ic.get_tablet_count(); ++tc)
	{
		slots.connect(ic.get_tablet(tc).sig_axis_move(), this, &CL_GUIWindowManagerTexture_Impl::on_input_mouse_move);
		slots.connect(ic.get_tablet(tc).sig_key_down(), this, &CL_GUIWindowManagerTexture_Impl::on_input_mouse_down);
		slots.connect(ic.get_tablet(tc).sig_key_up(), this, &CL_GUIWindowManagerTexture_Impl::on_input);
	}

	CL_GraphicContext gc = display_window.get_gc();
	frame_buffer = CL_FrameBuffer(gc);
}

CL_GUIWindowManagerTexture_Impl::~CL_GUIWindowManagerTexture_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture_Impl Operations:

void CL_GUIWindowManagerTexture_Impl::update_paint()
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowTexture *>::iterator it;
	for (it = window_map.begin(); it != window_map.end(); ++it)
	{
		if (it->second->dirty)
		{
			it->second->dirty = false;
			std::vector<CL_Rect>::size_type size = it->second->update_region_list.size();
			for (int i = 0; i < size; i++)
			{
				site->func_paint->invoke(it->first, it->second->update_region_list[i]);
			}
		}
	}
}

void CL_GUIWindowManagerTexture_Impl::default_repaint()
{
	CL_GraphicContext gc = display_window.get_gc();
	gc.clear(CL_Colorf::black); // Do not change this to CL_Colorf::transparent. It has unintended side effects on windows with an alpha channel.

	std::vector<CL_GUITopLevelWindowTexture *>::size_type index, size;
	size = z_order.size();
	for (index = size; index > 0; index--)
	{
		CL_GUITopLevelWindowTexture *toplevel_window = z_order[index-1];
		if (toplevel_window->visible)
		{
			CL_Image image(gc, toplevel_window->subtexture);
			image.draw(gc, toplevel_window->geometry.left,  toplevel_window->geometry.top);
		}
	}
	display_window.flip();
}

void CL_GUIWindowManagerTexture_Impl::on_displaywindow_window_close()
{
	if (activated_window == 0)
		return;

	// this isn't really right, but it will allow us to close our test GUI applications.
	site->func_close->invoke(activated_window);
}

void CL_GUIWindowManagerTexture_Impl::on_input(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	if (activated_window == 0)
		return;

	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event, new_input_state);

	invoke_input_received(activated_window, new_input_event, new_input_state);
}

void CL_GUIWindowManagerTexture_Impl::on_input_mouse_move(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
				func_input_intercept.invoke(new_input_event, new_input_state);

	CL_GUITopLevelWindow *toplevel_window;
	if (capture_mouse_window)
	{
		//(see win32 mouse capture for behaviour http://msdn.microsoft.com/en-us/library/ms646262.aspx )
		toplevel_window = capture_mouse_window;
	}
	else
	{
		toplevel_window = get_window_at_point(new_input_event.mouse_pos);
	}

	if (toplevel_window)
		invoke_input_received(toplevel_window, new_input_event, new_input_state);
}

void CL_GUIWindowManagerTexture_Impl::on_input_mouse_down(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event, new_input_state);

	CL_GUITopLevelWindow *toplevel_window;
	if (capture_mouse_window)
	{
		toplevel_window = capture_mouse_window;
	}
	else
	{
		toplevel_window = get_window_at_point(new_input_event.mouse_pos);
	}

	if (toplevel_window)
	{
		bool focus_change = false;
		if (activated_window)
		{
			if (toplevel_window != activated_window)
			{
				focus_change = true;
			}
		}
		else
		{
			focus_change = true;
		}
		if (focus_change)
		{
			// Change of focus required
			if (activated_window)
			{
				if (activated_window->focused_component)
				{
					activated_window->focused_component->set_focus(false);
				}
			}

			activated_window = toplevel_window;
		}

		invoke_input_received(toplevel_window, new_input_event, new_input_state);
	}
}

void CL_GUIWindowManagerTexture_Impl::clear_frame_buffer(CL_GUITopLevelWindowTexture *toplevel_window )
{
	setup_frame_buffer(toplevel_window);
	CL_GraphicContext gc = display_window.get_gc();
	gc.set_frame_buffer(frame_buffer);
	CL_Draw::fill(gc, toplevel_window->subtexture.get_geometry(), CL_Colorf::transparent);
	gc.reset_frame_buffer();
}

void CL_GUIWindowManagerTexture_Impl::setup_frame_buffer(CL_GUITopLevelWindowTexture *toplevel_window )
{
	CL_GraphicContext gc = display_window.get_gc();

	CL_Texture texture = toplevel_window->subtexture.get_texture();

	// Only attach the color buffer if it has changed
	if ( (frame_buffer_texture_attached.is_null()) || (texture != frame_buffer_texture_attached) )
	{
		frame_buffer.attach_color_buffer(0, texture);
		frame_buffer_texture_attached = texture;
	}

	if (toplevel_window->depth_buffer.is_null())
	{
		if (frame_buffer_depth_attached)
		{
			frame_buffer.detach_depth_buffer();
			frame_buffer_depth_attached = false;
		}
	}
	else
	{
		frame_buffer_depth_attached = true;
		frame_buffer.attach_depth_buffer(toplevel_window->depth_buffer);
	}

	if (toplevel_window->stencil_buffer.is_null())
	{
		if (frame_buffer_stencil_attached)
		{
			frame_buffer.detach_stencil_buffer();
			frame_buffer_stencil_attached = false;
		}
	}
	else
	{
		frame_buffer.attach_stencil_buffer(toplevel_window->stencil_buffer);
		frame_buffer_stencil_attached = true;
	}
}

void CL_GUIWindowManagerTexture_Impl::bring_to_front(CL_GUITopLevelWindow *handle)
{
	CL_GUITopLevelWindowTexture *texture = window_map[handle];
	if(texture)
	{
		z_order.erase(std::find(z_order.begin(), z_order.end(), texture));
		z_order.insert(z_order.begin(), texture);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture_Impl Implementation:

CL_GUITopLevelWindow *CL_GUIWindowManagerTexture_Impl::get_window_at_point(const CL_Point &point)
{
	std::vector<CL_GUITopLevelWindowTexture *>::size_type index, size;
	size = z_order.size();
	for (index = 0; index < size; index++)
	{
		CL_GUITopLevelWindowTexture *toplevel_window_texture = z_order[index];
		if (toplevel_window_texture->visible)
		{
			if (toplevel_window_texture->geometry.contains(point))
			{
				return toplevel_window_texture->window;
			}
		}
	}
	return 0;
}

void CL_GUIWindowManagerTexture_Impl::invoke_input_received(CL_GUITopLevelWindow *window, const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	CL_GUITopLevelWindowTexture *texture_window = window_map[window];
	CL_InputEvent inp_event = input_event;
	inp_event.mouse_pos.x -= texture_window->geometry.left;
	inp_event.mouse_pos.y -= texture_window->geometry.top;
	site->func_input_received->invoke(window, inp_event, input_state);
}
