/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "css_layout_cursor.h"

namespace clan
{

CSSLayoutCursor::CSSLayoutCursor()
: x(0), y(0), relative_x(0), relative_y(0), margin_y(0), negative_margin_y(0), resources(0)
{
}

void CSSLayoutCursor::add_margin(CSSUsedValue extra_margin_y)
{
	if (extra_margin_y >= 0.0f)
		margin_y = max(margin_y, extra_margin_y);
	else
		negative_margin_y = min(negative_margin_y, extra_margin_y);
}

void CSSLayoutCursor::apply_margin()
{
	y += get_total_margin();
	margin_y = 0;
	negative_margin_y = 0;
}

CSSActualValue CSSLayoutCursor::get_total_margin() const
{
	return used_to_actual(margin_y + negative_margin_y);
}

}
