/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "display_window_impl.h"
#include "../Render/graphic_context_impl.h"
#include "../setup_display.h"
#include "API/Display/Window/input_device.h"

namespace clan
{
	DisplayWindow::DisplayWindow()
	{
	}

	DisplayWindow::DisplayWindow(
		const std::string &title,
		float width,
		float height,
		bool start_fullscreen,
		bool allow_resize,
		int flipping_buffers)
	{
		DisplayWindowDescription description;
		description.set_title(title);
		description.set_size(Size(width, height), false);
		description.set_fullscreen(start_fullscreen);

		if (start_fullscreen)
		{
			description.show_caption(false);
		}

		description.set_allow_resize(allow_resize);
		description.set_flipping_buffers(flipping_buffers);

		*this = DisplayWindow(description);
	}

	DisplayWindow::DisplayWindow(const DisplayWindowDescription &description)
	{
		SetupDisplay::start();

		auto target = DisplayTarget::get_current_target();
		if (!target)
			throw Exception("No display target set");

		impl = std::shared_ptr<DisplayWindow_Impl>(new DisplayWindow_Impl);
		impl->provider = target->alloc_display_window();
		impl->provider->create(&impl->site, description);
	}

	DisplayWindow::DisplayWindow(DisplayWindowProvider *provider)
		: impl(std::make_shared<DisplayWindow_Impl>())
	{
		impl->provider = provider;
	}

	DisplayWindow::~DisplayWindow()
	{
	}

	DisplayWindowHandle DisplayWindow::get_handle() const
	{
		return impl->provider->get_handle();
	}

	Rectf DisplayWindow::get_geometry() const
	{
		Rect geometryi = impl->provider->get_geometry();
		Rectf geometry;
		geometry.left = geometryi.left / impl->provider->get_pixel_ratio();
		geometry.top = geometryi.top / impl->provider->get_pixel_ratio();
		geometry.right = geometryi.right / impl->provider->get_pixel_ratio();
		geometry.bottom = geometryi.bottom / impl->provider->get_pixel_ratio();
		return geometry;
	}

	Rectf DisplayWindow::get_viewport() const
	{
		Rect viewporti = impl->provider->get_viewport();
		Rectf viewport;
		viewport.left = viewporti.left / impl->provider->get_pixel_ratio();
		viewport.top = viewporti.top / impl->provider->get_pixel_ratio();
		viewport.right = viewporti.right / impl->provider->get_pixel_ratio();
		viewport.bottom = viewporti.bottom / impl->provider->get_pixel_ratio();
		return viewport;
	}

	bool DisplayWindow::has_focus() const
	{
		return impl->provider->has_focus();
	}

	GraphicContext& DisplayWindow::get_gc() const
	{
		throw_if_null();
		return impl->provider->get_gc();
	}

	InputDevice &DisplayWindow::get_keyboard()
	{
		throw_if_null();
		return impl->provider->get_keyboard();
	}

	InputDevice &DisplayWindow::get_mouse()
	{
		throw_if_null();
		return impl->provider->get_mouse();
	}


	std::vector<InputDevice> &DisplayWindow::get_game_controllers()
	{
		throw_if_null();
		return impl->provider->get_game_controllers();
	}

	InputDevice &DisplayWindow::get_input_device(const std::string &device_name)
	{
		throw_if_null();

		if (device_name == get_keyboard().get_device_name())
			return get_keyboard();
		else if (device_name == get_mouse().get_device_name())
			return get_mouse();

		auto &game = get_game_controllers();
		for (auto &elem : game)
		{
			if (elem.get_device_name() == device_name)
				return elem;
		}

		static InputDevice null_device;
		return null_device;
	}

	Signal<void()> &DisplayWindow::sig_lost_focus()
	{
		return impl->site.sig_lost_focus;
	}

	Signal<void()> &DisplayWindow::sig_got_focus()
	{
		return impl->site.sig_got_focus;
	}

	Signal<void(float, float)> &DisplayWindow::sig_resize()
	{
		return impl->site.sig_resize;
	}

	Signal<void()> &DisplayWindow::sig_paint()
	{
		return impl->site.sig_paint;
	}

	Signal<void()> &DisplayWindow::sig_window_close()
	{
		return impl->site.sig_window_close;
	}

	Signal<void()> &DisplayWindow::sig_window_destroy()
	{
		return impl->site.sig_window_destroy;
	}

	Signal<void()> &DisplayWindow::sig_window_minimized()
	{
		return impl->site.sig_window_minimized;
	}

	Signal<void()> &DisplayWindow::sig_window_maximized()
	{
		return impl->site.sig_window_maximized;
	}

	Signal<void()> &DisplayWindow::sig_window_restored()
	{
		return impl->site.sig_window_restored;
	}

