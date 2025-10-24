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
*/

#pragma once

#ifdef __OBJC__

#include <list>
#include <map>
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Core/System/sharedptr.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/System/cl_platform.h"
#include "API/Core/Signals/callback_v0.h"
#include "cocoa_controller.h"
#include <QuartzCore/CAEAGLLayer.h>

class CL_InputDeviceProvider_CocoaKeyboard;
class CL_InputDeviceProvider_CocoaMouse;
class CL_DisplayMessageQueue_Cocoa;
class CL_DisplayWindowSite;
class CL_DisplayWindowDescription;
class CL_PixelBuffer;
class CL_DataBuffer;
class CL_CursorProvider_Cocoa;
class CL_InputEvent;
class CL_Colorf;
class CL_CocoaMouseInputDeviceProvider;

class CL_CocoaWindow
{
public:
	CL_CocoaWindow();
	~CL_CocoaWindow();

public:
	CL_Rect get_geometry() const;
	CL_Rect get_viewport() const;
	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
    bool is_fullscreen() const;
	bool is_visible() const;
	CL_Size get_minimum_size(bool client_area) const;
	CL_Size get_maximum_size(bool client_area) const;
	CL_String get_title() const;
	CL_Callback_v0 &func_on_resized() { return callback_on_resized; }
	CL_InputContext &get_ic() { return ic; }
	const CL_InputContext &get_ic() const { return ic; }
	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;
    
    CAEAGLLayer *get_drawable();

public:
	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);

	CL_Point client_to_screen(const CL_Point &client);
	CL_Point screen_to_client(const CL_Point &screen);

	void show_system_cursor();
	void set_cursor(CL_CursorProvider_Cocoa *cursor);
	void set_cursor(enum CL_StandardCursor type);
	void hide_system_cursor();

	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);

	void set_title(const CL_StringRef &new_title);
	void set_enabled(bool enable);

	void set_position(const CL_Rect &pos, bool client_area);
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

	void set_clipboard_text(const CL_StringRef &text);
	CL_String get_clipboard_text() const;
	CL_PixelBuffer get_clipboard_image() const;
	void set_clipboard_image(const CL_PixelBuffer &image);
	void request_repaint(const CL_Rect &rect);
	void set_modifier_keys(CL_InputEvent &key);
    
    void on_touches_began(const std::vector<CL_Vec2f> &touches);
    void on_touches_moved(const std::vector<CL_Vec2f> &touches);
    void on_touches_ended(const std::vector<CL_Vec2f> &touches);
    void on_touches_cancelled(const std::vector<CL_Vec2f> &touches);

private:
    CL_CocoaMouseInputDeviceProvider *get_mouse();
    
	CL_DisplayWindowSite *site;
	CL_InputDevice keyboard, mouse;
	CL_Point mouse_pos;
	std::map<int,int> repeat_count;
	CL_Callback_v0 callback_on_resized;
	CL_InputContext ic;
    UIWindow *window;
    CL_CocoaController *controller;
};

#endif
