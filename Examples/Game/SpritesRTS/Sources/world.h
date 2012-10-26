/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    
*/

#pragma once

#include <ClanLib/core.h>
#include <list>
using namespace clan;

class Surface;
class InputEvent;
class GameObject;
class TankVehicle;

class World
{
// Construction
public:
	World(DisplayWindow &display_window);
	~World();
	
// Attributes:
public:
	ResourceManager resources;
	
	GraphicContext get_gc() { return gc; }

// Operations:
public:
	void initLevel();
	
	bool hitCheck(CollisionOutline *outline, GameObject *other);
	
	void addObject(GameObject *object);
	void addTank(TankVehicle *tank);

	void run();

// Implementation:
private:
	void draw();
	void update();

	int calcTimeElapsed();

	void onKeyDown(const InputEvent &key);
	void onMouseDown(const InputEvent &key);
	void onMouseUp(const InputEvent &key);
	void onMouseMove(const InputEvent &key);
	void on_window_close();

	Slot slotMouseDown;
	Slot slotMouseUp;
	Slot slotMouseDblClick;
	Slot slotMouseMove;
	Slot slotKeyDown;
	
	Image background;
	
	bool dragging;
	bool mouseDown;
	Rect dragArea;
	float highlightValue;
		
	std::list<GameObject *> objects;
	std::list<TankVehicle *> tanks;

	DisplayWindow window;
	GraphicContext gc;

	bool quit;
public:
	BlendState blendstate_cl_blend_zero_cl_blend_one_minus_src_alpha;
	BlendState blendstate_cl_blend_src_alpha_cl_blend_one_minus_src_alpha;
	BlendState blendstate_default;

};
