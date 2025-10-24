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
#include "../css_box_length.h"

class CL_CSSBoxBackgroundOrigin : public CL_CSSBoxProperty
{
public:
	CL_CSSBoxBackgroundOrigin();
	void compute(const CL_CSSBoxBackgroundOrigin *parent, CL_CSSResourceCache *layout, float em_size, float ex_size);
	CL_String to_string() const;

	enum Type
	{
		type_value,
		type_inherit
	} type;

	enum OriginType
	{
		origin_border_box,
		origin_padding_box,
		origin_content_box
	};

	std::vector<OriginType> values;
};
