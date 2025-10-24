#ifndef HEADER_PUZZLE_H
#define HEADER_PUZZLE_H

#include <vector>
#include <utility>

class Puzzle
{
public:
	Puzzle(int width, int height);
	~Puzzle();

	// Typedefs
public:
	typedef std::vector<std::vector<bool> > MapType;
	typedef std::vector<std::pair<int, int> > HintVectorType;

	// Attributes
public:
	// Return width of puzzle
	int Width() const;

	// Return height of puzzle
	int Height() const;

	// Gives a hints to solve a puzzle.
	HintVectorType const & Hints() const;

	// Return true, if puzzle is solved
	bool IsSolved() const;

	// Get state of puzzle at specified coords
	bool operator()(int x, int y) const;

	// Operations
public:
	// Change state at specified coords
	void FlipAt(int x, int y);

	// Shuffle puzzle
	void Shuffle();

	//Puzzle & operator=(Puzzle const & copy);

private:
	// Solve puzzle.
	//void Solve();

	bool CheckSolution() const;

private:
	MapType map;
	HintVectorType hints;

    int width;
	int height;

	bool is_solved;
};

#endif // HEADER_PUZZLE_H
