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

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "font.h"

class CL_FontProvider;
class CL_FontProvider_Win32;
class CL_FontProvider_Freetype;

/// \brief Native Font class.
///
/// This is a CL_PixelBuffer based font, using the native OS Font
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Font_Native : public CL_Font
{
/// \name Construction
/// \{

public:
	CL_Font_Native( CL_GraphicContext context, const CL_StringRef &typeface_name, int height);

	CL_Font_Native( CL_GraphicContext context, const CL_FontDescription &desc);

	CL_Font_Native();

	~CL_Font_Native();

/// \}
/// \name Attributes
/// \{

public:

#ifdef WIN32
	/// \brief Retrieves the font provider.
	CL_FontProvider_Win32 *get_provider();
#else
	/// \brief Retrieves the font provider.
	CL_FontProvider_Freetype *get_provider();
#endif

/// \}
/// \name Operations
/// \{
public:

	/// \brief Registers a font for lookup when creating fonts.
	static void register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
