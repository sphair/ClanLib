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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#include "Display/precomp.h"
#include "font_impl.h"
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
#include "API/Core/IOData/path_help.h"

#ifdef WIN32
#include "FontEngine/font_engine_win32.h"
#elif defined(__APPLE__)
#include "FontEngine/font_engine_cocoa.h"
#else
#include "X11/font_engine_freetype.h"
#include "X11/font_provider_freetype.h"
#include "../Display/X11/font_config.h"
#endif

#include "../2D/sprite_impl.h"


namespace clan
{

class FontEngine_Sprite : public FontEngine
{
public:
	~FontEngine_Sprite() { }
	FontMetrics get_metrics() {return FontMetrics(); }
	FontPixelBuffer get_font_glyph_standard(int glyph, bool anti_alias) { return FontPixelBuffer(); }
	FontPixelBuffer get_font_glyph_subpixel(int glyph) { return FontPixelBuffer(); }
	Shape2D load_glyph_outline(int glyph_index, int &out_advance_x) { return Shape2D(); }
};

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

void Font_Impl::load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename )
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
	std::string path = PathHelp::get_fullpath(filename, PathHelp::path_type_file);
	std::string new_filename = PathHelp::get_filename(filename, PathHelp::path_type_file);
	FileSystem vfs(path);
	font_engine = new FontEngine_Win32(desc, new_filename, vfs);
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



void Font_Impl::load_font( GraphicContext &context, const FontDescription &desc, const std::string &filename, FileSystem &fs)
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
	font_engine = new FontEngine_Win32(desc, filename, fs);
	glyph_cache.font_metrics = font_engine->get_metrics();
    
#elif defined(__APPLE__)
    
    font_engine = new FontEngine_Cocoa(desc, filename);
    glyph_cache.font_metrics = font_engine->get_metrics();

#else
	IODevice io_dev = fs.open_file(filename);

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

void Font_Impl::load_font( Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics)
{
	free_font();
	font_engine = new FontEngine_Sprite();

	glyph_cache.anti_alias = true;
	glyph_cache.enable_subpixel = false;
	glyph_cache.font_metrics = metrics;

	const int length = StringHelp::utf8_length(glyph_list);

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
		std::string::size_type space_pos = glyph_list.find(' ');
		
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

	UTF8_Reader reader(glyph_list.data(), glyph_list.length());
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
		Point offset(sprite_frame.offset);
		offset.y -= glyph_cache.font_metrics.get_ascent();

		glyph_cache.insert_glyph(canvas, glyph, sub_texture, offset, increment);

		sprite_index++;
	}

	// Did the glyphs not contain a space?
	std::string::size_type space_pos = glyph_list.find(' ');
	if (space_pos == std::string::npos)
	{
		FontPixelBuffer pb;
		pb.empty_buffer = true;
		pb.increment.x = spacelen;
		pb.glyph = ' ';
		glyph_cache.insert_glyph(canvas, pb);

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
			int glyph_width = sprite.get_frame_size(i).width;
			average_character_width += glyph_width;
			if (glyph_width > max_character_width)
				max_character_width = glyph_width;
		}
		if (length)
			average_character_width /= length;

	}
	if (glyph_cache.font_metrics.get_max_character_width() == 0.0f)
	{
		glyph_cache.font_metrics.set_max_character_width(max_character_width);
	}
	if (glyph_cache.font_metrics.get_average_character_width() == 0.0f)
	{
		glyph_cache.font_metrics.set_average_character_width(average_character_width);
	}
	if (glyph_cache.font_metrics.get_height() == 0)
	{
		glyph_cache.font_metrics.set_height(height);
	}
}

}
