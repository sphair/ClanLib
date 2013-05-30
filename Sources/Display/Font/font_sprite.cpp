/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Display/Font/font_metrics.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "font_provider_sprite.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/sprite.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font Construction:

Font_Sprite::Font_Sprite()
{
}

Font_Sprite::Font_Sprite( Sprite sprite, const std::string &letters, int spacelen, bool monospace, FontMetrics metrics) : Font(new FontProvider_Sprite())
{
	get_provider()->load_font(sprite, letters, spacelen, monospace, metrics);
}

Font_Sprite::Font_Sprite( const Font &font) : Font(font)
{
	if (!get_provider())
	{
		throw Exception("Font is not of type Font_Sprite");
	}
}

Font_Sprite::~Font_Sprite()
{
}

/////////////////////////////////////////////////////////////////////////////
// Font_Sprite Attributes:

FontProvider_Sprite *Font_Sprite::get_provider() const
{
	return static_cast <FontProvider_Sprite *> (Font::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// Font_Sprite Operations:

void Font_Sprite::set_font_metrics(const FontMetrics &metrics)
{
	get_provider()->set_font_metrics(metrics);
}

void Font_Sprite::draw_text(Canvas &canvas, float xpos, float ypos, float scale_x, float scale_y, const std::string &text, const Colorf &color)
{
	FontMetrics fm = get_font_metrics();
	float line_spacing = scale_y * ((float) (fm.get_height() + fm.get_external_leading()));
	std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
	for (std::vector<std::string>::size_type i=0; i<lines.size(); i++)
	{
		get_provider()->draw_text(canvas, xpos, ypos, scale_x, scale_y, lines[i], color);
		ypos += line_spacing;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Font_Sprite Implementation:

}
