/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "graphic_store.h"

GraphicStore::GraphicStore(CL_GraphicContext &gc) : shader_texture(gc)
{
}

GraphicStore::~GraphicStore()
{
}

void GraphicStore::LoadImages( CL_GraphicContext &gc )
{
	// Load the texture

/*
	CL_PixelBuffer test(1024, 768, cl_rgba8);
	unsigned int *dptr = (unsigned int *) test.get_data();
	for(int ypos = 0; ypos < 768; ypos++)
	{
		for(int xpos = 0; xpos < 1024; xpos++, dptr++)
		{
			*dptr = 0x777777ff;
			if ((xpos & 15) <= 3)
			{
				*dptr = 0xffffffff;
			}
			if ((ypos & 15) <= 3)
			{
				*dptr = 0xffffffff;
			}
		}
	}

	texture_underwater = CL_Texture(gc, 1024, 768, cl_rgba8);
	texture_underwater.set_subimage(0, 0, test, test.get_size());
	*/
	texture_underwater = CL_Texture(gc, "../../Game/SpritesRTS/Gfx/background.png");
	texture_underwater.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat, cl_wrap_repeat);

	texture_background = CL_Texture(gc, "../../Game/DiceWar/Resources/lobby_background1.png");
	texture_underwater.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat, cl_wrap_repeat);
}
