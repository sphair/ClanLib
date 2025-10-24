
#pragma once

class ListviewExMode
{
public:
	virtual ~ListviewExMode() { }

	virtual int get_line_step() = 0;
	virtual CL_Size get_absolute_size() = 0;

	virtual CL_Point to_absolute_position(const CL_Point &position) = 0;
	virtual CL_Rect to_absolute_position(const CL_Rect &position) = 0;
	virtual CL_Point to_relative_position(const CL_Point &position) = 0;
	virtual CL_Rect to_relative_position(const CL_Rect &position) = 0;

	virtual int hit_test(const CL_Point &absolute_position) = 0;
	virtual int insert_mark_hit_test(const CL_Point &absolute_position) = 0;

	virtual CL_Rect get_item_rect(int item_index, int column = 0) = 0;
	virtual CL_Rect get_item_icon_rect(int item_index, int column = 0) = 0;
	virtual CL_Rect get_item_text_rect(int item_index, int column = 0) = 0;

	virtual void draw_area(CL_GraphicContext &gc, const CL_Rect &absolute_rect, const CL_Rect &clip_rect) = 0;
	virtual void select_items(const CL_Rect &selection_rect) = 0;
};
