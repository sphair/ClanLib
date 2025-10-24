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
*/

#pragma once

class CL_CSSBoxLength
{
public:
	enum Type
	{
		type_mm,
		type_cm,
		type_in,
		type_pt,
		type_pc,
		type_px,
		type_em,
		type_ex,
		type_computed_px
	};

	CL_CSSBoxLength()
	: type(type_px), value(0.0f)
	{
	}

	CL_CSSBoxLength(float value, Type type)
	: type(type), value(value)
	{
	}

	CL_String to_string() const
	{
		if (value == 0.0f)
			return "0";
		CL_String v = CL_StringHelp::float_to_text(value);
		switch (type)
		{
		default:
		case type_mm:
			return v + "mm";
		case type_cm:
			return v + "cm";
		case type_in:
			return v + "in";
		case type_pt:
			return v + "pt";
		case type_pc:
			return v + "pc";
		case type_px:
			return v + "px";
		case type_em:
			return v + "em";
		case type_ex:
			return v + "ex";
		case type_computed_px:
			return v + "computed-px";
		}
	}

	Type type;
	float value;
};
