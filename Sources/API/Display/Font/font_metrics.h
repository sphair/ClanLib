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

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{


#pragma once


#include "../api_display.h"
#include "../../Core/System/sharedptr.h"

class CL_FontMetrics_Impl;

/// \brief Font metrics class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_FontMetrics
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a font metrics description.
	CL_FontMetrics();

	CL_FontMetrics(
		float height,
		float ascent=0.0,
		float descent=0.0,
		float internal_leading=0.0,
		float external_leading=0.0,
		float average_character_width=0.0,
		float max_character_width=0.0,
		float weight=0.0,
		float overhang=0.0,
		float digitized_aspect_x=0.0,
		float digitized_aspect_y=0.0,
		bool italic=false,
		bool underline=false,
		bool struck_out=false,
		bool fixed_pitch=false
	);

	~CL_FontMetrics();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the height of the font.
	float get_height() const;

	/// \brief Returns the font ascender.
	float get_ascent() const;

	/// \brief Returns the font descender.
	float get_descent() const;

	/// \brief Returns the amount of leading (space) inside the bounds set by the get_height() function.
	float get_internal_leading() const;

	/// \brief Returns the amount of extra leading (space) that the application adds between rows.
	float get_external_leading() const;

	/// \brief Returns the average width of characters in the font.
	/** <p>The average width is generally defined as the width of the letter x.  The
	    value does not include the overhang required for bold or italic characters.</p>*/
	float get_average_character_width() const;

	/// \brief Returns the width of the widest character in the font.
	float get_max_character_width() const;

	/// \brief Returns the weight of the font.
	float get_weight() const;

	/// \brief Returns the extra width per string that may be added to some synthesized fonts.
	float get_overhang() const;

	/// \brief Returns the horizontal aspect of the device for which the font was designed.
	float get_digitized_aspect_x() const;

	/// \brief Returns the vertical aspect of the device for which the font was designed.
	float get_digitized_aspect_y() const;

	/// \brief Returns the value of the first character defined in the font.
	CL_String::char_type get_first_char() const;

	/// \brief Returns the value of the last character defined in the font
	CL_String::char_type get_last_char() const;

	/// \brief Returns the value of the character to be substituted for characters not in the font.
	CL_String::char_type get_default_char() const;

	/// \brief Returns the value of the character that will be used to define word breaks for text justification.
	CL_String::char_type get_word_break_char() const;

	/// \brief Returns true if the font is italic.
	bool is_italic() const;

	/// \brief Returns true if the font is underlined.
	bool is_underlined() const;

	/// \brief Returns true if the font is struck out.
	bool is_struck_out() const;

	/// \brief Returns true if the font is a mono space font.
	bool is_fixed_pitch() const;


/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_FontMetrics_Impl> impl;
/// \}
};


/// \}
