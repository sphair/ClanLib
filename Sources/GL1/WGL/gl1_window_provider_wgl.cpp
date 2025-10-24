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
**    Kevin J Bluck
**    Mark Page
*/

#include "GL1/precomp.h"
#include "gl1_window_provider_wgl.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL1/opengl1.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1_window_description.h"
#include "API/Core/Text/logger.h"
#include "Display/Win32/cursor_provider_win32.h"
#include "Display/Win32/dwm_functions.h"
#include "../gl1_window_description_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "../gl1_target_provider.h"
#include "gl1_creation_helper.h"
#include <commctrl.h>
#include "pbuffer_impl.h"

CL_RenderWindowProvider_WGL::CL_RenderWindowProvider_WGL(CL_GL1WindowProvider_WGL & window, HGLRC wgl_context, bool own_context)
	: window(window), wgl_context(wgl_context), own_context(own_context)
{
}

CL_RenderWindowProvider_WGL::~CL_RenderWindowProvider_WGL()
{
	if( wgl_context && own_context)
	{
		wglDeleteContext(wgl_context);
	}
}

int CL_RenderWindowProvider_WGL::get_viewport_width() const
{
	return window.get_viewport().get_width();
}

int CL_RenderWindowProvider_WGL::get_viewport_height() const
{
	return window.get_viewport().get_height();
}

void CL_RenderWindowProvider_WGL::flip_buffers(int interval) const
{
	window.flip(interval);
}

void CL_RenderWindowProvider_WGL::make_current() const
{
	wglMakeCurrent(window.get_device_context(), wgl_context );
}

