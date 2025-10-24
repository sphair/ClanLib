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

#ifndef header_layout_item
#define header_layout_item

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/component.h"
#include "API/Core/System/sharedptr.h"

class CL_Layout_Generic;

//: Layout item in a layout tree.
class CL_LayoutItem
{
//! Construction:
public:
	//: Constructs a layout item.
	CL_LayoutItem(CL_Component *component);

	CL_LayoutItem(CL_SharedPtr<CL_Layout_Generic> layout);

	~CL_LayoutItem();

//! Attributes:
public:
	//: Returns the preferred size of this layout item.
	CL_Size get_preferred_size() const;

	//: Returns the minimum size of this layout item.
	CL_Size get_minimum_size() const;

	//: Returns the maximum size of this layout item.
	CL_Size get_maximum_size() const;

	//: Returns the vertical size policy of this layout item.
	CL_LayoutPolicy get_vertical_policy() const;

	//: Returns the horizontal size policy of this layout item.
	CL_LayoutPolicy get_horizontal_policy() const;

	//: Component this layout item represents.
	//- <p>Null if its not a component.</p>
	CL_Component *component;

	//: Layout this layout item represents.
	//- <p>Null if its not a layout.</p>
	CL_SharedPtr<CL_Layout_Generic> layout;

//! Operations:
public:
	//: Copy assignment operator.
	CL_LayoutItem &operator =(const CL_LayoutItem &copy);

	//: Set layout item to use this size and position.
	void set_position(const CL_Rect &pos);

//! Implementation:
private:
};

#endif