	std::function<void(Rectf &)> &DisplayWindow::func_window_resize()
	{
		return impl->site.func_window_resize;
	}

	std::function<bool()> &DisplayWindow::func_minimize_clicked()
	{
		return impl->site.func_minimize_clicked;
	}

#ifdef WIN32
	std::function<bool(HWND, UINT, WPARAM, LPARAM)> &DisplayWindow::func_window_message()
	{
		return impl->site.func_window_message;
	}
	Signal<void(HWND, UINT, WPARAM, LPARAM)> &DisplayWindow::sig_window_message()
	{
		return impl->site.sig_window_message;
	}

#endif

	Signal<void()> &DisplayWindow::sig_window_moved()
	{
		return impl->site.sig_window_moved;
	}

	Signal<void()> &DisplayWindow::sig_window_flip()
	{
		return impl->sig_window_flip;
	}

	void DisplayWindow::throw_if_null() const
	{
		if (!impl)
			throw Exception("DisplayWindow is null");
	}


	bool DisplayWindow::is_minimized() const
	{
		return impl->provider->is_minimized();
	}

	bool DisplayWindow::is_maximized() const
	{
		return impl->provider->is_maximized();
	}

	bool DisplayWindow::is_visible() const
	{
		return impl->provider->is_visible();
	}

	bool DisplayWindow::is_fullscreen() const
	{
		return impl->provider->is_fullscreen();
	}

	DisplayWindowProvider *DisplayWindow::get_provider() const
	{
		return impl->provider;
	}

	bool DisplayWindow::is_clipboard_text_available() const
	{
		return impl->provider->is_clipboard_text_available();
	}

	bool DisplayWindow::is_clipboard_image_available() const
	{
		return impl->provider->is_clipboard_image_available();
	}

	std::string DisplayWindow::get_clipboard_text() const
	{
		return impl->provider->get_clipboard_text();
	}

	PixelBuffer DisplayWindow::get_clipboard_image() const
	{
		return impl->provider->get_clipboard_image();
	}

	Sizef DisplayWindow::get_minimum_size(bool client_area)
	{
		Size sizei = impl->provider->get_minimum_size(client_area);
		Sizef sizef;
		sizef.width = sizei.width / impl->provider->get_pixel_ratio();
		sizef.height = sizei.height / impl->provider->get_pixel_ratio();
		return sizef;
	}

	Sizef DisplayWindow::get_maximum_size(bool client_area)
	{
		Size sizei = impl->provider->get_maximum_size(client_area);
		Sizef sizef;
		sizef.width = sizei.width / impl->provider->get_pixel_ratio();
		sizef.height = sizei.height / impl->provider->get_pixel_ratio();
		return sizef;
	}

	std::string DisplayWindow::get_title() const
	{
		return impl->provider->get_title();
	}

#ifdef WIN32
	void DisplayWindow::set_cursor_handle(HCURSOR cursor)
	{
		impl->provider->set_cursor_handle(cursor);
	}
#endif

	Pointf DisplayWindow::client_to_screen(const Pointf &client)
	{
		Point clienti;
		clienti.x = (int)std::round(client.x * impl->provider->get_pixel_ratio());
		clienti.y = (int)std::round(client.y * impl->provider->get_pixel_ratio());
		Point screeni = impl->provider->client_to_screen(clienti);
		Pointf screen;
		screen.x = screeni.x / impl->provider->get_pixel_ratio();
		screen.y = screeni.y / impl->provider->get_pixel_ratio();
		return screen;
	}

	Pointf DisplayWindow::screen_to_client(const Pointf &screen)
	{
		Point screeni;
		screeni.x = (int)std::round(screen.x * impl->provider->get_pixel_ratio());
		screeni.y = (int)std::round(screen.y * impl->provider->get_pixel_ratio());
		Point clienti = impl->provider->screen_to_client(screeni);
		Pointf client;
		client.x = clienti.x / impl->provider->get_pixel_ratio();
		client.y = clienti.y / impl->provider->get_pixel_ratio();
		return client;
	}

	void DisplayWindow::capture_mouse(bool capture)
	{
		impl->provider->capture_mouse(capture);
	}

	void DisplayWindow::request_repaint()
	{
		impl->provider->request_repaint();
	}

	void DisplayWindow::set_title(const std::string &title)
	{
		impl->provider->set_title(title);
	}

	void DisplayWindow::set_position(const Rectf &rect, bool client_area)
	{
		Rect recti;
		recti.left = (int)std::round(rect.left * impl->provider->get_pixel_ratio());
		recti.top = (int)std::round(rect.top * impl->provider->get_pixel_ratio());
		recti.right = (int)std::round(rect.right * impl->provider->get_pixel_ratio());
		recti.bottom = (int)std::round(rect.bottom * impl->provider->get_pixel_ratio());
		impl->provider->set_position(recti, client_area);
	}

