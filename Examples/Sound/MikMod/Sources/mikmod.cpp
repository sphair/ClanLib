/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "mikmod.h"
#include "setupmikmod.h"

// The start of the Application
int MikMod::start(const std::vector<std::string> &args)
{
	// Initialize mikmod
	SetupMikMod setup_mikmod;

	clan::SoundOutput sound_output(44100, 192);

	clan::SoundBuffer boss = clan::SoundBuffer("Resources/boss.mod");


	clan::SoundBuffer_Session session = boss.prepare();
	session.play();

#ifdef WIN32
	clan::Console::write_line("Press any key to exit");
#else
	clan::Console::write_line("CTRL-C to exit");
#endif

	while(session.is_playing())
	{
#ifdef WIN32
		if (kbhit())
			break;
#endif

		clan::KeepAlive::process(100);
	}

	return 0;
}



