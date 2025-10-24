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
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/System/error.h"
#include "API/Display/glyph_buffer.h"
#include "API/Display/graphic_context.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"

#include <cmath>
#include <map>
#include <climits>

#define cl_min(a,b) ((a < b) ? a : b)
#define cl_max(a,b) ((a > b) ? a : b)

CL_GlyphBuffer::CL_GlyphBuffer() :
	scale_x(1.0f), scale_y(1.0f),
	rot_angle(0.0f), rot_origin(origin_center),
	rot_x(0), rot_y(0), trans_origin(origin_top_left),
	trans_x(0), trans_y(0)
	{}

void CL_GlyphBuffer::Range::verify(const CL_GlyphBuffer& gb)
{
	if (start < 0)
		start = 0;
	
	if (end > static_cast<int>(gb.get_glyphs().size()))
		end = gb.get_glyphs().size();

	if (end < 0)
		end = 0;
	
	if (start > end)
		start = end;
}

void CL_GlyphBuffer::Range::remove_invis_left(const CL_GlyphBuffer& gb)
{
	verify(gb);

	while (
	start < end &&
	gb.get_font_for(start)->second.is_glyph(gb.get_glyphs()[start].character) == 0)
		++start;
}

void CL_GlyphBuffer::Range::remove_invis_right(const CL_GlyphBuffer& gb)
{
	verify(gb);

	while (
	end > start &&
	gb.get_font_for(end-1)->second.is_glyph(gb.get_glyphs()[end-1].character) == 0)
		--end;
}

std::map<int, CL_Font>::iterator CL_GlyphBuffer::get_font_for(int glyph_index)
{
	if (font_markers.size() == 0)
		return font_markers.end();
	
	if (glyph_index < 0)
		glyph_index = 0;
	
	if (glyph_index > static_cast<int>(glyphs.size() - 1))
		glyph_index = glyphs.size() - 1;
	
	//Find a font_marker associated with an index above the given one, or the end
	std::map<int, CL_Font>::iterator it = font_markers.begin();
	for (; it != font_markers.end(); ++it)
		if (it->first > glyph_index)
			break;
	
	if (it == font_markers.begin())
		return font_markers.end();
	
	return --it;
}

std::map<int, CL_Font>::const_iterator CL_GlyphBuffer::get_font_for(int glyph_index) const
{
	if (glyph_index < 0)
		glyph_index = 0;
	
	if (glyph_index > static_cast<int>(glyphs.size() - 1))
		glyph_index = glyphs.size() - 1;
	
	//Upper bound finds the first element whose key is greater than the given key
	//So after decrementing it, then it will be the font marker which covers the given glyph
	//Remember that font markers affect glyphs with indices >= their key number, up to the next marker's key
	std::map<int, CL_Font>::const_iterator it = font_markers.upper_bound(glyph_index);
	if (it == font_markers.begin())
		return font_markers.end();
	return --it;
}

CL_Rect CL_GlyphBuffer::bounding_rect(Range range, CL_Rect dest, bool ignore_invis) const
{
	CL_Rect retVal = internal_rect(range, ignore_invis);
	CL_Point offset = i2e_offset(retVal, dest);
	
	retVal.left += offset.x;
	retVal.top += offset.y;
	retVal.right += offset.x;
	retVal.bottom += offset.y;
	
	return retVal;
}

