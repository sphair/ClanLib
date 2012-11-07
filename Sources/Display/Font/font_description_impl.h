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
**    Mark Page
*/

#pragma once

namespace clan
{

class FontDescription_Impl
{
/// \name Construction
/// \{

public:
	FontDescription_Impl()
	{
		//typeface_name = "tahoma";
		height = 0;
		average_width = 0;
		escapement = 0.0f;
		orientation = 0.0f;
		weight = 0;
		italic = false;
		underline = false;
		strikeout = false;
		fixed_pitch = false;
		anti_alias = true;
		subpixel = true;
		charset = FontDescription::charset_default;
	}

	virtual ~FontDescription_Impl() { return; }


/// \}
/// \name Attributes
/// \{

public:
	std::string typeface_name;
	std::string filename;
	int height;
	int average_width;
	float escapement;
	float orientation;
	int weight;
	bool italic;
	bool underline;
	bool strikeout;
	bool fixed_pitch;
	bool anti_alias;
	bool subpixel;
	FontDescription::Charset charset;

/// \}
};

}
