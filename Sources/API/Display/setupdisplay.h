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

//! clanDisplay="System"
//! header=display.h

#ifndef header_setupdisplay
#define header_setupdisplay

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

//: ClanDisplay initialization functions.
//- !group=Display/System!
//- !header=display.h!
class CL_API_DISPLAY CL_SetupDisplay
{
//! Construction:
public:
	//: Initializes clanDisplay.
	//- <p>Constructing a CL_SetupDisplay object is equalent to calling CL_SetupDisplay::init().</p>
	//- <p>When the CL_SetupDisplay instance is destroyed, CL_SetupDisplay::deinit() is called.</p>
	CL_SetupDisplay(bool register_resources_only = false);

	~CL_SetupDisplay();

//! Operations:
public:
	//: Initialize the clanDisplay library.
	static void init(bool register_resources_only = false);
	
	//: Deinitialize the clanDisplay library.
	static void deinit();
};

#endif
