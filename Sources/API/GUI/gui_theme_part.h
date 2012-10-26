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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include <memory>

namespace clan
{

class Size;
class Rect;
class Canvas;
class Font;
class Canvas;

class GUIComponent;
class GUIThemePartProperty;
class GUITheme;
class GUIThemePart_Impl;

/// \brief GUI theme part description.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI GUIThemePart
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a GUI theme part.
	GUIThemePart();

	GUIThemePart(
		GUIComponent *component,
		const std::string &relative_element_name = std::string());

	~GUIThemePart();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the CSS element name for the part in its current states.
	std::string get_element_name() const;

	/// \brief Returns the setting of a state.
	bool get_state(const std::string &name) const;

	/// \brief Returns the value of a property.
	std::string get_property(const GUIThemePartProperty &property) const;

	/// \brief Returns the value of a property as an integer.
	int get_property_int(const GUIThemePartProperty &property) const;

	/// \brief Get Component
	///
	/// \return component
	GUIComponent *get_component() const;

	/// \brief Returns the position of the content box for a given render box.
	Rect get_content_box(const Rect &render_box_rect) const;

	/// \brief Returns the content shrink box of the theme part.
	Rect get_content_shrink_box() const;

	/// \brief Returns the position and size of a render box for a given content box.
	Rect get_render_box(const Rect &content_box_rect) const;

	/// \brief Returns the minimum size of a render box.
	Size get_minimum_size() const;

	/// \brief Returns the minimum width of a render box.
	int get_minimum_width() const;

	/// \brief Returns the minimum height of a render box.
	int get_minimum_height() const;

	/// \brief Returns the maximum size of a render box.
	Size get_maximum_size() const;

	/// \brief Returns the maximum width of a render box.
	int get_maximum_width() const;

	/// \brief Returns the maximum height of a render box.
	int get_maximum_height() const;

	/// \brief Returns the preferred size of a render box.
	Size get_preferred_size() const;

	/// \brief Returns the preferred width of a render box.
	int get_preferred_width() const;

	/// \brief Returns the preferred height of a render box.
	int get_preferred_height() const;

	/// \brief Returns the part font.
	Font get_font() const;

	/// \brief Calculates the text size using the font defined for the current state.
	Size get_text_size(Canvas &canvas, const std::string &str) const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets a part state. Returns true if the state was modified, false if it was already in wanted state.
	bool set_state(const std::string &name, bool flag);

	/// \brief Render box
	///
	/// \param canvas = Canvas
	/// \param rect = Rect
	/// \param clip_rect = Rect
	void render_box(Canvas &canvas, const Rect &rect, const Rect &clip_rect);

	Rect render_text(
		Canvas &canvas,
		const std::string &text,
		const Rect &content_rect, 
		const Rect &clip_rect);

	struct VerticalTextPosition
	{
		float top;
		float baseline;
		float bottom;
	};

	/// \brief Calculates the vertical text position for the current font
	VerticalTextPosition get_vertical_text_align(Canvas &canvas, Font &font, const Rect &content_rect);

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<GUIThemePart_Impl> impl;

	friend class GUIThemePartProperty_Impl;
/// \}
};

}

/// \}
