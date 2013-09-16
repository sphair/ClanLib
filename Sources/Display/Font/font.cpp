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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Resources/display_cache.h"
#include "font_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font Construction:

Font::Font()
{
}

Font::Font(Canvas &canvas, const std::string &typeface_name, int height)
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = Font(canvas, desc);
}

Font::Font( Canvas &canvas, const FontDescription &desc)
{
	*this = Font(canvas, desc, "");
}

Font::Font( Canvas &canvas, const FontDescription &desc, const std::string &ttf_filename) : impl(new Font_Impl)
{
	impl->load_font( canvas, desc, ttf_filename);
}

Font::Font( Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics) : impl(new Font_Impl)
{
	impl->load_font(canvas, sprite, glyph_list, spacelen, monospace, metrics);
}

/////////////////////////////////////////////////////////////////////////////
// Font Resources:

Resource<Font> Font::resource(Canvas &canvas, const FontDescription &desc, const ResourceManager &resources)
{
	return DisplayCache::get(resources).get_font(canvas, desc);
}


Font Font_Impl::load(Canvas &canvas, const FontDescription &reference_desc, const std::string &id, const XMLResourceDocument &doc, Callback_2<Resource<Sprite>, GraphicContext &, const std::string &> cb_get_sprite)
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

		font_metrics.set_weight(400.0f);
		if (sprite_element.has_attribute("weight")) 
			font_metrics.set_weight(StringHelp::text_to_float(sprite_element.get_attribute("weight", "0")));

		if (sprite_element.has_attribute("overhang")) 
			font_metrics.set_overhang(StringHelp::text_to_float(sprite_element.get_attribute("overhang", "0")));

		font_metrics.set_digitized_aspect_x(96.0f);
		if (sprite_element.has_attribute("digitized_aspect_x")) 
			font_metrics.set_digitized_aspect_x(StringHelp::text_to_float(sprite_element.get_attribute("digitized_aspect_x", "0")));

		font_metrics.set_digitized_aspect_y(96.0f);
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

		return Font(canvas, spr_glyphs.get(), letters, spacelen, monospace, font_metrics);
	}

	DomElement ttf_element = resource.get_element().named_item("ttf").to_element();
	if (ttf_element.is_null())
		ttf_element = resource.get_element().named_item("freetype").to_element();

	if (!ttf_element.is_null())
	{
		FontDescription desc = reference_desc.clone();

		std::string filename;

		if (ttf_element.has_attribute("file"))
			filename = ttf_element.get_attribute("file"); desc.set_typeface_name(ttf_element.get_attribute("file"));

		if (ttf_element.has_attribute("typeface"))
			desc.set_typeface_name(ttf_element.get_attribute("typeface"));
		else
			throw Exception(string_format("Font resource '%1' has no 'typeface' attribute", resource.get_name()));

		if (ttf_element.has_attribute("height"))
			desc.set_height(ttf_element.get_attribute_int("height", 0));

		if (ttf_element.has_attribute("average_width"))
			desc.set_average_width(ttf_element.get_attribute_int("average_width", 0));

		if (ttf_element.has_attribute("anti_alias"))
			desc.set_anti_alias(ttf_element.get_attribute_bool("anti_alias", true));

		if (ttf_element.has_attribute("subpixel"))
			desc.set_subpixel(ttf_element.get_attribute_bool("subpixel", true));

		return Font(canvas, desc, filename);
	}

	throw Exception(string_format("Font resource %1 did not have a <sprite> or <ttf> child element", resource.get_name()));

}


/////////////////////////////////////////////////////////////////////////////
// Font Attributes:

bool Font::is_null() const
{
	return !impl;
}

/////////////////////////////////////////////////////////////////////////////
// Font Operations:

void Font::draw_text(Canvas &canvas, float dest_x, float dest_y, const std::string &text, const Colorf &color)
{
	if (impl)
	{
		FontMetrics fm = get_font_metrics();
		int line_spacing = fm.get_height() + fm.get_external_leading();
		std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
		for (std::vector<std::string>::size_type i=0; i<lines.size(); i++)
		{
			impl->draw_text(canvas, dest_x, dest_y, lines[i], color);
			dest_y += line_spacing;
		}
	}
}

void Font::draw_text(Canvas &canvas, int dest_x, int dest_y, const std::string &text, const Colorf &color)
{
	draw_text(canvas, (float) dest_x, (float) dest_y, text, color);
}

void Font::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color)
{
	draw_text(canvas, position.x, position.y, text, color);
}

void Font::draw_text_ellipsis(Canvas &canvas, float dest_x, float dest_y, Rectf content_box, const std::string &text, const Colorf &color)
{
	if (impl)
	{
		GraphicContext &gc = canvas.get_gc();
		FontMetrics fm = get_font_metrics();
		int ascent = fm.get_ascent();
		int descent = fm.get_descent();
		int line_spacing = fm.get_height() + fm.get_external_leading();
		std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
		for (std::vector<std::string>::size_type i=0; i<lines.size(); i++)
		{
			if (i == 0 || (dest_y - ascent >= content_box.top && dest_y + descent < content_box.bottom))
			{
				Size size = get_text_size(gc, lines[i]);
				if (dest_x + size.width <= content_box.right)
				{
					draw_text(canvas, dest_x, dest_y, lines[i], color);
				}
				else
				{
					Size ellipsis = get_text_size(gc, "...");

					int seek_start = 0;
					int seek_end = lines[i].size();
					int seek_center = (seek_start + seek_end) / 2;

					UTF8_Reader utf8_reader(lines[i].data(), lines[i].length());
					while (true)
					{
						utf8_reader.set_position(seek_center);
						utf8_reader.move_to_leadbyte();
						if (seek_center != utf8_reader.get_position())
							utf8_reader.next();
						seek_center = utf8_reader.get_position();

						utf8_reader.set_position(seek_start);
						utf8_reader.next();
						if (utf8_reader.get_position() == seek_end)
							break;

						Size text_size = get_text_size(gc, lines[i].substr(0, seek_center));

						if (dest_x + text_size.width + ellipsis.width >= content_box.right)
							seek_end = seek_center;
						else
							seek_start = seek_center;
						seek_center = (seek_start+seek_end)/2;
					}

					draw_text(canvas, dest_x, dest_y, lines[i].substr(0, seek_center) + "...", color);
				}

				dest_y += line_spacing;
			}
		}
	}
}

void Font::draw_text_ellipsis(Canvas &canvas, int dest_x, int dest_y, Rect content_box, const std::string &text, const Colorf &color)
{
	draw_text_ellipsis(canvas, (float) dest_x, (float) dest_y, Rectf(content_box.left, content_box.top, content_box.right, content_box.bottom), text, color);
}

void Font::draw_text_ellipsis(Canvas &canvas, const Pointf &position, Rectf content_box, const std::string &text, const Colorf &color)
{
	draw_text_ellipsis(canvas, position.x, position.y, content_box, text, color);
}

Size Font::get_text_size(GraphicContext &gc, const std::string &text)
{
	Size total_size;

	if (impl)
	{
		FontMetrics fm = get_font_metrics();
		int line_spacing = fm.get_external_leading();
		std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
		for (std::vector<std::string>::size_type i=0; i<lines.size(); i++)
		{
			Size line_size = impl->get_text_size(gc, lines[i]);

			if ((line_size.width == 0) && (line_size.height == 0) && (lines.size() > 1)) // blank line
				line_size.height = fm.get_descent() + fm.get_ascent(); 

			if ((i+1) != lines.size())	// Do not add the line spacing on the last line
				line_size.height += line_spacing;

			if (total_size.width < line_size.width)	// Find the widest line
				total_size.width = line_size.width;

			total_size.height += line_size.height;
		}
	}
	
	return total_size;
}

Size Font::get_glyph_size(GraphicContext &gc, unsigned int glyph)
{
	std::string text = StringHelp::unicode_to_utf8(glyph);

	if (impl)
	{
		return impl->get_text_size(gc, text);
	}
	return Size();
}

FontMetrics Font::get_font_metrics()
{
	if (impl)
		return impl->get_font_metrics();
	return FontMetrics();
}

int Font::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	if (impl)
		return impl->get_character_index(gc, text, point);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Font Implementation:


}
