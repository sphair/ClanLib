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
*/

#include <cstdlib>
#include <iostream>
#include "API/Core/System/setupcore.h"
#include "API/Application/clanapp.h"
#include <Carbon/carbon.h>

int main(int argc, char **argv)
{
	
	if (CL_ClanApplication::app == NULL)
	{
		std::cout << "ClanLib: No global CL_ClanApplication instance!!!" << std::endl;
		return 255;
	}
	
	// this code will set the startup working directory to the
	// application bundle's Resources folder instead of whatever it was, (which
	// is usually set to something somewhat unhelpful by default).  -- SMH nov 01 2005 
	// (converted to be here from inputsource_file.cpp -- mrfun may 18 2006)
	
	char cwd[PATH_MAX+2];
	CFURLRef ref = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	if (ref && CFURLGetFileSystemRepresentation(ref, true, (UInt8*)cwd, PATH_MAX))
	{
		CFRelease(ref);
		chdir(cwd); //move to this apps Resources folder
	}
	
	CL_SetupCore::init();

	int retval = CL_ClanApplication::app->main(argc, argv);
	CL_SetupCore::deinit();

	return retval;
}

CL_ClanApplication *CL_ClanApplication::app = NULL;

CL_ClanApplication::CL_ClanApplication()
{
	CL_ClanApplication::app = this;
}

CL_ClanApplication::~CL_ClanApplication()
{
	CL_ClanApplication::app = NULL;
}
