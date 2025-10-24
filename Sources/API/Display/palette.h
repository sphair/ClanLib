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

#ifndef header_palette
#define header_palette

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "color.h"
#include <string.h>

//: 256 color ARGB palette class.
//- !group=Display/Display 2D!
//- !header=display.h!
class CL_Palette
{
//! Construction:
public:
	//: Constructs a palette.
	//param copy: Integer array of ARGB8888 color components to use as initial palette.
	CL_Palette() { return; }

	CL_Palette(const unsigned int *copy) { memcpy(colors, copy, sizeof(int)*256); }

//! Attributes:
public:
	//: Palette color entries.
	CL_Color colors[256];

	//: Return color at index.
	const CL_Color &operator[](int index) const { return colors[index]; }

	CL_Color &operator[](int index) { return colors[index]; }

	//: Returns the palette in ARGB8888 format.
	operator const unsigned int*() const { return (const unsigned int *) colors; }

	operator unsigned int*() { return (unsigned int *) colors; }
};

#endif
