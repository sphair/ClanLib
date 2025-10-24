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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "../api_display.h"
#include "../2D/color.h"
#include <cstring>

/// \brief 256 color ARGB palette class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Palette
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a palette.
	///
	CL_Palette() { return; }

	/// \brief Constructs a Palette
	///
	/// \param copy Integer array of ARGB8888 color components to use as initial palette.
	CL_Palette(const unsigned int *copy) { memcpy(colors, copy, sizeof(int)*256); }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Palette color entries.
	CL_Color colors[256];

	/// \brief Return color at index.
	const CL_Color &operator[](int index) const { return colors[index]; }

	CL_Color &operator[](int index) { return colors[index]; }

	/// \brief Returns the palette in ARGB8888 format.
	operator const unsigned int*() const { return (const unsigned int *) colors; }

	operator unsigned int*() const { return (unsigned int *) colors; }
/// \}
};

/// \}
