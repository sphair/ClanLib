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

#ifndef header_layout_generic
#define header_layout_generic

#include "API/GUI/layout.h"
#include "layout_item.h"

class CL_Layout_Generic
{
//! Construction:
public:
	CL_Layout_Generic(CL_LayoutType type);

	~CL_Layout_Generic();

//! Attributes:
public:
	CL_LayoutType type;

	CL_LayoutItem *parent;

	int grid_columns;

	std::vector<CL_LayoutItem *> elements;

	int margin;

	int spacing;

//! Operations:
public:
	//: Returns the preferred size of this layout.
	CL_Size get_preferred_size() const;

	//: Returns the minimum size of this layout.
	CL_Size get_minimum_size() const;

	//: Returns the maximum size of this layout.
	CL_Size get_maximum_size() const;

	//: Returns the vertical size policy of this layout.
	CL_LayoutPolicy get_vertical_policy() const;

	//: Returns the horizontal size policy of this layout.
	CL_LayoutPolicy get_horizontal_policy() const;

	//: Set layout to use this size and position.
	void set_position(const CL_Rect &pos);

//! Implementation:
private:
};

#endif
