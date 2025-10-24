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

#ifndef header_text_styler
#define header_text_styler

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

#include <string>
#include <map>
#include <list>

#include "font.h"
#include "glyph_buffer.h"
#include "../Core/Math/origin.h"
#include "../Core/Math/rect.h"
#include "../Core/Math/point.h"
#include "../Core/Resources/resource.h"

class CL_GraphicContext;
class CL_ResourceManager;

//: Draws specially formatted text in one or several fonts.
//- !group=Display/Fonts!
//- !header=display.h!
//- <p> A CL_TextStyler consists of a collection of named CL_Fonts.
//- When passed a string to draw, it uses vaguely HTMLish tags to select 
//- which font to draw text in. For example:</p>
//- <code> "Here's some text, {italic}isn't it {bold}amazing?{/bold}{/italic}" </code>
//- <p> CL_TextStyler does not have any mechanisms on its own for bold
//- and italic text, so in order for this to work, there must be fonts named 'bold' and
//- 'italic' that it knows of. Text outside font tags is
//- drawn in the font named 'default', if it exists. No blending 
//- or anything like that is performed with nested fonts; the innermost font
//- is the one used, and the nesting is for convienence.</p>
//- <p>There are also special tags which are bounded by [ and ].
//- These can be used for justification, scaling, color, and newlines.</p>
//- <p>Justification can be
//- either left, center, or right, shown by this example:</p>
//- <code> "This line is left justified.\n[j center]This line is center
//- justified.\n[j right]This line is right justified.\n[/j][/j]Back to
//- left justification." </code>
//- <p>Scaling tags affect both x and y scaling the same amount when given a single number, but
//- can be given two numbers to affect both:</p>
//- <code> "Regular size.[s 2.0 1.0]Double wide text.[/s][s 1.0 2.0]Double tall text." </code>
//- <p>Scaling tags are also cumulative, as shown by this example:</p>
//- <code> "This text is at regular size.[s 2.0]This text is at double size.[s 2.0]This text is at quadruple
//- size.[/s]Double size again.[/s]Regular size again." </code>
//- <p>Color tags allow specification of RGB and RGBA values, as well as use of named colors (see CL_Color's
//- documentation for a reference). The color overrides the font's existing color entirely, and does not blend
//- with it. Be sure that the blending functions of a font are set properly before using that font in a TextStyler
//- with color tags.</p>
//- <code> "This text is in whatever the font's default color is.[c blue]This text is blue.[c red]This text
//- is red, not blue-red; there's no blending between nested colors.[/c][/c][c 0 255 0]This text is green.[/c]
//- [c 255 0 255 175]This text is translucent purple; the fourth number is the alpha value, which
//- is set to opaque (255) if unspecified.[/c]" </code>
//- <p>Newline tags are only helpful when loading TextStyler-intented strings from XML resource
//- files, where there's no easy way to insert an actual newline character:</p>
//- <code> "First line.[n]Second line." </code>
//- <p>To draw a real a left brace or left square bracket and not start a tag, just escape it by putting two
//- of the characters in a row; right braces and right square brackets don't need to be escaped,
//- and CL_TextStyler will take them literally if they don't seem to end an open tag. References to fonts which are
//- not in the CL_TextStyler's font map will be silently ignored. Using bad nesting order is
//- also allowed (i.e. {a}{b}{/a}{/b}). However, you can't have a partial tag in the string
//- (that is, "blah blah {tag doesnt end"), and you are not allowed to
//- put any of the four tag opening/closing characters inside a tag, or in
//- a font name, even if they're escaped. </p>
//- <p>Like with CL_Font, scaling affects the calculation
//- of any bounding rectangles (such as the result returned
//- by draw() or bounding_rect(), or the rectangle calculated internally by
//- draw() for alignment). This is because scaling
//- the CL_TextStyler is effectively just changing the point size of the glyphs,
//- and that affects all sorts of things, such as word wrapping.</p>

class CL_API_DISPLAY CL_TextStyler
{
//! Construction:
public:
	//: Constructs a text styler.
	CL_TextStyler();
	CL_TextStyler(const CL_TextStyler& other);
	CL_TextStyler(
		const std::string &resource_id,
		CL_ResourceManager *manager);
	
