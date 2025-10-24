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

#include "../css_box_property.h"

class CL_CSSBoxContent : public CL_CSSBoxProperty
{
public:
	CL_CSSBoxContent();
	void compute(const CL_CSSBoxContent *parent, CL_CSSResourceCache *layout, float em_size, float ex_size, bool before_or_after_pseudo_element);
	CL_String to_string() const;

	enum Type
	{
		type_normal,
		type_none,
		type_string,
		type_uri,
		type_counter,
		type_attr,
		type_open_quote,
		type_close_quote,
		type_no_open_quote,
		type_no_close_quote,
		type_inherit
	};
	Type type;
	CL_String str;
};
