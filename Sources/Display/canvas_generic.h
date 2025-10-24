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

#ifndef header_canvas_generic
#define header_canvas_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/canvas.h"

class CL_Surface_Generic;

class CL_Canvas_Generic
{
//! Construction:
public:
	CL_Canvas_Generic() : ref_count(0) { return; }

	virtual ~CL_Canvas_Generic() { return; }

//! Attributes:
public:
	virtual CL_GraphicContext *get_gc() = 0;

	virtual CL_PixelBuffer get_pixeldata(const CL_Rect &area) = 0;

	virtual int get_width() const = 0;
	
	virtual int get_height() const = 0;
	
//! Operations:
public:
	void add_ref() { ref_count++; }

	void release_ref() { ref_count--; if (ref_count == 0) delete this; }

	virtual void set_pixeldata(const CL_Point &dest, const CL_Rect &src, const CL_PixelBuffer &data) = 0;

	virtual void select_surface(CL_Surface_Generic *surface) = 0;
	
	virtual void sync_surface() = 0;

//! Implementation:
private:
	int ref_count;
};

#endif
