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
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include <memory>

namespace clan
{

class GUIThemePart;
class Canvas;
class ResourceManager;
class GUIComponent;
class Colorf;
class Size;
class Rect;
class Font;
class GUITheme_Impl;
class GUIThemeProvider;
class Canvas;

/// \brief GUI theme
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI GUITheme
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a theme
	///
	/// \param provider = The theme provider
	GUITheme(GUIThemeProvider *provider);

	~GUITheme();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Retrieves the provider.
	GUIThemeProvider *get_provider() const;

	/// \brief Allows the theme to alter GUI part properties.
	///
	/// When a component queries a theme part for a property, the property
	/// is searched in the CSS document and then passed to get_property.  If
	/// the theme does not wish to alter the value of the property, it simply
	/// returns the css_value parameter.  The string returned by this function
	/// is the result of GUIThemePart::get_property.
	std::string get_property(
		const GUIThemePart &part,
		const std::string &name,
		const std::string &css_value);

	/// \brief Returns the resource manager associated with the theme.
	ResourceManager get_resources() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the resource manager to be used to retrieve resources.
	void set_resources(ResourceManager resources);

	/// \brief Adds a resource manager to be used to retrieve resources.
	void add_resources(ResourceManager resources);

	/// \brief Renders a background box for the given theme part.
	void render_box(
		Canvas &canvas,
		GUIThemePart &part,
		const Rect &rect,
		const Rect &clip_rect);

	Rect render_text(
		Canvas &canvas,
		GUIThemePart &part,
		Font &font,
		const std::string &text,
		const Rect &content_rect,
		const Rect &clip_rect);

	/// \brief Notifies the theme that a component has been destroyed.
	///
	// This notification allows a theme to clean up cached theme part
	// resources used by the component.
	void component_destroyed(GUIComponent *component);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<GUITheme_Impl> impl;
/// \}
};

}

/// \}
