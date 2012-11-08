
#pragma once

class GameWorld;

class Age2D
{
public:
	Age2D(CL_GraphicContext &gc, GameWorld *game);
	void render(CL_GraphicContext &gc);

private:
	GameWorld *game;
	CL_Sprite tiles;
	CL_Sprite pacman[4]; // left, right, up, down
	CL_Sprite ghost[2]; // green and red ghost animations
	CL_Vec2f look_at;
};
