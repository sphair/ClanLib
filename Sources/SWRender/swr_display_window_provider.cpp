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
**	  Harry Storbacka
*/

#include "SWRender/precomp.h"
#include "swr_display_window_provider.h"
#include "swr_target_provider.h"
#include "swr_graphic_context_provider.h"
#include "Canvas/pixel_canvas.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/display_window_description.h"

#ifdef WIN32
#include "Display/Win32/cursor_provider_win32.h"
#else if !defined(__APPLE__)
#include "Display/X11/cursor_provider_x11.h"
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderDisplayWindowProvider Construction:

SWRenderDisplayWindowProvider::SWRenderDisplayWindowProvider()
: site(0)
{
#ifdef WIN32
	window.set_allow_drop_shadow(true);
#endif
	window.func_on_resized().set(this, &SWRenderDisplayWindowProvider::on_window_resized);
}

SWRenderDisplayWindowProvider::~SWRenderDisplayWindowProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderDisplayWindowProvider Attributes:

Rect SWRenderDisplayWindowProvider::get_geometry() const
{
	return window.get_geometry();
}

Rect SWRenderDisplayWindowProvider::get_viewport() const
{
	return window.get_viewport();
}

bool SWRenderDisplayWindowProvider::has_focus() const
{
	return window.has_focus();
}

bool SWRenderDisplayWindowProvider::is_minimized() const
{
	return window.is_minimized();
}

bool SWRenderDisplayWindowProvider::is_maximized() const
{
	return window.is_maximized();
}

bool SWRenderDisplayWindowProvider::is_visible() const
{
	return window.is_visible();
}

GraphicContext& SWRenderDisplayWindowProvider::get_gc()
{
	return gc;
}

InputContext SWRenderDisplayWindowProvider::get_ic()
{
	return window.get_ic();
}

#ifdef WIN32
HWND SWRenderDisplayWindowProvider::get_hwnd() const
{
	return window.get_hwnd();
}
#endif

bool SWRenderDisplayWindowProvider::is_clipboard_text_available() const
{
	return window.is_clipboard_text_available();
}

bool SWRenderDisplayWindowProvider::is_clipboard_image_available() const
{
	return window.is_clipboard_image_available();
}

Size SWRenderDisplayWindowProvider::get_minimum_size(bool client_area) const
{
	return window.get_minimum_size(client_area);
}

Size SWRenderDisplayWindowProvider::get_maximum_size( bool client_area/*=false*/ ) const
{
	return window.get_maximum_size(client_area);
}

std::string SWRenderDisplayWindowProvider::get_title() const
{
	return window.get_title();
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderDisplayWindowProvider Operations:

Point SWRenderDisplayWindowProvider::client_to_screen(const Point &client)
{
	return window.client_to_screen(client);
}

Point SWRenderDisplayWindowProvider::screen_to_client(const Point &screen)
{
	return window.screen_to_client(screen);
}

void SWRenderDisplayWindowProvider::capture_mouse(bool capture)
{
	window.capture_mouse(capture);
}

void SWRenderDisplayWindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &description)
{
	site = new_site;
	flip_timer_set = false;

	refresh_rate = description.get_refresh_rate();
	if (!refresh_rate)	// Default the refresh rate to 100 if not defined
		refresh_rate = 100;
	swap_interval = description.get_swap_interval();

#ifdef WIN32
	window.create(site, description);
#elif !defined(__APPLE__)
	Display *disp = window.get_display();

	int bpp = 24;
	XVisualInfo visual_info;
	int screen =  DefaultScreen(disp); //RootWindow(disp,  0);

	if (XMatchVisualInfo(disp, screen, 24, TrueColor, &visual_info)) {bpp = 24;} 
	else if (XMatchVisualInfo(disp, screen, 16, TrueColor, &visual_info)) {bpp = 16;}
	else if (XMatchVisualInfo(disp, screen, 15, TrueColor, &visual_info)) {bpp = 15;}
	else if (XMatchVisualInfo(disp, screen, 32, TrueColor, &visual_info)) {bpp = 32;}
	else if (XMatchVisualInfo(disp, screen, 8, PseudoColor, &visual_info)) {bpp = 8;}
	else if (XMatchVisualInfo(disp, screen, 8, GrayScale, &visual_info)) {bpp = 8;}
	else if (XMatchVisualInfo(disp, screen, 8, StaticGray, &visual_info)) {bpp = 8;}
	else if (XMatchVisualInfo(disp, screen, 1, StaticGray, &visual_info)) {bpp = 1;}
	else { throw Exception("Cannot match visual info"); }

	window.create(&visual_info, site, description);
#endif
	gc = GraphicContext(new SWRenderGraphicContextProvider(this));
}

