#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

class World;
class CL_CollisionOutline;

class GameObject
{
// Construction
public:
	GameObject(World *world) { this->world = world; }
	virtual ~GameObject() {}

// Attributes
public:
	
// Operations
public:
	virtual void draw() {};
	virtual bool update(float timeElapsed) { return false; }
	virtual bool hitCheck(CL_CollisionOutline *outline, GameObject *object) { return false; }
	
protected:
	World *world;
};

#endif