CL_Rect CL_GlyphBuffer::internal_rect(Range range, bool ignore_invis) const
{
	range.verify(*this);
	
	//Use a weird inverse rectangle to start out with
	//Since we get the bounding rect by just comparing each side of each glyph with respective sides of the rect-in-progress...
	//Using this rect means the first glyph we compare against determines the initial rectangle
	CL_Rect retVal(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
	
	//For every glyph in the buffer...
	for (Iter it(*this, range); !it.at_end(range); ++it)
	{
		std::vector<Glyph>::const_iterator glyph = glyphs.begin() + it.get_glyph_num(); //Glyph vector element
		const CL_Font& fnt = it.get_font();
		
		//If ignore_invis is turned on, check to see if this glyph is supplied by the font
		if (!ignore_invis || (ignore_invis && fnt.is_glyph(glyph->character)))
		{
			float scale_eff_x, scale_eff_y;
			it.get_scale_eff(scale_eff_x, scale_eff_y);
			
			//Rectangle containing the glyph
			CL_Rect gr(
				glyph->pos.x,
				glyph->pos.y,
				glyph->pos.x + (int)(fnt.get_width(glyph->character) * scale_eff_x),
				glyph->pos.y + (int)(fnt.get_height() * scale_eff_y));
			
			//Compensate for rotation of the glyph and font rotation
			float angle_eff = it.get_angle_eff();
			if (angle_eff != 0.0)
			{
				CL_Origin grot_hotspot; int grot_x, grot_y;
				fnt.get_glyph_rot_hotspot(grot_hotspot, grot_x, grot_y);
				gr = gr.get_rot_bounds(grot_hotspot, grot_x, grot_y, angle_eff);
			}
			
			//Push on the sides of the returned rect so it contains this glyph
			retVal.left = cl_min(gr.left, retVal.left);
			retVal.right = cl_max(gr.right, retVal.right);
			retVal.top = cl_min(gr.top, retVal.top);
			retVal.bottom = cl_max(gr.bottom, retVal.bottom);
		}
	}
	
	//If there weren't any real glyphs, then return a rectangle with zero size
	if (retVal == CL_Rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN))
		retVal = CL_Rect(0,0,0,0);
	
	return retVal;
}

std::vector<CL_GlyphBuffer::Range> CL_GlyphBuffer::get_lines(CL_GlyphBuffer::Range range) const
{
	range.verify(*this);
	std::vector<CL_GlyphBuffer::Range> ret;
	
	std::map<int, CL_Font>::const_iterator curfnt;
	if (range.start > 0)
		curfnt = get_font_for(range.start-1);
	else
		curfnt = font_markers.begin();
	
	//Move the beginning of the input range to the beginning of the line
	while (range.start > 0)
	{
		//If curfnt isnt the font of range.start-1, make it that font
		if (curfnt->first > range.start-1)
			--curfnt;
		
		if (is_linebreak(range.start-1, curfnt->second))
			break;
		
		--range.start;
	}
	
	//Advance until we've passed the end of the range and hit the linebreak at or after that
	std::map<int, CL_Font>::const_iterator nextfnt = curfnt;
	++nextfnt;
	for (int idx = range.start;; ++idx)
	{
		//If curfnt isnt the font of idx, make it that font
		if (nextfnt != font_markers.end() && idx >= nextfnt->first)
		{
			curfnt = nextfnt;
			++nextfnt;
		}
		
		//If this glyph is the last glyph on a line...
		if (is_linebreak(idx, curfnt->second))
		{
			//Add a line range to the return value
			if (ret.size() == 0)
				ret.push_back(CL_GlyphBuffer::Range(range.start, idx+1));
			else
				ret.push_back(CL_GlyphBuffer::Range((ret.end()-1)->end, idx+1));
			
			if (idx >= range.end)
				break;
		}
	}

	return ret;
}

CL_GlyphBuffer::Range CL_GlyphBuffer::get_line(int idx) const
{
	CL_GlyphBuffer::Range ret(idx, idx);
	
	std::map<int, CL_Font>::const_iterator idxfnt;
	if (idx > 0)
		idxfnt = get_font_for(idx-1);
	else
		idxfnt = font_markers.begin();

	std::map<int, CL_Font>::const_iterator curfnt = idxfnt;
	
	//Move the range start back to the beginning of the line
	while (ret.start > 0)
	{
		//If curfnt isnt the font of range.start-1, make it that font
		if (curfnt->first > ret.start-1)
			--curfnt;
		
		if (is_linebreak(ret.start-1, curfnt->second))
			break;
		
		--ret.start;
	}
	
	//Move the range end to the end of the line
	curfnt = idxfnt;
	std::map<int, CL_Font>::const_iterator nextfnt = curfnt;
	++nextfnt;
	while (1)
	{
		//If curfnt isnt the font of idx, make it that font
		if (nextfnt != font_markers.end() && idx >= nextfnt->first)
		{
			curfnt = nextfnt;
			++nextfnt;
		}
		
		//If ret.end is the last glyph on the line, then we're done
		if (is_linebreak(ret.end, curfnt->second)) {
			++ret.end;
			break;
		}
		
		++ret.end;
	}
	
	return ret;
}

//Utility function for add(), puts a map into another map with a key offset
template <typename T>
void add_map_range(
	const std::map<int, T>& src,
	std::map<int, T>& dest,
	CL_GlyphBuffer::Range range,
	int offset)
{
	const typename std::map<int, T>::const_iterator end = src.upper_bound(range.end);
	for (typename std::map<int, T>::const_iterator it = src.lower_bound(range.start); it != end; ++it)
		dest[it->first + offset] = it->second;
}

CL_GlyphBuffer &CL_GlyphBuffer::add(
	const CL_GlyphBuffer &other,
	Range range,
	int x,
	int y)
{
	//Offset to make copied font markers/effects apply to after the end of the new buffer
	const int offset = glyphs.size();

	glyphs.reserve(glyphs.size() + other.glyphs.size());
	
	if ((x == 0) && (y == 0))
	{
		for (
			std::vector<Glyph>::const_iterator it = other.glyphs.begin() + range.start;
			it != other.glyphs.begin() + range.end;
			++it)
		{
			glyphs.push_back(*it);
		}
	}
	else
	{
		for (
			std::vector<Glyph>::const_iterator it = other.glyphs.begin() + range.start;
			it != other.glyphs.begin() + range.end;
			++it)
		{
			glyphs.push_back(Glyph(CL_Point(it->pos.x + x, it->pos.y + y), it->character,it->glyphscale_x,it->glyphscale_y));
		}
	}
	
	add_map_range(other.font_markers, font_markers, range, offset);
	add_map_range(other.color_effects, color_effects, range, offset);
	add_map_range(other.scale_x_effects, scale_x_effects, range, offset);
	add_map_range(other.scale_y_effects, scale_y_effects, range, offset);
	add_map_range(other.angle_effects, angle_effects, range, offset);
	
	return *this;
}

CL_GlyphBuffer &CL_GlyphBuffer::remove(Range range)
{
	range.verify(*this);
	
	//Remove the elements themselves
	glyphs.erase(glyphs.begin() + range.start, glyphs.begin() + range.end);
	
	//Remove any map entries associated with the range, slide back any map entries after the range
	cut_range(font_markers, range.start, range.end);
	cut_range(color_effects, range.start, range.end);
	cut_range(scale_x_effects, range.start, range.end);
	cut_range(scale_y_effects, range.start, range.end);
	cut_range(angle_effects, range.start, range.end);

	return *this;
}

CL_GlyphBuffer &CL_GlyphBuffer::change_font(const CL_Font &font)
{
	font_markers[glyphs.size()] = font;
	return *this;
}

void CL_GlyphBuffer::clear()
{
	glyphs.clear();
	font_markers.clear();
	angle_effects.clear();
	scale_x_effects.clear();
	scale_y_effects.clear();
	color_effects.clear();
}

void CL_GlyphBuffer::justify_left(CL_GlyphBuffer::Range range, int left)
{
	range.verify(*this);
	std::vector<CL_GlyphBuffer::Range> ranges = get_lines(range);
	
	for (std::vector<CL_GlyphBuffer::Range>::iterator it = ranges.begin(); it != ranges.end(); ++it)
	{
		CL_GlyphBuffer::Range vis_range = *it;
		CL_Rect line_rect = internal_rect(vis_range);
		int offset = left - line_rect.left;
		
		for (int glyph = it->start; glyph != it->end; ++glyph)
			glyphs[glyph].pos.x += offset;
	}
}

void CL_GlyphBuffer::justify_center(CL_GlyphBuffer::Range range, int center)
{
	range.verify(*this);
	std::vector<CL_GlyphBuffer::Range> ranges = get_lines(range);
	
	for (std::vector<CL_GlyphBuffer::Range>::iterator it = ranges.begin(); it != ranges.end(); ++it)
	{
		CL_GlyphBuffer::Range vis_range = *it;
		vis_range.remove_invis_both(*this);
		CL_Rect line_rect = internal_rect(vis_range);
		int offset = center - ((line_rect.left + line_rect.right)/2);
		
		for (int glyph = it->start; glyph != it->end; ++glyph)
			glyphs[glyph].pos.x += offset;
	}
}

