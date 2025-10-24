/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Mark Page
*/

#pragma once


#include "API/Display/TargetProviders/font_provider.h"
#include "font_cache_native.h"

class CL_FreetypeFont;
class CL_GraphicContextProvider;
class CL_FontPixelBuffer;
class CL_Colorf;

class CL_FontProvider_Win32 : public CL_FontProvider
{
/// \name Construction
/// \{

public:
	CL_FontProvider_Win32();

	virtual ~CL_FontProvider_Win32();


/// \}
/// \name Attributes
/// \{

public:
	HFONT get_handle();

	CL_StringRef get_unique_name() const { return unique_name; }

	CL_FontMetrics get_font_metrics(CL_GraphicContext &gc);


/// \}
/// \name Operations
/// \{

public:
	void load_font( CL_GraphicContext &gc, const CL_StringRef &typeface_name, int height);

	void load_font( CL_GraphicContext &gc, const CL_FontDescription &desc);

	void destroy();

	void draw_text(CL_GraphicContext &gc, int x, int y, const CL_StringRef &text, const CL_Colorf &color);

	CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	CL_FontPixelBuffer get_font_glyph(CL_GraphicContextProvider *gc_provider, int glyph, bool anti_alias, const CL_Colorf &color);

	/// \brief Registers a font for lookup when creating fonts.
	static void register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface);

public:
	// Contains the font height. This is required as windows fonts with negative heights report
	// a height different from create() height
	int size_height;

	// Contains the anti alias setting
	bool anti_alias;

	// Contains the underline setting
	bool underlined_font;

	// Contains the strikeout setting
	bool strikeout_font;

	int otms_underscore_position;
	int otms_underscore_size;
	int otms_strikeout_size;
	int otms_strikeout_position;


/// \}
/// \name Implementation
/// \{

private:
	FontGlyphCache *font_cache_load(CL_GraphicContextProvider *gc_provider, const CL_StringRef &text);

	CL_String unique_name;
	FontGlyphCache *glyph_cache;
	CL_GraphicContextProvider *target;
	HFONT handle;
/// \}
};



