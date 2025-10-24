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

#include "precomp.h"
#include "layout_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Layout_Generic construction:

CL_Layout_Generic::CL_Layout_Generic(CL_LayoutType type) : type(type), grid_columns(-1), margin(0), spacing(4)
{
}

CL_Layout_Generic::~CL_Layout_Generic()
{
	for (std::vector<CL_LayoutItem *>::size_type i = 0; i < elements.size(); i++)
	{
		delete elements[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Layout_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Layout_Generic operations:

#define cl_max(a,b) ((a > b) ? a : b)

CL_Size CL_Layout_Generic::get_preferred_size() const
{
	CL_Size size(0,0);
	std::vector<CL_LayoutItem *>::size_type i;
	for (i=0; i<elements.size(); i++)
	{
		CL_Size cur = elements[i]->get_preferred_size();

		if (type == layout_vertical)
		{
			size.width = cl_max(size.width, cur.width);
			size.height += cur.height;
			if (i > 0) size.height += spacing;
		}
		else if (type == layout_horizontal)
		{
			size.width += cur.width;
			size.height = cl_max(size.height, cur.height);
			if (i > 0) size.width += spacing;
		}
	}
	size.width += margin*2;
	size.height += margin*2;
	return size;
}

CL_Size CL_Layout_Generic::get_minimum_size() const
{
	CL_Size size(0,0);
	std::vector<CL_LayoutItem *>::size_type i;
	for (i=0; i<elements.size(); i++)
	{
		CL_Size cur = elements[i]->get_minimum_size();

		if (type == layout_vertical)
		{
			size.width = cl_max(size.width, cur.width);
			size.height += cur.height;
			if (i > 0) size.height += spacing;
		}
		else if (type == layout_horizontal)
		{
			size.width += cur.width;
			size.height = cl_max(size.height, cur.height);
			if (i > 0) size.width += spacing;
		}
	}
	size.width += margin*2;
	size.height += margin*2;
	return size;
}

CL_Size CL_Layout_Generic::get_maximum_size() const
{
	CL_Size size(0,0);
	std::vector<CL_LayoutItem *>::size_type i;
	for (i=0; i<elements.size(); i++)
	{
		CL_Size cur = elements[i]->get_maximum_size();

		if (type == layout_vertical)
		{
			size.width = cl_max(size.width, cur.width);
			size.height += cur.height;
			if (i > 0) size.height += spacing;
		}
		else if (type == layout_horizontal)
		{
			size.width += cur.width;
			size.height = cl_max(size.height, cur.height);
			if (i > 0) size.width += spacing;
		}
	}
	size.width += margin*2;
	size.height += margin*2;
	return size;
}

CL_LayoutPolicy CL_Layout_Generic::get_vertical_policy() const
{
	std::vector<CL_LayoutItem *>::size_type i;
	for (i=0; i<elements.size(); i++)
	{
		if (elements[i]->get_vertical_policy() == layout_expanding) return layout_expanding;
	}
	return layout_minimum;
}

CL_LayoutPolicy CL_Layout_Generic::get_horizontal_policy() const
{
	std::vector<CL_LayoutItem *>::size_type i;
	for (i=0; i<elements.size(); i++)
	{
		if (elements[i]->get_horizontal_policy() == layout_expanding) return layout_expanding;
	}
	return layout_minimum;
}

void CL_Layout_Generic::set_position(const CL_Rect &pos)
{
	std::vector<CL_LayoutItem *>::size_type i;

	int minimum_width = 0;
	int minimum_height = 0;
	int num_expanding_x = 0;
	int num_expanding_y = 0;

	for (i=0; i<elements.size(); i++)
	{
		if (elements[i]->get_horizontal_policy() == layout_minimum)
		{
			minimum_width += elements[i]->get_preferred_size().width;
		}
		else
		{
			num_expanding_x++;
		}

		if (elements[i]->get_vertical_policy() == layout_minimum)
		{
			minimum_height += elements[i]->get_preferred_size().height;
		}
		else
		{
			num_expanding_y++;
		}

		if (type == layout_horizontal && i > 0) minimum_width += spacing;
		if (type == layout_vertical && i > 0) minimum_height += spacing;
	}

	int x = pos.left + margin;
	int y = pos.top + margin;
	int expandable_width = pos.get_width() - margin*2 - minimum_width;
	int expandable_height = pos.get_height() - margin*2 - minimum_height;

	// Do vertical and horizontal layout:
	switch (type)
	{
	case layout_vertical:
		for (i=0; i<elements.size(); i++)
		{
			if (elements[i]->get_vertical_policy() == layout_minimum && num_expanding_y > 0)
			{
				int element_height = elements[i]->get_preferred_size().height;
				elements[i]->set_position(
					CL_Rect(
						pos.left+margin,
						y,
						pos.right-margin,
						y+element_height));
				y += element_height;
			}
			else if (num_expanding_y > 0)
			{
				int element_height =
					expandable_height*(i+1)/num_expanding_y - expandable_height*i/num_expanding_y;

				elements[i]->set_position(
					CL_Rect(
						pos.left+margin,
						y,
						pos.right-margin,
						y+element_height));
				y += element_height;
			}
			else
			{
				elements[i]->set_position(
					CL_Rect(
						pos.left+margin,
						pos.top+margin+(pos.get_height()-margin*2)*i/elements.size(),
						pos.right-margin,
						pos.top+margin+(pos.get_height()-margin*2)*(i+1)/elements.size()));
			}

			y += spacing;
		}
		break;

	case layout_horizontal:
		for (i=0; i<elements.size(); i++)
		{
			if (elements[i]->get_horizontal_policy() == layout_minimum && num_expanding_x > 0)
			{
				int element_width = elements[i]->get_preferred_size().width;
				elements[i]->set_position(
					CL_Rect(
						x,
						pos.top+margin,
						x+element_width,
						pos.bottom-margin));
				x += element_width;
			}
			else if (num_expanding_x > 0)
			{
				int element_width =
					expandable_width*(i+1)/num_expanding_x - expandable_width*i/num_expanding_x;

				elements[i]->set_position(
					CL_Rect(
						x,
						pos.top+margin,
						x+element_width,
						pos.bottom-margin));
				x += element_width;
			}
			else
			{
				elements[i]->set_position(
					CL_Rect(
						pos.left+margin+(pos.get_width()-margin*2)*i/elements.size(),
						pos.top+margin,
						pos.left+margin+(pos.get_width()-margin*2)*(i+1)/elements.size(),
						pos.bottom-margin));
			}

			x += spacing;
		}
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Layout_Generic implementation:
