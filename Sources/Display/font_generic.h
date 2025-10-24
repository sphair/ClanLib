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

#ifndef header_font_generic
#define header_font_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/sprite.h"
#include "API/Core/Math/origin.h"
#include "API/Display/font.h"
#include "API/Core/Math/point.h"
#include "API/Core/System/clonable.h"
#include <string>

class CL_GraphicContext;
class CL_GlyphBuffer;
class CL_Size;
class CL_Rect;
class CL_Font_Target;

class CL_Font_Generic : public CL_Clonable
{
//! Construction:
public:
	CL_Font_Generic(); 

	CL_Font_Generic(const CL_Font_Generic &copy);
	
	virtual ~CL_Font_Generic();

//! Attributes:
public:
	//: Alters the percieved width of each glyph
	int width_offset;
	
	//: Alters the percieved height of each line
	int height_offset;
	
	//: Alignment
	CL_Origin trans_origin;

	int trans_x;
	
	int trans_y;
	
	//: A list of characters (other than the newline character) that seperate words
	std::string delims;

	CL_SharedPtr<CL_Font_Target> target;
	
//! Operations:
public:
	virtual CL_Clonable *clone() const { return new CL_Font_Generic(*this); }

//! Implementation
private:
};

#endif
