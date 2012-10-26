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
*/

#pragma once

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

			Console::write_line("Removed light %1 (container = %2)", loc, (int) lights.size());
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

	void draw_clips(GraphicContext gc)
	{
		for(std::map<unsigned int, Light*>::iterator it = lights.begin(); it != lights.end(); it++)
		{
			if( (*it).second )
				(*it).second->cutout(gc);
		}
	};

	void draw(GraphicContext gc)
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
