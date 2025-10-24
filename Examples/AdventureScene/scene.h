	
#ifndef header_scene
#define header_scene

#if _MSC_VER > 1000
#pragma once
#endif

#include "object.h"

class CL_Surface;

class Scene
{
public:
	Scene(const std::string &background_file, const std::string &mask_file, const std::string &walk_file);
	~Scene();
	
	void add_object(Object *object);
	void update_objects();

	void draw();
	
	CL_PixelBuffer walk_buffer;
	
private:
	CL_Surface background;
	CL_Surface mask;
	CL_Surface walk;
	
	std::vector<Object *> objects;
};

#endif