CL_GL1WindowProvider_WGL &CL_RenderWindowProvider_WGL::get_window()
{
	return window;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_WGL Construction:

CL_GL1WindowProvider_WGL::CL_GL1WindowProvider_WGL()
: win32_window(),
  opengl_context(0), device_context(0), hwnd(0), shadow_window(false), dwm_layered(false), site(0), fullscreen(false),
  wglSwapIntervalEXT(0), swap_interval(-1)
{
	win32_window.func_on_resized().set(this, &CL_GL1WindowProvider_WGL::on_window_resized);
}

CL_GL1WindowProvider_WGL::~CL_GL1WindowProvider_WGL()
{
	if (opengl_context)
	{
		// save gc provider pointer so we can delete it later from shared list
		CL_GraphicContextProvider *destroyed_gc_provider = gc.get_provider();

		// Delete context from list of gc's that share textures.
		if (CL_SharedGCData::get_instance()) // Check that the cache hasn't been destroyed yet
		{
			std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
			if (gc_providers.size() == 1)
				CL_SharedGCData::dispose_objects();
			for (std::vector<CL_GraphicContextProvider*>::iterator it=gc_providers.begin(); it != gc_providers.end(); ++it)
			{
				if (destroyed_gc_provider == (*it))
				{
					gc_providers.erase(it);
					break;
				}
			}
		}

		// Destroy graphic context before the window is destroyed
		gc = CL_GraphicContext();

		// Delete the context
		if (wglGetCurrentContext() == opengl_context)
			CL_GL1::set_active(0);
		wglDeleteContext(opengl_context);

		opengl_context = 0;
	}
	if (device_context)
	{
		ReleaseDC(win32_window.get_hwnd(), device_context);
		device_context = 0;
		hwnd = 0;
	}
	if (shadow_window)
	{
		if (hwnd)
			DestroyWindow(hwnd);
		shadow_window = false;
	}

	hwnd = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_WGL Attributes:

// std::list<HGLRC> CL_GL1WindowProvider_WGL::opengl_contexts;

CL_Rect CL_GL1WindowProvider_WGL::get_geometry() const
{
	return win32_window.get_geometry();
}

CL_Rect CL_GL1WindowProvider_WGL::get_viewport() const
{
	return win32_window.get_viewport();
}

bool CL_GL1WindowProvider_WGL::is_fullscreen() const
{
	return fullscreen;
}

bool CL_GL1WindowProvider_WGL::has_focus() const
{
	return win32_window.has_focus();
}

bool CL_GL1WindowProvider_WGL::is_minimized() const
{
	return win32_window.is_minimized();
}

bool CL_GL1WindowProvider_WGL::is_maximized() const
{
	return win32_window.is_maximized();
}

bool CL_GL1WindowProvider_WGL::is_visible() const
{
	return win32_window.is_visible();
}

bool CL_GL1WindowProvider_WGL::is_clipboard_text_available() const
{
	return win32_window.is_clipboard_text_available();
}

bool CL_GL1WindowProvider_WGL::is_clipboard_image_available() const
{
	return win32_window.is_clipboard_image_available();
}

CL_Size CL_GL1WindowProvider_WGL::get_minimum_size(bool client_area) const
{
	return win32_window.get_minimum_size(client_area);
}

CL_Size CL_GL1WindowProvider_WGL::get_maximum_size(bool client_area) const
{
	return win32_window.get_maximum_size(client_area);
}

CL_String CL_GL1WindowProvider_WGL::get_title() const
{
	return win32_window.get_title();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_WGL Operations:

CL_Point CL_GL1WindowProvider_WGL::client_to_screen(const CL_Point &client)
{
	return win32_window.client_to_screen(client);
}

CL_Point CL_GL1WindowProvider_WGL::screen_to_client(const CL_Point &screen)
{
	return win32_window.screen_to_client(screen);
}

void CL_GL1WindowProvider_WGL::create_shadow_window(HWND wnd)
{
	WINDOWINFO window_info;
	memset(&window_info, 0, sizeof(WINDOWINFO));
	window_info.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(wnd, &window_info);
	if (window_info.dwStyle & WS_VISIBLE)
		window_info.dwStyle -= WS_VISIBLE;

	hwnd = CreateWindowEx(
		0, //window_info.dwExStyle,
		WC_STATIC, //(LPCTSTR) window_info.atomWindowType,
		TEXT(""),
		window_info.dwStyle,
		0, // window_info.rcWindow.left,
		0, // window_info.rcWindow.top,
		window_info.rcWindow.right - window_info.rcWindow.left,
		window_info.rcWindow.bottom - window_info.rcWindow.top,
		GetParent(wnd), 0, GetModuleHandle(0), 0);
	if (hwnd == 0)
		throw CL_Exception("Unable to create display window (opengl offscreen window)");

	shadow_window = true;
}

void CL_GL1WindowProvider_WGL::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc)
{
	site = new_site;
	fullscreen = desc.is_fullscreen();

	win32_window.create(site, desc);

	if (!opengl_context)
	{
		CL_GL1WindowDescription gldesc(desc);
		hwnd = win32_window.get_hwnd();
		dwm_layered = false;

		if (desc.is_layered() && !DwmFunctions::is_composition_enabled())
		{
			create_shadow_window(hwnd);
		}
		else 
		{
			if (desc.is_layered())
				dwm_layered = true;
		}
		device_context = GetDC(hwnd);

		HGLRC share_context = get_share_context();

		CL_GL1CreationHelper helper(hwnd, device_context);
		helper.set_multisampling_pixel_format(gldesc);
		opengl_context = helper.create_opengl3_context(share_context, 3, 0);
		if (!opengl_context)
			opengl_context = helper.create_opengl2_context(share_context);

		gc = CL_GraphicContext(new CL_GL1GraphicContextProvider(new CL_RenderWindowProvider_WGL(*this, opengl_context, false)));
		CL_SharedGCData::get_gc_providers().push_back(gc.get_provider());
	}

	wglSwapIntervalEXT = (ptr_wglSwapIntervalEXT)CL_GL1::get_proc_address("wglSwapIntervalEXT");
	swap_interval = desc.get_swap_interval();
	if (wglSwapIntervalEXT && swap_interval != -1)
		wglSwapIntervalEXT(swap_interval);
}

void CL_GL1WindowProvider_WGL::on_window_resized()
{
	if (shadow_window)
	{
		RECT rect = { 0,0,0,0 };
		GetClientRect(win32_window.get_hwnd(), &rect);
		SetWindowPos(hwnd, 0, 0, 0, rect.right, rect.bottom, SWP_NOREPOSITION|SWP_NOZORDER);
	}

	if (gc.get_provider())
		((CL_GL1GraphicContextProvider *) gc.get_provider())->on_window_resized();
}

void CL_GL1WindowProvider_WGL::show_system_cursor()
{
	win32_window.show_system_cursor();
}

CL_CursorProvider *CL_GL1WindowProvider_WGL::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	return new CL_CursorProvider_Win32(sprite_description, hotspot);
}

void CL_GL1WindowProvider_WGL::set_cursor(CL_CursorProvider *cursor)
{
	win32_window.set_cursor(static_cast<CL_CursorProvider_Win32 *>(cursor));
}

void CL_GL1WindowProvider_WGL::set_cursor(CL_StandardCursor type)
{
	win32_window.set_cursor(type);
}

void CL_GL1WindowProvider_WGL::hide_system_cursor()
{
	win32_window.hide_system_cursor();
}

void CL_GL1WindowProvider_WGL::set_title(const CL_StringRef &new_title)
{
	win32_window.set_title(new_title);
}

void CL_GL1WindowProvider_WGL::set_position(const CL_Rect &pos, bool client_area)
{
	win32_window.set_position(pos, client_area);
}

void CL_GL1WindowProvider_WGL::set_size(int width, int height, bool client_area)
{
	win32_window.set_size(width, height, client_area);
}

void CL_GL1WindowProvider_WGL::set_minimum_size( int width, int height, bool client_area )
{
	win32_window.set_minimum_size(width,height,client_area);
}

void CL_GL1WindowProvider_WGL::set_maximum_size( int width, int height, bool client_area )
{
	win32_window.set_maximum_size(width,height,client_area);
}

void CL_GL1WindowProvider_WGL::set_enabled(bool enable)
{
	win32_window.set_enabled(enable);
}

void CL_GL1WindowProvider_WGL::minimize()
{
	win32_window.minimize();
}

void CL_GL1WindowProvider_WGL::restore()
{
	win32_window.restore();
}

void CL_GL1WindowProvider_WGL::maximize()
{
	win32_window.maximize();
}

void CL_GL1WindowProvider_WGL::show(bool activate)
{
	win32_window.show(activate);
}

void CL_GL1WindowProvider_WGL::hide()
{
	win32_window.hide();
}

void CL_GL1WindowProvider_WGL::bring_to_front()
{
	win32_window.bring_to_front();
}

void CL_GL1WindowProvider_WGL::flip(int interval)
{
	CL_GL1::set_active(get_gc());

	if (shadow_window)
	{
		int width = get_viewport().get_width();
		int height = get_viewport().get_height();

		GLint old_viewport[4], old_matrix_mode;
		GLfloat old_matrix_projection[16], old_matrix_modelview[16];
		cl1GetIntegerv(GL_VIEWPORT, old_viewport);
		cl1GetIntegerv(GL_MATRIX_MODE, &old_matrix_mode);
		cl1GetFloatv(GL_PROJECTION_MATRIX, old_matrix_projection);
		cl1GetFloatv(GL_MODELVIEW_MATRIX, old_matrix_modelview);
		GLboolean blending = cl1IsEnabled(GL_BLEND);
		cl1Disable(GL_BLEND);

		cl1Viewport(0, 0, width, height);
		cl1MatrixMode(GL_PROJECTION);
		cl1LoadIdentity();
		cl1MultMatrixf(CL_Mat4f::ortho_2d(0.0f, (float)width, 0.0f, (float)height));
		cl1MatrixMode(GL_MODELVIEW);
		cl1LoadIdentity();

		cl1ReadBuffer(GL_BACK);
		cl1RasterPos2i(0, 0);
		cl1PixelZoom(1.0f, 1.0f);

		CL_PixelBuffer pixelbuffer(width, height, cl_rgba8);
		cl1PixelStorei(GL_PACK_ALIGNMENT, 1);
		cl1PixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer.get_pitch() / pixelbuffer.get_bytes_per_pixel());
		cl1PixelStorei(GL_PACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_PACK_SKIP_ROWS, 0);
		cl1ReadPixels(
			0, 0,
			width, height,
			GL_RGBA,
			GL_UNSIGNED_INT_8_8_8_8,
			pixelbuffer.get_data());

		win32_window.update_layered(pixelbuffer);

		if (blending)
			cl1Enable(GL_BLEND);
		cl1Viewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
		cl1MatrixMode(GL_PROJECTION);
		cl1LoadMatrixf(old_matrix_projection);
		cl1MatrixMode(GL_MODELVIEW);
		cl1LoadMatrixf(old_matrix_modelview);
		cl1MatrixMode(old_matrix_mode);
	}
	else
	{
		if (interval != -1 && interval != swap_interval)
		{
			swap_interval = interval;
			if (wglSwapIntervalEXT)
				wglSwapIntervalEXT(swap_interval);
		}

		BOOL retval = SwapBuffers(get_device_context());

		if (dwm_layered)
		{
			int width = get_viewport().get_width();
			int height = get_viewport().get_height();

			cl1ReadBuffer(GL_FRONT);

			CL_PixelBuffer pixelbuffer(width, height, cl_r8);
			cl1PixelStorei(GL_PACK_ALIGNMENT, 1);
			cl1PixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer.get_pitch() / pixelbuffer.get_bytes_per_pixel());
			cl1PixelStorei(GL_PACK_SKIP_PIXELS, 0);
			cl1PixelStorei(GL_PACK_SKIP_ROWS, 0);
			cl1ReadPixels(
				0, 0,
				width, height,
				GL_ALPHA,
				GL_BYTE, // use GL_BITMAP here for even less transfer?
				pixelbuffer.get_data());

			win32_window.update_layered(pixelbuffer);
		}

	}
}

