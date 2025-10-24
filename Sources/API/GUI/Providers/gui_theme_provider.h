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
**    Kenneth Gangstoe
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "../api_gui.h"

class CL_GUIThemePart;
class CL_GraphicContext;
class CL_ResourceManager;
class CL_GUIComponent;
class CL_Colorf;
class CL_Size;
class CL_Rect;
class CL_Font;

/// \brief GUI theme interface.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIThemeProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_GUIThemeProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Allows the theme to alter GUI part properties.
	///
	/// When a component queries a theme part for a property, the property
	/// is searched in the CSS document and then passed to get_property.  If
	/// the theme does not wish to alter the value of the property, it simply
	/// returns the css_value parameter.  The string returned by this function
	/// is the result of CL_GUIThemePart::get_property.
	virtual CL_StringRef get_property(
		const CL_GUIThemePart &part,
		const CL_StringRef &name,
		const CL_StringRef &css_value) = 0;

	/// \brief Returns the resource manager associated with the theme.
	virtual CL_ResourceManager get_resources() const = 0;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Destroys the provider.
	virtual void destroy() = 0;

	/// \brief Adds a resource manager to be used to retrieve resources.
	virtual void add_resources(CL_ResourceManager resources) = 0;

	/// \brief Sets the resource manager to be used to retrieve resources.
	virtual void set_resources(CL_ResourceManager resources) = 0;

	/// \brief Renders a background box for the given theme part.
	virtual void render_box(
		CL_GraphicContext &gc,
		CL_GUIThemePart &part,
		const CL_Rect &rect,
		const CL_Rect &clip_rect) = 0;

	virtual CL_Rect render_text(
		CL_GraphicContext &gc,
		CL_GUIThemePart &part,
		CL_Font &font,
		const CL_StringRef &text,
		const CL_Rect &content_rect,
		const CL_Rect &clip_rect) = 0;

	/// \brief Notifies the theme that a component has been destroyed.
	///
	// This notification allows a theme to clean up cached theme part
	// resources used by the component.
	virtual void component_destroyed(CL_GUIComponent *component) = 0;
/// \}
};

/// \}

