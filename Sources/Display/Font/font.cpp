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
#include "API/Display/Font/font_sprite.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Resources/display_cache.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Font_Impl Class:

class Font_Impl
{
public:
	Font_Impl() : provider(0) { }

	~Font_Impl()
	{
		if (provider)
			delete provider;
	}

	FontProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// Font Construction:

Font::Font()
{
}

Font::Font(Canvas &canvas, const std::string &typeface_name, int height)
: impl(new Font_Impl)
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = Font(canvas, desc);
}

Font::Font( Canvas &canvas, const FontDescription &desc)
: impl(new Font_Impl)
{
	Font_System new_font(canvas, desc);
	*this = new_font;
}

Font::Font(FontProvider *provider)
: impl(new Font_Impl)
{
	impl->provider = provider;
}

Font::Font(const Font &copy)
{
	impl = copy.impl;
}

Font::~Font()
{
}

/////////////////////////////////////////////////////////////////////////////
// Font Resources:

Resource<Font> Font::resource(Canvas &canvas, const FontDescription &desc, const ResourceManager &resources)
{
	return DisplayCache::get(resources).get_font(canvas, desc);
}

/////////////////////////////////////////////////////////////////////////////
// Font Attributes:

bool Font::is_null() const
{
	return !impl;
}

FontProvider *Font::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
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
			get_provider()->draw_text(canvas, dest_x, dest_y, lines[i], color);
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
			Size line_size = get_provider()->get_text_size(gc, lines[i]);

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
		return get_provider()->get_text_size(gc, text);
	}
	return Size();
}

FontMetrics Font::get_font_metrics()
{
	if (impl)
		return get_provider()->get_font_metrics();
	return FontMetrics();
}

int Font::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	if (impl)
		return get_provider()->get_character_index(gc, text, point);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Font Implementation:

}
