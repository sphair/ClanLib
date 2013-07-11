/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "property_item_line_edit.h"

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
	: PropertyItemLineEdit(get_tag_name(type)), type(type)
	{
	}

	void selection_changed(const std::vector<GridObject *> &selection)
	{
		if (!selection.empty())
		{
			GridObject *object = selection[0];
			switch (type)
			{
			case type_x1:
				value = StringHelp::int_to_text(object->get_geometry().left);
				break;
			case type_y1:
				value = StringHelp::int_to_text(object->get_geometry().top);
				break;
			case type_x2:
				value = StringHelp::int_to_text(object->get_geometry().right);
				break;
			case type_y2:
				value = StringHelp::int_to_text(object->get_geometry().bottom);
				break;
			case type_width:
				value = StringHelp::int_to_text(object->get_width());
				break;
			case type_height:
				value = StringHelp::int_to_text(object->get_height());
				break;
			}
		}
	}

	void apply_changes(GridObject *object)
	{
		int new_value = StringHelp::text_to_int(value);

		Rect geometry = object->get_geometry();
		int width = geometry.get_width();
		int height = geometry.get_height();

		switch (type)
		{
		case type_x1:
			geometry.left = new_value;
			geometry.right = geometry.left + width;
			break;
		case type_y1:
			geometry.top = new_value;
			geometry.bottom = geometry.top + height;
			break;
		case type_x2:
			geometry.right = new_value;
			geometry.left = geometry.right - width;
			break;
		case type_y2:
			geometry.bottom = new_value;
			geometry.top = geometry.bottom - height;
			break;
		case type_width:
			geometry.right = geometry.left + new_value;
			break;
		case type_height:
			geometry.bottom = geometry.top + new_value;
			break;
		}
		if (geometry.left < geometry.right && geometry.top < geometry.bottom)
			object->set_geometry(geometry);
	}

private:
	static std::string get_tag_name(PositionType type)
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
