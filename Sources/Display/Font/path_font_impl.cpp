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
#include "path_font_impl.h"

#ifdef __APPLE__
#include "FontEngine/font_engine_cocoa.h"
#elif defined(WIN32)
#include "FontEngine/font_engine_win32.h"
#else
#include "X11/font_engine_freetype.h"
#include "../Display/X11/font_config.h"
#endif

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/path_font.h"
#include "API/Display/2D/path.h"
#include "../2D/render_batch_triangle.h"
#include "../2D/canvas_impl.h"
#include "API/Core/Text/utf8_reader.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PathFont_Impl Construction:

PathFont_Impl::PathFont_Impl() : font_engine(NULL)
{
}

void PathFont_Impl::load_font(const FontDescription &desc, const std::string &filename, FileSystem fs)
{
#ifdef WIN32
	font_engine = new FontEngine_Win32(desc, filename, fs);
#elif defined(__APPLE__)
	font_engine = new FontEngine_Cocoa(desc, filename, fs);
#else
	std::string new_filename = filename;
	if (filename.empty())
	{
		// Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		std::string font_file_path = fc.match_font(desc);
		std::string path = PathHelp::get_fullpath(filename, PathHelp::path_type_file);
		new_filename = PathHelp::get_filename(filename, PathHelp::path_type_file);
		fs = FileSystem(path);
	}

	IODevice io_dev = fs.open_file(new_filename);

	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Ensure width and height are positive
	if (average_width < 0) average_width =-average_width;
	if (height < 0) height =-height;

	font_engine = new FontEngine_Freetype(io_dev, average_width, height);
#endif
	font_metrics = font_engine->get_metrics();
}


PathFont_Impl::~PathFont_Impl()
{
	if (font_engine)
	{
		delete font_engine;
	}
}

/////////////////////////////////////////////////////////////////////////////
// PathFont_Impl Attributes:


FontMetrics PathFont_Impl::get_font_metrics()
{
	return font_metrics;
}

/////////////////////////////////////////////////////////////////////////////
// PathFont_Impl Operations:

GlyphMetrics PathFont_Impl::get_metrics(Canvas &canvas, unsigned int glyph)
{
	store_in_char_cache(glyph);
	return char_cache[glyph].metrics;
}

GlyphMetrics PathFont_Impl::measure_text(Canvas &canvas, const std::string &string)
{
	GlyphMetrics total_metrics;
	total_metrics.bbox_offset.x = std::numeric_limits<float>::max();
	total_metrics.bbox_offset.y = std::numeric_limits<float>::max();

	int line_spacing = static_cast<int>(font_metrics.get_line_height() + 0.5f);
	bool first_char = true;
	Rectf text_bbox;

	UTF8_Reader reader(string.data(), string.length());
	while (!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();

		if (glyph == '\n')
		{
			total_metrics.advance.width = 0;
			total_metrics.advance.height += line_spacing;
			continue;
		}

		GlyphMetrics metrics = get_metrics(canvas, glyph);
		metrics.bbox_offset.x += total_metrics.advance.width;
		metrics.bbox_offset.y += total_metrics.advance.height;

		if (first_char)
		{
			text_bbox = Rectf(metrics.bbox_offset, metrics.bbox_size);
			first_char = false;
		}
		else
		{
			Rectf glyph_bbox(metrics.bbox_offset, metrics.bbox_size);
			text_bbox.bounding_rect(glyph_bbox);
		}

		total_metrics.advance += metrics.advance;
	}

	total_metrics.bbox_offset = text_bbox.get_top_left();
	total_metrics.bbox_size = text_bbox.get_size();
	return total_metrics;
}

void PathFont_Impl::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Brush &brush)
{
	float offset_x = 0;
	float offset_y = 0;

	int line_spacing = font_metrics.get_height() + font_metrics.get_external_leading();

	const Mat4f original_transform = canvas.get_transform();
	UTF8_Reader reader(text.data(), text.length());
	while (!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();
	
		if (glyph == '\n')
		{
			offset_x = 0;
			offset_y += line_spacing;
			continue;
		}

		store_in_char_cache(glyph);

		canvas.set_transform(original_transform * Mat4f::translate(position.x + offset_x, position.y + offset_y, 0));
		char_cache[glyph].path.fill(canvas, brush);
		offset_x += char_cache[glyph].metrics.advance.width;

	}
	canvas.set_transform(original_transform);
}

void PathFont_Impl::store_in_char_cache(unsigned int glyph)
{
	if( char_cache.find(glyph) == char_cache.end() )
	{
		vector_glyph &vg = char_cache[glyph];
		font_engine->load_glyph_path(glyph, vg.path, vg.metrics);
	}
}

/////////////////////////////////////////////////////////////////////////////
// PathFont_Impl Implementation:

}