	//: Destruction
	virtual ~CL_TextStyler() {resource.unload();}

//! Attributes:
public:
	//: Returns a reference to the font map.
	std::map<std::string, CL_Font>& get_fonts()
		{return fonts;}
	const std::map<std::string, CL_Font>& get_fonts() const
		{return fonts;}
	
	//: Returns scale for x and y.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void get_scale(float &x, float &y) const
		{x = scale_x, y = scale_y;}
	
	//: Returns translation hotspot.
	void get_alignment(CL_Origin &origin, int &x, int &y) const
		{origin = trans_origin; x = trans_x; y = trans_y;}
	
	//: Returns the drawn height of some source text, or of the default font if no arguments passed.
	//return: The height in pixels.
	//param str: The source string to parse and render.
	//param start: A starting iterator, inclusive.
	//param end: An ending iterator, exclusive.
	//param max_size: Acts like the size of a dest rectangle passed to draw(), for height truncating and word wrapping.
	//- <p> Generally, get_height(" ") will return zero, since usually 
	//- the space glyph isn't given in the font, and so it has a height and width of zero. 
	//- Calling get_height() on a CL_TextStyler without arguments is the same as calling
	//- get_height() without arguments on that CL_TextStyler's default font.</p>
	int get_height() const;
	
	int get_height(
		const std::string &str,
		CL_Size max_size = CL_Size(0,0)) const
		{return get_size(str, max_size).height;}
	
	int get_height(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_Size max_size = CL_Size(0,0)) const
		{return get_size(start, end, max_size).height;}
	
	//: Returns the drawn width of some source text.
	//return: The width in pixels.
	//param str: The source string to parse and render.
	//param start: A starting iterator, inclusive.
	//param end: An ending iterator, exclusive.
	//param max_size: Acts like the size of a dest rectangle passed to draw(), for height truncating and word wrapping.
	//- <p>Newlines have a width of zero unless you gave them a glyph (which is pretty useless anyways).
	//- Other unknown characters have the width of a space in their respective font.</p>
	int get_width(
		const std::string &str,
		CL_Size max_size = CL_Size(0,0)) const
		{return get_size(str, max_size).width;}
	
	int get_width(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_Size max_size = CL_Size(0,0)) const
		{return get_size(start, end, max_size).width;}
	
	//: Returns the drawn size of some source text.
	//return: The size in pixels.
	//param str: The source string to parse and render.
	//param start: A starting iterator, inclusive.
	//param end: An ending iterator, exclusive.
	//param max_size: Acts like the size of a dest rectangle passed to draw(), for height truncating and word wrapping.
	//- <p>Newlines have a width of zero unless you gave them a glyph (which is pretty useless anyways).
	//- Other unknown characters have the width of a space in their respective font.</p>
	CL_Size get_size(
		const std::string &str,
		CL_Size max_size = CL_Size(0,0)) const
		{return get_size(str.begin(), str.end(), max_size);}
	
	CL_Size get_size(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_Size max_size = CL_Size(0,0)) const;
	
	//: Calculate the rectangle that would be occupied by a draw operation.
	//param x, y: Anchor position of where to render text. Actual rendering position depends on the alignment mode.
	//param dest: Rectangle to draw text in. The text will be word-wrapped against delimiters to fit inside the rectangle.
	//param str: The source string to parse and render.
	//param start: String position to begin rendering at, inclusive.
	//param end: String position to end rendering at, exclusive.
	//- <p> You can specify a dest rectangle with a width or height of zero or less to disable word wrapping
	//- or height truncating, respectively. </p>
	//- <p> It's more efficient to draw text into a GlyphBuffer, then use that for drawing and bounding
	//- rect calculation. This method is just for convienence. </p>
	CL_Rect bounding_rect(
		int x,
		int y,
		const std::string& str) const
		{return bounding_rect(CL_Rect(x, y, x, y), str.begin(), str.end());}
	
	CL_Rect bounding_rect(
		CL_Rect dest,
		const std::string& str) const
		{return bounding_rect(dest, str.begin(), str.end());}
	
	CL_Rect bounding_rect(
		int x,
		int y,
		std::string::const_iterator start,
		std::string::const_iterator end) const
		{return bounding_rect(CL_Rect(x, y, x, y), start, end);}
	
	CL_Rect bounding_rect(
		CL_Rect dest,
		std::string::const_iterator start,
		std::string::const_iterator end) const;
	
	//: Resource owning this text styler, if any.
	CL_Resource resource;

//! Operations:
public:
	//: Copy assignment
	CL_TextStyler& operator=(const CL_TextStyler& other);
	
	//: Adds a new font to the map
	void add_font(std::string name, const CL_Font& font)
		{fonts.insert(std::pair<std::string, CL_Font>(name, font));}
	
	//: Draws text onto a graphics context.
	//return: The number of glyphs that were drawn.
	//param x, y: Anchor position of where to draw at. Actual rendering position depends on the alignment mode.
	//param dest: Rectangle to draw text in. The text will be word-wrapped against delimiters to fit inside the rectangle.
	//param str: The source string to parse and render.
	//param start: String position to begin rendering at, inclusive.
	//param end: String position to end rendering at, exclusive.
	//param context: Graphic context on which to render upon. If null, will use CL_Display's current graphic context.
	//- <p> You can specify a dest rectangle with a width or height of zero or less to disable word wrapping
	//- or height truncating, respectively. </p>
	int draw(
		int x,
		int y,
		const std::string& str,
		CL_GraphicContext *context = 0) const
		{return draw(CL_Rect(x, y, x, y), str.begin(), str.end(), context);}
	
	int draw(
		CL_Rect dest,
		const std::string& str,
		CL_GraphicContext *context = 0) const
		{return draw(dest, str.begin(), str.end(), context);}
	
	int draw(
		int x,
		int y,
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_GraphicContext *context = 0) const
		{return draw(CL_Rect(x, y, x, y), start, end, context);}
	
	int draw(
		CL_Rect dest,
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_GraphicContext *context = 0) const;
	
	//: Inserts data into a CL_GlyphBuffer, treating the glyphs already there as part of a previous draw_to_gb().
	//return: The number of glyphs that were drawn.
	//param str: The input string to draw.
	//param start: String position to begin drawing at, inclusive.
	//param end: String position to end drawing at, exclusive.
	//param max_size: Sets size for word wrapping and height truncating. Either can be zero to disable that feature.
	//param gb: The glyph buffer to mess with.
	//- <p> The CL_GlyphBuffer's contents (the glyphs vector, the font markers map, and the effects maps),
	//- if any, must not have been created/altered by anything but CL_Font::draw_to_gb() and/or CL_TextStyler::draw_to_gb()
	//- for this method to work.</p>
	//- <p> If you are doing draw_to_gb several sequential times
	//- to the same CL_GlyphBuffer, then you must pass the same maximum width to each call of
	//- draw_to_gb. </p>
	//- <p> You can tell when you've filled the buffer up to the height in max_size when a call to this method returns
	//- anything less than the size of the string. There's also the chance that it will backtrack and return
	//- a negative value if it runs out
	//- of vertical space in the process of wrapping a just-completed word; this is still a sign that you've
	//- ran out of vertical space.</p>
	//- <p> Scale tags create new font markers in the GB, they don't use the GB scale effects map. </p>
	int draw_to_gb(
		const std::string &str,
		CL_GlyphBuffer &gb,
		CL_Size max_size = CL_Size(0,0)) const
		{return draw_to_gb(str.begin(), str.end(), gb, max_size);}
	
	int draw_to_gb(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_GlyphBuffer &gb,
		CL_Size max_size = CL_Size(0,0)) const;
	
	//: Set scale for x and y directions individually.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void set_scale(float x, float y)
		{scale_x = x; scale_y = y;}
	
	//: Sets translation hotspot.
	void set_alignment(CL_Origin origin, int x = 0, int y = 0)
		{trans_origin = origin; trans_x = x; trans_y = y;}
	
// Implementation:
private:
	//: Scale.
	float scale_x;
	float scale_y;
	
	//: Alignment.
	CL_Origin trans_origin;
	int trans_x;
	int trans_y;
	
	//: The font map.
	std::map<std::string, CL_Font> fonts;
};

#endif
