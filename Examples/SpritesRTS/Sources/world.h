#ifndef _WORLD_H_
#define _WORLD_H_

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
	World();
	~World();
	
// Attributes:
public:
	CL_ResourceManager *resources;
	
// Operations:
public:
	void initLevel();
	
	bool hitCheck(CL_CollisionOutline *outline, GameObject *other);
	
	void addObject(GameObject *object);
	void addTank(TankVehicle *tank);

	void run();

// Slots:
public:
	void on_quit() { quit = true; }

// Implementation:
private:
	void draw();
	void update();

	float calcTimeElapsed();

	void onKeyDown(const CL_InputEvent &key);
	void onMouseDown(const CL_InputEvent &key);
	void onMouseUp(const CL_InputEvent &key);
	void onMouseMove(const CL_InputEvent &key);

	CL_Slot slotMouseDown;
	CL_Slot slotMouseUp;
	CL_Slot slotMouseMove;
	CL_Slot slotKeyDown;
	
	CL_Surface *background;

	bool quit;

	bool dragging;
	bool mouseDown;
	CL_Rect dragArea;
	float highlightValue;
		
	std::list<GameObject *> objects;
	std::list<TankVehicle *> tanks;
};

#endif
