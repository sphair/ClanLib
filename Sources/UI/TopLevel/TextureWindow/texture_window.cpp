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
*/

#include "UI/precomp.h"
#include "API/UI/TopLevel/texture_window.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "UI/View/positioned_layout.h"
#include "texture_window_impl.h"

namespace clan
{
	TextureWindow::TextureWindow(Canvas &canvas) : impl(std::make_shared<TextureWindow_Impl>(this, canvas))
	{
	}

	void TextureWindow::update()
	{
		impl->update();
	}

	void TextureWindow::set_background_color(const Colorf &background_color)
	{
		impl->background_color = background_color;
	}

	void TextureWindow::set_clear_background(bool enable)
	{
		impl->clear_background_enable = enable;
	}

	Canvas TextureWindow::get_canvas() const
	{
		return impl->canvas;
	}

	void TextureWindow::set_needs_render()
	{
		impl->needs_render = true;
	}

	void TextureWindow::present_popup(const Pointf &pos, const std::shared_ptr<View> &popup)
	{
	}

	void TextureWindow::dismiss_popup()
	{
	}

	void TextureWindow::present_modal(const std::string &title, const std::shared_ptr<View> &modal)
	{
	}

	void TextureWindow::dismiss_modal()
	{
	}

	Pointf TextureWindow::client_to_screen_pos(const Pointf &pos)
	{
		return pos;
	}

	Pointf TextureWindow::screen_to_client_pos(const Pointf &pos)
	{
		return pos;
	}

	void TextureWindow::set_viewport(const Rectf &rect)
	{
		if (rect != impl->canvas_rect)
		{
			impl->canvas_rect = rect;
			set_needs_render();
		}
	}

	void TextureWindow::set_cursor_window(const DisplayWindow &new_cursor_window)
	{
		impl->cursor_window = new_cursor_window;
	}

	void TextureWindow::set_event_window(const DisplayWindow &event_window, const Mat4f &transform_mouse_matrix)
	{
		impl->set_event_window(event_window, transform_mouse_matrix);
	}

	Rectf TextureWindow::get_viewport() const
	{
		return impl->canvas_rect;
	}

	void TextureWindow::on_window_close()
	{
		impl->on_window_close();
	}

	void TextureWindow::on_lost_focus()
	{
		impl->on_lost_focus();
	}

	void TextureWindow::on_got_focus()
	{
		impl->on_got_focus();
	}

	void TextureWindow::on_key_down(const clan::InputEvent &e)
	{
		impl->on_key_down(e);
	}

	void TextureWindow::on_key_up(const clan::InputEvent &e)
	{
		impl->on_key_up(e);
	}

	void TextureWindow::on_mouse_down(const clan::InputEvent &e)
	{
		impl->on_mouse_down(e);
	}

	void TextureWindow::on_mouse_dblclk(const clan::InputEvent &e)
	{
		impl->on_mouse_dblclk(e);
	}

	void TextureWindow::on_mouse_up(const clan::InputEvent &e)
	{
		impl->on_mouse_up(e);
	}

	void TextureWindow::on_mouse_move(const clan::InputEvent &e)
	{
		impl->on_mouse_move(e);
	}
}
