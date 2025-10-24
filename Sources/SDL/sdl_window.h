/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#pragma once

#include <list>
#include <map>
#include "API/Display/api_display.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/timer.h"
#include "API/Core/System/sharedptr.h"
#include "API/Core/Math/point.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/display_window_message.h"
#include <SDL/SDL.h>

class CL_InputDeviceProvider_SDLKeyboard;
class CL_InputDeviceProvider_SDLMouse;
class CL_InputDeviceProvider_SDLJoystick;
class CL_DisplayMessageQueue_SDL;
class CL_TimerProvider;
class CL_DisplayWindowSite;
class CL_DisplayWindowDescription;
class CL_PixelBuffer;
class CL_DataBuffer;
class CL_CursorProvider_SDL;
class CL_TimerProvider_SDL;

class CL_API_DISPLAY CL_SDLWindow
{
public:
	CL_SDLWindow(CL_DisplayMessageQueue_SDL *message_queue);
	~CL_SDLWindow();

public:
	SDL_Surface *get_screen() {return screen; }

	CL_Rect get_geometry() const;
	CL_Rect get_viewport() const;
	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
	bool is_visible() const;
	CL_Callback_v0 &func_on_resized() { return callback_on_resized; }
	CL_InputContext &get_ic() { return ic; }
	const CL_InputContext &get_ic() const { return ic; }
	bool is_clipboard_text_available() const;
	bool is_painting() const { return 0; }
	bool is_layered() const { return layered; }

public:
	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);
	CL_Point client_to_screen(const CL_Point &client);
	CL_Point screen_to_client(const CL_Point &screen);
	void show_system_cursor();
	void set_cursor(CL_CursorProvider_SDL *cursor);
	void set_cursor(enum CL_StandardCursor type);
	void hide_system_cursor();
	void set_title(const CL_StringRef &new_title);
	void set_position(const CL_Rect &pos, bool client_area);
	void set_size(int width, int height, bool client_area);
	void set_minimum_size(int width, int height, bool client_area);
	void set_maximum_size(int width, int height, bool client_area);
	void set_enabled(bool enable);
	void minimize();
	void restore();
	void maximize();
	void show(bool activate);
	void hide();
	void bring_to_front();
	void capture_mouse(bool capture);
	void set_timer(CL_TimerProvider *timer);
	void kill_timer(CL_TimerProvider *timer);
	void set_clipboard_text(const CL_StringRef &text);
	CL_String get_clipboard_text() const;
	void set_clipboard_image(const CL_PixelBuffer &image);
	static CL_PixelBuffer create_bitmap_data(const CL_PixelBuffer &image);
	void invalidate_rect(const CL_Rect &rect);

	bool has_messages();
	bool get_message(SDL_Event &clan_event);

	void flip();

private:
	void destroy();
	void set_windowed();
	bool check_timers(void);
	void process_timer_events();
	void received_keyboard_input(SDL_Event &event);

	CL_DisplayWindowSite *site;

	SDL_Surface *screen;
	bool fullscreen;

	CL_InputDeviceProvider_SDLKeyboard *keyboard;
	CL_InputDeviceProvider_SDLMouse *mouse;
	CL_InputDeviceProvider_SDLJoystick *joystick;
	CL_DisplayMessageQueue_SDL *message_queue;
	std::list<CL_TimerProvider_SDL *> timer_list;
	CL_Callback_v0 callback_on_resized;
	CL_InputContext ic;
	CL_Size minimum_size;
	CL_Size maximum_size;
	bool layered;

	friend class CL_InputDeviceProvider_SDLKeyboard;
};
