/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "precomp.h"

#include "app.h"
#include "world.h"

int App::main(const std::vector<std::string> &args)
{
	// Create a window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib RTS Demo");
	desc.set_size(clan::Size(1024, 768), true);
	clan::DisplayWindow window(desc);

	clan::SoundOutput output(44100);

	// Create world
	World world(window);

	// Run the main loop
	world.run();

	return 0;
}
