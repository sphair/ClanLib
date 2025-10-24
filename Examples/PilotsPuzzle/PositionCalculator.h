#ifndef HEADER_POSITION_CALCULATOR_H
#define HEADER_POSITION_CALCULATOR_H

#include <ClanLib/core.h>

class Puzzle;

class PositionCalculator
{
public:
	PositionCalculator(Puzzle const & puzzle, CL_Size const & sprite_size, int step = 5);
	~PositionCalculator();
public:
	CL_Point ScreenToField(CL_Point const & screen_coord) const;
	CL_Point FieldToScreen(CL_Point const & field_coord) const;
private:
	CL_Size sprite_size;
	int step;
	CL_Size field_size;
	CL_Point left_top;
};

#endif // HEADER_POSITION_CALCULATOR_H