void SWRenderDisplayWindowProvider::show_system_cursor()
{
	window.show_system_cursor();
}

CursorProvider *SWRenderDisplayWindowProvider::create_cursor(const CursorDescription &cursor_description, const Point &hotspot)
{
#ifdef WIN32
	return new CursorProvider_Win32(sprite_description, hotspot);
#elif !defined(__APPLE__)
	return new CursorProvider_X11(sprite_description, hotspot);
#endif
}

void SWRenderDisplayWindowProvider::set_cursor(CursorProvider *cursor)
{
#ifdef WIN32
	window.set_cursor(static_cast<CursorProvider_Win32 *>(cursor));
#elif !defined(__APPLE__)
	window.set_cursor(static_cast<CursorProvider_X11 *>(cursor));
#endif
}

void SWRenderDisplayWindowProvider::set_cursor(StandardCursor type)
{
	window.set_cursor(type);
}

void SWRenderDisplayWindowProvider::hide_system_cursor()
{
	window.hide_system_cursor();
}

void SWRenderDisplayWindowProvider::set_title(const std::string &new_title)
{
	window.set_title(new_title);
}

void SWRenderDisplayWindowProvider::set_position(const Rect &pos, bool client_area)
{
	window.set_position(pos, client_area);
}

void SWRenderDisplayWindowProvider::set_size(int width, int height, bool client_area)
{
	window.set_size(width, height, client_area);
}

void SWRenderDisplayWindowProvider::set_minimum_size( int width, int height, bool client_area )
{
	window.set_minimum_size(width,height,client_area);
}

void SWRenderDisplayWindowProvider::set_maximum_size( int width, int height, bool client_area )
{
	window.set_maximum_size(width,height,client_area);
}
void SWRenderDisplayWindowProvider::set_enabled(bool enable)
{
	window.set_enabled(enable);
}

void SWRenderDisplayWindowProvider::minimize()
{
	window.minimize();
}

void SWRenderDisplayWindowProvider::restore()
{
	window.restore();
}

void SWRenderDisplayWindowProvider::maximize()
{
	window.maximize();
}

void SWRenderDisplayWindowProvider::show(bool activate)
{
	window.show(activate);
}

void SWRenderDisplayWindowProvider::hide()
{
	window.hide();
}

void SWRenderDisplayWindowProvider::bring_to_front()
{
	window.bring_to_front();
}

void SWRenderDisplayWindowProvider::flip(int interval)
{
	SWRenderGraphicContextProvider *gc_provider = static_cast<SWRenderGraphicContextProvider*>(gc.get_provider());
	PixelCanvas *canvas = gc_provider->get_canvas();

#ifdef WIN32
	HWND hwnd = window.get_hwnd();
	HDC hdc = GetDC(hwnd);
	draw_image(hdc, get_viewport(), canvas->to_pixelbuffer());
	ReleaseDC(hwnd, hdc);
#elif !defined(__APPLE__)
	PixelBuffer &image = canvas->to_pixelbuffer();
	draw_image(get_viewport(), image, Rect(0, 0, image.get_width(), image.get_height()));
#endif

	if (interval == -1)
		interval = swap_interval;    // use swap interval from the previous flip
	else
		swap_interval = interval;

	if (interval<=0)
	{
		flip_timer_set = false;
	}
	else
	{
		if (!flip_timer_set)
		{
			flip_last_time = System::get_time();
			flip_timer_set = true;
		}
		else
		{
			unsigned int current_time = System::get_time();

			int time_diff = current_time - flip_last_time;

			interval *= 1000 / refresh_rate;
			int time_wait = interval - time_diff;

			if ( (time_wait > 0) && (time_wait < interval) )
			{
				System::sleep(time_wait);
				flip_last_time = current_time + time_wait;
			}
			else
			{
				flip_last_time = current_time;
			}
		}
	}
}

void SWRenderDisplayWindowProvider::update(const Rect &rect)
{
	SWRenderGraphicContextProvider *gc_provider = static_cast<SWRenderGraphicContextProvider*>(gc.get_provider());
	PixelCanvas *canvas = gc_provider->get_canvas();
#ifdef WIN32
	HWND hwnd = window.get_hwnd();
	HDC hdc = GetDC(hwnd);
	draw_image(hdc, rect, canvas->to_pixelbuffer(), rect);
	ReleaseDC(hwnd, hdc);
#elif !defined(__APPLE__)
	PixelBuffer &image = canvas->to_pixelbuffer();
	draw_image(rect, image, rect);
#endif
}

void SWRenderDisplayWindowProvider::set_clipboard_text(const std::string &text)
{
	window.set_clipboard_text(text);
}

void SWRenderDisplayWindowProvider::set_clipboard_image(const PixelBuffer &buf)
{
	window.set_clipboard_image(buf);
}

std::string SWRenderDisplayWindowProvider::get_clipboard_text() const
{
	return window.get_clipboard_text();
}

PixelBuffer SWRenderDisplayWindowProvider::get_clipboard_image() const
{
	return window.get_clipboard_image();
}

void SWRenderDisplayWindowProvider::request_repaint(const Rect &rect)
{
	window.request_repaint(rect);
}

void SWRenderDisplayWindowProvider::set_large_icon(const PixelBuffer &image)
{
	window.set_large_icon(image);
}

void SWRenderDisplayWindowProvider::set_small_icon(const PixelBuffer &image)
{
	window.set_small_icon(image);
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderDisplayWindowProvider Implementation:

void SWRenderDisplayWindowProvider::on_window_resized()
{
	if (gc.get_provider())
		((SWRenderGraphicContextProvider *) gc.get_provider())->on_window_resized();
}

#ifdef WIN32
void SWRenderDisplayWindowProvider::draw_image(HDC hdc, const Rect &dest, const PixelBuffer &image)
{
	draw_image(hdc, dest, image, Rect(0, 0, image.get_width(), image.get_height()));
}

void SWRenderDisplayWindowProvider::draw_image(HDC hdc, const Rect &dest, const PixelBuffer &image, const Rect &src)
{
	BITMAPV5HEADER bmp_header;
	memset(&bmp_header, 0, sizeof(BITMAPV5HEADER));
	bmp_header.bV5Size = sizeof(BITMAPV5HEADER);
	bmp_header.bV5Width = image.get_width();
	bmp_header.bV5Height = -image.get_height();
	bmp_header.bV5Planes = 1;
	bmp_header.bV5BitCount = 32;
	bmp_header.bV5Compression = BI_BITFIELDS;
	bmp_header.bV5RedMask = image.get_red_mask();
	bmp_header.bV5GreenMask = image.get_green_mask();
	bmp_header.bV5BlueMask = image.get_blue_mask();
	bmp_header.bV5AlphaMask = image.get_alpha_mask();
	//StretchDIBits(hdc, dest.left, dest.top, dest.get_width(), dest.get_height(), src.left, src.top, src.get_width(), src.get_height(), image.get_data(), (BITMAPINFO *) &bmp_header, DIB_RGB_COLORS, SRCCOPY);
	SetDIBitsToDevice(hdc, dest.left, dest.top, dest.get_width(), dest.get_height(), src.left, image.get_height()-src.bottom, 0, image.get_height(), image.get_data(), (BITMAPINFO *) &bmp_header, DIB_RGB_COLORS);
}
#elif !defined(__APPLE__)
void SWRenderDisplayWindowProvider::draw_image(const Rect &dest, const PixelBuffer &image, const Rect &src)
{
	XImage ximage;
	memset(&ximage, 0, sizeof(ximage));

	ximage.width = image.get_width();
	ximage.height = image.get_height();
	ximage.format = ZPixmap;
	ximage.data = (char *) image.get_data();
	ximage.byte_order = LSBFirst;

	int image_pitch = image.get_pitch();
	int image_bpp = image.get_bytes_per_pixel();

	if (image_bpp == 1)
	{
		ximage.bitmap_unit = 8;
	}
	else if (image_bpp == 2)
	{
		ximage.bitmap_unit = 16;
	}
	else if (image_bpp == 3)
	{
		ximage.bitmap_unit = 24;
	}
	else	ximage.bitmap_unit = 32;

	ximage.bitmap_pad = ximage.bitmap_unit;

	ximage.bitmap_bit_order = LSBFirst;
	ximage.depth = 24;
	ximage.bytes_per_line = image_pitch;

	ximage.bits_per_pixel = image_bpp * 8;

	ximage.red_mask = image.get_red_mask();
	ximage.green_mask = image.get_green_mask();
	ximage.blue_mask = image.get_blue_mask();

	if (!XInitImage(&ximage))
	{
		throw Exception("Cannot initialise image");
	}

	GC xgc = XCreateGC(window.get_display(), window.get_window(), 0, NULL); 

	XPutImage(window.get_display(), window.get_window(), xgc, &ximage, src.left, src.top, dest.left, dest.top, src.get_width(), src.get_height());
	XFreeGC(window.get_display(), xgc);
}

#endif

}
