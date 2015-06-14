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
**    Mark Page
*/

#include "GL/precomp.h"
#include "opengl_window_provider_android.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl_window_description.h"
#include "API/Core/Text/logger.h"
#include "../../opengl_window_description_impl.h"
#include "API/Display/Image/pixel_buffer.h"
#include "../../GL3/gl3_graphic_context_provider.h"
#include "../../GL1/gl1_graphic_context_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Construction:

	OpenGLWindowProvider::OpenGLWindowProvider(OpenGLWindowDescription &opengl_desc) : opengl_desc(opengl_desc)
	{

	}

	OpenGLWindowProvider::~OpenGLWindowProvider()
	{
		if (!gc.is_null())
		{
			if (using_gl3)
			{
				GL3GraphicContextProvider *gl_provider = dynamic_cast<GL3GraphicContextProvider*>(gc.get_provider());
				if (gl_provider)
					gl_provider->dispose();
			}
			else
			{
				GL1GraphicContextProvider *gl_provider = dynamic_cast<GL1GraphicContextProvider*>(gc.get_provider());
				if (gl_provider)
					gl_provider->dispose();
			}
		}


		if (display != EGL_NO_DISPLAY)
		{
			if (context != EGL_NO_CONTEXT && eglGetCurrentContext() == context)
				OpenGL::set_active(0);

			eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (context != EGL_NO_CONTEXT)
			{
				eglDestroyContext(display, context);
			}
			if (surface != EGL_NO_SURFACE)
			{
				eglDestroySurface(display, surface);
			}
			eglTerminate(display);
		}
		display = EGL_NO_DISPLAY;
		context = EGL_NO_CONTEXT;
		surface = EGL_NO_SURFACE;

	}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Attributes:

	Rect OpenGLWindowProvider::get_geometry() const
	{

		if (display != EGL_NO_DISPLAY && surface != EGL_NO_SURFACE)
		{
			EGLint w, h;
			eglQuerySurface(display, surface, EGL_WIDTH, &w);
			eglQuerySurface(display, surface, EGL_HEIGHT, &h);
			return Rect(0, 0, w, h);
		}
		return Rect();
	}

	Rect OpenGLWindowProvider::get_viewport() const
	{
		return get_geometry();
	}

	float OpenGLWindowProvider::get_pixel_ratio() const
	{
		return 0.0f;
	}

	bool OpenGLWindowProvider::is_fullscreen() const
	{
		return false;
	}

	bool OpenGLWindowProvider::has_focus() const
	{
		return false;
	}

	bool OpenGLWindowProvider::is_minimized() const
	{
		return false;
	}

	bool OpenGLWindowProvider::is_maximized() const
	{
		return false;
	}

	bool OpenGLWindowProvider::is_visible() const
	{
		return false;
	}

	Size OpenGLWindowProvider::get_minimum_size(bool client_area) const
	{
		return Size();
	}

	Size OpenGLWindowProvider::get_maximum_size(bool client_area) const
	{
		return Size();
	}

	std::string OpenGLWindowProvider::get_title() const
	{
		return std::string();
	}

	bool OpenGLWindowProvider::is_clipboard_text_available() const
	{
		return false;
	}

	bool OpenGLWindowProvider::is_clipboard_image_available() const
	{
		return false;
	}

	ProcAddress *OpenGLWindowProvider::get_proc_address(const std::string& function_name) const
	{
		return (void(*)())eglGetProcAddress(function_name.c_str());
	}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Operations:

	void OpenGLWindowProvider::make_current() const
	{
		if (display != EGL_NO_DISPLAY && surface != EGL_NO_SURFACE && context != EGL_NO_CONTEXT)
			eglMakeCurrent(display, surface, surface, context);
	}

	Point OpenGLWindowProvider::client_to_screen(const Point &client)
	{
		return client;
	}

	Point OpenGLWindowProvider::screen_to_client(const Point &screen)
	{
		return screen;
	}

	void OpenGLWindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
	{
		window_handle = desc.get_handle();
		if (window_handle.window == nullptr)
			throw Exception("Window handle must exist in the display description");

		ANativeWindow *window = window_handle.window;

		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};
		EGLint format;
		EGLint numConfigs;
		EGLConfig config;

		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(display, 0, 0);

		/* Here, the application chooses the configuration it desires. In this
		* sample, we have a very simplified selection process, where we pick
		* the first EGLConfig that matches our criteria */
		if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs))
			throw Exception("eglChooseConfig failed");
		if (numConfigs < 1)
			throw Exception("Found configs failed");

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		* As soon as we picked a EGLConfig, we can safely reconfigure the
		* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(window, 0, 0, format);

		surface = eglCreateWindowSurface(display, config, window, NULL);
		if (surface == EGL_NO_SURFACE)
			throw Exception("eglCreateWindowSurface failed");

		context = eglCreateContext(display, config, NULL, NULL);
		if (context == EGL_NO_CONTEXT)
			throw Exception("eglCreateWindowSurface failed");

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
		{
			throw Exception("Unable to eglMakeCurrent");
		}
		bool use_gl3;
		int desc_version_major = opengl_desc.get_version_major();
		int desc_version_minor = opengl_desc.get_version_minor();

		// Do not attempt GL3, if not requested that version
		if (desc_version_major < 3)
		{
			use_gl3 = false;
		}
		else if (!opengl_desc.get_allow_lower_versions())	// Else, if we do not allow lower versions, only attempt GL3
		{
			use_gl3 = true;
		}
		else
		{
			// Choose the target depending on the current opengl version
			int gl_version_major;
			int gl_version_minor;
			get_opengl_version(gl_version_major, gl_version_minor);
			if (gl_version_major < 3)
			{
				use_gl3 = false;
			}
			else
			{
				use_gl3 = true;
			}

		}

		if (use_gl3)
		{
			using_gl3 = true;
			gc = GraphicContext(new GL3GraphicContextProvider(this));
		}
		else
		{
			using_gl3 = false;
			gc = GraphicContext(new GL1GraphicContextProvider(this));
		}
		swap_interval = desc.get_swap_interval();
		if (swap_interval != -1)
			eglSwapInterval(display, swap_interval);

	}

	void OpenGLWindowProvider::show_system_cursor()
	{
	}

	CursorProvider *OpenGLWindowProvider::create_cursor(const CursorDescription &cursor_description)
	{
		throw Exception("Implement me");
	}

	void OpenGLWindowProvider::set_cursor(CursorProvider *cursor)
	{
	}

	void OpenGLWindowProvider::set_cursor(StandardCursor type)
	{
	}

	void OpenGLWindowProvider::hide_system_cursor()
	{
	}

	void OpenGLWindowProvider::set_title(const std::string &new_title)
	{
	}

	void OpenGLWindowProvider::set_position(const Rect &pos, bool client_area)
	{
	}

	void OpenGLWindowProvider::set_size(int width, int height, bool client_area)
	{
	}

	void OpenGLWindowProvider::set_minimum_size(int width, int height, bool client_area)
	{
	}

	void OpenGLWindowProvider::set_maximum_size(int width, int height, bool client_area)
	{
	}

	void OpenGLWindowProvider::set_enabled(bool enable)
	{
	}

	void OpenGLWindowProvider::minimize()
	{
	}

	void OpenGLWindowProvider::restore()
	{
	}

	void OpenGLWindowProvider::maximize()
	{
	}

	void OpenGLWindowProvider::toggle_fullscreen()
	{
	}

	void OpenGLWindowProvider::show(bool activate)
	{
	}

	void OpenGLWindowProvider::hide()
	{
	}

	void OpenGLWindowProvider::bring_to_front()
	{
	}

	void OpenGLWindowProvider::flip(int interval)
	{
		if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE)
			return;
			
		OpenGL::set_active(get_gc());
		glFlush();

		if (interval != -1 && interval != swap_interval)
		{
			swap_interval = interval;
			eglSwapInterval(display, swap_interval);
		}

		eglSwapBuffers(display, surface);
		OpenGL::check_error();
	}

	void OpenGLWindowProvider::update(const Rect &_rect)
	{
		// Implement me, if possible
		flip(0);
	}

	void OpenGLWindowProvider::capture_mouse(bool capture)
	{
	}

	void OpenGLWindowProvider::set_clipboard_text(const std::string &text)
	{
	}

	std::string OpenGLWindowProvider::get_clipboard_text() const
	{
		return std::string();
	}

	void OpenGLWindowProvider::request_repaint(const Rect &rect)
	{
	}

	void OpenGLWindowProvider::set_large_icon(const PixelBuffer &image)
	{
	}

	void OpenGLWindowProvider::set_small_icon(const PixelBuffer &image)
	{
	}

	void OpenGLWindowProvider::enable_alpha_channel(const Rect &blur_rect)
	{
	}

	void OpenGLWindowProvider::extend_frame_into_client_area(int left, int top, int right, int bottom)
	{
	}

	void OpenGLWindowProvider::set_clipboard_image(const PixelBuffer &buf)
	{
	}

	PixelBuffer OpenGLWindowProvider::get_clipboard_image() const
	{
		return PixelBuffer();
	}

	void OpenGLWindowProvider::set_pixel_ratio(float ratio)
	{
	}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Implementation:

	void OpenGLWindowProvider::get_opengl_version(int &version_major, int &version_minor)
	{
		make_current();
#undef glGetString
		std::string version = (char*)::glGetString(GL_VERSION);

		version_major = 0;
		version_minor = 0;

		std::vector<std::string> split_version = StringHelp::split_text(version, ".");
		if (split_version.size() > 0)
			version_major = StringHelp::text_to_int(split_version[0]);
		if (split_version.size() > 1)
			version_minor = StringHelp::text_to_int(split_version[1]);

	}

}
