/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DisplayWindow Construction:

DisplayWindow::DisplayWindow()
{
}

DisplayWindow::DisplayWindow(
	const std::string &title,
	int width,
	int height,
	bool start_fullscreen,
	bool allow_resize,
	int flipping_buffers,
	DisplayTarget target)
: impl(new DisplayWindow_Impl)
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

	DisplayTargetProvider *target_provider = target.get_provider();
	impl->provider = target_provider->alloc_display_window();
	impl->provider->create(&impl->site, description);

}

DisplayWindow::DisplayWindow(
	const DisplayWindowDescription &description,
	DisplayTarget target)
: impl(new DisplayWindow_Impl)
{
	DisplayTargetProvider *target_provider = target.get_provider();
	impl->provider = target_provider->alloc_display_window();
	impl->provider->create(&impl->site, description);
}

DisplayWindow::DisplayWindow(DisplayWindowProvider *provider)
: impl(new DisplayWindow_Impl)
{
	impl->provider = provider;
}

DisplayWindow::~DisplayWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// DisplayWindow Attributes:

#ifdef WIN32
HWND DisplayWindow::get_hwnd() const
{
	return impl->provider->get_hwnd();
}
#elif defined(__APPLE__)
	// nothing
#else
Display *DisplayWindow::get_display() const
{
	return impl->provider->get_display();
}

Window DisplayWindow::get_window() const
{
	return impl->provider->get_window();
}
#endif

Rect DisplayWindow::get_geometry() const
{
	return impl->provider->get_geometry();
}

Rect DisplayWindow::get_viewport() const
{
	return impl->provider->get_viewport();
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

InputContext DisplayWindow::get_ic() const
{
	throw_if_null();
	return impl->provider->get_ic();
}

Signal_v0 &DisplayWindow::sig_lost_focus()
{
	return impl->sig_lost_focus;
}

Signal_v0 &DisplayWindow::sig_got_focus()
{
	return impl->sig_got_focus;
}

Signal_v2<int, int> &DisplayWindow::sig_resize()
{
	return impl->sig_resize;
}

Signal_v1<const Rect &> &DisplayWindow::sig_paint()
{
	return impl->sig_paint;
}

Signal_v0 &DisplayWindow::sig_window_close()
{
	return impl->sig_window_close;
}

Signal_v0 &DisplayWindow::sig_window_destroy()
{
	return impl->sig_window_destroy;
}

Signal_v0 &DisplayWindow::sig_window_minimized()
{
	return impl->sig_window_minimized;
}

Signal_v0 &DisplayWindow::sig_window_maximized()
{
	return impl->sig_window_maximized;
}

Signal_v0 &DisplayWindow::sig_window_restored()
{
	return impl->sig_window_restored;
}

Callback_v1<Rect &> &DisplayWindow::func_window_resize()
{
	return impl->func_window_resize;
}

Callback_0<bool> &DisplayWindow::func_minimize_clicked()
{
	return impl->func_minimize_clicked;
}

#ifdef WIN32
Callback_4<bool, HWND, UINT, WPARAM, LPARAM> &DisplayWindow::func_window_message()
{
	return impl->func_window_message;
}
#endif

Signal_v0 &DisplayWindow::sig_window_moved()
{
	return impl->sig_window_moved;
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

Size DisplayWindow::get_minimum_size( bool client_area )
{
	return impl->provider->get_minimum_size(client_area);
}

Size DisplayWindow::get_maximum_size( bool client_area )
{
	return impl->provider->get_maximum_size(client_area);
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

/////////////////////////////////////////////////////////////////////////////
// DisplayWindow Operations:

Point DisplayWindow::client_to_screen(const Point &client)
{
	return impl->provider->client_to_screen(client);
}

Point DisplayWindow::screen_to_client(const Point &screen)
{
	return impl->provider->screen_to_client(screen);
}

void DisplayWindow::capture_mouse(bool capture)
{
	impl->provider->capture_mouse(capture);
}

void DisplayWindow::request_repaint(const Rect &rect)
{
	impl->provider->request_repaint(rect);
}

void DisplayWindow::set_title(const std::string &title)
{
	impl->provider->set_title(title);
}

void DisplayWindow::set_position(const Rect &pos, bool client_area)
{
	impl->provider->set_position(pos, client_area);
}

void DisplayWindow::set_position(int x, int y)
{
	Rect geometry = impl->provider->get_geometry();
	impl->provider->set_position(Rect(x, y, x + geometry.get_width(), y + geometry.get_height()), false);
}

void DisplayWindow::set_size(int width, int height, bool client_area)
{
	impl->provider->set_size(width, height, client_area);
}

void DisplayWindow::set_minimum_size( int width, int height, bool client_area)
{
	impl->provider->set_minimum_size(width, height, client_area);
}

void DisplayWindow::set_maximum_size( int width, int height, bool client_area)
{
	impl->provider->set_maximum_size(width, height, client_area);
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

void DisplayWindow::update(const Rect &rect)
{
	impl->provider->update(rect);
}

void DisplayWindow::flip(int interval)
{
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

void DisplayWindow::set_clipboard_image( const PixelBuffer &buf )
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

/////////////////////////////////////////////////////////////////////////////
// DisplayWindow Implementation:

}
