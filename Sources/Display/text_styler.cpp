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
#include "API/Display/text_styler.h"
#include "API/Display/graphic_context.h"
#include "API/Display/glyph_buffer.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"

#include "resourcedata_text_styler.h"

#include <algorithm>
#include <list>
#include <map>
#include <stack>
#include <vector>

void scale_restore(std::vector<std::pair<CL_Font,std::pair<float,float> > > &scalesaver) {
	while (!scalesaver.empty()) {
		scalesaver.back().first.set_scale(scalesaver.back().second.first,scalesaver.back().second.second);
		scalesaver.pop_back();
	}
}

//Used in draw_to_gb
//Returns the font for a given state of font tags, scale tags, and color tags
CL_Font get_font(
	const CL_TextStyler& ts,
	const std::list<std::string>& fnttags,
	const std::list<std::pair<float, float> >& scaletags,
	const std::stack<CL_Color>& colorstack,
	std::vector<std::pair<CL_Font,std::pair<float,float> > > &scalesaver)
{
	scale_restore(scalesaver);

	std::map<std::string, CL_Font>::const_iterator fntit = ts.get_fonts().end();
	
	//Find the top-most font tag that corresponds with an existing font in the TextStyler
	for (std::list<std::string>::const_iterator it = fnttags.begin(); it != fnttags.end(); ++it) {
		if ((fntit = ts.get_fonts().find(*it)) != ts.get_fonts().end()) break;
	}
	
	//Can't find a font tag that corresponds, find the default font
	if (fntit == ts.get_fonts().end())
		fntit = ts.get_fonts().find("default");
	
	//If there's no default font, and no matching font tags, barf
	if (fntit == ts.get_fonts().end())
		throw CL_Error("CL_TextStyler error: No default font and no matching tagged fonts");
	
	CL_Font fnt = fntit->second;
	
	if (!colorstack.empty())
		fnt.set_color(colorstack.top());
	
	float scale_x, scale_y;
	ts.get_scale(scale_x, scale_y);
	for(std::list<std::pair<float, float> >::const_iterator it2 = scaletags.begin(); it2 != scaletags.end(); ++it2)
	{
		scale_x *= it2->first;
		scale_y *= it2->second;
	}
	
	float fntx, fnty;
	fnt.get_scale(fntx, fnty);
	scalesaver.push_back(std::make_pair(fnt,std::make_pair(fntx,fnty)));

	if (scale_x != 1.0 || scale_y != 1.0)
	{
		fnt.set_scale(scale_x * fntx, scale_y * fnty);
	}
	
	return fnt;
}

//Adds/removes from a font tag stack based on a given tag
void apply_font_tag(
	std::list<std::string>& fnttags, 
	std::string::const_iterator start,
	std::string::const_iterator end)
{
	//Exclude the marker characters and only deal with the contents of the tag
	++start;
	--end;
	
	//Check if its a tag to open or close a block
	if (*start != '/')
	{
		fnttags.push_front(std::string(start, end));
	}
	else
	{
		//Exclude the '/'
		++start;
		
		//Check each entry in the font tag stack until we find a match with this tag's contents to remove
		for (std::list<std::string>::iterator it = fnttags.begin(); it != fnttags.end(); ++it)
		{
			if (end - start == static_cast<int>(it->size()) && std::equal(start, end, it->begin())) {
				fnttags.erase(it);
				break;
			}
		}
	}
}

//Adds/removes from the justification stack based on a given tag
void apply_justif_tag(
	std::stack<unsigned char>& justifstack,
	std::map<int, unsigned char>& justifmarkers,
	int glyph_cnt,
	std::string::const_iterator start,
	std::string::const_iterator end)
{
	//Exclude the tag opener
	++start;
	
	//Apply this tag to the stack
	//Scope closing tag
	if (*start == '/' && !justifstack.empty())
	{
		justifstack.pop();
	}
	else
	{
		//Move past the "j " at the beginning of the tag and add the justif to the stack
		justifstack.push(*(start+2));
	}
	
	//Record the new entry in the justif markers map
	if (justifstack.empty())
		justifmarkers[glyph_cnt] = 'l';
	else
		justifmarkers[glyph_cnt] = justifstack.top();
}

