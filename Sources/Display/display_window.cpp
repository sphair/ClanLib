/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/display_mode.h"
#include "API/Display/display_window.h"
#include "API/Display/display_window_description.h"
#include "display_window_generic.h"
#include "display_target.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow construction:

CL_DisplayWindow::CL_DisplayWindow()
: impl(0)
{
}

CL_DisplayWindow::CL_DisplayWindow(
	const std::string &title,
	int width,
	int height,
	bool start_fullscreen,
	bool allow_resize,
	int flipping_buffers)
: impl(0)
{
	impl = CL_DisplayTarget::current()->create_window();
	cl_assert(impl != 0); // platform not implemented?
	if (impl) impl->add_ref();

	CL_DisplayWindowDescription desc;
	desc.set_title(title);
	desc.set_size(CL_Size(width, height));
	desc.set_fullscreen(start_fullscreen);
	desc.set_flipping_buffers(flipping_buffers);
	desc.set_allow_resize(allow_resize);

	impl->create_window(desc);

	// Automatically map first created display window as 'CL_Display current'.
	if (CL_DisplayWindow_Generic::current == 0) CL_DisplayWindow_Generic::current = this;

	impl->sig_resize(get_width(), get_height());
}

CL_DisplayWindow::CL_DisplayWindow(const CL_DisplayWindowDescription &description)
: impl(0)
{
	impl = CL_DisplayTarget::current()->create_window();
	cl_assert(impl != 0); // platform not implemented?
	if (impl) impl->add_ref();

	impl->create_window(description);

	// Automatically map first created display window as 'CL_Display current'.
	if (CL_DisplayWindow_Generic::current == 0) CL_DisplayWindow_Generic::current = this;

	impl->sig_resize(get_width(), get_height());
}

CL_DisplayWindow::~CL_DisplayWindow()
{
	if (impl) impl->release_ref();
	if (CL_DisplayWindow_Generic::current == this) CL_DisplayWindow_Generic::current = 0;
}

CL_DisplayWindow::CL_DisplayWindow(CL_DisplayWindow_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();

	// Automatically map first created display window as 'CL_Display current'.
	if (CL_DisplayWindow_Generic::current == 0) CL_DisplayWindow_Generic::current = this;
}

CL_DisplayWindow::CL_DisplayWindow(const CL_DisplayWindow &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
	if (CL_DisplayWindow_Generic::current == &copy) CL_DisplayWindow_Generic::current = this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow attributes:

int CL_DisplayWindow::get_width() const
{
	return impl->get_width();
}

int CL_DisplayWindow::get_height() const
{
	return impl->get_height();
}

bool CL_DisplayWindow::is_fullscreen() const
{
	return impl->is_fullscreen();
}

bool CL_DisplayWindow::has_focus() const
{
	return impl->has_focus();
}

CL_PixelBuffer CL_DisplayWindow::get_buffer(int i) const
{
	return impl->get_buffer(i);
}

int CL_DisplayWindow::get_buffer_count() const
{
	return impl->get_buffer_count();
}

CL_PixelBuffer CL_DisplayWindow::get_front_buffer()
{
	return impl->get_buffer(0);
}

CL_PixelBuffer CL_DisplayWindow::get_back_buffer()
{
	return impl->get_buffer(impl->get_buffer_count()-1);
}

CL_Signal_v2<int, int> &CL_DisplayWindow::sig_resize()
{
	return impl->sig_resize;
}

CL_Signal_v1<const CL_Rect &> &CL_DisplayWindow::sig_paint()
{
	return impl->sig_paint;
}

CL_Signal_v0 &CL_DisplayWindow::sig_lost_focus()
{
	return impl->sig_lost_focus;
}

CL_Signal_v0 &CL_DisplayWindow::sig_got_focus()
{
	return impl->sig_got_focus;
}

CL_Signal_v0 &CL_DisplayWindow::sig_window_close()
{
	return impl->sig_window_close;
}

CL_GraphicContext *CL_DisplayWindow::get_gc()
{
	return impl->get_gc();
}

CL_InputContext *CL_DisplayWindow::get_ic()
{
	return impl->get_ic();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow operations:

CL_DisplayWindow &CL_DisplayWindow::operator =(const CL_DisplayWindow &copy)
{
	if (impl != copy.impl)
	{
		if (impl) impl->release_ref();
		impl = copy.impl;
		if (impl) impl->add_ref();
	}
	if (CL_DisplayWindow_Generic::current == &copy) CL_DisplayWindow_Generic::current = this;
	return *this;
}

void CL_DisplayWindow::set_fullscreen(int width, int height, int bpp, int refresh_rate)
{
	impl->set_fullscreen(width, height, bpp, refresh_rate);
	impl->sig_resize(get_width(), get_height());
}

void CL_DisplayWindow::set_fullscreen(const CL_DisplayMode &display_mode)
{
	set_fullscreen(
		display_mode.get_resolution().width, 
		display_mode.get_resolution().height, 
		display_mode.get_bpp(),
		display_mode.get_refresh_rate());
}

void CL_DisplayWindow::set_windowed()
{
	impl->set_windowed();
	impl->sig_resize(get_width(), get_height());
}

void CL_DisplayWindow::set_title(const std::string &title)
{
	impl->set_title(title);
}

void CL_DisplayWindow::set_position(const CL_Rect &pos)
{
	impl->set_position(pos);
}

void CL_DisplayWindow::set_position(int x, int y)
{
	impl->set_position(x, y);
}

void CL_DisplayWindow::set_size(int width, int height)
{
	impl->set_size(width, height);
}

void CL_DisplayWindow::set_buffer_count(int flipping_buffers)
{
	impl->set_buffer_count(flipping_buffers);
}

void CL_DisplayWindow::update(const CL_Rect &rect)
{
	impl->update(rect);
}

void CL_DisplayWindow::flip(int interval)
{
	impl->do_flip(interval);
}

void CL_DisplayWindow::set_cursor(const CL_Sprite &cursor)
{
	impl->set_cursor(cursor);
}

void CL_DisplayWindow::show_cursor()
{
	impl->show_cursor();
}

void CL_DisplayWindow::hide_cursor()
{
	impl->hide_cursor();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow implementation:
