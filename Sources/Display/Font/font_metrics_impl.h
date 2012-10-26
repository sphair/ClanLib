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
*/

#pragma once

#include "API/Display/api_display.h"

namespace clan
{

/// \brief Input events interface.
class CL_API_DISPLAY FontMetrics_Impl
{
/// \name Construction
/// \{

public:
	FontMetrics_Impl(
		float height,
		float ascent,
		float descent,
		float internal_leading,
		float external_leading,
		float average_character_width,
		float max_character_width,
		float weight,
		float overhang,
		float digitized_aspect_x,
		float digitized_aspect_y,
		bool italic=false,
		bool underline=false,
		bool struck_out=false,
		bool fixed_pitch=false
		);

	~FontMetrics_Impl();


/// \}
/// \name Attributes
/// \{

public:

	std::string::value_type get_first_char() const;

	std::string::value_type get_last_char() const;

	std::string::value_type get_default_char() const;

	std::string::value_type get_word_break_char() const;

	float height;
	float ascent;
	float descent;
	float internal_leading;
	float external_leading;
	float average_character_width;
	float max_character_width;
	float weight;
	float overhang;
	float digitized_aspect_x;
	float digitized_aspect_y;
	bool italic;
	bool underlined;
	bool struck_out;
	bool fixed_pitch;

/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}
