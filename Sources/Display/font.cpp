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
#include "API/Display/font.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/graphic_context.h"
#include "API/Display/glyph_buffer.h"
#include "API/Display/color.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "font_generic.h"
#include "font_target.h"
#include "font_target_sprite.h"
#include "resourcedata_font.h"
#include "display_target.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>

#define cl_min(a,b) ((a < b) ? a : b)
#define cl_max(a,b) ((a > b) ? a : b)

/////////////////////////////////////////////////////////////////////////////
// CL_Font construction:

CL_Font::CL_Font()
{
}

CL_Font::CL_Font(const CL_Font &copy)
: impl(copy.impl)
{
	resource = copy.resource;
	resource.load();
}

CL_Font::CL_Font(
	const std::string &resource_id,
	CL_ResourceManager *manager)
{
	resource = manager->get_resource(resource_id);
	resource.load();
	
	CL_ResourceData_Font *data = (CL_ResourceData_Font *) resource.get_data("font");

	if (!data)
		throw CL_Error("Resource '" + resource_id + "' is not of type 'font'");

	operator=(data->get_font());
}

CL_Font::CL_Font(
	const CL_Sprite &letters,
	const std::string &letter_chars,
	int space_width,
	bool monospace)
: impl(new CL_Font_Generic)
{
	impl->target = CL_SharedPtr<CL_Font_Target>(
		new CL_Font_Target_Sprite(
			impl.get(),
			letters,
			letter_chars,
			space_width,
			monospace));
}

CL_Font::CL_Font(
	const std::string &font_name,
	int height,
	int width,
	bool bold,
	bool italic,
	bool underline,
	bool strikeout)
: impl(new CL_Font_Generic)
{
	//First thirty two characters are undrawable in ASCII
	std::string letters;
	letters.reserve(256-32);
	for (int x = 32; x < 256; ++x)
		letters.append(1, (unsigned int)(x));

	impl->target = CL_SharedPtr<CL_Font_Target>(
		CL_DisplayTarget::current()->create_system_font_target(
			impl.get(),
			font_name,
			letters,
			height,
			width,
			bold,
			italic,
			underline,
			strikeout));
}

CL_Font::CL_Font(
	const std::string &font_name,
	const std::string &letter_chars,
	int height,
	int width,
	bool bold,
	bool italic,
	bool underline,
	bool strikeout)
: impl(new CL_Font_Generic)
{
	impl->target = CL_SharedPtr<CL_Font_Target>(
		CL_DisplayTarget::current()->create_system_font_target(
			impl.get(),
			font_name,
			letter_chars,
			height,
			width,
			bold,
			italic,
			underline,
			strikeout));
}

CL_Font::~CL_Font()
{
	resource.unload();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font attributes:

std::string CL_Font::get_delims() const
{
	return impl->delims;
}

int CL_Font::get_width_offset() const
{
	return impl->width_offset;
}

int CL_Font::get_height_offset() const
{
	return impl->height_offset;
}

void CL_Font::get_scale(float &x, float &y) const
{
	impl->target->get_scale(x, y);
}

float CL_Font::get_alpha() const
{
	return impl->target->get_alpha();
}

void CL_Font::get_color(float &red, float &green, float &blue, float &alpha) const
{
	impl->target->get_color(red, green, blue, alpha);
}

CL_Color CL_Font::get_color() const
{
	float r,g,b,a;
	get_color(r, g, b, a);
	return CL_Color(int(r*255.0f), int(g*255.0f), int(b*255.0f), int(a*255.0f));
}

void CL_Font::get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const
{
	impl->target->get_blend_func(src, dest);
}

void CL_Font::get_glyph_rot_hotspot(CL_Origin &origin, int &x, int &y) const
{
	impl->target->get_rot_hotspot(origin, x, y);
}

void CL_Font::get_alignment(CL_Origin &origin, int &x, int &y) const
{
	origin = impl->trans_origin;
	x = impl->trans_x;
	y = impl->trans_y;
}

int CL_Font::get_height() const
{
	return impl->target->get_height();
}

int CL_Font::get_height(const std::string &str, CL_Size max_size) const
{
	return get_size(str, max_size).height;
}

int CL_Font::get_height(
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_Size max_size) const
{
	return get_size(start, end, max_size).height;
}

int CL_Font::get_width(unsigned int letter) const
{
	return impl->target->get_width(letter);
}

int CL_Font::get_width(
	const std::string &str,
	CL_Size max_size) const
{
	return get_size(str, max_size).width;
}

int CL_Font::get_width(
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_Size max_size) const
{
	return get_size(start, end, max_size).width;
}

CL_Size CL_Font::get_size(
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_Size max_size) const
{
	CL_GlyphBuffer gb;
	draw_to_gb(start, end, gb, max_size);
	return gb.get_size();
}

CL_Size CL_Font::get_size(unsigned int letter) const
{
	return CL_Size(get_width(letter), get_height());
}

CL_Size CL_Font::get_size(
	const std::string &str,
	CL_Size max_size) const
{
	return get_size(str.begin(), str.end(), max_size);
}

CL_Rect CL_Font::bounding_rect(
	CL_Rect dest,
	std::string::const_iterator start,
	std::string::const_iterator end) const
{
	CL_GlyphBuffer gb;
	draw_to_gb(start, end, gb, dest.get_size());
	gb.set_alignment(impl->trans_origin, impl->trans_x, impl->trans_y);
	return gb.bounding_rect(dest);
}

CL_Rect CL_Font::bounding_rect(int x, int y, const std::string &str) const
{
	return bounding_rect(CL_Rect(x, y, x, y), str.begin(), str.end());
}

CL_Rect CL_Font::bounding_rect(CL_Rect dest, const std::string &str) const
{
	return bounding_rect(dest, str.begin(), str.end());
}

CL_Rect CL_Font::bounding_rect(
	int x, int y,
	std::string::const_iterator start,
	std::string::const_iterator end) const
{
	return bounding_rect(CL_Rect(x, y, x, y), start, end);
}

bool CL_Font::is_glyph(unsigned int chr) const
{
	return impl->target->is_glyph(chr);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font operations:

CL_Font &CL_Font::operator =(const CL_Font &copy)
{
	impl = copy.impl;
	
	resource.unload();
	resource = copy.resource;
	resource.load();

	return *this;
}

CL_Font::operator bool() const
{
	return (impl.get() != 0);
}

int CL_Font::draw(
	CL_Rect dest,
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_GraphicContext *context) const
{
	if (context == 0)
		context = CL_Display::get_current_window()->get_gc();
	
	CL_GlyphBuffer gb;
	int dist = draw_to_gb(start, end, gb, dest.get_size());
	gb.set_alignment(impl->trans_origin, impl->trans_x, impl->trans_y);
	
	if (impl->trans_origin == origin_top_left)
		gb.fixed_draw(dest.left - impl->trans_x, dest.top - impl->trans_y, context);
	else
		gb.draw(dest, context);
	
	return dist;
}

int CL_Font::draw(
	int x, int y,
	const std::string &str,
	CL_GraphicContext *context) const
{
	return draw(CL_Rect(x, y, x, y), str.begin(), str.end(), context);
}

int CL_Font::draw(
	CL_Rect dest,
	const std::string &str,
	CL_GraphicContext *context) const
{
	return draw(dest, str.begin(), str.end(), context);
}

int CL_Font::draw(
	int x, int y,
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_GraphicContext *context) const
{
	return draw(CL_Rect(x, y, x, y), start, end, context);
}

//Given a CL_GlyphBuffer, ensures that a given line is flush with the line above it
//Returns the offset that was applied
int squish_line(CL_GlyphBuffer& gb, CL_GlyphBuffer::Range line)
{
	std::vector<CL_GlyphBuffer::Glyph>& glyphs = gb.get_glyphs();

	if (line.start <= line.end)
		return 0;
	
	if (glyphs.size() != 0 && line.start != 0)
	{
		int dist; //The offset we'll apply to y ord of each glyph in the line
		
		int curline_top = gb.internal_rect(line).top;
		if (curline_top == 0)
			return 0;
		
		if (line.start != 0)
		{
			int prevline_bottom = gb.internal_rect(gb.get_line(line.start-1)).bottom;
			
			if (prevline_bottom == 0)
				return 0;
			else
				dist = prevline_bottom - curline_top; 
		}
		else
		{
			dist = -curline_top; //Move to 0
		}
		
		for (int x = line.start; x != line.end; ++x)
			glyphs[x].pos.y += dist;

		return dist;
	}
	
	return 0;
}

int CL_Font::draw_to_gb(
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_GlyphBuffer &gb,
	CL_Size max_size) const
{
	std::vector<CL_GlyphBuffer::Glyph>& glyphs = gb.get_glyphs(); //Shortcut to the glyphs vector in the gb
	const int font_height = impl->target->get_height(); //Distance to move at carriage feed
	const int orig_size = glyphs.size(); //Size of the glyphs vector before we start messing with it
	
	//Barf if things look evil
	if (
		start >= end ||
		font_height <= 0 ||
		max_size.height < 0 ||
		max_size.width < 0)
	{
		return 0;
	}
	
	float scalx = 1.0f;
	float scaly = 1.0f;
	get_scale(scalx,scaly);

	CL_Color col = get_color();
	
	CL_Point pos(0, 0); //Drawing position, at the top of the current line
	int line_height = font_height;//Height of the current line
	int line_start = 0; //Index of the character beginning the current line
	bool squishing = false; //True if we need to do squishing (if there's fonts of differing height on this line)
	
	//If we aren't starting at the beginning of a line, then we need to compensate
	//Either the line is taller than this font, which means any new glyphs we add on this line need to be pushed down
	//Or this font is taller than previous glyphs on the line, which means we need to move down the previous glyphs
	if (glyphs.size() != 0)
	{
		const CL_GlyphBuffer::Range eline(gb.get_line(glyphs.size()-1)); //Glyphs in the final existing GB line
		const CL_Rect eline_rect(gb.internal_rect(eline, false)); //Rectangle of eline, incl. invis glyphs
		
		//Place pos directly after the last glyph in the buffer
		pos.x = eline_rect.right;
		pos.y = eline_rect.top;
		
		//If glyphs already on this line are taller than this font, then adjust height for this line
		if (eline_rect.get_height() > font_height)
			line_height = eline_rect.get_height();
		
		//If the last character of that line is a newline, do a carriage return
		//If it isn't, and our font is taller than previous glyphs on the line, then move those glyphs down to match
		if (glyphs[eline.end-1].character == '\n') {
			//Carriage return
			pos.x = 0;
			pos.y += line_height;
		}
		else if (eline_rect.get_height() < font_height)
		{
			//If we have enough room, move the existing glyphs down to meet our baseline
			if (max_size.height == 0 || pos.y + font_height <= max_size.height)
				for (int x = eline.start; x < eline.end; ++x)
					glyphs[x].pos.y += font_height - eline_rect.get_height();
		}
		
		line_start = gb.get_line(glyphs.size()-1).start;

		if (line_height != font_height)
			squishing = true;
	}
	
	//If inserting the first glyph would go over the height limit, end the draw now
	if (max_size.height != 0 && pos.y + font_height > max_size.height)
	{
		if (squishing)
			squish_line(gb, CL_GlyphBuffer::Range(line_start, glyphs.size()));
		
		return 0;
	}
	
	//Put a marker for this font into the GB, and reserve a bit of space in the GB's glyph vector
	gb.change_font(*this);
	glyphs.reserve(cl_min(end - start, 1000));
	
	//For every character in the input range (or as far through that range as we can get)
	for (std::string::const_iterator it = start; it != end; ++it)
	{
		const unsigned int chr = ((unsigned char) *it); // The character this glyph represents
		const int gwidth = get_width(chr); //Width of the glyph
		
		//If a character is wider than max width, stop the presses
		if (max_size.width != 0 && gwidth > max_size.width)
		{
			if (squishing)
				squish_line(gb, CL_GlyphBuffer::Range(line_start, glyphs.size()));
			
			return glyphs.size() - orig_size;
		}
		
		//If this character extends over the wrapping border, and word wrapping is on, and it has a glyph...
		if (max_size.width != 0 && pos.x + gwidth > max_size.width && is_glyph(chr))
		{
			//Carriage return
			pos.x = 0;
			pos.y += line_height;
			line_height = font_height;
			
			//Try to find a delimiter on the last line
			int delim_idx = -1;
			for (int glyph_idx = glyphs.size()-1; glyph_idx >= line_start; --glyph_idx)
			{
				if (
					impl->delims.find(glyphs[glyph_idx].character) != std::string::npos &&
					(!is_glyph(glyphs[glyph_idx].character) || gb.internal_rect(glyph_idx).right <= max_size.width)
				)
				{
					delim_idx = glyph_idx;
					break;
				}
			}
			
			//If this line has a delimiter...
			if (delim_idx != -1)
			{
				//Height of glyphs after the delimiter
				int wrapline_height = gb.get_height(CL_GlyphBuffer::Range(delim_idx+1, glyphs.size()));
				
				//Height of the line we're wrapping the text onto, which becomes the new last line
				line_height = cl_max(wrapline_height, font_height);
				
				//If we don't have enough vertical room to wrap glyphs here, end the draw
				if (max_size.height != 0 && pos.y + line_height > max_size.height)
				{
					//Clip out that final partial word
					gb.remove(CL_GlyphBuffer::Range(delim_idx + 1, glyphs.size()));
					
					if (squishing)
						squish_line(gb, CL_GlyphBuffer::Range(line_start, glyphs.size()));
								
					return glyphs.size() - orig_size;
				}
				else
				{
					//Move glyphs after the delimiter onto the next line, move pos right after them
					std::vector<CL_GlyphBuffer::Glyph>::size_type x;
					for (x = delim_idx + 1; x != glyphs.size(); ++x)
					{
						glyphs[x].pos = CL_Point(pos.x, glyphs[x].pos.y + line_height);
						pos.x += gb.get_width(static_cast<int>(x));
					}
					
					//Remove excess vertical space from the previous-to-last-line
					//Move the cursor and the line segment we just wrapped up to compensate
					if (squishing)
					{
						int squishdist = squish_line(gb, CL_GlyphBuffer::Range(line_start, delim_idx+1));
						pos.y += squishdist;
						for (x = delim_idx + 1; x != glyphs.size(); ++x)
							glyphs[x].pos.y += squishdist;
					}
					
					line_start = delim_idx + 1;
					squishing = false;
				}
			}
			//This line doesn't have a delimiter, so we have to break in the middle of a word
			else
			{
				//Remove any excess vertical space on the last line, move cursor to compensate
				if (squishing)
					pos.y += squish_line(gb, CL_GlyphBuffer::Range(line_start, glyphs.size()));
				
				line_start = glyphs.size();
				squishing = false;
				
				//If we don't have enough vertical room to put any glyphs here, end the draw
				if (max_size.height != 0 && pos.y + line_height > max_size.height)
					return glyphs.size() - orig_size;
			}
		}
		
		//Insert the glyph
		glyphs.push_back(CL_GlyphBuffer::Glyph(
			CL_Point(pos.x, pos.y + (line_height - font_height)), 
			chr,scalx,scaly,col));
		
		//Advance the cursor
		pos.x += gwidth;
		
		//If this character is a newline
		if (chr == '\n')
		{
			//Carriage return
			pos.x = 0;
			pos.y += line_height;
			line_height = font_height;
			
			//Remove excess vertical space, adjust cursor to compensate
			if (squishing)
				pos.y += squish_line(gb, CL_GlyphBuffer::Range(line_start, glyphs.size()));
			
			line_start = glyphs.size();
			squishing = false;
			
			//If we've exceeded height, then end the draw
			if (max_size.height != 0 && pos.y + line_height > max_size.height)
				return glyphs.size() - orig_size;
		}
	}
	
	//We made it through the entire input string, huzzah
	if (squishing)
		squish_line(gb, CL_GlyphBuffer::Range(line_start, glyphs.size()));
	
	return glyphs.size() - orig_size;
}

int CL_Font::draw_to_gb(
	const std::string &str,
	CL_GlyphBuffer &gb,
	CL_Size max_size) const
{
	return draw_to_gb(str.begin(), str.end(), gb, max_size);
}

void CL_Font::draw_glyph(int x, int y, unsigned int glyph, float ang, CL_GraphicContext *gc)
{
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();
	impl->target->draw_glyph(x, y, glyph, ang, gc);
}

void CL_Font::draw_character(int x, int y, unsigned int chr, float ang, CL_GraphicContext *gc)
{
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();
	impl->target->draw_character(x, y, chr, ang, gc);
}

void CL_Font::set_delims(const std::string& delims)
{
	impl->delims = delims;
}

void CL_Font::set_width_offset(int offset)
{
	impl->width_offset = offset;
}

void CL_Font::set_height_offset(int offset)
{
	impl->height_offset = offset;
}

void CL_Font::set_scale(float x, float y)
{
	impl->target->set_scale(x, y);
}

void CL_Font::set_alpha(float alpha)
{
	impl->target->set_alpha(alpha);
}

void CL_Font::set_color(float r, float g, float b, float a)
{
	impl->target->set_color(r, g, b, a);
}

void CL_Font::set_color(const CL_Color& c)
{
	set_color(float(c.get_red())/255.0f, float(c.get_green())/255.0f, float(c.get_blue())/255.0f, float(c.get_alpha())/255.0f);
}

void CL_Font::set_blend_func(CL_BlendFunc src, CL_BlendFunc dest)
{
	impl->target->set_blend_func(src, dest);
}

void CL_Font::set_glyph_rot_hotspot(CL_Origin origin, int x, int y)
{
	impl->target->set_rot_hotspot(origin, x, y);
}

void CL_Font::set_alignment(CL_Origin origin, int x, int y)
{
	impl->trans_origin = origin;
	impl->trans_x = x;
	impl->trans_y = y;
}

CL_Sprite& CL_Font::get_glyphs()
{
	CL_Font_Target* target = impl->target;
	
	CL_Font_Target_Sprite* sprite_target =
		static_cast<CL_Font_Target_Sprite*>(target);
	
	return sprite_target->get_glyphs();	
}

