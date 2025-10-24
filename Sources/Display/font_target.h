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

#ifndef header_font_target
#define header_font_target

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

#include "API/Display/blend_func.h"
#include "API/Core/Math/origin.h"

class CL_Font_Generic;
class CL_GraphicContext;

class CL_API_DISPLAY CL_Font_Target
{
//! Construction:
public:
	CL_Font_Target(CL_Font_Generic *attributes) : attributes(attributes) { return; }

	virtual ~CL_Font_Target() { return; }

//! Attributes:
public:
	//: If 0, variable width font. If another number, all characters are this width
	virtual int get_fixed_width() const=0;
	
	//: Gets the width of a single glyph directly from the map, including scaling and offsets
	virtual int get_width(unsigned int chr) const=0;
	
	//: Gets the height of the font, including scaling and offsets
	virtual int get_height() const=0;

	//: Returns true if the given character has any associated glyph
	virtual bool is_glyph(unsigned int chr) const=0;

	virtual void get_scale(float& x, float& y) const=0;

	virtual float get_alpha() const=0;
	
	virtual void get_color(float& r, float& g, float& b, float& a) const=0;
	
	virtual void get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const=0;
	
	virtual void get_rot_hotspot(CL_Origin &origin, int &x, int &y) const=0;

protected:
	CL_Font_Generic *attributes;

//! Operations:
public:
	virtual void set_scale(float x, float y)=0;

	virtual void set_alpha(float a)=0;

	virtual void set_color(float r, float g, float b, float a)=0;

	virtual void set_blend_func(CL_BlendFunc src, CL_BlendFunc dest)=0;

	virtual void set_rot_hotspot(CL_Origin origin, int x, int y)=0;

	//: Draws a glyph at a given location.
	//param x, y: The upper-left coordinates of where to draw the glyph.
	//param chr: The character to draw the glyph of.
	//param ang: The amount to rotate the glyph by. The hotspot is set with CL_Font::set_glyph_rot_hotspot().
	//param gc: Graphic context to render to.
	//- <p> If there is no such glyph for that character, nothing is drawn. </p>
	virtual void draw_glyph(int x, int y, unsigned int chr, float ang, CL_GraphicContext *gc) const=0;

	//: Draws a single character to a given spot.
	//param x, y: The upper-left coordinates of where to draw the character.
	//param chr: The character to draw the glyph of.
	//param ang: The amount to rotate the glyph by. The hotspot is set with CL_Font::set_glyph_rot_hotspot().
	//param gc: Graphic context to render to.
	//- <p> If there is no glyph for the given character, then nothing is drawn. </p>
	virtual void draw_character(int x, int y, unsigned int chr, float ang, CL_GraphicContext *gc) const=0;

//! Implementation:
private:
};

#endif
