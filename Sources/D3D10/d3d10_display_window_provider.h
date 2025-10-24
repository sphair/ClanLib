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


#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"

class CL_D3D10DisplayWindowProvider : public CL_DisplayWindowProvider
{
/// \name Construction
/// \{

public:
	CL_D3D10DisplayWindowProvider();
	~CL_D3D10DisplayWindowProvider();


/// \}
/// \name Attributes
/// \{

public:
	CL_Rect get_geometry() const;
	CL_Rect get_viewport() const;
	bool has_focus() const;
	bool is_minimized() const;
	bool is_maximized() const;
	bool is_visible() const;
	CL_GraphicContext& get_gc();
	CL_InputContext& get_ic();
	CL_String get_title() const;
	CL_Size get_minimum_size(bool client_area) const;
	CL_Size get_maximum_size(bool client_area) const;
	HWND get_hwnd() const;
	bool is_clipboard_text_available() const;
	bool is_clipboard_image_available() const;
	CL_String get_clipboard_text() const;
	CL_PixelBuffer get_clipboard_image() const;
	bool is_fullscreen() const { return false; }

/// \}
/// \name Operations
/// \{

public:
	CL_Point client_to_screen(const CL_Point &client);
	CL_Point screen_to_client(const CL_Point &screen);
	void capture_mouse(bool capture);
	void destroy();
	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);
	void show_system_cursor();
	CL_CursorProvider *create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot);
	void set_cursor(CL_CursorProvider *cursor);
	void set_cursor(CL_StandardCursor type);
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
	void flip(int interval);
	void update(const CL_Rect &rect);
	void set_clipboard_text(const CL_StringRef &text);
	void set_clipboard_image(const CL_PixelBuffer &buf);
	void request_repaint(const CL_Rect &rect);
	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);

/// \}
/// \name Implementation
/// \{

private:
	CL_GraphicContext gc;
	CL_InputContext ic;
/// \}
};
