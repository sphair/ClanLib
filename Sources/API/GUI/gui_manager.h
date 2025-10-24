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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{


#pragma once


#include "api_gui.h"
#include "../Core/System/sharedptr.h"
#include "../Core/System/weakptr.h"
#include "../Core/Signals/callback_1.h"
#include "../Core/Signals/callback_2.h"
#include "../Core/Math/point.h"
#include "accelerator_table.h"
#include "../Display/Window/timer.h"

class CL_Size;
class CL_Rect;
class CL_CSSDocument;
class CL_GUIComponent;
class CL_GUIMessage;
class CL_GUITheme;
class CL_GUIWindowManager;
class CL_GUIManager_Impl;
class CL_Font;
class CL_FontDescription;

/// \brief GUI manager.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIManager
{
/// \name Construction
/// \{

public:
	CL_GUIManager();

	CL_GUIManager(CL_SharedPtr<CL_GUIManager_Impl> impl);

	virtual ~CL_GUIManager();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the parent is the GUI manager.
	bool is_gui_manager() const { return true; }

	/// \brief Returns the GUI theme being used.
	CL_GUITheme *get_theme() const;

	/// \brief Returns the CSS document being used.
	CL_CSSDocument get_css_document() const;

	/// \brief Returns the windows manager being used.
	CL_GUIWindowManager *get_window_manager() const;

	/// \brief Returns the mouse capture component
	CL_GUIComponent *get_capture_component() const;

	/// \brief Returns the currently focused component.
	CL_GUIComponent *get_focused_component();

	/// \brief Returns if a dialog message pump loop should exit.
	bool get_exit_flag() const;

	/// \brief Returns the dialog exit code.
	int get_exit_code() const;

	/// \brief Returns userdata.
	CL_UnknownSharedPtr get_userdata();

	/// \brief Get the shared user defined GUI font (set with set_named_font() ). Returns NULL if not found
	CL_Font get_named_font(const CL_FontDescription &desc);

	CL_String get_clipboard_text() const;

/// \}
/// \name Events
/// \{

public:
	/// \brief bool func_filter_message(const CL_GUIMessage &message)
	CL_Callback_1<bool, CL_GUIMessage &> &func_filter_message();

	/// \brief int func_exec_handler(CL_AcceleratorTable &accel_table)
	CL_Callback_2<int, CL_AcceleratorTable &, bool> &func_exec_handler();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the GUI theme.
	void set_theme(CL_GUITheme *theme);

	/// \brief Sets the CSS document.
	void set_css_document(CL_CSSDocument css);

	/// \brief Sets the CSS document, by creating a CSSDocument from file automatically.
	void set_css_document(const CL_String &filename);

	/// \brief Sets the windows manager.
	void set_window_manager(CL_GUIWindowManager *window_manager);

	/// \brief Processes messages until exit_with_code is called.
	int exec(CL_AcceleratorTable &table, bool loop_until_complete = true);

	/// \brief Processes messages until exit_with_code is called - When an accelarator table is not required
	int exec(bool loop_until_complete = true) {CL_AcceleratorTable table; return exec(table, loop_until_complete);}

	/// \brief Reads the next message but leaves it in the queue.
	CL_GUIMessage peek_message(bool block);

	/// \brief Reads the next message.
	/** <p>If there is no next message available, this function blocks until it receives one.</p>*/
	CL_GUIMessage get_message();

	/// \brief Sends a GUI message to the message handler target for the message.
	void dispatch_message(CL_GUIMessage message);

	/// \brief Breaks the message loop.
	void exit_with_code(int exit_code);

	/// \brief Clears the flag indicating exec() should exit its message pump loop.
	void clear_exit_flag();

	/// \brief Post GUI message onto the message queue.
	/** <p>This function is thread safe.</p>*/
	void post_message(const CL_GUIMessage &message);

	/// \brief Send GUI message directly to the target.
	void send_message(CL_GUIMessage &message);

	/// \brief Set the mouse capture component.
	void set_capture_component(CL_GUIComponent *component, bool state);

	/// \brief Mark the specified area to be redrawn.
	void invalidate_rect(const CL_Rect &rect, CL_GUIComponent *root_component);

	/// \brief Set userdata.
	void set_userdata(CL_UnknownSharedPtr ptr);

	/// \brief Set the shared user defined GUI font - referenced using the specified font description.
	void set_named_font(const CL_Font &font, const CL_FontDescription &desc);

	void render_windows();

	/// \brief Set clipboard text.
	void set_clipboard_text(const CL_StringRef &str);

	/// \brief Redirect proximity events from tablet to the specified component.
	void set_tablet_proximity_component(CL_GUIComponent *, bool state);

/// \}
/// \name Implementation
/// \{

private:
	void process_standard_gui_keys(CL_GUIMessage &message);
	CL_SharedPtr<CL_GUIManager_Impl> impl;

	friend class CL_GUIComponent_Impl;

	friend class CL_GUIThemePart;

	friend class CL_GUIThemePart_Impl;
/// \}
};


/// \}
