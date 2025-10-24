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

#ifndef header_display_window_generic
#define header_display_window_generic

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/display_window.h"
#include "API/Display/sprite.h"

class CL_DisplayWindowDescription;
class CL_DisplayMode;

class CL_API_DISPLAY CL_DisplayWindow_Generic
{
//! Construction:
public:
	CL_DisplayWindow_Generic();

	virtual ~CL_DisplayWindow_Generic();

//! Attributes:
public:
	virtual int get_width() const=0;

	virtual int get_height() const=0;

	virtual bool is_fullscreen() const=0;

	virtual bool has_focus() const=0;

	virtual CL_PixelBuffer get_buffer(int i) const=0;

	virtual int get_buffer_count() const=0;

	virtual CL_GraphicContext *get_gc()=0;

	virtual CL_InputContext *get_ic()=0;

	//: CL_Display's 'current window' reference.
	static CL_DisplayWindow *current;

//! Operations:
public:
	void add_ref();

	void release_ref();

	virtual void create_window(const CL_DisplayWindowDescription &desc)=0;

	virtual void set_fullscreen(int width, int height, int bpp, int refresh_rate)=0;

	virtual void set_windowed()=0;

	virtual void set_title(const std::string &title)=0;

	virtual void set_position(const CL_Rect &pos)=0;

	virtual void set_position(int x, int y)=0;

	virtual void set_size(int width, int height)=0;

	virtual void set_buffer_count(int flipping_buffers)=0;

	virtual void update(const CL_Rect &rect)=0;

	virtual void flip(int interval)=0;

	virtual void show_system_cursor()=0;

	virtual void hide_system_cursor()=0;

	void do_flip(int interval);

	void set_cursor(const CL_Sprite &sprite);

	void show_cursor();

	void hide_cursor();

//! Signals:
public:
	CL_Signal_v2<int, int> sig_resize;

	CL_Signal_v1<const CL_Rect &> sig_paint;

	CL_Signal_v0 sig_lost_focus;

	CL_Signal_v0 sig_got_focus;

	CL_Signal_v0 sig_window_close;

//! Implementation:
private:
	virtual void cursor_changed() { return; }

	CL_Sprite cursor;

	bool cursor_visible;

	int ref_count;
};

#endif
