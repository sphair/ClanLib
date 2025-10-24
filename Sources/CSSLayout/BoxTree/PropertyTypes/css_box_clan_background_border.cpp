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
**    Magnus Norddahl
*/

#include "CSSLayout/precomp.h"
#include "css_box_clan_background_border.h"
#include "../../css_resource_cache.h"

CL_CSSBoxClanBackgroundBorder::CL_CSSBoxClanBackgroundBorder()
: type(type_length), length(0, CL_CSSBoxLength::type_px)
{
}

void CL_CSSBoxClanBackgroundBorder::compute(const CL_CSSBoxClanBackgroundBorder *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			length = parent->length;
		}
		else
		{
			type = type_length;
			length = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_px);
		}
	}

	if (type == type_length)
	{
		// Currently the length of -clan-background-border-* only supports pixels, which
		// are in the coordinate space of the background image and not the output device.
		length.type = CL_CSSBoxLength::type_computed_px;
	}
}
