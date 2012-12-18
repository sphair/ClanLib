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

#include "gui_component.h"

namespace clan
{

class GUIComponent;
class CSSBoxProperties;
class GUIThemePart_Impl;
class Font;
class Canvas;
class Rect;
class Size;
class Point;

/// \brief A GUI theme part represents a pseudo element in CSS.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI GUIThemePart
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	GUIThemePart();

	/// \brief Creates a GUI component.
	GUIThemePart(GUIComponent *parent, const std::string &tag_name);
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the standard W3C CSS properties active for this component
	const CSSBoxProperties &get_css_properties() const;

	/// \brief Returns the standard W3C CSS properties active for this component
	CSSBoxProperties &get_css_properties();

	/// \brief Gets the font
	Font get_font() const;

	/// \brief Returns the DOM tag name
	std::string get_tag_name() const;

	/// \brief Returns the DOM id attribute
	std::string get_id() const;

	/// \brief Returns the DOM class attribute
	std::string get_class() const;

	/// \brief Returns whether a DOM pseudo class is present or not.
	bool get_pseudo_class(const std::string &name) const;

	/// \brief Returns all pseudo classes currently present
	std::vector<std::string> get_pseudo_classes() const;

	/// \brief Returns the css size of a render box.
	Size get_css_size() const;

	/// \brief Returns the css width of a render box.
	int get_css_width() const;

	/// \brief Returns the css height of a render box.
	int get_css_height() const;

	/// \brief Calculates the text size using the font defined for the current state for use with render_text_span()
	Rect get_render_text_span_box(Canvas &canvas, const std::string &str, const Rect &content_rect) const;

	/// \brief Calculates the text box using the font defined for the current state for use with render_text()
	Rect get_render_text_box(Canvas &canvas, const std::string &str, const Rect &content_rect) const;

	/// \brief Calculates the text size using the font defined for the current state for use with render_text()
	Size get_render_text_size(Canvas &canvas, const std::string &str) const;

	/// \brief Returns the position of the content box for a given render box.
	Rect get_content_box(const Rect &render_box_rect) const;

	/// \brief Returns the content shrink box of the theme part.
	Rect get_content_shrink_box() const;

	/// \brief Returns the position and size of a border box for a given content box.
	Rect get_border_box(const Rect &content_box_rect) const;

	/// \brief Returns the value of a property.
	std::string get_property(const std::string &property, const std::string &default_value) const;

	/// \brief Returns the value of a property as an integer.
	int get_property_int(const std::string &property, const std::string &default_value) const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Renders the component and its children.
	void render_box(Canvas &canvas, const Rect &border_box);

	/// \brief Re-evaluates which CSS selectors match this component
	void update_style();

	/// \brief Sets the DOM tag name
	void set_tag_name(const std::string &name); 

	/// \brief Sets the DOM class
	void set_class(const std::string &name);

	/// \brief Sets the DOM id
	void set_id(const std::string &name);

	/// \brief Controls the presence of a DOM pseudo class
	///
	/// \return true if changed
	bool set_pseudo_class(const std::string &name, bool enable);

	Rect render_text_span(Canvas &canvas, const std::string &text, const Rect &content_rect);
	Rect render_text(Canvas &canvas, const std::string &text, const Rect &content_box);
	Rect render_text(Canvas &canvas, const std::string &text, const Rect &content_box, int baseline);

	/// \brief Calculates the vertical text position for the current font
	VerticalTextPosition get_vertical_text_align(Canvas &canvas, const Rect &content_rect);

/// \}

/// \name Implementation
/// \{
public:
	std::shared_ptr<GUIThemePart_Impl> impl;
/// \}
};

}
