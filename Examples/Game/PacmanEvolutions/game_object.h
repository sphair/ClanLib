
#pragma once

class GameWorld;

class GameObject
{
public:
	GameObject(GameWorld *world) : world(world) { }
	virtual ~GameObject() { }

	virtual float get_x() = 0;
	virtual float get_y() = 0;
	virtual bool turn(float time_elapsed) = 0;

protected:
	GameWorld *world;
};
