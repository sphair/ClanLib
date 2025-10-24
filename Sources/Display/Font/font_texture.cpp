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
#include "API/Display/2D/subtexture.h"
#include "API/Display/Font/font_texture.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_texture.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Font Construction:

CL_Font_Texture::CL_Font_Texture()
{
}

CL_Font_Texture::CL_Font_Texture(
	CL_GraphicContext context, const CL_StringRef &typeface_name, int height) : CL_Font( new CL_FontProvider_Texture(context))
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	load_font(context, desc);
}

CL_Font_Texture::CL_Font_Texture(
	CL_GraphicContext context, const CL_FontDescription &desc) : CL_Font( new CL_FontProvider_Texture(context))
{
	load_font(context, desc);
}


CL_Font_Texture::CL_Font_Texture( CL_GraphicContext context, CL_PixelBuffer *pixel_buffer, CL_Font_Texture_Position *positions, int num_positions ) : CL_Font(new CL_FontProvider_Texture(context))
{
	if ( (pixel_buffer != NULL) && (positions != NULL) )
	{
		while(positions->glyph != 0)	// Until end of list
		{
			if (num_positions != -1)	// List determined by a counter
			{
				if (num_positions == 0) break;	// All done
				num_positions--;
			}
			insert_glyph(context, *positions, *pixel_buffer);
			positions++;			
		}
	}
}


CL_Font_Texture::~CL_Font_Texture()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Texture Attributes:

CL_FontProvider_Texture *CL_Font_Texture::get_provider()
{
	return static_cast <CL_FontProvider_Texture *> (CL_Font::get_provider());
}

CL_Font_Texture_Glyph *CL_Font_Texture::get_glyph(const CL_GraphicContext &gc, int glyph)
{
	return (get_provider()->get_glyph(gc, glyph));
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Texture Operations:

void CL_Font_Texture::set_texture_group(CL_TextureGroup &new_texture_group)
{
	get_provider()->set_texture_group(new_texture_group);
}

void CL_Font_Texture::load_font( CL_GraphicContext &context, const CL_FontDescription &desc)
{
	get_provider()->load_font(context, desc);
}

void CL_Font_Texture::free_font()
{
	get_provider()->free_font();
}

void CL_Font_Texture::insert_glyph(const CL_GraphicContext &gc, CL_Font_Texture_Position &position, CL_PixelBuffer &pixel_buffer)
{
	get_provider()->insert_glyph(gc, position, pixel_buffer);
}

void CL_Font_Texture::insert_glyph(const CL_GraphicContext &gc, const CL_StringRef &text)
{
	get_provider()->insert_glyph(gc, text);
}

void CL_Font_Texture::set_font_metrics(const CL_FontMetrics &metrics)
{
	get_provider()->set_font_metrics(metrics);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Texture Implementation:

