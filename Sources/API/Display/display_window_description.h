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

#ifndef header_display_window_description
#define header_display_window_description

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

#include <string>
#include "../Core/Math/rect.h"
#include "../Core/System/sharedptr.h"

class CL_DisplayWindowDescription_Generic;

//: Display window description class.
//- !group=Display/Display 2D!
//- !header=display.h!
//- <p>This class allows you to setup a more advanced description when creating
//- a display window.</p>
class CL_API_DISPLAY CL_DisplayWindowDescription
{
//! Construction:
public:
	//: Constructs a window description with default values.
	CL_DisplayWindowDescription();

	CL_DisplayWindowDescription(const CL_DisplayWindowDescription &copy);

	virtual ~CL_DisplayWindowDescription();

//! Attributes:
public:
	//: Returns the window title stored in the description.
	const std::string &get_title() const;

	//: Returns the size of the window stored in description.
	CL_Size get_size() const;

	//: Returns the initial position of the window.
	CL_Rect get_position() const;

	//: Returns if window should be initially fullscreen.
	bool is_fullscreen() const;

	//: Returns the amount of flipping buffers to be created.
	int get_flipping_buffers() const;

	//: Returns true if window should be resizeable.
	bool get_allow_resize() const;
	
	//: Returns the numbers of bytes per pixel in the window.
	int get_bpp() const;

	//: Returns the refresh rate of the displaymode.
	//- <p>Specifies the frequency, in hertz (cycles per second).
	//- This value is also known as the vertical refresh rate.</p> 
	int get_refresh_rate() const;

#ifdef WIN32
	//: Windows 2000/XP only. Returns true if window is layered.
	bool is_layered() const;
#endif

//! Operations:
public:
	//: Copy assignment operator.
	CL_DisplayWindowDescription &operator =(const CL_DisplayWindowDescription &copy);

	//: Sets the title of the window.
	void set_title(const std::string &title);

	//: Sets the size of the window.
	void set_size(const CL_Size &size);

	//: Sets the position of the window.
	void set_position(const CL_Rect &position);

	//: Makes the window initially fullscreen.
	void set_fullscreen(bool fullscreen = true);

	//: Sets the amount of flipping buffers to be used.
	void set_flipping_buffers(int num_buffers = 2);

	//: Sets if windows should be resizeable.
	void set_allow_resize(bool allow_resize);
	
	//: Sets the number of bytes per pixel
	void set_bpp(int bpp);

	//: Set the refresh rate of the displaymode.
	//- <p>Specifies the frequency, in hertz (cycles per second).
	//- This value is also known as the vertical refresh rate.</p> 
	void set_refresh_rate(int refresh_rate);

#ifdef WIN32
	//: Windows 2000/XP only. Creates a layered window (complex shaped window).
	void set_layered(bool layered);
#endif

//! Implementation:
protected:
	CL_DisplayWindowDescription(CL_DisplayWindowDescription_Generic *impl);

public:
	CL_SharedPtr<CL_DisplayWindowDescription_Generic> impl;
};

#endif
