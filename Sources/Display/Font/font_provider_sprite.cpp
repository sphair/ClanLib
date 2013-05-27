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
**    Mark Page
*/

#include "Display/precomp.h"
#include "font_provider_sprite.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/2D/shape2d.h"
#include "API/Display/2D/sprite_description.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Sprite Construction:

FontProvider_Sprite::FontProvider_Sprite()
{
}

void FontProvider_Sprite::load_font(GraphicContext &gc, const std::string &resource_id, const XMLResourceDocument &resources )
{

	XMLResourceNode resource = resources.get_resource(resource_id);
	std::string type = resource.get_element().get_tag_name();
	
	if (type != "font")
		throw Exception(string_format("Resource '%1' is not of type 'font'", resource_id));

	DomElement bitmap_element = resource.get_element().named_item("bitmap").to_element();

	if (!bitmap_element.is_null())
	{
		if (!bitmap_element.has_attribute("glyphs")) 
			throw Exception(string_format("Font resource %1 has no 'glyphs' attribute.", resource.get_name()));
		
		if (!bitmap_element.has_attribute("letters")) 
			throw Exception(string_format("Font resource %1 has no 'letters' attribute.", resource.get_name()));

		SpriteDescription sprite_description(gc, bitmap_element.get_attribute("glyphs"), resources);

		spr_glyphs = Sprite(gc, sprite_description);

		const std::string &letters = bitmap_element.get_attribute("letters");

		int spacelen = StringHelp::text_to_int(bitmap_element.get_attribute("spacelen", "-1"));
		bool monospace = StringHelp::text_to_bool(bitmap_element.get_attribute("monospace", "false"));

		setup_glyphs(gc, letters, spacelen, monospace);

		// Modify the default font metrics, if specified

		if (bitmap_element.has_attribute("height")) 
			font_metrics.set_height(StringHelp::text_to_float(bitmap_element.get_attribute("height", "0")));

		if (bitmap_element.has_attribute("ascent")) 
			font_metrics.set_ascent(StringHelp::text_to_float(bitmap_element.get_attribute("ascent", "0")));

		if (bitmap_element.has_attribute("descent")) 
			font_metrics.set_descent(StringHelp::text_to_float(bitmap_element.get_attribute("descent", "0")));

		if (bitmap_element.has_attribute("internal_leading")) 
			font_metrics.set_internal_leading(StringHelp::text_to_float(bitmap_element.get_attribute("internal_leading", "0")));

		if (bitmap_element.has_attribute("external_leading")) 
			font_metrics.set_external_leading(StringHelp::text_to_float(bitmap_element.get_attribute("external_leading", "0")));

		if (bitmap_element.has_attribute("average_character_width")) 
			font_metrics.set_average_character_width(StringHelp::text_to_float(bitmap_element.get_attribute("average_character_width", "0")));

		if (bitmap_element.has_attribute("max_character_width")) 
			font_metrics.set_max_character_width(StringHelp::text_to_float(bitmap_element.get_attribute("max_character_width", "0")));

		if (bitmap_element.has_attribute("weight")) 
			font_metrics.set_weight(StringHelp::text_to_float(bitmap_element.get_attribute("weight", "0")));

		if (bitmap_element.has_attribute("overhang")) 
			font_metrics.set_overhang(StringHelp::text_to_float(bitmap_element.get_attribute("overhang", "0")));

		if (bitmap_element.has_attribute("digitized_aspect_x")) 
			font_metrics.set_digitized_aspect_x(StringHelp::text_to_float(bitmap_element.get_attribute("digitized_aspect_x", "0")));

		if (bitmap_element.has_attribute("digitized_aspect_y")) 
			font_metrics.set_digitized_aspect_y(StringHelp::text_to_float(bitmap_element.get_attribute("digitized_aspect_y", "0")));

		if (bitmap_element.has_attribute("italic")) 
			font_metrics.set_italic(StringHelp::text_to_bool(bitmap_element.get_attribute("italic", "0")));

		if (bitmap_element.has_attribute("underlined")) 
			font_metrics.set_underlined(StringHelp::text_to_bool(bitmap_element.get_attribute("underlined", "0")));

		if (bitmap_element.has_attribute("struck_out")) 
			font_metrics.set_struck_out(StringHelp::text_to_bool(bitmap_element.get_attribute("struck_out", "0")));

		if (bitmap_element.has_attribute("fixed_pitch")) 
			font_metrics.set_fixed_pitch(StringHelp::text_to_bool(bitmap_element.get_attribute("fixed_pitch", "0")));

	}
	else
	{
		throw Exception(string_format("Font resource %1 did not have a <bitmap> child element!", resource.get_name()));
	}
}

