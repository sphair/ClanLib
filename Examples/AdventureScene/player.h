
#ifndef header_player
#define header_player

#if _MSC_VER > 1000
#pragma once
#endif

#include "object.h"

class Scene;

class Player : public Object
{
public:
	Player(Scene *scene);
	
private:
	bool check_walk_map(int x, int y);
	float get_walk_map_depth(int x, int y);

	void on_key_down(const CL_InputEvent &key);
	void on_key_up(const CL_InputEvent &key);
	CL_Slot slot_key_down;

	Scene *scene;
	
	int width, height;
};

#endif
