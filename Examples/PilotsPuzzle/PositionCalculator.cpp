#include "PositionCalculator.h"

#include "Puzzle.h"
#include <ClanLib/display.h>

//--------------------------------------------------------------------------
PositionCalculator::PositionCalculator(Puzzle const & puzzle, CL_Size const & sprite_size, int step)
	: sprite_size(sprite_size)
	, step(step)
	, field_size(
		sprite_size.width * puzzle.Width() + step * (puzzle.Width() - 1),
		sprite_size.height * puzzle.Height() + step * (puzzle.Height() - 1))
	, left_top(
		(CL_Display::get_width() - field_size.width) / 2,
		(CL_Display::get_height() - field_size.height) / 2)
{
}

//--------------------------------------------------------------------------
PositionCalculator::~PositionCalculator()
{
}

//--------------------------------------------------------------------------
CL_Point PositionCalculator::ScreenToField(CL_Point const & screen_coord) const
{
	CL_Point bad_point(-1, -1);

	if (	screen_coord.x < left_top.x
		||	screen_coord.x > (left_top.x + field_size.width)
		||	screen_coord.y < left_top.y
		||	screen_coord.y > (left_top.y + field_size.height) )
	{
		return bad_point;
	}

	int tmp_x = (screen_coord.x - left_top.x) % (sprite_size.width + step);
	int tmp_y = (screen_coord.y - left_top.y) % (sprite_size.height + step);

	if (tmp_x < sprite_size.width && tmp_y < sprite_size.height)
	{
		return CL_Point(
			(screen_coord.x - left_top.x) / (sprite_size.width + step),
			(screen_coord.y - left_top.y) / (sprite_size.height + step));
	}
	
	return bad_point;
}

//--------------------------------------------------------------------------
CL_Point PositionCalculator::FieldToScreen(CL_Point const & field_coord) const
{
	return CL_Point(
		left_top.x + field_coord.x * (sprite_size.width + step),
		left_top.y + field_coord.y * (sprite_size.height + step));
}
