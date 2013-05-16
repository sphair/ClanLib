
#pragma once

class GameWorld;

class Age2D
{
public:
	Age2D(clan::Canvas &canvas, GameWorld *game);
	void render(clan::Canvas &canvas, int elapsed_time_ms);

private:
	GameWorld *game;
	clan::Sprite tiles;
	clan::Sprite pacman[4]; // left, right, up, down
	clan::Sprite ghost[2]; // green and red ghost animations
	clan::Vec2f look_at;
};
