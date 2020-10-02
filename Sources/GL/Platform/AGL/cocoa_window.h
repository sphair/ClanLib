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
*/

#pragma once

#ifdef __OBJC__

#include <list>
#include <map>
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/System/cl_platform.h"
#include "API/Core/Signals/signal.h"
#include "cocoa_controller.h"
#include <QuartzCore/CAEAGLLayer.h>

namespace clan
{

class InputDeviceProvider_CocoaKeyboard;
class InputDeviceProvider_CocoaMouse;
class DisplayMessageQueue_Cocoa;
class DisplayWindowSite;
class DisplayWindowDescription;
class PixelBuffer;
class DataBuffer;
class CursorProvider_Cocoa;
class InputEvent;
class Colorf;
class CocoaMouseInputDeviceProvider;

class CocoaWindow
{
public:
	CocoaWindow();
	~CocoaWindow();

public:
	Rect get_geometry() const;
	Rect get_viewport() const;
	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
    bool is_fullscreen() const;
	bool is_visible() const;
	Size get_minimum_size(bool client_area) const;
	Size get_maximum_size(bool client_area) const;
	std::string get_title() const;
	std::function<void()> &func_on_resized() { return callback_on_resized; }
	InputContext get_ic() { return ic; }
	const InputContext get_ic() const { return ic; }
	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;

    CAEAGLLayer *get_drawable();

public:
	void create(DisplayWindowSite *site, const DisplayWindowDescription &description);

	Point client_to_screen(const Point &client);
	Point screen_to_client(const Point &screen);

	void show_system_cursor();
	void set_cursor(CursorProvider_Cocoa *cursor);
	void set_cursor(enum StandardCursor type);
	void hide_system_cursor();

	void set_large_icon(const PixelBuffer &image);
	void set_small_icon(const PixelBuffer &image);

	void set_title(const std::string &new_title);
	void set_enabled(bool enable);

	void set_position(const Rect &pos, bool client_area);
	void set_size(int width, int height, bool client_area);
	void set_minimum_size(int width, int height, bool client_area);
	void set_maximum_size(int width, int height, bool client_area);
	void minimize();
	void restore();
	void maximize();

	void show(bool activate);
	void hide();

	void bring_to_front();

	void capture_mouse(bool capture);

	void set_clipboard_text(const std::string &text);
	std::string get_clipboard_text() const;
	PixelBuffer get_clipboard_image() const;
	void set_clipboard_image(const PixelBuffer &image);
	void request_repaint(const Rect &rect);
	void set_modifier_keys(InputEvent &key);

    void on_touches_began(const std::vector<Vec2f> &touches);
    void on_touches_moved(const std::vector<Vec2f> &touches);
    void on_touches_ended(const std::vector<Vec2f> &touches);
    void on_touches_cancelled(const std::vector<Vec2f> &touches);

private:
    CocoaMouseInputDeviceProvider *get_mouse();

	DisplayWindowSite *site;
	InputDevice keyboard, mouse;
	Point mouse_pos;
	std::map<int,int> repeat_count;
	std::function<void()> callback_on_resized;
	InputContext ic;
    UIWindow *window;
    CocoaController *controller;
};

}

#endif
