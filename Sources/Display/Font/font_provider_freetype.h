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
**    Mark Page
*/


#pragma once


#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/Font/font_freetype.h"
#include "API/Display/Font/font_metrics.h"

#include "font_cache_native.h"

#include <list>

class CL_FreetypeFont;
class CL_GraphicContextProvider;
class CL_FontPixelBuffer;
class CL_Colorf;

class CL_FontProvider_Freetype : public CL_FontProvider
{
/// \name Construction
/// \{

public:

	CL_FontProvider_Freetype();

	virtual ~CL_FontProvider_Freetype();


/// \}
/// \name Attributes
/// \{

public:

	CL_FreetypeFont *get_handle();

	CL_StringRef get_unique_name() const { return unique_name; }

	/// \brief Returns information about the current font.
	virtual CL_FontMetrics get_font_metrics(CL_GraphicContext &gc);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Get the registered font or the best font match if not found
	///
	/// \param desc = Description
	/// \param font_size_win32 = Input font sizes are win32 font sizes, and should be transformed to freetype font sizes
	///
	/// \return Updated description
	static CL_FontDescription get_registered_font(const CL_FontDescription &desc, bool font_size_win32);

	void load_font(const CL_StringRef &typeface_name, int height);

	void load_font(const CL_FontDescription &desc);

	/// \brief Destroys the font provider.
	virtual void destroy();

	/// \brief Print text on gc.
	virtual void draw_text(CL_GraphicContext &gc, int x, int y, const CL_StringRef &text, const CL_Colorf &color);

	/// \brief Calculate size of text string.
	virtual CL_Size get_text_size(CL_GraphicContext &gc, const CL_StringRef &text);

	CL_FontPixelBuffer get_font_glyph(CL_GraphicContextProvider *gc_provider, int glyph, bool anti_alias, const CL_Colorf &color);

	/// \brief Registers a font for lookup when creating fonts.
	static void register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface);

/// \}
/// \name Implementation
/// \{

private:
	FontGlyphCache *font_cache_load(CL_GraphicContextProvider *gc_provider, const CL_StringRef &text);

	// Contains the anti alias setting
	bool anti_alias;

	CL_String unique_name;

	CL_GraphicContextProvider *target;

	CL_FreetypeFont *handle;

	int size_height;

	static std::map<CL_String /*font_typeface*/, CL_String /*font_filename*/ > font_register_cache;

/// \}
};



