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

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_image
#define header_image

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"

class CL_Surface;
class CL_Image_Generic;

//: Image component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_Image : public CL_Component
{
//! Enums:
public:
	//: Mode enums
	enum Mode
	{
		normal,
		center,
		stretch,
		tile
	};

//! Construction:
public:
	//: Image Constructor
	CL_Image(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		CL_Surface *surface,
		bool delete_surface,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		const CL_Rect &pos,
		CL_Surface *surface,
		bool delete_surface,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		const CL_Point &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		const CL_Point &pos,
		CL_Surface *surface,
		bool delete_surface,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Destructor
	virtual ~CL_Image();

//! Attributes:
public:
	//: Returns the surface.
	CL_Surface *get_surface() const;

	//: Returns the current display mode.
	Mode get_mode() const;

//! Operations:
public:
	//: Sets the surface.
	void set_surface(CL_Surface *surface, bool delete_surface = false);

	//: Sets the display mode.
	void set_mode(Mode mode);

//! Implementation:
private:
	Mode mode;

	CL_Image(const CL_Image &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Image_Generic *impl;
};  

#endif
