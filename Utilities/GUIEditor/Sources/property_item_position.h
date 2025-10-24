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


#pragma once

#include "property_item.h"

class PropertyItemPosition : public PropertyItemLineEdit
{
public:
	enum PositionType
	{
		type_x1,
		type_y1,
		type_x2,
		type_y2,
		type_width,
		type_height
	};

	PropertyItemPosition(PositionType type)
	: PropertyItemLineEdit(get_type_name(type)), type(type)
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			HolderComponent *holder = selection[0];
			switch (type)
			{
			case type_x1:
				value = CL_StringHelp::int_to_text(holder->get_geometry().left);
				break;
			case type_y1:
				value = CL_StringHelp::int_to_text(holder->get_geometry().top);
				break;
			case type_x2:
				value = CL_StringHelp::int_to_text(holder->get_geometry().right);
				break;
			case type_y2:
				value = CL_StringHelp::int_to_text(holder->get_geometry().bottom);
				break;
			case type_width:
				value = CL_StringHelp::int_to_text(holder->get_width());
				break;
			case type_height:
				value = CL_StringHelp::int_to_text(holder->get_height());
				break;
			}
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		int new_value = CL_StringHelp::text_to_int(value);

		CL_Rect geometry = holder->get_geometry();
		switch (type)
		{
		case type_x1:
			geometry.left = new_value;
			break;
		case type_y1:
			geometry.top = new_value;
			break;
		case type_x2:
			geometry.right = new_value;
			break;
		case type_y2:
			geometry.bottom = new_value;
			break;
		case type_width:
			geometry.right = geometry.left + new_value;
			break;
		case type_height:
			geometry.bottom = geometry.top + new_value;
			break;
		}
		if (geometry.left < geometry.right && geometry.top < geometry.bottom)
			holder->set_geometry(geometry);
	}

private:
	static CL_String get_type_name(PositionType type)
	{
		switch (type)
		{
		case type_x1:
			return "x1";
		case type_y1:
			return "y1";
		case type_x2:
			return "x2";
		case type_y2:
			return "y2";
		case type_width:
			return "width";
		case type_height:
			return "height";
		default:
			return "position";
		}
	}

	PositionType type;
};
