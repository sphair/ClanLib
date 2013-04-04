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
**    Harry Storbacka
**    Mark Page
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
class GUILayout_Impl;
class GUILayoutProvider;

/// \brief Layout management base class.
class CL_API_GUI GUILayout
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null layout
	GUILayout();

	/// \brief Constructs a layout
	///
	/// \param provider = The layout provider
	GUILayout(GUILayoutProvider *provider);

	~GUILayout();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Retrieves the provider.
	GUILayoutProvider *get_provider() const;

	/// \brief Returns the size of the dialog, as set in the editor.
	Size get_css_size() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set geometry
	///
	/// \param pos = Rect
	void set_geometry(const Rect &pos);

	/// \brief Set preferred size
	///
	/// \param size = Size
	void set_preferred_size(const Size &size);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<GUILayout_Impl> impl;

/// \}
};

}

/// \}
