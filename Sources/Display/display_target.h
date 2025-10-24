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

#ifndef header_display_target
#define header_display_target

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

#include "API/Display/surface.h"

class CL_Font_Generic;
class CL_Font_Target;
class CL_DisplayWindow_Generic;
class CL_Surface_Target;
class CL_Canvas_Generic;

class CL_API_DISPLAY CL_DisplayTarget
{
//! Construction:
public:
	CL_DisplayTarget();

	virtual ~CL_DisplayTarget();

//! Attributes:
public:
	static CL_DisplayTarget *current();
	static CL_DisplayTarget *current_target;

//! Operations:
public:
	virtual CL_DisplayWindow_Generic *create_window()=0;

	virtual CL_Surface_Target *create_surface(
		CL_PixelBuffer provider,
		int flag)=0;

	virtual CL_Canvas_Generic *create_canvas()=0;

	virtual CL_Font_Target* create_system_font_target(
		CL_Font_Generic *attributes,
		const std::string &font_name,
		const std::string &letters,
		int height,
		int width,
		bool bold,
		bool italic,
		bool underline,
		bool strikeout)=0;

	virtual bool enable_packer()=0;

//! Implementation:
private:
};

#endif
