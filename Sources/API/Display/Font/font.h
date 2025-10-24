/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../Render/graphic_context.h"
#include "../Image/pixel_buffer.h"
#include "font_description.h"

class CL_FontProvider;
class CL_Font_Impl;

/// \brief Font class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Font
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a font.
	CL_Font();

	// \brief Constructs standard font - Currently this defaults to CL_Font_System
	CL_Font( CL_GraphicContext &context, const CL_StringRef &typeface_name, int height);

	// \brief Constructs standard font - Currently this defaults to CL_Font_System
	CL_Font( CL_GraphicContext &context, const CL_FontDescription &desc);

	/// \brief Constructs a Font
	///
	/// \param provider = Font Provider
	CL_Font(CL_FontProvider *provider);

	/// \brief Constructs a Font
	///
	/// \param copy = Font
	CL_Font(const CL_Font &copy);

	virtual ~CL_Font();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Retrieves the font provider.
	CL_FontProvider *get_provider() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(CL_GraphicContext &gc, int x, int y, const CL_StringRef &text, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(CL_GraphicContext &gc, float x, float y, const CL_StringRef &text, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Print text on gc.
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param position = Dest position
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text(CL_GraphicContext &gc, const CL_Pointf &position, const CL_StringRef &text, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Print text on gc adding elipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(CL_GraphicContext &gc, int x, int y, CL_Rect content_box, const CL_StringRef &text, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Print text on gc adding elipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(CL_GraphicContext &gc, float x, float y, CL_Rectf content_box, const CL_StringRef &text, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Print text on gc adding elipses if it does not fit
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = Graphic Context
	/// \param position = Dest position
	/// \param content_box = Rectangle the text is allowed within
	/// \param text = The text to draw
	/// \param color = The text color
	void draw_text_ellipsis(CL_GraphicContext &gc, const CL_Pointf &position, CL_Rectf content_box, const CL_StringRef &text, const CL_Colorf &color = CL_Colorf::white);

	/// \brief Calculate size of text string.
	///
	/// Multiline text (seperated by /n) is supported\n
	/// \n
	/// Note: The height also includes whitespace (to give the maximum font height), so "." and "X" returns the same height.\n
	/// The width is the pixel width\n
	/// \n
	/// The size is the increment value to the next glyph
	CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	/// \brief Gets the size of a specified glyph
	///
	/// The size is the increment value to the next glyph
	///
	/// \param glyph = The glyph to get
	/// \return The size
	CL_Size get_glyph_size(CL_GraphicContext &gc, unsigned int glyph);

	/// \brief Retrieves font metrics description for the selected font.
	CL_FontMetrics get_font_metrics();

	/// \brief Get the character index at a specified point
	///
	/// Multiline text (seperated by /n) is supported
	///
	/// \param gc = The Graphic Context
	/// \param text = The string
	/// \param point = The point
	/// \return The character index. -1 = Not at specified point
	int get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Font_Impl> impl;
/// \}
};

/// \brief Font pixel buffer format (holds a pixel buffer containing a glyph)
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_FontPixelBuffer
{
public:
	CL_FontPixelBuffer() : glyph(0), empty_buffer(true), offset(0,0), increment(0,0) { };

	/// \brief Glyph this pixel buffer refers to. 0 = Glyph if not valid
	unsigned int glyph;

	/// \brief True when the pixel buffer is empty
	bool empty_buffer;

	/// \brief The pixel buffer containing the glyph
	CL_PixelBuffer buffer;

	/// \brief The rect containing the image inside "buffer"
	CL_Rect buffer_rect;

	/// \brief Offset to draw the font to buffer
	/** For example:
	    x = pos_x + pixelbuffer.offset.x
	    y = pos_y + pixelbuffer.offset.y*/
	CL_Point offset;

	/// \brief Increment to draw the next glyph
	/** For example:
	    pos_x += pixelbuffer.increment.x;
	    pos_y += pixelbuffer.increment.y;*/
	CL_Point increment;
};

/// \}

