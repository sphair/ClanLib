/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "API/SDL/setupsdl.h"
#include "API/Core/System/error.h"
#include <SDL/SDL.h>
#include "sdl_target.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSDL construction:

static CL_SDLTarget *target = 0;

static int ref_count = 0;

CL_SetupSDL::CL_SetupSDL(bool register_resources_only)
{
	CL_SetupSDL::init(register_resources_only);
}

CL_SetupSDL::~CL_SetupSDL()
{
	CL_SetupSDL::deinit();
}

void CL_SetupSDL::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count != 1) return;

	if (register_resources_only) return;

	// Lawouach : I guess an error here is supposed to throw a CL_Error ?
	// BTW, it seems that SDL_INIT_AUDIO component has to be initiliazed before the display window under DirectX (well SDL is still based on DX 5 !!)
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0)
		throw CL_Error(SDL_GetError());

	// Enable unicode support in keyboard events:
	// Lawouach : This slows down the performances
	SDL_EnableUNICODE(1);

	// Enable key repeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

	target = new CL_SDLTarget;
}
	
void CL_SetupSDL::deinit()
{
	ref_count--;
	if (ref_count != 0) return;

	if(target)
		delete target;
	target = 0;

	// Lawouach : when we quit SDL_VIDEO subsystem we don't need to explictely free the display pointer though it wouldn't hurt
	SDL_Quit();
}
