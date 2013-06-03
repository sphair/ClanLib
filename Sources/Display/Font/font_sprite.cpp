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
#include "API/Display/Resources/display_cache.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_format.h"

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
// Font Resources:

Font_Sprite Font_Sprite::load(Canvas &canvas, const std::string &id, const XMLResourceDocument &doc, Callback_2<Resource<Sprite>, GraphicContext &, const std::string &> cb_get_sprite)
{
	XMLResourceNode resource = doc.get_resource(id);
	std::string type = resource.get_element().get_tag_name();
	
	if (type != "font")
		throw Exception(string_format("Resource '%1' is not of type 'font'", id));

	DomElement sprite_element = resource.get_element().named_item("sprite").to_element();

	FontMetrics font_metrics;

	if (!sprite_element.is_null())
	{
		if (!sprite_element.has_attribute("glyphs")) 
			throw Exception(string_format("Font resource %1 has no 'glyphs' attribute.", resource.get_name()));
		
		if (!sprite_element.has_attribute("letters")) 
			throw Exception(string_format("Font resource %1 has no 'letters' attribute.", resource.get_name()));

		GraphicContext gc = canvas;
		Resource<Sprite> spr_glyphs = cb_get_sprite.invoke(gc, sprite_element.get_attribute("glyphs"));

		const std::string &letters = sprite_element.get_attribute("letters");

		int spacelen = StringHelp::text_to_int(sprite_element.get_attribute("spacelen", "-1"));
		bool monospace = StringHelp::text_to_bool(sprite_element.get_attribute("monospace", "false"));

		// Modify the default font metrics, if specified

		if (sprite_element.has_attribute("height")) 
			font_metrics.set_height(StringHelp::text_to_float(sprite_element.get_attribute("height", "0")));

		if (sprite_element.has_attribute("ascent")) 
			font_metrics.set_ascent(StringHelp::text_to_float(sprite_element.get_attribute("ascent", "0")));

		if (sprite_element.has_attribute("descent")) 
			font_metrics.set_descent(StringHelp::text_to_float(sprite_element.get_attribute("descent", "0")));

		if (sprite_element.has_attribute("internal_leading")) 
			font_metrics.set_internal_leading(StringHelp::text_to_float(sprite_element.get_attribute("internal_leading", "0")));

		if (sprite_element.has_attribute("external_leading")) 
			font_metrics.set_external_leading(StringHelp::text_to_float(sprite_element.get_attribute("external_leading", "0")));

		if (sprite_element.has_attribute("average_character_width")) 
			font_metrics.set_average_character_width(StringHelp::text_to_float(sprite_element.get_attribute("average_character_width", "0")));

		if (sprite_element.has_attribute("max_character_width")) 
			font_metrics.set_max_character_width(StringHelp::text_to_float(sprite_element.get_attribute("max_character_width", "0")));

		if (sprite_element.has_attribute("weight")) 
			font_metrics.set_weight(StringHelp::text_to_float(sprite_element.get_attribute("weight", "0")));

		if (sprite_element.has_attribute("overhang")) 
			font_metrics.set_overhang(StringHelp::text_to_float(sprite_element.get_attribute("overhang", "0")));

		if (sprite_element.has_attribute("digitized_aspect_x")) 
			font_metrics.set_digitized_aspect_x(StringHelp::text_to_float(sprite_element.get_attribute("digitized_aspect_x", "0")));

		if (sprite_element.has_attribute("digitized_aspect_y")) 
			font_metrics.set_digitized_aspect_y(StringHelp::text_to_float(sprite_element.get_attribute("digitized_aspect_y", "0")));

		if (sprite_element.has_attribute("italic")) 
			font_metrics.set_italic(StringHelp::text_to_bool(sprite_element.get_attribute("italic", "0")));

		if (sprite_element.has_attribute("underlined")) 
			font_metrics.set_underlined(StringHelp::text_to_bool(sprite_element.get_attribute("underlined", "0")));

		if (sprite_element.has_attribute("struck_out")) 
			font_metrics.set_struck_out(StringHelp::text_to_bool(sprite_element.get_attribute("struck_out", "0")));

		if (sprite_element.has_attribute("fixed_pitch")) 
			font_metrics.set_fixed_pitch(StringHelp::text_to_bool(sprite_element.get_attribute("fixed_pitch", "0")));

		return Font_Sprite(spr_glyphs.get(), letters, spacelen, monospace, font_metrics);
	}
	else
	{
		throw Exception(string_format("Font resource %1 did not have a <sprite> child element!", resource.get_name()));
	}
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
