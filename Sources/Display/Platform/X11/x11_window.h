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
**    Chu Chin Kuan
*/

#pragma once

#include <cmath>
#include <list>
#include <map>
#include "API/Core/Signals/signal.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/cursor.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "clipboard_x11.h"
#include "x11_atoms.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xinerama.h>

#include <sys/time.h>

namespace clan
{
	class InputDeviceProvider_X11Keyboard;
	class InputDeviceProvider_X11Mouse;
	class InputDeviceProvider_LinuxJoystick;
	class DisplayMessageQueue_X11;
	class Rect;
	class DisplayWindowSite;
	class DisplayWindowDescription;
	class CursorProvider_X11;

	class X11Window
	{
	public:
		X11Window();
		~X11Window();

		Rect get_geometry() const;
		Rect get_viewport() const;

		float get_ppi() const { return ppi; }
		float get_pixel_ratio() const { return pixel_ratio; }

		bool has_focus() const;
		bool is_minimized() const;
		bool is_maximized() const;
		bool is_visible() const;
		bool is_fullscreen() const { return fullscreen; }

		Size get_minimum_size(bool client_area) const;
		Size get_maximum_size(bool client_area) const;

		std::string get_title() const { return window_title; }

		DisplayWindowHandle get_handle() const { return handle; }
		::Display *get_display() const { return handle.display; }
		::Window get_window() const { return handle.window; }
		int get_screen() const { return handle.screen; }

		InputDevice &get_keyboard() { return keyboard; }
		InputDevice &get_mouse() { return mouse; }
		std::vector<InputDevice> &get_game_controllers() { return joysticks; }

		std::function<void()> &func_on_resized() { return callback_on_resized; }
		std::function<bool(XButtonEvent &)> &func_on_clicked() { return callback_on_clicked; }

		bool is_clipboard_text_available() const;
		bool is_clipboard_image_available() const;

		std::string get_clipboard_text() const;
		PixelBuffer get_clipboard_image() const;

		const std::vector<int> &get_window_socket_messages() const;

		Point client_to_screen(const Point &client);
		Point screen_to_client(const Point &screen);

		void capture_mouse(bool capture);

		void request_repaint();

		void create(XVisualInfo *visual, DisplayWindowSite *site, const DisplayWindowDescription &description);

		void show_system_cursor();
		void hide_system_cursor();
		void set_cursor(CursorProvider_X11 *cursor);
		void set_cursor(enum StandardCursor type);

		void set_title(const std::string &new_title);
		void set_position(const Rect &pos, bool client_area);
		void set_size(int width, int height, bool client_area);
		void set_minimum_size(int width, int height, bool client_area);
		void set_maximum_size(int width, int height, bool client_area);

		void set_pixel_ratio(float ratio);

		void set_enabled(bool enable);
		void minimize();
		void restore();
		void maximize();
		void show(bool activate);
		void hide();
		void bring_to_front();

		void set_clipboard_text(const std::string &text);
		void set_clipboard_image(const PixelBuffer &buf);

		void set_large_icon(const PixelBuffer &image);
		void set_small_icon(const PixelBuffer &image);

		void process_message(XEvent &event, X11Window *mouse_capture_window);
		void process_window();

		void get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const;
		Point get_mouse_position() const;

		void toggle_fullscreen();

	private:
		void process_window_sockets();
		void process_window_resize(const Rect &new_rect);
		void update_frame_extents();
		void map_window();
		void unmap_window();
		Rect get_screen_position() const;
		void setup_swap_interval_pointers();
		void setup_joysticks();
		void close_window();
		void received_keyboard_input(XKeyEvent &event);
		void received_mouse_input(XButtonEvent &event);
		void received_mouse_move(XMotionEvent &event);
		void clear_structurenotify_events();

		bool modify_net_wm_state(bool add, Atom atom1, Atom atom2 = None);
		InputDeviceProvider_X11Keyboard *get_keyboard_provider() const;
		InputDeviceProvider_X11Mouse *get_mouse_provider() const;

		DisplayWindowHandle handle;
		Colormap color_map; //!< X11 window color-map. See usage in .cpp file.
		bool minimized; //!< Cached value. Do not use without calling is_minimized() to verify.
		bool maximized; //!< Cached value. Do not use without calling is_maximized() to verify
		bool restore_to_maximized; //!< When restoring from minimized, make window maximized?
		bool fullscreen;
		::Cursor system_cursor;
		::Cursor hidden_cursor;
		Pixmap cursor_bitmap;
		InputDevice keyboard, mouse;
		std::vector<InputDevice> joysticks;
		DisplayWindowSite *site;
		std::function<void()> callback_on_resized;
		std::function<bool(XButtonEvent &)> callback_on_clicked;
		Size minimum_size; //!< Minimum client area size.
		Size maximum_size; //!< Maximum client area size.
		std::string window_title;
		bool resize_allowed; //!< Is the WM allowed to resize this window?
		Clipboard_X11 clipboard;
		std::vector<int> current_window_events;
		bool is_window_mapped;
		XSizeHints *size_hints;

		X11Atoms atoms; //!< X11 Atom object container.

		//! Current window client area, which excludes the window frame.
		Rect client_area;

		//! Window frame extents. Lengths on each side of the window used by the WM
		//! to decorate the window. Never use size-related methods on this object.
		Rect frame_extents;

		/**
		 * True when there is a repaint requests.
		 * Used to call site->sig_paint().
		 * Cleared on process_window().
		 */
		bool repaint_request = false;

		float ppi = 96.0f;
		float pixel_ratio = 0.0f;	// 0.0f = Unset

		Rect initial_position;
	};
}
