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

#include "Display/precomp.h"
#include "API/Display/Font/font_native.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

#ifdef WIN32
#include "font_provider_win32.h"
#endif

#ifndef WIN32
#include "font_provider_freetype.h"
#include "Display/X11/font_config.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Font Construction:

CL_Font_Native::CL_Font_Native()
{
}

#ifdef WIN32
CL_Font_Native::CL_Font_Native(
	CL_GraphicContext context, const CL_StringRef &typeface_name, int height) : CL_Font( new CL_FontProvider_Win32())
{
	get_provider()->load_font(context, typeface_name, height);
}

CL_Font_Native::CL_Font_Native(
	CL_GraphicContext context, const CL_FontDescription &desc) : CL_Font( new CL_FontProvider_Win32())
{
	get_provider()->load_font(context, desc);
}

#else
CL_Font_Native::CL_Font_Native(
	CL_GraphicContext context, const CL_StringRef &typeface_name, int height) : CL_Font( new CL_FontProvider_Freetype())
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);

	CL_FontDescription new_desc = CL_FontProvider_Freetype::get_registered_font(desc, true);

	get_provider()->load_font(new_desc);

}

CL_Font_Native::CL_Font_Native(
	CL_GraphicContext context, const CL_FontDescription &desc) : CL_Font( new CL_FontProvider_Freetype())
{
	CL_FontDescription new_desc = CL_FontProvider_Freetype::get_registered_font(desc, true);
	get_provider()->load_font(new_desc);
}
#endif

CL_Font_Native::~CL_Font_Native()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Native Attributes:

#ifdef WIN32
CL_FontProvider_Win32 *CL_Font_Native::get_provider()
{
	return static_cast <CL_FontProvider_Win32 *> (CL_Font::get_provider());
}
#else
CL_FontProvider_Freetype *CL_Font_Native::get_provider()
{
	return static_cast <CL_FontProvider_Freetype *> (CL_Font::get_provider());
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Native Operations:

void CL_Font_Native::register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface)
{
#ifdef WIN32
	CL_FontProvider_Win32::register_font(font_filename, font_typeface);
#else
	CL_FontProvider_Freetype::register_font(font_filename, font_typeface);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Native Implementation:
