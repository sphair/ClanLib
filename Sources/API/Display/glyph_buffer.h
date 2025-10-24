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

//! clanDisplay="Fonts"
//! header=display.h

#ifndef header_glyph_buffer
#define header_glyph_buffer

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

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <map>
#include <vector>
#include <utility>

#include "font.h"
#include "../Core/Math/origin.h"
#include "color.h"

#if defined(__APPLE__) && defined(verify)
	// I guess sometimes "verify" is defined on OSX by some system headers
	// it manifests itself with an odd error on the following line (below):
	//   void verify(const CL_GlyphBuffer& gb);
	// there might be a better solution to this, but it seems to work out
	#undef verify
#endif

class CL_Point;
class CL_Rect;
class CL_GraphicContext;

//: Stores and draws glyphs in one or more fonts.
//- !group=Display/Fonts!
//- !header=display.h!
//- <p>CL_GlyphBuffer is mostly useful for text effects, for concatenating
//- multiple CL_Font or CL_TextStyler draw_to_gb()s, or for
//- when you need access to the low-level glyph positioning data. You can
//- also use it to save time by only rendering a bit of text once when you need
//- to, for example, both draw that text and know its bounding rectangle. But
//- for optimiziation purposes alone when dealing with unchanging text, you
//- should instead
//- have CL_Font or CL_TextStyler render onto a CL_Canvas or CL_Surface.</p>
//- <p>A glyph buffer consists of a series of characters and positions,
//- and markers for font changes and various other text-altering infomration. The internal data structures used
//- are made accessible and mutable externally. CL_Font
//- and CL_TextStyler both have draw_to_gb() methods, which allows them to
//- put data into a CL_GlyphBuffer rather than drawing to a CL_GraphicContext. </p>
//- <p>The effect maps for color, angle, and scale allow you to affect those properties
//- for individual glyphs during drawing and bounding rect calculation. The angle and scale maps are different
//- from the GlyphBuffer angle and scale properties; the properties apply to the entire CL_GlyphBuffer as a whole, while entries
//- in the effect maps affect individual glyphs seperately. It's not legal to insert
//- an effect map entry for a glyph that doesn't exist.</p>
//- <p>Unlike scale effects map entries, which are multiplied by the CL_Font's existing scale, 
//- color effect map entries entirely override the CL_Font's color. No blending between
//- the two colors is performed. Also, it's your responsibility to make sure that CL_Fonts have the right
//- blending functions.</p>
//- <p>In functions that calculate the size or bounding rectangle of the GlyphBuffer
//- (such as bouding_rect(), get_size(), and the alignment calculations within draw),
//- effect properties do take effect on the final rectangle, but GlyphBuffer rotation
//- and scaling (set with CL_GlyphBuffer::set_angle(), and CL_GlyphBuffer::set_scale(),
//- respectively) do not. This is consistent with the behavior of CL_Sprite and CL_Surface; the content
//- affects bounding rect calculation, but the other properties of the object
//- do not.</p>
class CL_API_DISPLAY CL_GlyphBuffer
{
//! Construction:
public:
	//: Constructs a glyph buffer.
	CL_GlyphBuffer();
	
	virtual ~CL_GlyphBuffer() {;}
	
//! Subclasses:
public:
	//: A character and position describing a glyph in the buffer, stored in the glyphs vector.
	struct Glyph
	{
		//:Constructs a new Glyph
		//p: The CL_Point that will be assigned to pos
		//c: The unsigned int that will be assigned to character
		Glyph() {;}
//		Glyph(CL_Point p, unsigned int c) : pos(p), character(c),glyphscale_x(1.0f),glyphscale_y(1.0f),fontcolor(CL_Color(0.0f,0.0f,0.0f,0.0f)) {;}
		Glyph(CL_Point p, unsigned int c,float scalex,float scaley,CL_Color col = CL_Color(0.0f,0.0f,0.0f,0.0f)) : pos(p), character(c),glyphscale_x(scalex),glyphscale_y(scaley),fontcolor(col) {;}
		
		//: The position where the glyph is drawn.
		CL_Point pos;
		
		//: The character which determines the glyph that's drawn.
		unsigned int character;
	
		//: the scalex a character has
		float glyphscale_x;
	
		//: the scaley a character has
		float glyphscale_y;

		//: the fontcolor
		CL_Color fontcolor;
	};

	//: Represents a subsection of the buffer
	struct Range
	{
		//:Constructs a new Range.
		//new_start: The starting index of the range, inclusive
		//new_end: The ending index of the range, exclusive
		//gb: Constructs a new Range that encompasses this entire CL_GlyphBuffer
		Range() : start(0), end(0) {;}
		Range(const CL_GlyphBuffer& gb) : start(0), end(int(gb.get_glyphs().size())) {;}
		Range(int new_start, int new_end) : start(new_start), end(new_end) {;}
		
		//: Makes range valid for a given GlyphBuffer.
		//- <p> This is automatically called when you pass the range to any GlyphBuffer method.</p>
		void verify(const CL_GlyphBuffer& gb);
		
		//: Pulls in the range at the beginning until it reaches a drawable glyph
		void remove_invis_left(const CL_GlyphBuffer& gb);
		
		//: Pulls in the range at the end until it reaches a drawable glyph
		void remove_invis_right(const CL_GlyphBuffer& gb);
		
		//: Pulls in the range at both sides until it reaches a drawable glyph
		void remove_invis_both(const CL_GlyphBuffer& gb)
			{remove_invis_left(gb); remove_invis_right(gb);}
		
		//The starting index of the range, inclusive.
		int start;
		
		//The ending index of the range, exclusive.
		int end;
	};
	
//! Attributes:
public:
	//: Returns a reference to the glyphs in the buffer and their drawing positions.
	//- <p>See CL_GlyphBuffer::Glyph for details./p>
	std::vector<Glyph>& get_glyphs() {return glyphs;}
	const std::vector<Glyph>& get_glyphs() const {return glyphs;}
	
	//: Returns a reference to the font map.
	//- <p>Elements of the glyphs vector before the first font marker are ignored. Each element
	//- within this map causes a change in the font while drawing, where the key of the
	//- element is the index in the glyphs vector before which the font is changed.</p>
	std::map<int, CL_Font>& get_font_markers() {return font_markers;}
	const std::map<int, CL_Font>& get_font_markers() const {return font_markers;}
	
	//: Returns a reference to the color effect map.
	std::map<int, CL_Color>& get_color_effects() {return color_effects;}
	const std::map<int, CL_Color>& get_color_effects() const {return color_effects;}
	
	//: Returns a reference to the x scale effect map.
	//- <p> Scaling occurs away from the upper-left corner of each glyph. </p>
	std::map<int, float>& get_scale_x_effects() {return scale_x_effects;}
	const std::map<int, float>& get_scale_x_effects() const {return scale_x_effects;}
	
	//: Returns a reference to the y scale effect map.
	//- <p> Scaling occurs away from the upper-left corner of each glyph. </p>
	std::map<int, float>& get_scale_y_effects() {return scale_y_effects;}
	const std::map<int, float>& get_scale_y_effects() const {return scale_y_effects;}
	
	//: Returns a reference to the character angle effect map.
	std::map<int, float>& get_angle_effects() {return angle_effects;}
	const std::map<int, float>& get_angle_effects() const {return angle_effects;}
	
	//: Returns an iterator to the font_marker associated with a particular glyph.
	//- <p> If a negative index is supplied, assumes glyph 0. If an index
	//- past the last element is supplied, assumes the last element. If
	//- there are no CL_Fonts in the GlyphBuffer, or if there is no CL_Font
	//- associated with that glyph, returns the end iterator.</p>
	std::map<int, CL_Font>::iterator get_font_for(int glyph_index);
	std::map<int, CL_Font>::const_iterator get_font_for(int glyph_index) const;
	
	//: Returns the width of the entire buffer, or a sub-Range.
	//return: The width in pixels.
	//param range: A subsection of the glyphs vector to get the width of.
	//param idx: The vector index of a single glyph to get the width of.
	//param ignore_invis: If true, ignores invisible glyphs (glyphs without an actual visual glyph in the font)
	int get_width(bool ignore_invis = true) const
		{return get_width(Range(*this), ignore_invis);}
	int get_width(int idx, bool ignore_invis = true) const
		{return get_width(Range(idx, idx+1), ignore_invis);}
	int get_width(Range range, bool ignore_invis = true) const
		{return get_size(range, ignore_invis).width;}
	
	//: Returns the height of the entire buffer, or a sub-Range.
	//return: The height in pixels.
	//param range: A subsection of the glyphs vector to get the height of.
	//param idx: The vector index of a single glyph to get the height of.
	//param ignore_invis: If true, ignores invisible glyphs (glyphs without an actual visual glyph in the font)
	int get_height(bool ignore_invis = true) const
		{return get_height(Range(*this), ignore_invis);}
	
	int get_height(int idx, bool ignore_invis = true) const
		{return get_height(Range(idx, idx+1), ignore_invis);}
	
	int get_height(Range range, bool ignore_invis = true) const
		{return get_size(range, ignore_invis).height;}
	
	//: Returns the size of the entire buffer, or a sub-Range.
	//return: The size in pixels.
	//param range: A subsection of the glyphs vector to get the size of.
	//param idx: The vector index of a single glyph to get the size of.
	//param ignore_invis: If true, ignores invisible glyphs (glyphs without an actual visual glyph in the font)
	CL_Size get_size(bool ignore_invis = true) const
		{return get_size(Range(*this), ignore_invis);}
	
	CL_Size get_size(int idx, bool ignore_invis = true) const
		{return get_size(Range(idx, idx+1), ignore_invis);}
	
	CL_Size get_size(Range range, bool ignore_invis = true) const
		{return internal_rect(range, ignore_invis).get_size();}
	
	//: Returns current angle.
	//- <p> This rotates the entire glyph field when drawing.
	//- The fonts' glyph rotation origin must be origin_top_left:0:0 for this
	//- to produce legible results. See CL_Font::set_glyph_rotation_hotspot(). </p>
	float get_angle() const
		{return rot_angle;}
	
	//: Returns scale for x and y.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void get_scale(float &x, float &y) const
		{x = scale_x; y = scale_y;}
	
	//: Returns translation hotspot for the entire glyph field.
	void get_alignment(CL_Origin &origin, int &x, int &y) const
		{origin = trans_origin; x = trans_x; y = trans_y;}
	
	//: Returns rotation hotspot for the entire glyph field.
	void get_rotation_hotspot(CL_Origin &origin, int &x, int &y) const
		{origin = rot_origin; x = rot_x; y = rot_y;}
	
	//: Returns the area that would be drawn to if a draw were performed.
	//return: The area into which any pixels would be drawn. If buffer is empty, returns a rectangle with zero size.
	//param x, y: Anchor position to start "drawing" at. Actual rendering position depends on the alignment mode.
	//param dest: Rectangle to render glyphs in. The glyphs will be positioned within the rectangle depending on the alignment mode.
	//param range: The subsection of the glyphs vector to use.
	//param ignore_invis: If true, ignores invisible glyphs (glyphs without an actual visual glyph in the font)
	CL_Rect bounding_rect(int x = 0, int y = 0, bool ignore_invis = true) const
		{return bounding_rect(Range(*this), x, y, ignore_invis);}
	
	CL_Rect bounding_rect(Range range, int x = 0, int y = 0, bool ignore_invis = true) const
		{return bounding_rect(range, CL_Rect(x, y, x, y), ignore_invis);}
	
	CL_Rect bounding_rect(CL_Rect dest, bool ignore_invis = true) const
		{return bounding_rect(Range(*this), dest, ignore_invis);}
	
	CL_Rect bounding_rect(Range range, CL_Rect dest, bool ignore_invis = true) const;
	
	//: Returns the area occupied by glyphs inside the glyph buffer's coordinate space.
	//return: The area occupied by all glyphs, inside the buffer. If buffer is empty, returns a rectangle with zero size.
	//param range: The subsection of the glyphs vector to get the internal bounding rect of.
	//param idx: The vector index of a single glyph to get the internal bounding rect of.
	//param ignore_invis: If true, ignores invisible glyphs (glyphs without an actual visual glyph in the font)
	CL_Rect internal_rect(bool ignore_invis = true) const
		{return internal_rect(Range(*this), ignore_invis);}
	CL_Rect internal_rect(int idx, bool ignore_invis = true) const
		{return internal_rect(Range(idx, idx+1), ignore_invis);}
	CL_Rect internal_rect(Range range, bool ignore_invis = true) const;
	
	//: Returns a vector of CL_GlyphBuffer::Range, each of which contains one line.
	//param range: The subsection of the glyphs vector to look through, which is expanded both ways.
	//- <p> The CL_GlyphBuffer's contents (the glyphs vector, the font markers map, and the effects maps),
	//- if any, must not have been created/altered by anything but CL_Font::draw_to_gb() or CL_TextStyler::draw_to_gb()
	//- or the CL_GlyphBuffer justification functions or using CL_GlyphBuffer::remove to take glyphs off the end
	//- for this method to work.</p>
	std::vector<Range> get_lines() const
		{return get_lines(Range(*this));}
		
	std::vector<Range> get_lines(Range range) const;
	
	//: Returns a single CL_GlyphBuffer::Range containing the line that contains a given glyph.
	//param idx: The index in the glpyhs vector of the glyph.
	//- <p> The CL_GlyphBuffer's contents (the glyphs vector, the font markers map, and the effects maps),
	//- if any, must not have been created/altered by anything but CL_Font::draw_to_gb() or CL_TextStyler::draw_to_gb()
	//- or the CL_GlyphBuffer justification functions or using CL_GlyphBuffer::remove to take glyphs off the end
	//- for this method to work.</p>
	CL_GlyphBuffer::Range get_line(int idx) const;

//! Operations
public:
	//: Add elements onto the end of the glyphs vector.
	//return: A reference to this object, allowing you to chain multiple calls of add() together.
	//param x, y: The position to add the characters at (always upper-left positioning inside the buffer).
	//param center_x, center_y: Where to realign the origin of the input GlyphBuffer to in this GlyphBuffer.
	//param character: A single character to add without preprocessing.
	//param other: Another CL_GlyphBuffer to copy all glyphs and data from to the specified position.
	//param range: The subsection of the other GlyphBuffer to add onto the end of this one
	//- <p> It's usually more useful and efficient to have ClanLib draw many glyphs into the GlyphBuffer at once using
	//- CL_Font::draw_to_gb() or CL_TextStyler::draw_to_gb() than to use this function or the glyphs vector
	//- to manually add glyphs.</p>
	CL_GlyphBuffer &add(int x, int y, unsigned int character,float scalex = 1.0f,float scaley = 1.0f)
		{glyphs.push_back(Glyph(CL_Point(x,y), character,scalex,scaley)); return *this;}
	
	CL_GlyphBuffer &add(const CL_GlyphBuffer &other, int center_x = 0, int center_y = 0)
		{return add(other, Range(other), center_x, center_y);}
	
	CL_GlyphBuffer &add(const CL_GlyphBuffer &other, Range range, int center_x = 0, int center_y = 0);
	
	//: Removes an element or elements from the glyphs vector, reorganizes the font and effect maps to compensate.
	//return: A reference to this object, allowing you to chain multiple calls of remove() together.
	//param index: The index in the glyphs vector to remove (all elements beyond this are slid back)
	//param range: The subsection of the glyphs vector to erase
	CL_GlyphBuffer &remove(int index)
		{return remove(Range(index, index+1));}
	
	CL_GlyphBuffer &remove(Range range);
	
	//: Adds a font change marker, set to occur before the next glyph/location you add is drawn.
	//return: A reference to this object, allowing you to chain change_font() into chains of calls to add().
	//param font: The font to change to.
	CL_GlyphBuffer &change_font(const CL_Font &font);
	
	//: Remove all the character position and font marker data in the buffer.
	void clear();
	
	//: Draw the glyphs in the buffer.
	//param x, y: Anchor position to draw at. Actual drawing position depends on the alignment mode.
	//param gc: Graphic context on which to render. If null, will use CL_Display's current graphic context.
	//param dest: Rectangle to draw glyphs in. The glyphs will be positioned within the rectangle depending on the alignment mode.
	//param range: The subsection of the glyphs vector to use.
	void draw(
		int x = 0,
		int y = 0,
		CL_GraphicContext *gc = 0)
		{draw(Range(*this), CL_Rect(x, y, x, y), gc);}
	
	void draw(
		Range range,
		int x = 0,
		int y = 0,
		CL_GraphicContext *gc = 0)
		{draw(range, CL_Rect(x, y, x, y), gc);}
	
	void draw(
		CL_Rect dest,
		CL_GraphicContext *gc = 0)
		{draw(Range(*this), dest, gc);}
	
	void draw(
		Range range,
		CL_Rect dest,
		CL_GraphicContext *gc = 0)
		{CL_Rect irect = internal_rect(range); draw_glyphs(range, i2e_offset(irect, dest), irect, gc);}
	
	//: Draws the glyphs in the buffer without bounding rect alignment.
	//param x, y: Glyphs are drawn with the CL_GlyphBuffer's internal origin at this location.
	//param gc: Graphic context on which to render. If null, will use CL_Display's current graphic context.
	//param range: The subsection of the glyphs vector to use.
	void fixed_draw(
		int x = 0,
		int y = 0,
		CL_GraphicContext *gc = 0)
		{fixed_draw(Range(*this), x, y, gc);}
	
	void fixed_draw(
		Range range,
		int x = 0,
		int y = 0,
		CL_GraphicContext *gc = 0)
		{draw_glyphs(range, CL_Point(x, y), CL_Rect(0,0,0,0), gc);}
	
	//: Rejustifies lines in the GlyphBuffer to the left.
	//param range: The subsection of the glyphs vector to change, which is expanded out both ways to contain at least one line.
	//param left: The x coordinate of the vertical line that text should be moved up against.
	//- <p> The CL_GlyphBuffer's contents (the glyphs vector, the font markers map, and the effects maps),
	//- if any, must not have been created/altered by anything but CL_Font::draw_to_gb() or CL_TextStyler::draw_to_gb()
	//- or the CL_GlyphBuffer justification functions or using CL_GlyphBuffer::remove to take glyphs off the end
	//- for this method to work.</p>
	//- <p> To preserve indentation, this method takes into account whitespace (glyphs not provided by the font) when justifying.</p>
	void justify_left(int left = 0)
		{justify_left(Range(*this), left);}
	
	void justify_left(Range range, int left = 0);
	
	//: Rejustifies lines in the GlyphBuffer to the center.
	//param range: The subsection of the glyphs vector to change, which is expanded out both ways to contain at least one line.
	//param center: The x coordinate of the vertical line that text should be centered at.
	//- <p> The CL_GlyphBuffer's contents (the glyphs vector, the font markers map, and the effects maps),
	//- if any, must not have been created/altered by anything but CL_Font::draw_to_gb() or CL_TextStyler::draw_to_gb()
	//- or the CL_GlyphBuffer justification functions or using CL_GlyphBuffer::remove to take glyphs off the end
	//- for this method to work.</p>
	//- <p> This method ignores whitespace (glyphs not provided by the font) when justifying. </p>
	void justify_center(int center = 0)
		{justify_center(Range(*this), center);}
	
	void justify_center(Range range, int center = 0);
	
	//: Rejustifies lines in the GlyphBuffer to the right.
	//param range: The subsection of the glyphs vector to change, which is expanded out both ways to contain at least one line.
	//param right: The x coordinate of the vertical line that text should be moved up against.
	//- <p> The CL_GlyphBuffer's contents (the glyphs vector, the font markers map, and the effects maps),
	//- if any, must not have been created/altered by anything but CL_Font::draw_to_gb() or CL_TextStyler::draw_to_gb()
	//- or the CL_GlyphBuffer justification functions or using CL_GlyphBuffer::remove to take glyphs off the end
	//- for this method to work.</p>
	//- <p> This method ignores whitespace (glyphs not provided by the font) when justifying. </p>
	void justify_right(int right = 0)
		{justify_right(Range(*this), right);}
	
	void justify_right(Range range, int right = 0);
	
	//: Set absolute rotation angle.
	//- <p> This rotates the entire glyph field when drawing.
	//- The fonts' glyph rotation origin must be origin_top_left:0:0 for this
	//- to produce legible results. See CL_Font::set_glyph_rotation_hotspot(). </p>
	void set_angle(float angle);
	
	//: Add angle to current angle.
	//- <p> This rotates the entire glyph field when drawing.
	//- The fonts' glyph rotation origin must be origin_top_left:0:0 for this
	//- to produce legible results. See CL_Font::set_glyph_rotation_hotspot(). </p>
	void rotate(float angle)
		{angle += rot_angle; set_angle(angle);}
	
	//: Set scale for x and y directions individually.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void set_scale(float x, float y)
		{scale_x = x; scale_y = y;}
	
	//: Sets translation hotspot.
	void set_alignment(CL_Origin origin, int x = 0, int y = 0)
		{trans_origin = origin; trans_x = x; trans_y = y;}
	
	//: Sets rotation hotspot.
	void set_rotation_hotspot(CL_Origin origin, int x = 0, int y = 0)
		{rot_origin = origin; rot_x = x; rot_y = y;}
	
//! Implementation:
private:
	//: Used to iterate through the buffer, changing font and applying effects as needed
	class Iter;
	friend class Iter;
	class Iter {
		private:
			//Pointer to the GlyphBuffer
			const CL_GlyphBuffer* pgb;
			
			//The current index in the glyphs vector
			int glyph_num;
			
			//Do not allow default construction
			Iter() {}
			
		public:
			//Constructs a new Iter
			//param range: The beginning index in this Range is used as the initial glyph_num
			Iter(const CL_GlyphBuffer& new_gb, Range range)
				: pgb(&new_gb), glyph_num(range.start) {}
			
			//Advances to the next glyph (prefix)
			Iter& operator ++() {++glyph_num; return *this;}

			//Advances to the next glyph (postfix)
			const Iter operator ++(int) {Iter old = *this; ++(*this); return old;}
			
			//Returns true if the iterator has covered everything (i.e. is past-the-end)
			//param range: The ending index in this Range is checked against
			bool at_end(Range range) const {if (glyph_num >= range.end) return true; return false;}
			
			//Returns the index in the glyphs vector
			int get_glyph_num() const {return glyph_num;}
			
			//Returns the scale effect, or 1.0 if no scale effect for this glyph
			void get_scale_eff(float& x, float& y) const;
			
			//Returns the angle effect, or 0.0 if no angle effect for this glyph
			float get_angle_eff() const;
			
			//Returns the color effect, or given font's color if no color effect for this glyph
			CL_Color get_color_eff(const CL_Font& fnt) const;
			
			//Returns a non-mutable reference to the current font
			const CL_Font& get_font() const;
	};

	//: Returns true if the given glyph index is directly before a linebreak, manual or logical
	//param fnt: The font that idx is in
	bool is_linebreak(int idx, const CL_Font& fnt) const;
	
	//: Erases all elements of a map<int, T> in [start, end), then slides back elements
	template <typename T>
	static void cut_range(std::map<int, T>& target, int start, int end)
	{
		//Remove all the elements inside the range
		target.erase(target.lower_bound(start), target.upper_bound(end));
		
		//Move back all the elements from the end of the range to the end of the map
		for (typename std::map<int, T>::iterator it = target.lower_bound(end); it != target.end();)
		{
			target[it->first - (end - start)] = it->second;
			target.erase(it++);
		}
	}
	
	//: Returns the offset to align an internal bounding rectangle to an external bounding rectangle.
	CL_Point i2e_offset(CL_Rect src, CL_Rect dest) const;
	
	//: Returns the point to scale away from.
	//param irect: Internal rectangle containing all glyphs
	CL_Point get_scale_hotspot(CL_Rect irect) const;
	
	//: Returns the point to rotate around.
	//param irect: Internal rectangle containing all glyphs
	CL_Point get_rot_hotspot(CL_Rect irect) const;
	
	//: Draws all the glyphs in the buffer.
	//param offset: Added to each position right before drawing, to move to external coords
	//param irect: Result of internal_rect() passed in if caller has it, for efficiency's sake, else CL_Rect(0,0,0,0)
	//param gc: The graphics context to draw to (default if 0)
	//param range: The subsection of the glyphs vector to use
	void draw_glyphs(
		Range range,
		CL_Point offset,
		CL_Rect irect,
		CL_GraphicContext* gc) const;
	
	//: Scale.
	float scale_x;
	float scale_y;
	
	//: Angle and rotation hotspot.
	float rot_angle;
	CL_Origin rot_origin;
	int rot_x;
	int rot_y;
	
	//: Alignment.
	CL_Origin trans_origin;
	int trans_x;
	int trans_y;
	
	//: The glyphs and their positions.
	std::vector<Glyph> glyphs;
	
	//: The font change markers.
	mutable std::map<int, CL_Font> font_markers;
	
	//: Color, scale, and angle font effect maps.
	std::map<int, CL_Color> color_effects;
	std::map<int, float> scale_x_effects;
	std::map<int, float> scale_y_effects;
	std::map<int, float> angle_effects;
};

#endif