void CL_GlyphBuffer::justify_right(CL_GlyphBuffer::Range range, int right)
{
	range.verify(*this);
	std::vector<CL_GlyphBuffer::Range> ranges = get_lines(range);
	
	for (std::vector<CL_GlyphBuffer::Range>::iterator it = ranges.begin(); it != ranges.end(); ++it)
	{
		CL_GlyphBuffer::Range vis_range = *it;
		vis_range.remove_invis_right(*this);
		CL_Rect line_rect = internal_rect(vis_range);
		int offset = right - line_rect.right;
		
		for (int glyph = it->start; glyph != it->end; ++glyph)
			glyphs[glyph].pos.x += offset;
	}
}

void CL_GlyphBuffer::set_angle(float angle)
{
	while(angle >= 360.0f)
		angle -= 360.0f;
	while(angle < 0.0f)
		angle += 360.0f;
	
	rot_angle = angle;
}

void CL_GlyphBuffer::Iter::get_scale_eff(float& x, float& y) const
{
	const std::map<int, float>& scale_x_effects = pgb->get_scale_x_effects();
	std::map<int, float>::const_iterator x_eff = scale_x_effects.find(glyph_num);
	if (x_eff != scale_x_effects.end())
		x = x_eff->second;
	else
		x = 1.0;
	
	const std::map<int, float>& scale_y_effects = pgb->get_scale_y_effects();
	std::map<int, float>::const_iterator y_eff = scale_y_effects.find(glyph_num);
	if (y_eff != scale_y_effects.end())
		y = y_eff->second;
	else
		y = 1.0;
}

float CL_GlyphBuffer::Iter::get_angle_eff() const
{
	const std::map<int, float>& angle_effects = pgb->get_angle_effects();
	std::map<int, float>::const_iterator ang_eff = angle_effects.find(glyph_num);
	if (ang_eff != angle_effects.end())
		return ang_eff->second;
	else
		return 0.0;
}

CL_Color CL_GlyphBuffer::Iter::get_color_eff(const CL_Font& fnt) const
{
	const std::map<int, CL_Color>& color_effects = pgb->get_color_effects();
	std::map<int, CL_Color>::const_iterator col_eff = color_effects.find(glyph_num);
	if (col_eff != color_effects.end()) {
		return col_eff->second; } 
	const Glyph &glph = pgb->glyphs[glyph_num];
	if (glph.fontcolor == CL_Color(0.0f,0.0f,0.0f,0.0f)) { return fnt.get_color(); }
	return glph.fontcolor;
}

const CL_Font& CL_GlyphBuffer::Iter::get_font() const
{
	std::map<int, CL_Font>::const_iterator it = pgb->get_font_for(glyph_num);
	if (it == pgb->get_font_markers().end())
		throw CL_Error("No font in the font markers map to draw the glyph(s) with");
	return it->second;
}

bool CL_GlyphBuffer::is_linebreak(int idx, const CL_Font& fnt) const
{
	const unsigned int chr = glyphs[idx].character;
	
	//This glyph and the glyph after it are on different lines if:
	if (
	//...this is the last glyph of the buffer, which makes it the last glyph of the last line
	idx == static_cast<int>(glyphs.size()-1) ||
	//...this glyph is a manual newline character ('\n')
	chr == '\n' || 
	//...the next glyph's left side is to the left of this glyph's right side
	glyphs[idx+1].pos.x < glyphs[idx].pos.x + fnt.get_width(chr)
	)
		return true;

	return false;
}

CL_Point CL_GlyphBuffer::i2e_offset(CL_Rect src, CL_Rect dest) const
{
	//Align the source rectangle to the origin
	CL_Point retVal(-src.left, -src.top);
	retVal -= calc_origin(trans_origin, CL_Size(src.get_width(), src.get_height()));
	
	//Align the origin to the dest rectangle
	retVal.x += dest.left;
	retVal.y += dest.top;
	retVal += calc_origin(trans_origin, CL_Size(dest.get_width(), dest.get_height()));
	
	//Apply the translation offsets
	retVal.x -= trans_x;
	retVal.y -= trans_y;
	
	return retVal;
}

CL_Point CL_GlyphBuffer::get_scale_hotspot(CL_Rect irect) const
{
	CL_Point scale_hotspot(irect.left, irect.top);
	scale_hotspot += calc_origin(trans_origin, irect.get_size());
	scale_hotspot.x += trans_x;
	scale_hotspot.y += trans_y;
	
	return scale_hotspot;
}

