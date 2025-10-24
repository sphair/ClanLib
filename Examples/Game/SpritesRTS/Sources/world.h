/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

class CL_Surface;
class CL_InputEvent;
class GameObject;
class TankVehicle;

class World
{
// Construction
public:
	World(CL_DisplayWindow &display_window);
	~World();
	
// Attributes:
public:
	CL_ResourceManager resources;
	
	CL_GraphicContext get_gc() { return gc; }

// Operations:
public:
	void initLevel();
	
	bool hitCheck(CL_CollisionOutline *outline, GameObject *other);
	
	void addObject(GameObject *object);
	void addTank(TankVehicle *tank);

	void run();

// Implementation:
private:
	void draw();
	void update();

	int calcTimeElapsed();

	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
	void onMouseDown(const CL_InputEvent &key, const CL_InputState &state);
	void onMouseUp(const CL_InputEvent &key, const CL_InputState &state);
	void onMouseMove(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();

	CL_Slot slotMouseDown;
	CL_Slot slotMouseUp;
	CL_Slot slotMouseMove;
	CL_Slot slotKeyDown;
	
	CL_Texture background;
	
	bool dragging;
	bool mouseDown;
	CL_Rect dragArea;
	float highlightValue;
		
	std::list<GameObject *> objects;
	std::list<TankVehicle *> tanks;

	CL_DisplayWindow window;
	CL_GraphicContext gc;

	bool quit;
};
