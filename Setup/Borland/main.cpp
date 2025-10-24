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
**    (if your name is missing here, please add it)
*/

//---------------------------------------------------------------------------

#include <iostream.h>
#include <string>
#include <conio.h>
#include "generator.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
	char tasm;

	Workspace work;

	std::list<std::string> libs_list_shared;
	std::list<std::string> libs_list_release;
	std::list<std::string> libs_list_debug;
	std::list<std::string> defines_list;

	defines_list.push_back("WIN32");
	defines_list.push_back("_WINDOWS");
	defines_list.push_back("USE_OPENGL");
	defines_list.push_back("USE_NETWORK");
	defines_list.push_back("BORLAND");
	defines_list.push_back("USE_CLANSOUND");
	defines_list.push_back("DIRECTINPUT_VERSION=0x800");
	defines_list.push_back("HAVE_BOOLEAN");

	cout << "Welcome to ClanLib Borland Configurer!" <<endl;
	cout<<"Do you have TASM?[y/n]";
	cin>>tasm;
	if(tasm == 'y' || tasm == 'Y')
	{
		defines_list.push_back("USE_I386_ASSEMBLER");
		defines_list.push_back("USE_TASM");
	}

	Project app("Application","libApp","application.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project core("Core","libCore","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project display("Display","libDisplay","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  //	Project dx7("DX7","libdx7","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
    //	Project dx8("DX8","libdx8","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project gl("GL","libGL","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project gui("GUI","libGUI","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
        Project guistyleSilver("GUIStyleSilver","libGUIStyleSilver","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project guistyleBoring("GUIStyleBoring","libGUIStyleBoring","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
   //	Project jpeg("JPEG","libJpeg","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project javascript("JavaScript","libJavaScript","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);

	Project mikmod("MikMod","libMikMod","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project network("Network","libNetwork","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  //	Project png("PNG","libPng","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project SDL("SDL","libSDL-Clanlib","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project sound("Sound","libSound","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
	Project vorbis("Vorbis","libVorbis","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);

	work.projects.push_back(app);
	work.projects.push_back(core);
	work.projects.push_back(display);
	work.projects.push_back(gl);
	work.projects.push_back(gui);
        work.projects.push_back(guistyleSilver);
        work.projects.push_back(guistyleBoring);
    //	work.projects.push_back(jpeg);
     	work.projects.push_back(javascript);
	work.projects.push_back(mikmod);
	work.projects.push_back(network);
       //	work.projects.push_back(png);
	work.projects.push_back(sound);
        work.projects.push_back(SDL);
	work.projects.push_back(vorbis);
       //	work.projects.push_back(dx7);
       //	work.projects.push_back(dx8);

	create_release(work);
	create_makefile(work);



	cout <<"Project creation complete(press any key to quit)." <<endl;

	while(!kbhit()) ;

	return 0;
}
//---------------------------------------------------------------------------
 