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

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_display_mode
#define header_display_mode

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

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <vector>
#include <string>

class CL_Size;
class CL_DisplayMode_Generic;

//: Display mode class.
//- !group=Display/Display 2D!
//- !header=display.h!
//- <p>This class contains information about one display mode available.
//- It also contains a static function to get a list of all available display modes.</p>
//- <p>Code example which dumps a list over all available modes:</p>
//- <pre>
//- std::vector < CL_DisplayMode > &modes = CL_DisplayMode::get_display_modes();
//- for(unsigned int i=0; i < modes.size(); ++i)
//-     std::cout << modes[i].get_string() << std::endl;
//- </pre>
class CL_API_DISPLAY CL_DisplayMode
{
//! Construction:
public:
	CL_DisplayMode(const CL_Size &res, int bpp, int refresh_rate);

	CL_DisplayMode(const CL_DisplayMode &copy);

	virtual ~CL_DisplayMode();

//! Attributes:
public:
	//: Returns the resolution of the displaymode.
	CL_Size get_resolution() const;

	//: Returns the numbers of bytes per pixel of the displaymode.
	int get_bpp() const;

	//: Returns the refresh rate of the displaymode.
	//- <p>Specifies the frequency, in hertz (cycles per second).
	//- This value is also known as the vertical refresh rate.</p>
	int get_refresh_rate() const;

	//: Returns the information as a string in the format "640x480x32@60"
	std::string get_string() const;

	//: Returns a list over all available display modes.
	static std::vector<CL_DisplayMode> &get_display_modes();

//! Operations:
public:
	//: Copy assignment operator.
	CL_DisplayMode &operator =(const CL_DisplayMode &copy);

//! Implementation:
private:
	CL_DisplayMode_Generic *impl;
};

#endif
