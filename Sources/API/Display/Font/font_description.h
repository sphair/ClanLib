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
**    Mark Page
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/Text/string_types.h"
#include "../../Core/System/sharedptr.h"

class CL_DisplayWindow;
class CL_FontDescription_Impl;

/// \brief Font description class.
///
///  This class allows you to setup a more advanced description when creating a font.
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_FontDescription
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a font description with default values.
	CL_FontDescription();

	virtual ~CL_FontDescription();

	/// \brief Create null object
	///
	/// \return Font Description
	static CL_FontDescription create_null_object();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the typeface name
	const CL_String &get_typeface_name() const;

	/// \brief Returns the font height
	int get_height() const;

	/// \brief Returns the font average width
	int get_average_width() const;

	/// \brief Returns the font escapement
	float get_escapement() const;

	/// \brief Returns the font orientation
	float get_orientation() const;

	/// \brief Returns the font weight
	int get_weight() const;

	/// \brief Returns the font italic setting
	bool get_italic() const;

	/// \brief Returns the font underline setting
	bool get_underline() const;

	/// \brief Returns the font strikeout setting
	bool get_strikeout() const;

	/// \brief Returns the font fixed pitch setting
	bool get_fixed_pitch() const;

	/// \brief Get the font anti-alias setting
	bool get_anti_alias() const;

	/// \brief Returns true if set_anti_alias() was called. (Else, the anti-alias default setting depends on the implementation
	bool get_anti_alias_set() const;

	/// \brief Returns true if the font is identical
	bool operator==(const CL_FontDescription &other) const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Copy assignment operator.
	CL_FontDescription &operator =(const CL_FontDescription &copy);

	// \brief Copy the entire font description (not just the implementation)
	void clone(const CL_FontDescription &copy);

	/// \brief Sets the typeface name
	void set_typeface_name(const CL_String &name);

	/// \brief Sets the font height
	void set_height(int value);

	/// \brief Sets the font average width
	void set_average_width(int value);

	/// \brief Sets the font escapement
	void set_escapement(float value);

	/// \brief Sets the font orientation
	void set_orientation(float value);

	/// \brief Sets the font weight
	void set_weight(int value);

	/// \brief Sets the font italic setting
	void set_italic(bool setting = true);

	/// \brief Sets the font underline setting
	void set_underline(bool setting = true);

	/// \brief Sets the font strikeout setting
	void set_strikeout(bool setting = true);

	/// \brief Sets the font fixed pitch setting
	void set_fixed_pitch(bool setting = true);

	/// \brief Sets the font anti-alias setting
	void set_anti_alias(bool setting = true);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_FontDescription_Impl> impl;
/// \}
};

/// \}

