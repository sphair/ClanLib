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

class CL_CSSStackingContext;
class CL_CSSResourceCache;
class CL_CSSBlockFormattingContext;

#include "css_used_value.h"

class CL_CSSLayoutCursor
{
public:
	CL_CSSLayoutCursor();
	void add_margin(CL_CSSUsedValue margin_y);
	void apply_margin();
	CL_CSSActualValue get_total_margin() const;

	CL_CSSActualValue x;
	CL_CSSActualValue y;
	CL_CSSUsedValue relative_x;
	CL_CSSUsedValue relative_y;
	CL_CSSResourceCache *resources;
private:
	CL_CSSUsedValue margin_y;
	CL_CSSUsedValue negative_margin_y;
};
