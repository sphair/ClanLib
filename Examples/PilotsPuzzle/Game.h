#ifndef HEADER_GAME_H
#define HEADER_GAME_H

#include <ClanLib/core.h>

#include "Puzzle.h"
#include "SpriteManager.h"
#include "PositionCalculator.h"

class CL_InputEvent;

class Game
{
public:
	Game(int width, int height, SpriteManager const & sprites);
	Game(Game const & copy);
	~Game();

public:
	enum DrawStatus
	{
		NO_HINTS,
		SHOW_HINTS
	};

public:
	void Draw(DrawStatus draw_status = NO_HINTS);
	bool IsSolved() const;
	Game & operator=(Game const & copy);

private:
	void DrawPuzzle(DrawStatus draw_status);
	void DrawMouse(CL_Point const & coord);

private:
	void OnKeyDown(CL_InputEvent const & key);
	void MouseClick(CL_Point const & coord);

private:
	Puzzle puzzle;

	SpriteManager sprites;
	PositionCalculator position_calc;

	CL_Slot on_key_down;
};

#endif // HEADER_GAME_H