FontProvider_Sprite::~FontProvider_Sprite()
{
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Sprite Attributes:

FontMetrics FontProvider_Sprite::get_font_metrics()
{
	return font_metrics;
}

FontMetrics FontProvider_Sprite::get_metrics()
{
	return font_metrics;
}

Size FontProvider_Sprite::get_text_size(GraphicContext &gc, const std::string &text)
{
	int width = 0;

	for (std::string::size_type p = 0; p < text.length(); p++)
	{
		Font_Sprite_Glyph *gptr = get_glyph(text[p]);

		int glyph_width;
		if (gptr)
		{
			glyph_width = spr_glyphs.get_frame_size(gptr->sprite_index).width;
		}
		else
		{
			glyph_width = spacelen;
		}

		width += glyph_width;
	}
	int current_height;
	if (width == 0)
	{
		current_height = 0;
	}
	else
	{
		current_height = height;
	}

	return (Size(width, current_height));
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Sprite Operations:

FontPixelBuffer FontProvider_Sprite::get_font_glyph_standard(int glyph, bool anti_alias)
{
	// Not supported
	return FontPixelBuffer();
}

FontPixelBuffer FontProvider_Sprite::get_font_glyph_subpixel(int glyph)
{
	// Not supported
	return FontPixelBuffer();
}


int FontProvider_Sprite::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	int dest_x = 0;
	int dest_y = 0;
	int character_counter = 0;

	FontMetrics fm = get_font_metrics();
	int font_height = fm.get_height();
	int font_external_leading = fm.get_external_leading();

	std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
	for (std::vector<std::string>::size_type i=0; i<lines.size(); i++)
	{
		int xpos = dest_x;
		int ypos = dest_y;

		std::string &textline = lines[i];
		std::string::size_type string_length = textline.length();

		// Scan the string
		for (std::string::size_type p = 0; p < string_length; p++)
		{
			Font_Sprite_Glyph *gptr = get_glyph(textline[p]);

			int glyph_width;
			if (gptr)
			{
				glyph_width = spr_glyphs.get_frame_size(gptr->sprite_index).width;
			}
			else
			{
				glyph_width = spacelen;
			}


			Rect position(xpos, ypos - font_height, Size(glyph_width, font_height + font_external_leading));
			if (position.contains(point))
				return ((int) p) + character_counter;

			xpos += glyph_width;
		}

		dest_y += font_height + font_external_leading;

		character_counter += string_length + 1;		// (Including the '\n')

	}
	return -1;	// Not found
}

void FontProvider_Sprite::draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color) 
{
	std::string::size_type string_length = text.length();
	float ascent = font_metrics.get_ascent();

	// Scan the string
	for (std::string::size_type p = 0; p < string_length; p++)
	{
		Font_Sprite_Glyph *gptr = get_glyph(text[p]);
		if (gptr)
		{
			spr_glyphs.set_frame(gptr->sprite_index);
			spr_glyphs.set_color(color);
			spr_glyphs.draw(canvas, xpos, ypos - ascent);
			xpos += spr_glyphs.get_frame_size(gptr->sprite_index).width;
		}
		else
		{
			xpos += spacelen;
		}
	}
}

void FontProvider_Sprite::draw_text(Canvas &canvas, float xpos, float ypos, float scale_x, float scale_y, const std::string &text, const Colorf &color)
{
	std::string::size_type string_length = text.length();

	float f_spacelen = spacelen;
	float ascent = font_metrics.get_ascent() * scale_x;

	// Scan the string
	for (std::string::size_type p = 0; p < string_length; p++)
	{
		Font_Sprite_Glyph *gptr = get_glyph(text[p]);
		if (gptr)
		{
			spr_glyphs.set_frame(gptr->sprite_index);
			Size frame_size = spr_glyphs.get_frame_size(gptr->sprite_index);
			float dest_width = scale_x * frame_size.width;
			float dest_height = scale_y * frame_size.height;
			spr_glyphs.set_color(color);
			spr_glyphs.draw(canvas, Rectf(xpos, ypos - ascent, Sizef(dest_width, dest_height)));
			xpos += dest_width;
		}
		else
		{
			xpos += f_spacelen;
		}
	}
}

void FontProvider_Sprite::set_font_metrics(const FontMetrics &metrics)
{
	font_metrics = metrics;
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Sprite Implementation:

void FontProvider_Sprite::setup_glyphs( GraphicContext &gc, const std::string &letters, int new_spacelen, bool monospace)
{
	fixed_width = 0;
	height = 0;

	const int length = letters.size();

	if ((length > spr_glyphs.get_frame_count()) || (length == 0))
	{
		throw Exception(string_format("Font error: Letter characters: %1, Available font glyphs: %2", 
				length,
				spr_glyphs.get_frame_count()));
	}

	glyph_list.reserve(length + glyph_list.size());

	//If monospace font requested, find the width of the widest glyph
	//Then set the fixed_width var to that width, which gwidth() checks for
	//Also set space to that width
	if (monospace)
	{
		for (int i=0; i < length; ++i)
		{
			int glyph_width = spr_glyphs.get_frame_size(i).width;
			if (glyph_width > fixed_width)
				fixed_width = glyph_width;
		}
		
		new_spacelen = fixed_width;
	}
	//If not monospace, and space width not specified, then use average width as space width
	else if (new_spacelen < 0)
	{
		std::string::size_type space_pos = letters.find(' ');
		
		if (space_pos != std::string::npos)
		{
			//If there is a character for space, then use it
			new_spacelen = spr_glyphs.get_frame_size((int)space_pos).width;
		}
		else
		{
			//Make the space size the average of all character sizes
			new_spacelen = 0;
			
			for (int pos = 0; pos < length; ++pos)
			{
				new_spacelen += spr_glyphs.get_frame_size((int)(pos)).width;
			}
			
			new_spacelen /= length;
		}
	}
	
	for (int i=0; i < length; ++i)
	{
		int glyph_height = spr_glyphs.get_frame_size(i).height;
		if (glyph_height > height)
			height = glyph_height;
	}
	
	spr_glyphs.set_alignment(origin_top_left, 0, 0);
	spr_glyphs.set_rotation_hotspot(origin_top_left, 0, 0);
	spacelen = new_spacelen;

	//If monospace font requested, find the width of the widest glyph
	//Then set the fixed_width var to that width, which gwidth() checks for
	if (monospace)
	{
		for (int i=0; i < length; ++i)
		{
			int glyph_width = spr_glyphs.get_frame_size(i).width;
			if (glyph_width > fixed_width)
				fixed_width = glyph_width;
		}
	}	

	// Setup char to glyph map:

	std::string::size_type string_length = letters.length();
	for (std::string::size_type p = 0; p < string_length; p++)
	{
		Font_Sprite_Glyph font_glyph;
		font_glyph.glyph = letters[p];
		font_glyph.sprite_index = p;
		glyph_list.push_back(font_glyph);
	}

	float average_character_width=0.0f;
	float max_character_width=0.0f;

	if (monospace)
	{
		average_character_width = fixed_width;
		max_character_width = fixed_width;
	}
	else
	{
		for (int i=0; i < length; ++i)
		{
			int glyph_width = spr_glyphs.get_frame_size(i).width;
			average_character_width += glyph_width;
			if (glyph_width > max_character_width)
				max_character_width = glyph_width;
		}
		if (length)
			average_character_width /= length;

	}

	font_metrics = FontMetrics(
		height,		// height
		height,		// ascent
		0,			// descent
		0,			// internal_leading
		0,			// external_leading
		average_character_width, // average_character_width
		max_character_width, // max_character_width
		400.0f,		// weight
		0.0f,		// overhang
		96.0f,		// digitized_aspect_x
		96.0f,		// digitized_aspect_y
		false,		// italic
		false,		// underline
		false,		// struck_out
		false);		// fixed_pitch
}

Font_Sprite_Glyph *FontProvider_Sprite::get_glyph(unsigned int glyph)
{
	std::vector< Font_Sprite_Glyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt].glyph == glyph)
			return &(glyph_list[cnt]);
	}
	return NULL;
}

Shape2D FontProvider_Sprite::load_glyph_outline(int c, int &out_advance_x)
{
	throw Exception("Implement Me");
}


}
