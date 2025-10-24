/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow Construction:

CL_DisplayWindow::CL_DisplayWindow()
{
}

CL_DisplayWindow::CL_DisplayWindow(
	const CL_StringRef &title,
	int width,
	int height,
	bool start_fullscreen,
	bool allow_resize,
	int flipping_buffers,
	CL_DisplayTarget target)
: impl(new CL_DisplayWindow_Impl)
{
	CL_DisplayWindowDescription description;
	description.set_title(title);
	description.set_size(CL_Size(width, height), false);
	description.set_fullscreen(start_fullscreen);

	if (start_fullscreen)
	{
		description.set_decorations(false);
		description.show_caption(false);
	}

	description.set_allow_resize(allow_resize);
	description.set_flipping_buffers(flipping_buffers);

	CL_DisplayTargetProvider *target_provider = target.get_provider();
	impl->provider = target_provider->alloc_display_window();
	impl->provider->create(&impl->site, description);

}

CL_DisplayWindow::CL_DisplayWindow(
	const CL_DisplayWindowDescription &description,
	CL_DisplayTarget target)
: impl(new CL_DisplayWindow_Impl)
{
	CL_DisplayTargetProvider *target_provider = target.get_provider();
	impl->provider = target_provider->alloc_display_window();
	impl->provider->create(&impl->site, description);
}

CL_DisplayWindow::CL_DisplayWindow(CL_DisplayWindowProvider *provider)
: impl(new CL_DisplayWindow_Impl)
{
	impl->provider = provider;
}

CL_DisplayWindow::~CL_DisplayWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow Attributes:

#ifdef WIN32
HWND CL_DisplayWindow::get_hwnd() const
{
	return impl->provider->get_hwnd();
}
#elif defined(__APPLE__)
	// nothing
#else
Display *CL_DisplayWindow::get_display() const
{
	return impl->provider->get_display();
}

Window CL_DisplayWindow::get_window() const
{
	return impl->provider->get_window();
}
#endif

CL_Rect CL_DisplayWindow::get_geometry() const
{
	return impl->provider->get_geometry();
}

CL_Rect CL_DisplayWindow::get_viewport() const
{
	return impl->provider->get_viewport();
}

bool CL_DisplayWindow::has_focus() const
{
	return impl->provider->has_focus();
}

CL_GraphicContext& CL_DisplayWindow::get_gc() const
{
	return impl->provider->get_gc();
}

CL_InputContext& CL_DisplayWindow::get_ic() const
{
	return impl->provider->get_ic();
}

CL_Signal_v0 &CL_DisplayWindow::sig_lost_focus()
{
	return impl->sig_lost_focus;
}

CL_Signal_v0 &CL_DisplayWindow::sig_got_focus()
{
	return impl->sig_got_focus;
}

CL_Signal_v2<int, int> &CL_DisplayWindow::sig_resize()
{
	return impl->sig_resize;
}

