/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"
#include "../../CSSLayout/css_layout.h"

namespace clan
{

class Window_Impl;

/// \brief Window component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI Window : public GUIComponent
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a Window
	///
	/// \param owner = GUIComponent
	/// \param description = GUITop Level Description
	Window(GUIComponent *owner, const GUITopLevelDescription &description);

	/// \brief Constructs a Window
	///
	/// \param manager = GUIManager
	/// \param description = GUITop Level Description
	Window(GUIManager *manager, const GUITopLevelDescription &description);

	virtual ~Window();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the title of the window.
	std::string get_title() const;

	/// \brief Returns true if the window is minimized.
	bool is_minimized() const;

	/// \brief Returns true if the window is maximized.
	bool is_maximized() const;

	/// \brief Returns the preferred size of the window.
	Size get_preferred_size() const;

	/// \brief Returns the client area geometry. Client area is the content area when titlebar and borders are excluded.
	Rect get_client_area() const;

	/// \brief Get Draggable
	///
	/// \return draggable
	bool get_draggable() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets the window title.
	void set_title(const std::string &str);

	/// \brief Brings the window to front.
	void bring_to_front();

	/// \brief Set if the window can be dragged by the caption
	///
	/// \brief enable = true = enable
	void set_draggable(bool enable = true);

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<Window_Impl> impl;

/// \}
};

}

/// \}
