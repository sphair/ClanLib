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
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include <memory>
#include "API/GL/opengl_context_description.h"
#include "API/GL/opengl_wrap.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace clan
{

	class OpenGLContextDescription;

	class OpenGLWindowProvider : public DisplayWindowProvider
	{
/// \name Construction
/// \{

	public:
		OpenGLWindowProvider(OpenGLContextDescription &opengl_desc);
		~OpenGLWindowProvider();


/// \}
/// \name Attributes
/// \{

	public:
		Rect get_geometry() const override;
		Rect get_viewport() const override;
		bool is_fullscreen() const override;
		bool has_focus() const override;
		bool is_minimized() const override;
		bool is_maximized() const override;
		bool is_visible() const override;
		std::string get_title() const override;
		Size get_minimum_size(bool client_area) const override;
		Size get_maximum_size(bool client_area) const override;
		DisplayWindowHandle get_handle() const override { return DisplayWindowHandle(); }
		GraphicContext& get_gc() override { return gc; }
		bool is_clipboard_text_available() const override;
		bool is_clipboard_image_available() const override;
		std::string get_clipboard_text() const override;
		PixelBuffer get_clipboard_image() const override;
		float get_pixel_ratio() const override;
		ProcAddress *get_proc_address(const std::string& function_name) const;
		bool is_double_buffered() const { return double_buffered; }

		EGLDisplay get_display() const { return display; }
		EGLContext get_context() const { return context; }

/// \}
/// \name Operations
/// \{

	public:
		void make_current() const;
		Point client_to_screen(const Point &client) override;
		Point screen_to_client(const Point &screen) override;
		void create(DisplayWindowSite *site, const DisplayWindowDescription &description) override;

		void show_system_cursor() override;
		CursorProvider *create_cursor(const CursorDescription &cursor_description) override;
		void set_cursor(CursorProvider *cursor) override;
		void set_cursor(StandardCursor type) override;
		void hide_system_cursor() override;
		void set_title(const std::string &new_title) override;
		void set_position(const Rect &pos, bool client_area) override;
		void set_size(int width, int height, bool client_area) override;
		void set_minimum_size(int width, int height, bool client_area) override;
		void set_maximum_size(int width, int height, bool client_area) override;
		void set_enabled(bool enable) override;
		void minimize() override;
		void restore() override;
		void maximize() override;
		void toggle_fullscreen() override;
		void show(bool activate) override;
		void hide() override;
		void bring_to_front() override;

		/// \brief Flip OpenGL buffers.
		void flip(int interval) override;

		/// \brief Capture/Release the mouse.
		void capture_mouse(bool capture) override;

		/// \brief Stores text in the clipboard.
		void set_clipboard_text(const std::string &text) override;

		void set_clipboard_image(const PixelBuffer &buf) override;

		/// \brief Invalidates a region of a screen, causing a repaint.
		void request_repaint() override;

		void set_large_icon(const PixelBuffer &image) override;
		void set_small_icon(const PixelBuffer &image) override;

		void enable_alpha_channel(const Rect &blur_rect) override;
		void extend_frame_into_client_area(int left, int top, int right, int bottom) override;

		void set_pixel_ratio(float ratio) override;

		InputDevice &get_keyboard() override;
		InputDevice &get_mouse() override;
		std::vector<InputDevice> &get_game_controllers() override;

/// \}
/// \name Implementation
/// \{

	private:
		void get_opengl_version(int &version_major, int &version_minor);

		GraphicContext gc;
		bool using_gl3 = false;
		bool double_buffered = false;
		int swap_interval = 0;

		OpenGLContextDescription opengl_desc;
		DisplayWindowHandle window_handle;
		EGLDisplay display = EGL_NO_DISPLAY;
		EGLSurface surface = EGL_NO_SURFACE;
		EGLContext context = EGL_NO_CONTEXT;
/// \}
	};

}
