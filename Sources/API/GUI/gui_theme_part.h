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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"

class CL_Size;
class CL_Rect;
class CL_GraphicContext;
class CL_Font;

class CL_GUIComponent;
class CL_GUIThemePartProperty;
class CL_GUITheme;
class CL_GUIThemePart_Impl;

/// \brief GUI theme part description.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIThemePart
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a GUI theme part.
	CL_GUIThemePart();

	CL_GUIThemePart(
		CL_GUIComponent *component,
		const CL_StringRef &relative_element_name = CL_StringRef());

	~CL_GUIThemePart();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the CSS element name for the part in its current states.
	CL_StringRef get_element_name() const;

	/// \brief Returns the setting of a state.
	bool get_state(const CL_StringRef &name) const;

	/// \brief Returns the value of a property.
	CL_StringRef get_property(const CL_GUIThemePartProperty &property) const;

	/// \brief Returns the value of a property as an integer.
	int get_property_int(const CL_GUIThemePartProperty &property) const;

	/// \brief Get Component
	///
	/// \return component
	CL_GUIComponent *get_component() const;

	/// \brief Returns the position of the content box for a given render box.
	CL_Rect get_content_box(const CL_Rect &render_box_rect) const;

	/// \brief Returns the content shrink box of the theme part.
	CL_Rect get_content_shrink_box() const;

	/// \brief Returns the position and size of a render box for a given content box.
	CL_Rect get_render_box(const CL_Rect &content_box_rect) const;

	/// \brief Returns the minimum size of a render box.
	CL_Size get_minimum_size() const;

	/// \brief Returns the minimum width of a render box.
	int get_minimum_width() const;

	/// \brief Returns the minimum height of a render box.
	int get_minimum_height() const;

	/// \brief Returns the maximum size of a render box.
	CL_Size get_maximum_size() const;

	/// \brief Returns the maximum width of a render box.
	int get_maximum_width() const;

	/// \brief Returns the maximum height of a render box.
	int get_maximum_height() const;

	/// \brief Returns the preferred size of a render box.
	CL_Size get_preferred_size() const;

	/// \brief Returns the preferred width of a render box.
	int get_preferred_width() const;

	/// \brief Returns the preferred height of a render box.
	int get_preferred_height() const;

	/// \brief Returns the part font.
	CL_Font get_font() const;

	/// \brief Calculates the text size using the font defined for the current state.
	CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &str) const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets a part state. Returns true if the state was modified, false if it was already in wanted state.
	bool set_state(const CL_StringRef &name, bool flag);

	/// \brief Render box
	///
	/// \param gc = Graphic Context
	/// \param rect = Rect
	/// \param clip_rect = Rect
	void render_box(CL_GraphicContext &gc, const CL_Rect &rect, const CL_Rect &clip_rect);

	CL_Rect render_text(
		CL_GraphicContext &gc,
		const CL_StringRef &text,
		const CL_Rect &content_rect, 
		const CL_Rect &clip_rect);

	struct VerticalTextPosition
	{
		float top;
		float baseline;
		float bottom;
	};

	/// \brief Calculates the vertical text position for the current font
	VerticalTextPosition get_vertical_text_align(CL_GraphicContext &gc, CL_Font &font, const CL_Rect &content_rect);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_GUIThemePart_Impl> impl;

	friend class CL_GUIThemePartProperty_Impl;
/// \}
};
/// \}
