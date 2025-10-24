#include "Game.h"
#include "SpriteManager.h"

#include <ClanLib/display.h>

//--------------------------------------------------------------------------
Game::Game(int width, int height, SpriteManager const & sprites)
	: puzzle(width, height)
	, sprites(sprites)
	, position_calc(puzzle, sprites.HandleSize())
{
	on_key_down = CL_Mouse::sig_key_down().connect(this, &Game::OnKeyDown);
}

//--------------------------------------------------------------------------
Game::Game(Game const & copy)
	: puzzle(copy.puzzle)
	, sprites(copy.sprites)
	, position_calc(copy.position_calc)
{
	on_key_down = CL_Mouse::sig_key_down().connect(this, &Game::OnKeyDown);
}

//--------------------------------------------------------------------------
Game & Game::operator=(Game const & copy)
{
	if (this == &copy)
		return *this;

	puzzle = copy.puzzle;
	sprites = copy.sprites;
	position_calc = copy.position_calc;
	
	return *this;
}

//--------------------------------------------------------------------------
Game::~Game()
{
}

//--------------------------------------------------------------------------
void Game::Draw(DrawStatus draw_status)
{
	DrawPuzzle(draw_status);
	DrawMouse(CL_Point(CL_Mouse::get_x(), CL_Mouse::get_y()));
}

//--------------------------------------------------------------------------
void Game::DrawPuzzle(DrawStatus draw_status)
{
	// draw main field
	for(int y = 0; y < puzzle.Height(); ++y)
		for(int x = 0; x < puzzle.Width(); ++x)
		{
			CL_Sprite & handle = puzzle(x, y) ? sprites(SpriteManager::HANDLE_ON) : sprites(SpriteManager::HANDLE_OFF);
			CL_Point p(position_calc.FieldToScreen(CL_Point(x, y)));
			handle.draw(p.x, p.y);
		}

	if (draw_status == SHOW_HINTS)
	{
		// draw hints
		Puzzle::HintVectorType const & hints = puzzle.Hints();
		for(int i = 0; i < (int)hints.size(); ++i)
		{
			int x = hints[i].first;
			int y = hints[i].second;

			CL_Sprite & handle = puzzle(x, y) ? sprites(SpriteManager::HANDLE_ON_HINT) : sprites(SpriteManager::HANDLE_OFF_HINT);
			CL_Point p(position_calc.FieldToScreen(CL_Point(x, y)));
			handle.draw(p.x, p.y);
		}
	}
}

//--------------------------------------------------------------------------
void Game::DrawMouse(CL_Point const & coord)
{
	CL_Point bad_point(-1, -1);
	CL_Point field(position_calc.ScreenToField(coord));
	if (field != bad_point)
	{
		CL_Sprite & handle = puzzle(field.x, field.y) ? sprites(SpriteManager::HANDLE_ON_HIGHLIGHT) : sprites(SpriteManager::HANDLE_OFF_HIGHLIGHT);
		CL_Point p(position_calc.FieldToScreen(field));
		handle.draw(p.x, p.y);
	}
}

//--------------------------------------------------------------------------
void Game::OnKeyDown(CL_InputEvent const & key)
{
	if (key.id == CL_MOUSE_LEFT)
		MouseClick(key.mouse_pos);
}

//--------------------------------------------------------------------------
void Game::MouseClick(CL_Point const & coord)
{
	CL_Point bad_point(-1, -1);
	CL_Point field(position_calc.ScreenToField(coord));
	if (field != bad_point)
		puzzle.FlipAt(field.x, field.y);
}

//--------------------------------------------------------------------------
bool Game::IsSolved() const
{
	return puzzle.IsSolved();
}
