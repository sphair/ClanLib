/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../../Core/System/sharedptr.h"
#include "color.h"

class CL_Font;
class CL_GraphicContext;
class CL_Point;
class CL_Size;
class CL_SpanLayout_Impl;

enum CL_SpanAlign
{
	cl_left,
	cl_right,
	cl_center,
	cl_justify
};

/// \brief Span layout class
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_SpanLayout
{
/// \name Construction
/// \{
public:
	CL_SpanLayout();
	~CL_SpanLayout();

/// \}

/// \name Operations
/// \{
public:
	void clear();
	void add_text(const CL_String &text, const CL_Font &font, const CL_Colorf &color = CL_Colorf::white, int id = -1);
	void layout(CL_GraphicContext &gc, int max_width);
	void set_position(const CL_Point &pos);
	CL_Size get_size() const;
	int hit_test(const CL_Point &pos) const;
	void draw_layout(CL_GraphicContext &gc);

	/// \brief Sets the text alignment
	///
	/// Alignment is applied when layout() is called
	///
	/// \param align = The alignment
	void set_align(CL_SpanAlign align);

/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_SpanLayout_Impl> impl;
/// \}
};

/// \}
