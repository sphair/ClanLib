/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_gradient
#define header_gradient

#if _MSC_VER > 1000
#pragma once
#endif

#include "color.h"

//: Colorgradient description class.
//- !group=Display/Display 2D!
//- !header=display.h!
class CL_Gradient
{
//! Construction:
public:
	//: Constructs a color gradient.
	//param top_left: Top left color component.
	//param top_right: Top right color component.
	//param bottom_left: Bottom left color component.
	//param bottom_right: Bottom right color component.
	CL_Gradient() { return; }

	CL_Gradient(const CL_Color &top_left, const CL_Color &top_right, const CL_Color &bottom_left, const CL_Color &bottom_right)
		: top_left(top_left), top_right(top_right), bottom_left(bottom_left), bottom_right(bottom_right) { return; }

//! Attributes:
public:
	CL_Color top_left;
	CL_Color top_right;
	CL_Color bottom_left;
	CL_Color bottom_right;
};

#endif