CL_Signal_v1<const CL_Rect &> &CL_DisplayWindow::sig_paint()
{
	return impl->sig_paint;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_close()
{
	return impl->sig_window_close;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_destroy()
{
	return impl->sig_window_destroy;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_minimized()
{
	return impl->sig_window_minimized;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_maximized()
{
	return impl->sig_window_maximized;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_restored()
{
	return impl->sig_window_restored;
}

CL_Callback_v1<CL_Rect &> &CL_DisplayWindow::func_window_resize()
{
	return impl->func_window_resize;
}

CL_Callback_0<bool> &CL_DisplayWindow::func_minimize_clicked()
{
	return impl->func_minimize_clicked;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_moved()
{
	return impl->sig_window_moved;
}

void CL_DisplayWindow::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_DisplayWindow is null");
}


bool CL_DisplayWindow::is_minimized() const
{
	return impl->provider->is_minimized();
}

bool CL_DisplayWindow::is_maximized() const
{
	return impl->provider->is_maximized();
}

bool CL_DisplayWindow::is_visible() const
{
	return impl->provider->is_visible();
}

bool CL_DisplayWindow::is_fullscreen() const
{
	return impl->provider->is_fullscreen();
}

CL_DisplayWindowProvider *CL_DisplayWindow::get_provider() const
{
	return impl->provider;
}

bool CL_DisplayWindow::is_clipboard_text_available() const
{
	return impl->provider->is_clipboard_text_available();
}

bool CL_DisplayWindow::is_clipboard_image_available() const
{
	return impl->provider->is_clipboard_image_available();
}

CL_String CL_DisplayWindow::get_clipboard_text() const
{
	return impl->provider->get_clipboard_text();
}

CL_PixelBuffer CL_DisplayWindow::get_clipboard_image() const
{
	return impl->provider->get_clipboard_image();
}

CL_Size CL_DisplayWindow::get_minimum_size( bool client_area )
{
	return impl->provider->get_minimum_size(client_area);
}

CL_Size CL_DisplayWindow::get_maximum_size( bool client_area )
{
	return impl->provider->get_maximum_size(client_area);
}

CL_String CL_DisplayWindow::get_title() const
{
	return impl->provider->get_title();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow Operations:

CL_Point CL_DisplayWindow::client_to_screen(const CL_Point &client)
{
	return impl->provider->client_to_screen(client);
}

CL_Point CL_DisplayWindow::screen_to_client(const CL_Point &screen)
{
	return impl->provider->screen_to_client(screen);
}

void CL_DisplayWindow::capture_mouse(bool capture)
{
	impl->provider->capture_mouse(capture);
}

void CL_DisplayWindow::request_repaint(const CL_Rect &rect)
{
	impl->provider->request_repaint(rect);
}

void CL_DisplayWindow::set_title(const CL_StringRef &title)
{
	impl->provider->set_title(title);
}

void CL_DisplayWindow::set_position(const CL_Rect &pos, bool client_area)
{
	impl->provider->set_position(pos, client_area);
}

void CL_DisplayWindow::set_position(int x, int y)
{
	CL_Rect geometry = impl->provider->get_geometry();
	impl->provider->set_position(CL_Rect(x, y, x + geometry.get_width(), y + geometry.get_height()), false);
}

void CL_DisplayWindow::set_size(int width, int height, bool client_area)
{
	impl->provider->set_size(width, height, client_area);
}

void CL_DisplayWindow::set_minimum_size( int width, int height, bool client_area)
{
	impl->provider->set_minimum_size(width, height, client_area);
}

void CL_DisplayWindow::set_maximum_size( int width, int height, bool client_area)
{
	impl->provider->set_maximum_size(width, height, client_area);
}

void CL_DisplayWindow::set_enabled(bool enable)
{
	impl->provider->set_enabled(enable);
}

void CL_DisplayWindow::set_visible(bool visible, bool activate)
{
	if (visible)
		impl->provider->show(activate);
	else
		impl->provider->hide();
}

void CL_DisplayWindow::minimize()
{
	impl->provider->minimize();
}

void CL_DisplayWindow::restore()
{
	impl->provider->restore();
}

void CL_DisplayWindow::maximize()
{
	impl->provider->maximize();
}

void CL_DisplayWindow::show(bool activate)
{
	impl->provider->show(activate);
}

void CL_DisplayWindow::hide()
{
	impl->provider->hide();
}

void CL_DisplayWindow::bring_to_front()
{
	impl->provider->bring_to_front();
}

void CL_DisplayWindow::update(const CL_Rect &rect)
{
	get_gc().flush_batcher();
	impl->provider->update(rect);
}

void CL_DisplayWindow::flip(int interval)
{
	get_gc().flush_batcher();
	impl->provider->flip(interval);
}

void CL_DisplayWindow::show_cursor()
{
	impl->provider->show_system_cursor();
}

void CL_DisplayWindow::set_cursor(const CL_Cursor &cursor)
{
	impl->current_cursor = cursor;
	impl->provider->set_cursor(cursor.get_provider());
}

void CL_DisplayWindow::set_cursor(CL_StandardCursor type)
{
	impl->current_cursor = CL_Cursor();
	impl->provider->set_cursor(type);
}

void CL_DisplayWindow::hide_cursor()
{
	impl->provider->hide_system_cursor();
}

void CL_DisplayWindow::set_clipboard_text(const CL_StringRef &text)
{
	impl->provider->set_clipboard_text(text);
}

void CL_DisplayWindow::set_clipboard_image( const CL_PixelBuffer &buf )
{
	impl->provider->set_clipboard_image(buf);
}

void CL_DisplayWindow::set_large_icon(const CL_PixelBuffer &image)
{
	impl->provider->set_large_icon(image);
}

void CL_DisplayWindow::set_small_icon(const CL_PixelBuffer &image)
{
	impl->provider->set_small_icon(image);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow Implementation:
