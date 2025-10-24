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

#ifndef header_font
#define header_font

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
#include <utility> //For std::pair

#include "../Core/Math/origin.h"
#include "blend_func.h"
#include "color.h"
#include "../Core/Math/rect.h"
#include "../Core/Math/point.h"
#include "../Core/Math/size.h"
#include "../Core/Resources/resource.h"
#include "../Core/System/lazycopyptr.h"
#include "../Core/System/clonable.h"

class CL_GlyphBuffer;
class CL_Sprite;
class CL_TextStyler;
class CL_Font_Generic;
class CL_GraphicContext;
class CL_ResourceManager;

//: Draws text using system fonts or glyph sprites.
//- !group=Display/Fonts!
//- !header=display.h!
//- <p>A font can be constructed either from a CL_Sprite (aka a bitmap font) or from a system
//- font A bitmap font uses a CL_Sprite as the source for the font
//- glyphs (letters), where each frame frame in the sprite
//- represents one glyph. A string (<i>letter_chars</i>) is then
//- describing which character each glyph corresponds to. If the
//- sprite contains the letters ABCZXY123 in that order, then the
//- string should be "ABCZXY123". A system font uses the underlaying windowing system to create
//- the font glyphs. This means that in Windows you can choose any
//- TTF font, and same applies to X11 if the font server supports
//- it.</p>
//- <p>Newlines always have a width of zero.
//- Other than that, characters for which glyphs weren't specified have the width of a space.</p>
//- <p>Unlike CL_Surface and CL_Sprite, scaling affects the calculation
//- of any bounding rectangles (such as the result returned
//- by draw(), bounding_rect(), or get_size(), or the rectangle calculated internally by
//- draw() for alignment). This is because scaling
//- the CL_Font is effectively just changing the point size of the glyphs,
//- and that affects all sorts of things, such as word wrapping.</p>
//- <p>Word wrapping works automatically whenever you pass CL_Font a destination rectangle
//- or size with non-zero width. CL_Font uses the delims string (which can be changed
//- using the set_delims() method)
//- to determine where divisions between words are. Word wrapping does allow
//- blank characters (characters for which there isn't a glyph supplied)
//- to extend over the border line; this helps wrapped text to remain flush.</p>
class CL_API_DISPLAY CL_Font
{
//! Construction:
public:
	//: Constructs a font.
	//- <p>If spacelen is unspecified, CL_Font attempts to resolve
	//- it itself. First, it looks to see if you have specified a
	//- space character in letter_chars: if you have, it uses that
	//- glyph's width as the space width (as well as using it
	//- to actually draw the space). If it does not find a space
	//- glyph, then it will look at every glyph's width and make
	//- the space width the average of that.</p>
	//param resource_id: Font resource name.
	//param manager: Resource manager used to load font.
	//param glyphs: CL_Sprite containing the letters of a bitmap font.
	//param letters: String mapping each frame of the bitmap font sprite to letters.
	//param spacelen: Width in pixels of the space glyph. If -1, uses the average of all glyph widths.
	//param monospace: If true, treats all glyphs as being the same width.
	//param font_name: System font name (eg. "Arial").
	//param letter_chars: The characters to include in the font (if a sprite font, must be in frame order).
	//param height: Height of font in pixels.
	//param width: Width of font in pixels. If 0, uses best fitting width for the specified height.
	//param bold: If true, will use bold font.
	//param italic: If true, will use italic font.
	//param underline: If true, will use underlined font.
	//param strikeout: If true, will use striked out font.
	CL_Font();
	
	CL_Font(const CL_Font &copy);
	
	CL_Font(
		const std::string &resource_id,
		CL_ResourceManager *manager);
	
	CL_Font(
		const CL_Sprite &glyphs,
		const std::string &letters,
		int spacelen = -1,
		bool monospace = false);
	
	CL_Font(
		const std::string &font_name,
		const std::string &letters,
		int height,
		int width = 0,
		bool bold = false,
		bool italic = false,
		bool underline = false,
		bool strikeout = false);
	
	CL_Font(
		const std::string &font_name,
		int height,
		int width = 0,
		bool bold = false,
		bool italic = false,
		bool underline = false,
		bool strikeout = false);
	
	virtual ~CL_Font();

//! Attributes:
public:
	//: Returns delimiters string.
	//- <p> This string contains characters (other than newline) that divide words apart.
	//- Do not include newline in this string, it's an implicit delimiter.</p>
	std::string get_delims() const;
	
	//: Returns width offset.
	//- <p> The width offset can be used to kern glyphs together or spread them apart. </p> 
	int get_width_offset() const;
	
	//: Returns height offset.
	//- <p> The height offset can be used to create space between lines, or to merge them together. </p> 
	int get_height_offset() const;
	
	//: Returns current scale.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void get_scale(float &x, float &y) const;

	//: Returns current alpha.
	//- <p> 0.0f is full transparency, and 1.0f is full visibility. </p>
	float get_alpha() const;

	//: Returns current color.
	//- <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>
	void get_color(float &red, float &green, float &blue, float &alpha) const;
	CL_Color get_color() const;

	//: Returns blending functions.
	void get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const;
	
	//: Returns glyph rotation hotspot.
	//- <p> This is for the optional angle parameter to draw_glyphs(). </p>
	void get_glyph_rot_hotspot(CL_Origin &origin, int &x, int &y) const;
	
	//: Returns translation hotspot.
	void get_alignment(CL_Origin &origin, int &x, int &y) const;
	
	//: Returns the drawn height of the entire font or a string.
	//return: The height in pixels.
	//param str: String to get the height of.
	//param start: A starting iterator, inclusive.
	//param end: An ending iterator, exclusive.
	//param max_size: Same effect as the size of the dest rectangle passed to draw(), for word wrapping and height truncating.
	//- <p> The height of the entire font is the height of the tallest glyph in the font; this
	//- is what is returned if you specify no arguments. The point of having the height functions
	//- accept a string is to compensate for strings spanning multiple lines. Generally, get_height(" ") will return zero, since usually 
	//- the space glyph isn't given in the font, and so it has a height and width of zero. </p>
	int get_height() const;
	
	int get_height(
		const std::string &str,
		CL_Size max_size = CL_Size(0,0)) const;
	
	int get_height(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_Size max_size = CL_Size(0,0)) const;
	
	//: Returns the drawn width of a character or string.
	//return: The width in pixels.
	//param letter: Character to get the width of.
	//param str: String to get the width of.
	//param start: A starting iterator, inclusive.
	//param end: An ending iterator, exclusive.
	//param max_size: Same effect as the size of the dest rectangle passed to draw(), for word wrapping and height truncating.
	int get_width(unsigned int letter) const;
	
	int get_width(
		const std::string &str,
		CL_Size max_size = CL_Size(0,0)) const;
	
	int get_width(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_Size max_size = CL_Size(0,0)) const;
	
	//: Returns the drawn size of a string.
	//return: The size in pixels.
	//param letter: Character to get the size of. If unrecognized, returns space width.
	//param str: String to get the size of.
	//param start: A starting iterator, inclusive.
	//param end: An ending iterator, exclusive.
	//param max_size: Same effect as the size of the dest rectangle passed to draw(), for word wrapping and height truncating.
	CL_Size get_size(unsigned int letter) const;
	
	CL_Size get_size(
		const std::string &str,
		CL_Size max_size = CL_Size(0,0)) const;
	
	CL_Size get_size(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_Size max_size = CL_Size(0,0)) const;
	
	//: Calculate the rectangle that would be occupied by a draw operation.
	//param str: The input string to process.
	//param start: String position to begin processing at, inclusive.
	//param end: String position to end processing at, exclusive.
	//param x, y: Anchor position to simulate draw at. Actual position depends on the alignment mode.
	//param dest: Rectangle to draw text in. The text will be word-wrapped against delimiters to fit inside the rectangle.
	//- <p> You can specify a dest rectangle with a width or height of zero or less to disable word wrapping
	//- or height truncating, respectively. </p>
	CL_Rect bounding_rect(int x, int y, const std::string &str) const;
	
	CL_Rect bounding_rect(CL_Rect dest, const std::string &str) const;
	
	CL_Rect bounding_rect(
		int x, int y,
		std::string::const_iterator start,
		std::string::const_iterator end) const;
	
	CL_Rect bounding_rect(
		CL_Rect dest,
		std::string::const_iterator start,
		std::string::const_iterator end) const;
	
	//: Returns whether or not a glyph exists for a given character
	//- <p> This is the same as checking if get_width(chr) returns zero. </p>
	bool is_glyph(unsigned int chr) const;
	
	//: Resource owning this font, if any.
	CL_Resource resource;
	
//! Operations:
public:
	//: Copy assignment operator.
	CL_Font &operator =(const CL_Font &copy);
	
	//: Return true if the CL_Font object is valid
	operator bool() const;

	//: Draws text to a graphic context.
	//return: The number of glyphs that were drawn.
	//param str: The input string to draw.
	//param start: String position to begin drawing at, inclusive.
	//param end: String position to end drawing at, exclusive.
	//param x, y: Anchor position to draw at. Actual drawing position depends on the alignment mode.
	//param gc: Graphic context on which to draw. If null, will use CL_Display's current graphic context.
	//param dest: Rectangle to draw text in. The text will be word-wrapped against delimiters to fit inside the rectangle.
	//- <p> You can specify a dest rectangle with a width or height of zero or less to disable word wrapping
	//- or height truncating, respectively. </p>
	int draw(
		int x,
		int y,
		const std::string &str,
		CL_GraphicContext *context = 0) const;
	
	int draw(
		CL_Rect dest,
		const std::string &str,
		CL_GraphicContext *context = 0) const;
	
	int draw(
		int x,
		int y,
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_GraphicContext *context = 0) const;
	
	int draw(
		CL_Rect dest,
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_GraphicContext *context = 0) const;
	
	//: Inserts data into a CL_GlyphBuffer, treating the glyphs already there as part of a previous draw_to_gb().
	//return: The number of glyphs that were inserted. This could be negative if it backtracks.
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
	//- <p> Each glyph inserted into the CL_GlyphBuffer corresponds with exactly one character of the input string, with
	//- the order in the vector and the order in the string being the same. That is, if you start out with an empty CL_GlyphBuffer,
	//- run this method on it, and afterwords the CL_GlyphBuffer contains 100 glyphs, then you know the first 100 characters
	//- of the source string range were processed. This includes whitespace and newline characters.</p>
	//- <p> If you call this method on a CL_GlyphBuffer with a last line that is center-justified or left-justified, be aware
	//- that there's no way for this function to differentiate that line from a left-justified line with an indent, and it
	//- always assumes the latter. </p>
	//- <p> You can tell when you've filled the buffer up to the height in max_size when a call to this method returns
	//- anything less than the size of the string. There's also the chance that it will backtrack and return
	//- a negative value if it runs out
	//- of vertical space in the process of wrapping a just-completed word; this is still a sign that you've
	//- ran out of vertical space.</p>
	int draw_to_gb(
		const std::string &str,
		CL_GlyphBuffer &gb,
		CL_Size max_size = CL_Size(0,0)) const;

	int draw_to_gb(
		std::string::const_iterator start,
		std::string::const_iterator end,
		CL_GlyphBuffer &gb,
		CL_Size max_size = CL_Size(0,0)) const;
	
	//: Draws a single glyph to a given spot.
	//param x, y: The upper-left coordinates of where to draw the glyph.
	//param chr: The character to draw the glyph of.
	//param ang: The amount to rotate the glyph by. The hotspot is set with CL_Font::set_glyph_rot_hotspot().
	//param gc: Graphic context to render to. If null, will render to the CL_Display currently set window.
	//- <p> If there is no glyph for the given index, then nothing is drawn. </p>
	void draw_glyph(int x, int y, unsigned int glyph, float ang = 0.0, CL_GraphicContext *gc = 0);
	
	//: Draws a single character to a given spot.
	//param x, y: The upper-left coordinates of where to draw the character.
	//param chr: The character to draw the glyph of.
	//param ang: The amount to rotate the glyph by. The hotspot is set with CL_Font::set_glyph_rot_hotspot().
	//param gc: Graphic context to render to. If null, will render to the CL_Display currently set window.
	//- <p> If there is no glyph for the given character, then nothing is drawn. </p>
	void draw_character(int x, int y, unsigned int chr, float ang = 0.0, CL_GraphicContext *gc = 0);
	
	//: Sets delimiters string.
	//- <p> This string contains characters (other than newline) that divide words apart.
	//- Do not include newline in this string, it's an implicit delimiter.</p>
	void set_delims(const std::string &delims);
	
	//: Sets width offset.
	//- <p> The width offset can be used to kern glyphs together or spread them apart. </p> 
	void set_width_offset(int offset);
	
	//: Sets height offset.
	//- <p> The height offset can be used to create space between lines, or to merge them together. </p> 
	void set_height_offset(int offset);
	
	//: Sets scale for x and y directions individually.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void set_scale(float x, float y);
	
	//: Sets transparency.
	//- <p> 0.0f is full transparency, and 1.0f is full visibility. </p>
	void set_alpha(float alpha);
	
	//: Sets the color.
	//- <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>
	void set_color(float r, float g, float b, float a = 1.0f);
	void set_color(const CL_Color& c);
	
	//: Sets blending functions.
	void set_blend_func(CL_BlendFunc src, CL_BlendFunc dest);
	
	//: Sets glyph rotation hotspot.
	//- <p> This is for the optional angle parameter to draw_glyphs(). </p>
	void set_glyph_rot_hotspot(CL_Origin origin, int x = 0, int y = 0);
	
	//: Sets translation hotspot.
	void set_alignment(CL_Origin origin, int x = 0, int y = 0);
	
	//  Get the sprite glyphs
	CL_Sprite& get_glyphs();
	
//! Implementation:
private:
	CL_LazyCopyPtr<CL_Clonable, CL_Font_Generic> impl;
};

#endif
