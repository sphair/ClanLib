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
#include "API/Display/Font/font_sprite.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_sprite.h"
#include "API/Core/Text/string_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Font Construction:

CL_Font_Sprite::CL_Font_Sprite()
{
}

CL_Font_Sprite::CL_Font_Sprite( CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources ) : CL_Font(new CL_FontProvider_Sprite(gc, resource_id, resources))
{
}

CL_Font_Sprite::~CL_Font_Sprite()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Sprite Attributes:

CL_FontProvider_Sprite *CL_Font_Sprite::get_provider() const
{
	return static_cast <CL_FontProvider_Sprite *> (CL_Font::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Sprite Operations:

void CL_Font_Sprite::set_font_metrics(const CL_FontMetrics &metrics)
{
	get_provider()->set_font_metrics(metrics);
}

void CL_Font_Sprite::draw_text(CL_GraphicContext &gc, float xpos, float ypos, float scale_x, float scale_y, const CL_StringRef &text, const CL_Colorf &color)
{
	CL_FontMetrics fm = get_font_metrics(gc);
	float line_spacing = scale_y * ((float) (fm.get_height() + fm.get_external_leading()));
	std::vector<CL_String> lines = CL_StringHelp::split_text(text, "\n", false);
	for (std::vector<CL_String>::size_type i=0; i<lines.size(); i++)
	{
		get_provider()->draw_text(gc, xpos, ypos, scale_x, scale_y, lines[i], color);
		ypos += line_spacing;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Sprite Implementation:
