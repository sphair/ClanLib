#pragma once
#ifndef LIGHTCONTAINER_H
#define LIGHTCONTAINER_H

#include "Light.h"

class LightContainer
{
public:
	LightContainer()
	{
		index = 0;
	};

	~LightContainer()
	{
		// clear() should be called after the main gameloop ends.
	};

	void add(Light *light)
	{
		index++;

		light->set_id(index);
		lights[index] = light;
	};
	
	void remove(unsigned int loc)
	{
		if(lights[loc])
		{
			delete lights[loc];
			lights.erase(loc);

			CL_Console::write_line("Removed light %1 (container = %2)", loc, (int) lights.size());
		}
	};

	void clear()
	{
		for(std::map<unsigned int, Light*>::iterator it = lights.begin(); it != lights.end(); it++)
		{
			delete (*it).second;
		}
		lights.clear();
	};

	void update(float micro_second)
	{
		for(std::map<unsigned int, Light*>::iterator it = lights.begin(); it != lights.end(); it++)
		{
			if( (*it).second )
			{
				(*it).second->update(micro_second);
				if( (*it).second->get_alive() == false )
				{
					remove( (*it).first );
					break;
				}
			}
		}
	};

	void draw_clips(CL_GraphicContext gc)
	{
		for(std::map<unsigned int, Light*>::iterator it = lights.begin(); it != lights.end(); it++)
		{
			if( (*it).second )
				(*it).second->cutout(gc);
		}
	};

	void draw(CL_GraphicContext gc)
	{
		for(std::map<unsigned int, Light*>::iterator it = lights.begin(); it != lights.end(); it++)
		{
			if( (*it).second )
				(*it).second->draw(gc);
		}
	};

	unsigned int size()
	{
		return lights.size();
	};

private:
	std::map<unsigned int, Light*>		lights;
	unsigned int						index;
};

#endif