void CL_GL1WindowProvider_WGL::update(const CL_Rect &_rect)
{
	int width = get_viewport().get_width();
	int height = get_viewport().get_height();

	CL_Rect rect = _rect;
	if (rect.left < 0)
		rect.left = 0;
	if (rect.top < 0)
		rect.top = 0;
	if (rect.right > width)
		rect.right = width;
	if (rect.bottom > height)
		rect.bottom = height;
	if (rect.right <= rect.left || rect.bottom <= rect.top)
		return;

	CL_GL1::set_active(gc);

	GLint old_viewport[4], old_matrix_mode;
	GLfloat old_matrix_projection[16], old_matrix_modelview[16];
	cl1GetIntegerv(GL_VIEWPORT, old_viewport);
	cl1GetIntegerv(GL_MATRIX_MODE, &old_matrix_mode);
	cl1GetFloatv(GL_PROJECTION_MATRIX, old_matrix_projection);
	cl1GetFloatv(GL_MODELVIEW_MATRIX, old_matrix_modelview);
	GLboolean blending = cl1IsEnabled(GL_BLEND);
	cl1Disable(GL_BLEND);

	cl1Viewport(0, 0, width, height);
	cl1MatrixMode(GL_PROJECTION);
	cl1LoadIdentity();
	cl1MultMatrixf(CL_Mat4f::ortho_2d(0.0f, (float)width, 0.0f, (float)height));
	cl1MatrixMode(GL_MODELVIEW);
	cl1LoadIdentity();

	if (shadow_window)
	{
		cl1ReadBuffer(GL_BACK);
		cl1RasterPos2i(0, 0);
		cl1PixelZoom(1.0f, 1.0f);

		// ** Currently update layered windows only supports full screen rect update **
		rect = CL_Rect(0,0, width, height);

		CL_PixelBuffer pixelbuffer(rect.get_width(), rect.get_height(), cl_rgba8);
		cl1PixelStorei(GL_PACK_ALIGNMENT, 1);
		cl1PixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer.get_pitch() / pixelbuffer.get_bytes_per_pixel());
		cl1PixelStorei(GL_PACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_PACK_SKIP_ROWS, 0);
		cl1ReadPixels(
			rect.left, height - rect.bottom,
			rect.right - rect.left, rect.bottom - rect.top,
			GL_RGBA,
			GL_UNSIGNED_INT_8_8_8_8,
			pixelbuffer.get_data());

		win32_window.update_layered(pixelbuffer);
	}
	else
	{
		GLboolean isdoublebuffered = GL_TRUE;
		cl1GetBooleanv(GL_DOUBLEBUFFER, &isdoublebuffered);
		if (isdoublebuffered)
		{
			cl1ReadBuffer(GL_BACK);
			cl1DrawBuffer(GL_FRONT);

			cl1RasterPos2i(rect.left, height - rect.bottom);
			cl1PixelZoom(1.0f, 1.0f);

			cl1CopyPixels(
				rect.left, height - rect.bottom,
				rect.right - rect.left, rect.bottom - rect.top,
				GL_COLOR);

			cl1DrawBuffer(GL_BACK);
			cl1Flush();
		}

		if (dwm_layered)
		{
			cl1DrawBuffer(GL_BACK);
			cl1ReadBuffer(GL_FRONT);

			// ** Currently update layered windows only supports full screen rect update **
			rect = CL_Rect(0,0, width, height);

			CL_PixelBuffer pixelbuffer(rect.get_width(), rect.get_height(), cl_r8);
			cl1PixelStorei(GL_PACK_ALIGNMENT, 1);
			cl1PixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer.get_pitch() / pixelbuffer.get_bytes_per_pixel());
			cl1PixelStorei(GL_PACK_SKIP_PIXELS, 0);
			cl1PixelStorei(GL_PACK_SKIP_ROWS, 0);
			cl1ReadPixels(
				rect.left, height - rect.bottom,
				rect.right - rect.left, rect.bottom - rect.top,
				GL_ALPHA,
				GL_BYTE, // use GL_BITMAP here for even less transfer?
				pixelbuffer.get_data());

			win32_window.update_layered(pixelbuffer);
		}

	}
	if (blending)
		cl1Enable(GL_BLEND);
	cl1Viewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
	cl1MatrixMode(GL_PROJECTION);
	cl1LoadMatrixf(old_matrix_projection);
	cl1MatrixMode(GL_MODELVIEW);
	cl1LoadMatrixf(old_matrix_modelview);
	cl1MatrixMode(old_matrix_mode);

}

