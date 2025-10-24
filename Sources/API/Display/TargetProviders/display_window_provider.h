/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/System/weakptr.h"
#include "../../Core/Text/string_types.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/Signals/signal_v2.h"
#include "../../Core/Signals/callback_v1.h"
#include "../Window/display_window.h"

#ifndef WIN32
#include <X11/Xlib.h>
#endif

class CL_Point;
class CL_DisplayWindowDescription;
class CL_GraphicContext;
class CL_InputContext;
class CL_CursorProvider;
class CL_SpriteDescription;

/// \brief Display Window site.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_DisplayWindowSite
{
/// \name Attributes
/// \{
public:
	/// \brief Pointer to lost focus signal.
	CL_Signal_v0 *sig_lost_focus;

	/// \brief Pointer to got focus signal.
	CL_Signal_v0 *sig_got_focus;

	/// \brief Pointer to resize signal.
	CL_Signal_v2<int, int> *sig_resize;

	/// \brief Pointer to paint signal.
	CL_Signal_v1<const CL_Rect &> *sig_paint;

	/// \brief Pointer to window close signal.
	CL_Signal_v0 *sig_window_close;

	/// \brief Pointer to window destroy signal.
	CL_Signal_v0 *sig_window_destroy;

	/// \brief Pointer to window minimized signal.
	CL_Signal_v0 *sig_window_minimized;

	/// \brief Pointer to window maximized signal.
	CL_Signal_v0 *sig_window_maximized;

	/// \brief Pointer to window resize callback function.
	CL_Callback_v1<CL_Rect &> *func_window_resize;

	/// \brief Pointer to minimized clicked callback function.
	CL_Callback_0<bool> *func_minimize_clicked;

	/// \brief Pointer to window moved signal.
	CL_Signal_v0 *sig_window_moved;
/// \}
};

/// \brief Interface for implementing a CL_DisplayWindow target.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_DisplayWindowProvider
{
/// \name Construction
/// \{
public:
	virtual ~CL_DisplayWindowProvider() { return; }

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the position and size of the window frame.
	virtual CL_Rect get_geometry() const = 0;

	/// \brief Returns the drawable area of the window.
	virtual CL_Rect get_viewport() const = 0;

	/// \brief Returns true if window has focus.
	virtual bool has_focus() const = 0;

	/// \brief Returns true if the window is minimized.
	virtual bool is_minimized() const = 0;

	/// \brief Returns true if the window is maximized.
	virtual bool is_maximized() const = 0;

	/// \brief Returns true if the window is visible.
	virtual bool is_visible() const = 0;

	/// \brief Returns true if the window is fullscreen.
	virtual bool is_fullscreen() const = 0;

	/// \brief Returns the minimum size of the window.
	virtual CL_Size get_minimum_size(bool client_area) const = 0;

	/// \brief Returns the maximum size of the window.
	virtual CL_Size get_maximum_size(bool client_area) const = 0;

	/// \brief Returns the maximum size of the window.
	virtual CL_String get_title() const = 0;

	/// \brief Return the graphic context for the window.
	virtual CL_GraphicContext& get_gc() = 0;

	/// \brief Return the input context for the window.
	virtual CL_InputContext& get_ic() = 0;

#ifdef WIN32

	/// \brief Get Hwnd
	///
	/// \return hwnd
	virtual HWND get_hwnd() const = 0;
#else
	/// \brief Returns the X11 display handle.
	virtual Display *get_display() const = 0;

	/// \brief Handle to X11 window handle.
	virtual Window get_window() const = 0;
#endif

	/// \brief Returns true if text is available in the clipboard.
	virtual bool is_clipboard_text_available() const = 0;

	/// \brief Returns true if an image is available in the clipboard.
	virtual bool is_clipboard_image_available() const = 0;

	/// \brief Returns the text stored in the clipboard.
	virtual CL_String get_clipboard_text() const = 0;

	/// \brief Returns the image stored in the clipboard.
	virtual CL_PixelBuffer get_clipboard_image() const = 0;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Convert from window client coordinates to screen coordinates.
	virtual CL_Point client_to_screen(const CL_Point &client) = 0;

	/// \brief Convert from screen coordinates to client coordinates.
	virtual CL_Point screen_to_client(const CL_Point &screen) = 0;

	/// \brief Capture/Release the mouse.
	virtual void capture_mouse(bool capture) = 0;

	/// \brief Invalidates a region of a screen, causing a repaint.
	virtual void request_repaint(const CL_Rect &rect) = 0;

	/// \brief Destroys the display window provider.
	virtual void destroy() = 0;

	/// \brief Creates window, assigning site and description to provider.
	virtual void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description) = 0;

	/// \brief Shows the mouse cursor.
	virtual void show_system_cursor() = 0;

	/// \brief Creates a new custom cursor.
	virtual CL_CursorProvider *create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot) = 0;

	/// \brief Sets the current cursor icon.
	virtual void set_cursor(CL_CursorProvider *cursor) = 0;

	/// \brief Sets the current cursor icon.
	virtual void set_cursor(enum CL_StandardCursor type) = 0;

	/// \brief Hides the mouse cursor.
	virtual void hide_system_cursor() = 0;

	/// \brief Change window title.
	virtual void set_title(const CL_StringRef &new_title) = 0;

	/// \brief Set window position and size.
	virtual void set_position(const CL_Rect &pos, bool client_area) = 0;

	/// \brief Set size
	///
	/// \param width = value
	/// \param height = value
	/// \param client_area = bool
	virtual void set_size(int width, int height, bool client_area) = 0;

	/// \brief Set minimum size
	///
	/// \param width = value
	/// \param height = value
	/// \param client_area = bool
	virtual void set_minimum_size(int width, int height, bool client_area) = 0;

	/// \brief Set maximum size
	///
	/// \param width = value
	/// \param height = value
	/// \param client_area = bool
	virtual void set_maximum_size(int width, int height, bool client_area) = 0;

	/// \brief Enables or disables a window.
	virtual void set_enabled(bool enable) = 0;

	/// \brief Minimizes the window.
	virtual void minimize() = 0;

	/// \brief Restores the window.
	virtual void restore() = 0;

	/// \brief Maximizes the window.
	virtual void maximize() = 0;

	/// \brief Displays the window in its current size and position.
	virtual void show(bool activate) = 0;

	/// \brief Hides the window.
	virtual void hide() = 0;

	/// \brief Raise window on top of other windows.
	virtual void bring_to_front() = 0;

	/// \brief Flip opengl buffers.
	virtual void flip(int interval) = 0;

	/// \brief Copy a region of the backbuffer to the frontbuffer.
	virtual void update(const CL_Rect &rect) = 0;

	/// \brief Stores text in the clipboard.
	virtual void set_clipboard_text(const CL_StringRef &text) = 0;

	/// \brief Stores an image in the clipboard.
	virtual void set_clipboard_image(const CL_PixelBuffer &buf) = 0;

	/// \brief Sets the large icon used for this window.
	virtual void set_large_icon(const CL_PixelBuffer &image) = 0;

	/// \brief Sets the small icon used for this window.
	virtual void set_small_icon(const CL_PixelBuffer &image) = 0;

/// \}
/// \name Implementation
/// \{
private:
/// \}
};

/// \}
