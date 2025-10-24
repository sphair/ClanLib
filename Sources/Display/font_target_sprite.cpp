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

#include "Display/display_precomp.h"
#include "font_target_sprite.h"
#include "font_generic.h"
#include "API/Core/Math/size.h"
#include "API/Core/Math/rect.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/sprite.h"
#include <cmath>
#include <algorithm>
#include <vector>

#define cl_min(a,b) ((a < b) ? a : b)
#define cl_max(a,b) ((a > b) ? a : b)

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Target_Sprite construction:

CL_Font_Target_Sprite::CL_Font_Target_Sprite(
	CL_Font_Generic *attributes,
	const CL_Sprite &new_glyphs,
	const std::string &letters,
	int new_spacelen,
	bool monospace)
: CL_Font_Target(attributes)
{
	fixed_width = 0;
	height = 0;

	//Size of char limit on most platforms is 256
	const int length = cl_min((int)(letters.size()), 256);
	
	if ((length > new_glyphs.get_frame_count()) || (length == 0))
	{
		throw CL_Error(
			CL_String::format(
				"Font error: Letter characters: %1, Available font glyphs: %2", 
				length,
				new_glyphs.get_frame_count()));
	}
	
	//If monospace font requested, find the width of the widest glyph
	//Then set the fixed_width var to that width, which gwidth() checks for
	//Also set space to that width
	if (monospace)
	{
		for (short i=0; i < length; ++i)
		{
			int glyph_width = new_glyphs.get_frame_size(i).width;
			if (glyph_width > fixed_width)
				fixed_width = glyph_width;
		}
		
		new_spacelen = fixed_width;
	}
	//If not monospace, and space width not specified, then use average width as space width
	else if (new_spacelen < 0)
	{
		std::string::size_type space_pos = letters.find(' ');
		
		if (space_pos != std::string::npos)
		{
			//If there is a character for space, then use it
			new_spacelen = new_glyphs.get_frame_size((int)space_pos).width;
		}
		else
		{
			//Make the space size the average of all character sizes
			new_spacelen = 0;
			
			for (int pos = 0; pos < length; ++pos)
			{
				new_spacelen += new_glyphs.get_frame_size((int)(pos)).width;
			}
			
			new_spacelen /= length;
		}
	}
	
	for (short i=0; i < length; ++i)
	{
		int glyph_height = new_glyphs.get_frame_size(i).height;
		if (glyph_height > height)
			height = glyph_height;
	}
	
	glyphs = new_glyphs;
	glyphs.set_alignment(origin_top_left, 0, 0);
	glyphs.set_rotation_hotspot(origin_top_left, 0, 0);
	spacelen = new_spacelen;
	
	//If monospace font requested, find the width of the widest glyph
	//Then set the fixed_width var to that width, which gwidth() checks for
	if (monospace)
	{
		for (short i=0; i < length; ++i)
		{
			int glyph_width = new_glyphs.get_frame_size(i).width;
			if (glyph_width > fixed_width)
				fixed_width = glyph_width;
		}
	}	

	// Setup char to glyph map:

	char_to_glyph_map.reserve(256);
	for (int index = 0; index < 256; index++) char_to_glyph_map.push_back(static_cast<unsigned int>(-1));

	std::string::const_iterator it = letters.begin();
	for (unsigned short index_letters=0; index_letters < letters.length(); ++index_letters)
	{
		unsigned int letter = ((unsigned char)*it);
		char_to_glyph_map[letter] = index_letters;
		++it;
	}
}

CL_Font_Target_Sprite::~CL_Font_Target_Sprite()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Target_Sprite attributes:

int CL_Font_Target_Sprite::get_fixed_width() const
{
	return fixed_width;
}

int CL_Font_Target_Sprite::get_width(unsigned int chr) const
{
	if (chr == '\n')
		return 0;
	
	int width = attributes->width_offset;
	
	if (fixed_width != 0)
	{
		width += fixed_width;
	}
	else
	{
		short glyph_num = char_to_glyph_map[chr];
		if (glyph_num != -1)
			width += glyphs.get_frame_size(glyph_num).width;
		else
			width += spacelen;
	}
	
	float scale_x, scale_y;
	get_scale(scale_x, scale_y);
	return cl_max(0, (int)(width*scale_x));
}

int CL_Font_Target_Sprite::get_height() const
{
	float scale_x, scale_y;
	get_scale(scale_x, scale_y);
	
	return (int)((height + attributes->height_offset) * scale_y);
}

bool CL_Font_Target_Sprite::is_glyph(unsigned int chr) const
{
	if (char_to_glyph_map[chr] == static_cast<unsigned int>(-1))
		return false;
	
	return true;
}

void CL_Font_Target_Sprite::get_scale(float& x, float& y) const
{
	glyphs.get_scale(x, y);
}

float CL_Font_Target_Sprite::get_alpha() const
{
	return glyphs.get_alpha();
}

void CL_Font_Target_Sprite::get_color(float& r, float& g, float& b, float& a) const
{
	glyphs.get_color(r, g, b, a);
}

void CL_Font_Target_Sprite::get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const
{
	glyphs.get_blend_func(src, dest);
}

void CL_Font_Target_Sprite::get_rot_hotspot(CL_Origin &origin, int &x, int &y) const
{
	glyphs.get_rotation_hotspot(origin, x, y);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Target_Sprite operations:

void CL_Font_Target_Sprite::set_scale(float x, float y)
{
	glyphs.set_scale(x, y);
}

void CL_Font_Target_Sprite::set_alpha(float a)
{
	glyphs.set_alpha(a);
}

void CL_Font_Target_Sprite::set_color(float r, float g, float b, float a)
{
	glyphs.set_color(r, g, b, a);
}

void CL_Font_Target_Sprite::set_blend_func(CL_BlendFunc src, CL_BlendFunc dest)
{
	glyphs.set_blend_func(src, dest);
}

void CL_Font_Target_Sprite::set_rot_hotspot(CL_Origin origin, int x, int y)
{
	glyphs.set_rotation_hotspot(origin, x, y);
}

void CL_Font_Target_Sprite::draw_glyph(int x, int y, unsigned int glyph_index, float ang, CL_GraphicContext *gc) const
{
	if (glyph_index != static_cast<unsigned int>(-1))
	{
		if (ang != 0.0)
			glyphs.rotate(ang);
		
		glyphs.set_frame(glyph_index);
		glyphs.draw(x, y, gc);

		if (ang != 0.0)
			glyphs.rotate(-ang);
	}
}

void CL_Font_Target_Sprite::draw_character(int x, int y, unsigned int chr, float ang, CL_GraphicContext *gc) const
{
	short glyph_num = char_to_glyph_map[chr];
	if (glyph_num != -1) draw_glyph(x, y, glyph_num, ang, gc);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Target_Sprite implementation:
