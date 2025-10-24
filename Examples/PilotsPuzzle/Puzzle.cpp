#include "Puzzle.h"
#include <algorithm>

#include <stdlib.h> // for random
#include <ClanLib/core.h> // for get_time

// private namespace
namespace
{
	int Random(int range)
	{
		return static_cast<int>(double(rand()) / RAND_MAX * range);
	}
}

//--------------------------------------------------------------------------
Puzzle::Puzzle(int width, int height)
	: map(width, std::vector<bool>(height, false))
	, width(width)
	, height(height)
	, is_solved(false)
{
	srand(CL_System::get_time());
	do
	{
		Shuffle();
	} while(IsSolved());
}

//--------------------------------------------------------------------------
Puzzle::~Puzzle()
{
}


//--------------------------------------------------------------------------
int Puzzle::Width() const
{
	return width;
}

//--------------------------------------------------------------------------
int Puzzle::Height() const
{
	return height;
}

//--------------------------------------------------------------------------
Puzzle::HintVectorType const & Puzzle::Hints() const
{
	return hints;
}

//--------------------------------------------------------------------------
bool Puzzle::IsSolved() const
{
	// the Hints are not necessary the best way to solve
	//return hints.empty();
	return is_solved;
}

//--------------------------------------------------------------------------
bool Puzzle::operator()(int x, int y) const
{
	if (x < width && y < height)
		return map[x][y];

	return false; // or throw error ?
}

//--------------------------------------------------------------------------
void Puzzle::FlipAt(int x, int y)
{
	if (	x >= 0 && x < width
		&&	y >= 0 && y < height)
	{
		// if it was in hints, remove
		// otherwise, add
		HintVectorType::value_type move(x, y);
		HintVectorType::iterator hint_iterator = std::find(hints.begin(), hints.end(), move);
		if (hint_iterator == hints.end())
			// not found, add move to hints
			hints.push_back(move);
		else
			// ok, remove hint
			hints.erase(hint_iterator);

		// Fliping row
		for(int row = 0; row < height; ++row)
			map[x][row] = !map[x][row];

		// Fliping column
		for(int column = 0; column < width; ++column)
			map[column][y] = !map[column][y];

		// Flip coord itself
		map[x][y] = !map[x][y];

		// Updating solution status
		is_solved = CheckSolution();
	}
}

//--------------------------------------------------------------------------
void Puzzle::Shuffle()
{
	int shuffle_times = 10 + Random(width * height);
	for(int i = 0; i < shuffle_times; ++i)
		FlipAt(Random(width), Random(height));
}

//--------------------------------------------------------------------------
//void Puzzle::Solve()
//{
//}

//--------------------------------------------------------------------------
bool Puzzle::CheckSolution() const
{
	for(int y = 0; y < height; ++y)
		for(int x = 0; x < width; ++x)
			if (map[x][y])
				return false;
	return true;
}