	void DisplayWindow::set_position(float x, float y)
	{
		int xi = (int)std::round(x * impl->provider->get_pixel_ratio());
		int yi = (int)std::round(y * impl->provider->get_pixel_ratio());
		Rect geometry = impl->provider->get_geometry();
		impl->provider->set_position(Rect(xi, yi, xi + geometry.get_width(), yi + geometry.get_height()), false);
	}

	void DisplayWindow::set_size(float width, float height, bool client_area)
	{
		int widthi = (int)std::round(width * impl->provider->get_pixel_ratio());
		int heighti = (int)std::round(height * impl->provider->get_pixel_ratio());
		impl->provider->set_size(widthi, heighti, client_area);
	}

	void DisplayWindow::set_minimum_size(float width, float height, bool client_area)
	{
		int widthi = (int)std::round(width * impl->provider->get_pixel_ratio());
		int heighti = (int)std::round(height * impl->provider->get_pixel_ratio());
		impl->provider->set_minimum_size(widthi, heighti, client_area);
	}

	void DisplayWindow::set_maximum_size(float width, float height, bool client_area)
	{
		int widthi = (int)std::round(width * impl->provider->get_pixel_ratio());
		int heighti = (int)std::round(height * impl->provider->get_pixel_ratio());
		impl->provider->set_maximum_size(widthi, heighti, client_area);
	}

	void DisplayWindow::set_enabled(bool enable)
	{
		impl->provider->set_enabled(enable);
	}

	void DisplayWindow::set_visible(bool visible, bool activate)
	{
		if (visible)
			impl->provider->show(activate);
		else
			impl->provider->hide();
	}

	void DisplayWindow::minimize()
	{
		impl->provider->minimize();
	}

	void DisplayWindow::restore()
	{
		impl->provider->restore();
	}

	void DisplayWindow::maximize()
	{
		impl->provider->maximize();
	}

	void DisplayWindow::toggle_fullscreen()
	{
		impl->provider->toggle_fullscreen();
	}


	void DisplayWindow::show(bool activate)
	{
		impl->provider->show(activate);
	}

	void DisplayWindow::hide()
	{
		impl->provider->hide();
	}

	void DisplayWindow::bring_to_front()
	{
		impl->provider->bring_to_front();
	}

	void DisplayWindow::flip(int interval)
	{
		impl->sig_window_flip();
		impl->provider->flip(interval);
	}

	void DisplayWindow::show_cursor()
	{
		impl->provider->show_system_cursor();
	}

	void DisplayWindow::set_cursor(const Cursor &cursor)
	{
		impl->current_cursor = cursor;
		impl->provider->set_cursor(cursor.get_provider());
	}

	void DisplayWindow::set_cursor(StandardCursor type)
	{
		impl->current_cursor = Cursor();
		impl->provider->set_cursor(type);
	}

	void DisplayWindow::hide_cursor()
	{
		impl->provider->hide_system_cursor();
	}

	void DisplayWindow::set_clipboard_text(const std::string &text)
	{
		impl->provider->set_clipboard_text(text);
	}

	void DisplayWindow::set_clipboard_image(const PixelBuffer &buf)
	{
		impl->provider->set_clipboard_image(buf);
	}

	void DisplayWindow::set_large_icon(const PixelBuffer &image)
	{
		impl->provider->set_large_icon(image);
	}

	void DisplayWindow::set_small_icon(const PixelBuffer &image)
	{
		impl->provider->set_small_icon(image);
	}

	void DisplayWindow::enable_alpha_channel(const Rectf &blur_rect)
	{
		Rect blur_recti;
		blur_recti.left = (int)std::round(blur_rect.left * impl->provider->get_pixel_ratio());
		blur_recti.top = (int)std::round(blur_rect.top * impl->provider->get_pixel_ratio());
		blur_recti.right = (int)std::round(blur_rect.right * impl->provider->get_pixel_ratio());
		blur_recti.bottom = (int)std::round(blur_rect.bottom * impl->provider->get_pixel_ratio());
		impl->provider->enable_alpha_channel(blur_recti);
	}

	void DisplayWindow::extend_frame_into_client_area(float left, float top, float right, float bottom)
	{
		int lefti = (int)std::round(left * impl->provider->get_pixel_ratio());
		int topi = (int)std::round(top * impl->provider->get_pixel_ratio());
		int righti = (int)std::round(right * impl->provider->get_pixel_ratio());
		int bottomi = (int)std::round(bottom * impl->provider->get_pixel_ratio());
		impl->provider->extend_frame_into_client_area(lefti, topi, righti, bottomi);
	}
}