void CL_GL1WindowProvider_WGL::capture_mouse(bool capture)
{
	win32_window.capture_mouse(capture);
}

void CL_GL1WindowProvider_WGL::set_clipboard_text(const CL_StringRef &text)
{
	win32_window.set_clipboard_text(text);
}

CL_String CL_GL1WindowProvider_WGL::get_clipboard_text() const
{
	return win32_window.get_clipboard_text();
}

void CL_GL1WindowProvider_WGL::request_repaint(const CL_Rect &rect)
{
	win32_window.request_repaint(rect);
}

void CL_GL1WindowProvider_WGL::set_large_icon(const CL_PixelBuffer &image)
{
	win32_window.set_large_icon(image);
}

void CL_GL1WindowProvider_WGL::set_small_icon(const CL_PixelBuffer &image)
{
	win32_window.set_small_icon(image);
}

CL_PBuffer_GL1 CL_GL1WindowProvider_WGL::create_pbuffer(CL_GL1GraphicContextProvider *gc_provider, CL_Size size)
{
	CL_PBuffer_GL1 pbuffer(gc_provider);

	pbuffer.create(*this, size);

	return pbuffer;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_WGL Implementation:

HGLRC CL_GL1WindowProvider_WGL::get_share_context()
{
	HGLRC share_context = 0;
	std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
	if (!gc_providers.empty())
	{
		CL_GL1GraphicContextProvider *gl_provider = dynamic_cast<CL_GL1GraphicContextProvider*>(gc_providers.back());
		if (gl_provider)
		{
			const CL_RenderWindowProvider *rwp = &gl_provider->get_render_window();
			const CL_RenderWindowProvider_WGL *render_window_wgl = dynamic_cast<const CL_RenderWindowProvider_WGL*>(rwp);
			if (render_window_wgl)
				share_context = render_window_wgl->wgl_context;
		}
	}
	return share_context;
}

void CL_GL1WindowProvider_WGL::set_clipboard_image( const CL_PixelBuffer &buf )
{
	win32_window.set_clipboard_image(buf);
}

CL_PixelBuffer CL_GL1WindowProvider_WGL::get_clipboard_image() const
{
	return win32_window.get_clipboard_image();
}