//Adds/removes from a scale stack based on a given tag
void apply_scale_tag(
	std::list<std::pair<float, float> >& scaletags,
	std::string::const_iterator start,
	std::string::const_iterator end)
{
	//Exclude the marker characters and only deal with the contents of the tag
	++start;
	--end;
	
	//Check if its a tag to open or close a block
	if (*start != '/')
	{
		//Move past the "s " at the beginning of the tag's inside
		start += 2;
		
		//If there's a space in there, then x and y specified seperately
		std::string::const_iterator spc_pos = std::find(start, end, ' ');
		if (spc_pos == end)
		{
			//No space, so just convert and insert the number as both x and y
			float val = CL_String::to_float(std::string(start, end));
			scaletags.push_front(std::pair<float, float>(val, val));
		}
		else
		{
			//Insert the two numbers as x and y respectively
			scaletags.push_front(std::pair<float, float>(
				CL_String::to_float(std::string(start, spc_pos)),
				CL_String::to_float(std::string(spc_pos+1, end))
			));
		}
	}
	else if (!scaletags.empty())
	{
		scaletags.pop_front();
	}
}

//Adds removes from a color stack based on a given tag
void apply_color_tag(
	std::stack<CL_Color>& colorstack,
	std::string::const_iterator start,
	std::string::const_iterator end)
{
	//Exclude the marker characters and only deal with the contents of the tag
	++start;
	--end;
	
	//Check if its a tag to open or close a block
	if (*start != '/')
	{	
		//Move past the "c " at the beginning of the tag contents
		start += 2;
		
		//Depending on how many spaces there are, we can tell if this is a named color, 3 part, or 4 part (w/ alpha)
		switch (std::count(start, end, ' ')) {
			case 0:
				{
					colorstack.push(CL_Color::find_color(std::string(start, end)));
					break;
				}
			case 2:
				{
					//RGB specified, grab them and set alpha to default
					CL_Color col;
					
					std::string::const_iterator colstart = start;
					std::string::const_iterator colend = std::find(colstart, end, ' ');
					col.set_red(CL_String::to_int(std::string(colstart, colend)));
					colstart = colend+1;
					colend = std::find(colstart, end, ' ');
					col.set_green(CL_String::to_int(std::string(colstart, colend)));
					colstart = colend+1;
					colend = end;
					col.set_blue(CL_String::to_int(std::string(colstart, colend)));
					col.set_alpha(255);
					
					colorstack.push(col);
				}
				break;
			case 3:
				{
					//RGBA specified
					CL_Color col;
					
					std::string::const_iterator colstart = start;
					std::string::const_iterator colend = std::find(colstart, end, ' ');
					col.set_red(CL_String::to_int(std::string(colstart, colend)));
					colstart = colend+1;
					colend = std::find(colstart, end, ' ');
					col.set_green(CL_String::to_int(std::string(colstart, colend)));
					colstart = colend+1;
					colend = std::find(colstart, end, ' ');
					col.set_blue(CL_String::to_int(std::string(colstart, colend)));
					colstart = colend+1;
					colend = end;
					col.set_alpha(CL_String::to_int(std::string(colstart, colend)));
					
					colorstack.push(col);
				}
				break;
			default:
				throw CL_Error("CL_TextStyler: Invalid color tag specified!");
				break;
		}
	}
	else if (!colorstack.empty())
	{
		colorstack.pop();
	}
}

CL_TextStyler::CL_TextStyler() :
	scale_x(1.0f), 
	scale_y(1.0f), 
	trans_origin(origin_top_left),
	trans_x(0), 
	trans_y(0) 
{
}

CL_TextStyler::CL_TextStyler(const CL_TextStyler& other) :
	resource(other.resource), 
	scale_x(other.scale_x), 
	scale_y(other.scale_y), 
	trans_origin(other.trans_origin),
	trans_x(other.trans_x), trans_y(other.trans_y),
	fonts(other.fonts)
{
	resource.load();
}

CL_TextStyler::CL_TextStyler(const std::string &resource_id, CL_ResourceManager* manager)
{
	CL_Resource res = manager->get_resource(resource_id);
	res.load();
	CL_ResourceData_TextStyler* data = (CL_ResourceData_TextStyler*)res.get_data("text_styler");
	operator=(data->get_text_styler());
	resource = res;
}

int CL_TextStyler::get_height() const
{
	std::map<std::string, CL_Font>::const_iterator x = fonts.find("default");
	if (x != fonts.end())
		return x->second.get_height();
	else
		throw CL_Error("CL_TextStyler error: Argument-less get_height() method requires a 'default' font, but there isn't one");
}

CL_Size CL_TextStyler::get_size(
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_Size max_size) const
{
	CL_GlyphBuffer gb;
	draw_to_gb(start, end, gb, max_size);
	return gb.get_size();
}

CL_Rect CL_TextStyler::bounding_rect(
	CL_Rect dest,
	std::string::const_iterator start,
	std::string::const_iterator end) const
{
	CL_GlyphBuffer gb;
	draw_to_gb(start, end, gb, dest.get_size());
	gb.set_alignment(trans_origin, trans_x, trans_y);
	return gb.bounding_rect(dest);
}

CL_TextStyler& CL_TextStyler::operator =(const CL_TextStyler& other)
{
	resource.unload();

	scale_x = other.scale_x;
	scale_y = other.scale_y;
	trans_origin = other.trans_origin;
	trans_x = other.trans_x;
	trans_y = other.trans_y;
	resource = other.resource;
	fonts = other.fonts;
	
	resource = other.resource;
	resource.load();
	
	return *this;
}

int CL_TextStyler::draw(
	CL_Rect dest,
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_GraphicContext *context) const
{
	if (context == 0)
		context = CL_Display::get_current_window()->get_gc();
	
	CL_GlyphBuffer gb;
	int ret = draw_to_gb(start, end, gb, dest.get_size());
	gb.set_alignment(trans_origin, trans_x, trans_y);
	
	if (trans_origin == origin_top_left)
		gb.fixed_draw(dest.left - trans_x, dest.top - trans_y, context);
	else
		gb.draw(dest, context);
	
	return ret;
}

int CL_TextStyler::draw_to_gb(
	std::string::const_iterator start,
	std::string::const_iterator end,
	CL_GlyphBuffer& gb,
	CL_Size max_size) const
{
	std::vector<std::pair<CL_Font,std::pair<float,float> > > scalesaver;
	std::list<std::string> fnttags; //Stack of open font tags, beginning is top
	std::list<std::pair<float, float> > scaletags; //Stack of open scale tags, beginning is top
	std::stack<CL_Color> colorstack; //Stack of color tags, overrides existing CL_Font color
	std::stack<unsigned char> justifstack; //l for left, c for center, r for right; represents current justif tag state
	std::map<int, unsigned char> justifmarkers; //Same letter connotations as above, marks places where justif will be changed later
	
	const int orig_size = gb.get_glyphs().size(); //So we can calculate the return value easily
	const std::string tagopns = "{["; //Characters that serve as tag openers
	
	//Left-justify the last line of the existing buffer, if there is one
	if (orig_size != 0)
	{
		CL_GlyphBuffer::Range last_line = gb.get_line(gb.get_glyphs().size()-1);
		gb.justify_left(last_line);
	}
	
	//Go through the source string in chunks (a chunk either being a tag, a bit of text, or an escaped tag opener)
	for (std::string::const_iterator it = start; it != end;)
	{
		//If it is a tag opening character...
		if (tagopns.find(*it) != std::string::npos)
		{
			//Barf if the last character of the input is a tag opener
			if (it == end-1)
				throw CL_Error("CL_TextStyler error: Incomplete tag, bailing out");
			
			//If it's escaped, then draw it by itself and move over both characters
			if (*(it+1) == *it)
			{
				const int glyph_cnt =
					get_font(*this, fnttags, scaletags, colorstack,scalesaver).draw_to_gb(std::string(1, *it), gb, max_size);
				if (glyph_cnt != 1)
					break;
				
				it += 2;
			}
			else
			{
				unsigned char eotchr;
				if (*it == '{')
					eotchr = '}';
				else
					eotchr = ']';
				
				std::string::const_iterator tag_end = std::find(it, end, eotchr);
				if (tag_end == end)
					throw CL_Error("CL_TextStyler error: Incomplete tag, bailing out");
				if (tag_end == it+1 || (*(it+1) == '/' && tag_end == it+2))
					throw CL_Error("CL_TextStyler error: Empty tag, bailing out");
				
				++tag_end;
				
				if (*it == '{')
					apply_font_tag(fnttags, it, tag_end);
				else if (*(it+1) == 's' || (*(it+1) == '/' && *(it+2) == 's'))
					apply_scale_tag(scaletags, it, tag_end);
				else if (*(it+1) == 'j' || (*(it+1) == '/' && *(it+2) == 'j'))
					apply_justif_tag(justifstack, justifmarkers, gb.get_glyphs().size(), it, tag_end);
				else if (*(it+1) == 'c' || (*(it+1) == '/' && *(it+2) == 'c'))
					apply_color_tag(colorstack, it, tag_end);
				else if (*(it+1) == 'n')
					get_font(*this, fnttags, scaletags, colorstack,scalesaver).draw_to_gb(std::string("\n"), gb, max_size);
				else
					throw CL_Error("CL_TextStyler error: Unknown tag, bailing out");
				
				it = tag_end;
			}
		}
		else
		{
			std::string::const_iterator chunk_end = std::find_first_of(it, end, tagopns.begin(), tagopns.end());
			
			const int glyph_cnt = get_font(*this, fnttags, scaletags, colorstack,scalesaver).draw_to_gb(it, chunk_end, gb, max_size);
			
			if (glyph_cnt != chunk_end - it)
				break;
			it = chunk_end;
		}
	}
	
	//The column passed to justify_right (and, halved, to justify_center)
	int right_side;
	if (max_size.width != 0)
		right_side = max_size.width;
	else
		right_side = gb.get_width();
	
	//Apply justification markers that were added based on any justif tags
	for (std::map<int, unsigned char>::iterator x = justifmarkers.begin(); x != justifmarkers.end(); ++x)
	{
		std::map<int, unsigned char>::iterator y = x;
		++y;
		
		//Get the range that this tag applies to
		CL_GlyphBuffer::Range prerange; //Range before line boundary calculation
		prerange.start = x->first;
		if (y == justifmarkers.end())
			prerange.end = gb.get_glyphs().size();
		else
			prerange.end = y->first;
		
		std::vector<CL_GlyphBuffer::Range> lines = gb.get_lines(prerange);
		
		if (lines.size() != 0)
		{
			//Range after line boundary calculation
			CL_GlyphBuffer::Range range(lines.begin()->start, (lines.end()-1)->end);
			
			switch (x->second)
			{
				case 'l':
					gb.justify_left(range);
					break;
				case 'c':
					gb.justify_center(range, right_side/2);
					break;
				case 'r':
					gb.justify_right(range, right_side);
					break;
				default:
					throw CL_Error("CL_TextStyler error: Unknown justification tag");
					break;
			}
		}
	}

	scale_restore(scalesaver);
	
	return gb.get_glyphs().size() - orig_size;
}
