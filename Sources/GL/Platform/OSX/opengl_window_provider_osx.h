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

#pragma once

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/GL/opengl_window_description.h"
#include "API/GL/opengl_wrap.h"
#include <memory>

namespace clan
{
	class InputDeviceProvider_OSXKeyboard;
	class InputDeviceProvider_OSXMouse;
	class OpenGLWindowDescription;
	class OpenGLWindowProvider_Impl;

	class OpenGLWindowProvider : public DisplayWindowProvider
	{
	/// \name Construction
	/// \{
	public:
		OpenGLWindowProvider(OpenGLWindowDescription &opengl_desc);
		~OpenGLWindowProvider();
	/// \}

	/// \name Attributes
	/// \{
	public:
		Rect get_geometry() const;
		Rect get_viewport() const;
		bool is_fullscreen() const;
		bool has_focus() const;
		bool is_minimized() const;
		bool is_maximized() const;
		bool is_visible() const;
		std::string get_title() const;
		Size get_minimum_size(bool client_area) const;
		Size get_maximum_size(bool client_area) const;
		DisplayWindowHandle get_handle() const override { DisplayWindowHandle handle; return handle; }
		GraphicContext& get_gc();
		InputContext get_ic();
		bool is_clipboard_text_available() const;
		bool is_clipboard_image_available() const;
		std::string get_clipboard_text() const;
		PixelBuffer get_clipboard_image() const;
		bool is_double_buffered() const;
		float get_pixel_ratio() const override;
	/// \}

	/// \name Operations
	/// \{
	public:
		void make_current() const;
		Point client_to_screen(const Point &client);
		Point screen_to_client(const Point &screen);
		void create(DisplayWindowSite *site, const DisplayWindowDescription &description);

		void show_system_cursor();
		CursorProvider *create_cursor(const CursorDescription &cursor_description);
		void set_cursor(CursorProvider *cursor);
		void set_cursor(StandardCursor type);
		void hide_system_cursor();
		void set_title(const std::string &new_title);
		void set_position(const Rect &pos, bool client_area);
		void set_size(int width, int height, bool client_area);
		void set_minimum_size(int width, int height, bool client_area);
		void set_maximum_size( int width, int height, bool client_area);
		void set_enabled(bool enable);
		void minimize();
		void restore();
		void maximize();
		void toggle_fullscreen();
		void show(bool activate);
		void hide();
		void bring_to_front();
		void flip(int interval);
		void capture_mouse(bool capture);
		void set_clipboard_text(const std::string &text);
		void set_clipboard_image(const PixelBuffer &buf);
		void request_repaint(const Rect &rect);
		void set_large_icon(const PixelBuffer &image);
		void set_small_icon(const PixelBuffer &image);

		void enable_alpha_channel(const Rect &blur_rect);
		void extend_frame_into_client_area(int left, int top, int right, int bottom);

		ProcAddress *get_proc_address(const std::string& function_name) const;
		
		void set_pixel_ratio(float ratio) override;
	/// \}

	/// \name Implementation
	/// \{
	private:
		InputDevice keyboard, mouse;

		InputDeviceProvider_OSXKeyboard *get_keyboard();
		InputDeviceProvider_OSXMouse *get_mouse();

		std::unique_ptr<OpenGLWindowProvider_Impl> impl;
		friend class OpenGLWindowProvider_Impl;
	/// \}
	};
}
