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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Font/font_freetype.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_freetype.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Font Construction:

CL_Font_Freetype::CL_Font_Freetype()
{
}

CL_Font_Freetype::CL_Font_Freetype(CL_GraphicContext &gc,
	const CL_StringRef &typeface_name, int height) : CL_Font( new CL_FontProvider_Freetype(gc))
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc);
}

CL_Font_Freetype::CL_Font_Freetype(CL_GraphicContext &gc,
	const CL_StringRef &typeface_name, int height, CL_IODevice &file) : CL_Font( new CL_FontProvider_Freetype(gc))
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, file);
}

CL_Font_Freetype::CL_Font_Freetype(CL_GraphicContext &gc,
	const CL_StringRef &typeface_name, int height, const CL_VirtualDirectory &directory) : CL_Font( new CL_FontProvider_Freetype(gc))
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	get_provider()->load_font(desc, directory);
}

CL_Font_Freetype::CL_Font_Freetype(CL_GraphicContext &gc,
	const CL_FontDescription &desc) : CL_Font( new CL_FontProvider_Freetype(gc))
{
	get_provider()->load_font(desc);
}

CL_Font_Freetype::CL_Font_Freetype(CL_GraphicContext &gc,
	const CL_FontDescription &desc, CL_IODevice &file) : CL_Font( new CL_FontProvider_Freetype(gc))
{
	get_provider()->load_font(desc, file);
}

CL_Font_Freetype::CL_Font_Freetype(CL_GraphicContext &gc,
	const CL_FontDescription &desc, const CL_VirtualDirectory &directory) : CL_Font( new CL_FontProvider_Freetype(gc))
{
	get_provider()->load_font(desc, directory);
}

CL_Font_Freetype::~CL_Font_Freetype()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Freetype Attributes:

CL_FontProvider_Freetype *CL_Font_Freetype::get_provider() const
{
	return static_cast <CL_FontProvider_Freetype *> (CL_Font::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Freetype Operations:

void CL_Font_Freetype::set_texture_group(CL_TextureGroup &new_texture_group)
{
	get_provider()->set_texture_group(new_texture_group);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Freetype Implementation:

