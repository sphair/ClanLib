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

#ifdef WIN32
#include "FontEngine/font_engine_win32.h"
#elif defined(__APPLE__)
#include "FontEngine/font_engine_cocoa.h"
#else
#include "X11/font_engine_freetype.h"
#include "X11/font_provider_freetype.h"
#include "../Display/X11/font_config.h"
#endif

#include "glyph_cache.h"
#include "../2D/sprite_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font_Impl Class:

class Font_Impl
{
public:
	Font_Impl();
	~Font_Impl();
	
	void load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename);
	void load_font( Canvas &canvas, Sprite &sprite, const std::string &letters, int spacelen, bool monospace, const FontMetrics &metrics);
	void draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color);
	Size get_text_size(GraphicContext &gc, const std::string &text);
	FontMetrics get_font_metrics();
	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

private:
	void free_font();

	FontEngine *font_engine;
	GlyphCache glyph_cache;
};

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

Font::Font( Canvas &canvas, const FontDescription &desc) : impl(new Font_Impl)
{
	impl->load_font( canvas, desc, "");
}

Font::Font( Canvas &canvas, Sprite &sprite, const std::string &letters, int spacelen, bool monospace, const FontMetrics &metrics) : impl(new Font_Impl)
{
	impl->load_font(canvas, sprite, letters, spacelen, monospace, metrics);
}

/////////////////////////////////////////////////////////////////////////////
// Font Resources:

Resource<Font> Font::resource(Canvas &canvas, const FontDescription &desc, const ResourceManager &resources)
{
	return DisplayCache::get(resources).get_font(canvas, desc);
}


Font Font::load(Canvas &canvas, const std::string &id, const XMLResourceDocument &doc, Callback_2<Resource<Sprite>, GraphicContext &, const std::string &> cb_get_sprite)
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

		return Font(canvas, spr_glyphs.get(), letters, spacelen, monospace, font_metrics);
	}

	DomElement freetype_element = resource.get_element().named_item("ttf").to_element();
	if (freetype_element.is_null())
		freetype_element = resource.get_element().named_item("freetype").to_element();

	if (!freetype_element.is_null())
	{
		FontDescription desc;

		if (freetype_element.has_attribute("file"))
			desc.set_typeface_name(freetype_element.get_attribute("file"));
		else
			throw Exception(string_format("Font resource '%1' has no 'file' attribute", resource.get_name()));

		if (freetype_element.has_attribute("height"))
			desc.set_height(freetype_element.get_attribute_int("height", 0));
		else
			throw Exception(string_format("Font resource '%1' has no 'height' attribute", resource.get_name()));

		if (freetype_element.has_attribute("average_width"))
			desc.set_average_width(freetype_element.get_attribute_int("average_width", 0));

		if (freetype_element.has_attribute("anti_alias"))
			desc.set_anti_alias(freetype_element.get_attribute_bool("anti_alias", true));

		if (freetype_element.has_attribute("subpixel"))
			desc.set_subpixel(freetype_element.get_attribute_bool("subpixel", true));

		return Font(canvas, desc);
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

Font_Impl::Font_Impl() : font_engine(NULL)
{
}

Font_Impl::~Font_Impl()
{
	free_font();
}
	

void Font_Impl::free_font()
{
	if (font_engine)
	{
		delete(font_engine);
		font_engine = NULL;
	}
}

void Font_Impl::load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename)
{
	free_font();

	if (desc.get_subpixel())
	{
		glyph_cache.enable_subpixel = true;
		glyph_cache.anti_alias = true;	// Implies anti_alias is set
	}
	else
	{
		glyph_cache.enable_subpixel = false;
		glyph_cache.anti_alias = desc.get_anti_alias();
	}

#ifdef WIN32
	font_engine = new FontEngine_Win32(desc, filename);
	glyph_cache.font_metrics = font_engine->get_metrics();
    
#elif defined(__APPLE__)
    
    font_engine = new FontEngine_Cocoa(desc, filename);
    glyph_cache.font_metrics = font_engine->get_metrics();

#else
	std::string font_file_path = filename;
	if (font_file_path.empty())
	{
	    // Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		font_file_path = fc.match_font(desc);
	}

	std::string path = PathHelp::get_fullpath(font_file_path, PathHelp::path_type_file);
	std::string new_filename = PathHelp::get_filename(font_file_path, PathHelp::path_type_file);
	FileSystem vfs(path);
	IODevice io_dev = vfs.open_file(new_filename);

	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Ensure width and height are positive
	if (average_width < 0) average_width =-average_width;
	if (height < 0) height =-height;

	font_engine = new FontEngine_Freetype(io_dev, average_width, height);

	glyph_cache.font_metrics = font_engine->get_metrics();

#endif
}

void Font_Impl::draw_text(Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color)
{
	glyph_cache.draw_text(font_engine, canvas, xpos, ypos, text, color);
}

Size Font_Impl::get_text_size(GraphicContext &gc, const std::string &text)
{
	return glyph_cache.get_text_size(font_engine, gc, text);
}

FontMetrics Font_Impl::get_font_metrics()
{
	return glyph_cache.get_font_metrics();
}

int Font_Impl::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	return glyph_cache.get_character_index(font_engine, gc, text, point);
}

void Font_Impl::load_font( Canvas &canvas, Sprite &sprite, const std::string &letters, int spacelen, bool monospace, const FontMetrics &metrics)
{
	glyph_cache.anti_alias = true;
	glyph_cache.enable_subpixel = false;
	glyph_cache.font_metrics = metrics;
	FontMetrics font_metrics;

	const int length = StringHelp::utf8_length(letters);

	if ((length > sprite.get_frame_count()) || (length == 0))
	{
		throw Exception(string_format("Font error: Letter characters: %1, Available font glyphs: %2", 
				length,
				sprite.get_frame_count()));
	}

	//If monospace font requested, find the width of the widest glyph
	//Then set the fixed_width var to that width
	//Also set space to that width, if unset
	int fixed_width = 0;
	if (monospace)
	{
		for (int i=0; i < length; ++i)
		{
			int glyph_width = sprite.get_frame_size(i).width;
			if (glyph_width > fixed_width)
				fixed_width = glyph_width;
		}
		
		if (spacelen)
			spacelen = fixed_width;
	}

	//If not monospace, and space width not specified, then use average width as space width
	else if (spacelen <= 0)
	{
		std::string::size_type space_pos = letters.find(' ');
		
		if (space_pos != std::string::npos)
		{
			//If there is a character for space, then use it
			spacelen = sprite.get_frame_size((int)space_pos).width;
		}
		else
		{
			//Make the space size the average of all character sizes
			spacelen = 0;
			
			for (int pos = 0; pos < length; ++pos)
			{
				spacelen += sprite.get_frame_size((int)(pos)).width;
			}
			
			spacelen /= length;
		}
	}
	
	int height = 0;
	for (int i=0; i < length; ++i)
	{
		int glyph_height = sprite.get_frame_size(i).height;
		if (glyph_height > height)
			height = glyph_height;
	}
	
	// Setup char to glyph map:

	UTF8_Reader reader(letters.data(), letters.length());
	int sprite_index = 0;
	while(!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();
		
		const Sprite_Impl::SpriteFrame &sprite_frame = sprite.impl->frames[sprite_index];

		Subtexture sub_texture(sprite_frame.texture, sprite_frame.position);

		Point increment;
		if (fixed_width)
		{
			increment.x = fixed_width;
		}
		else
		{
			increment.x = sprite_frame.position.get_width();
		}

		glyph_cache.insert_glyph(canvas, glyph, sub_texture, sprite_frame.offset, increment);

		sprite_index++;
	}

	// Did the letter not contain spaces?
	std::string::size_type space_pos = letters.find(' ');
	if (space_pos != std::string::npos)
	{
		FontPixelBuffer pb;
		pb.empty_buffer = true;
		pb.increment.x = spacelen;
		pb.glyph = ' ';
		glyph_cache.insert_glyph(canvas, pb);

	}

}

}