CL_Point CL_GlyphBuffer::get_rot_hotspot(CL_Rect irect) const
{
	CL_Point rot_hotspot(irect.left, irect.top);
	rot_hotspot += calc_origin(rot_origin, irect.get_size());
	rot_hotspot.x += rot_x;
	rot_hotspot.y += rot_y;
	
	return rot_hotspot;
}

void CL_GlyphBuffer::draw_glyphs(
	Range range,
	CL_Point offset,
	CL_Rect irect,
	CL_GraphicContext* gc) const
{
	range.verify(*this);
	
	if (gc == 0)
		gc = CL_Display::get_current_window()->get_gc();
	
	CL_Point rot_hotspot;
	CL_Point scale_hotspot(0,0);
	
	//True if we're doing total GlyphBuffer rotation
	bool rotate = false;
	if (rot_angle != 0.0)
	{
		rotate = true;
		if (irect == CL_Rect(0,0,0,0))
			irect = internal_rect();
		rot_hotspot = get_rot_hotspot(irect);
	}
	
	//True if we're doing total GlyphBuffer scaling
	bool scale = false;
	if (scale_x != 1.0 || scale_y != 1.0)
	{
		scale = true;
		if (irect == CL_Rect(0,0,0,0))
			irect = internal_rect();
		scale_hotspot = get_scale_hotspot(irect);
	}
	
	//For every glyph in the buffer...
	for (Iter it(*this, range); !it.at_end(range); ++it)
	{
		std::vector<Glyph>::const_iterator glyph = glyphs.begin() + it.get_glyph_num(); //Reference to the glyph vector element
		CL_Point draw_pos(glyph->pos);
		
		//Apply GB scaling if enabled
		if (scale)
		{
			//The offset from scale_hotspot to the new drawing position
			CL_Point scale_offset = draw_pos - scale_hotspot;
			scale_offset.x = (int)(scale_offset.x*scale_x);
			scale_offset.y = (int)(scale_offset.y*scale_y);
			
			draw_pos = scale_hotspot + scale_offset;
		}
		
		//Apply GB rotation if enabled
		if (rotate)
			draw_pos = draw_pos.rotate(rot_hotspot, rot_angle);
		
		//Move the glyph into the dest rectangle
		draw_pos += offset;
		
		//We reimplement get_font_for() here so that we can get a mutable iterator
		//That's because there's no such thing as a mutable method 
		std::map<int, CL_Font>::iterator fntiter = font_markers.upper_bound(it.get_glyph_num());
		if (fntiter == font_markers.begin())
			throw CL_Error("No font in the font markers map to draw the glyph(s) with");
		CL_Font& font = (--fntiter)->second;
		
		if (font.is_glyph(glyph->character))
		{
			//Record the font's original color and scale
			CL_Color orig_color = font.get_color();
			float orig_scale_x, orig_scale_y;
			font.get_scale(orig_scale_x, orig_scale_y);
			
			//Temporarily set the Font's color and scale to the current glyph's properties
			CL_Color eff_color = it.get_color_eff(font);
			float scale_eff_x, scale_eff_y;
			it.get_scale_eff(scale_eff_x, scale_eff_y);

			scale_eff_x *= glyph->glyphscale_x;
			scale_eff_y *= glyph->glyphscale_y;
			
			//If there's any scale effect or scale property, apply it
			if (eff_color != orig_color)
				font.set_color(eff_color);
			if (scale_x != 1.0 || scale_eff_x != orig_scale_x || scale_y != 1.0 || scale_eff_y != orig_scale_y)
				font.set_scale(scale_eff_x * scale_x, scale_eff_y * scale_y);
			
			//Draw the glyph, using the current angle
			font.draw_character(draw_pos.x, draw_pos.y, glyph->character, it.get_angle_eff() + rot_angle, gc);
			
			//Restore the original values, if they were changed
			if (eff_color != orig_color)
				font.set_color(orig_color);
			if (scale_x != 1.0 || scale_eff_x != orig_scale_x || scale_y != 1.0 || scale_eff_y != orig_scale_y)
				font.set_scale(orig_scale_x, orig_scale_y);
		}
	}
}
