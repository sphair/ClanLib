
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
