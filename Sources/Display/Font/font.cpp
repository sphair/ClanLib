/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Resources/display_cache.h"
#include "font_impl.h"

namespace clan
{
	Font::Font()
	{
	}

	Font::Font(FontFamily &font_family, float height)
	{
		font_family.throw_if_null();
		FontDescription desc;
		desc.set_height(height);
		*this = Font(font_family, desc);
	}

	Font::Font(FontFamily &font_family, const FontDescription &desc)
	{
		impl = std::make_shared<Font_Impl>(font_family, desc);
	}

	Font::Font(const std::string &typeface_name, float height)
	{
		FontDescription desc;
		desc.set_height(height);
		*this = Font(typeface_name, desc);
	}

	Font::Font(const std::string &typeface_name, const FontDescription &desc)
	{
		FontFamily font_family(typeface_name);
		*this = Font(font_family, desc);
	}

	Font::Font(const FontDescription &desc, const std::string &ttf_filename)
	{
		std::string path = PathHelp::get_fullpath(ttf_filename, PathHelp::path_type_file);
		std::string new_filename = PathHelp::get_filename(ttf_filename, PathHelp::path_type_file);
		FileSystem vfs(path);

		FontFamily font_family(new_filename);
		font_family.add(desc, new_filename, vfs);
		impl = std::make_shared<Font_Impl>(font_family, desc);
	}

	Font::Font(const FontDescription &desc, const std::string &ttf_filename, FileSystem fs)
	{
		std::string new_filename = PathHelp::get_filename(ttf_filename, PathHelp::path_type_file);
		FontFamily font_family(new_filename);
		font_family.add(desc, ttf_filename, fs);
		impl = std::make_shared<Font_Impl>(font_family, desc);
	}

	Font::Font(Canvas &canvas, const std::string &typeface_name, Sprite &sprite, const std::string &glyph_list, float spacelen, bool monospace, const FontMetrics &metrics)
	{
		FontDescription desc;
		desc.set_height(metrics.get_height());

		FontFamily font_family(typeface_name);
		font_family.add(canvas, sprite, glyph_list, spacelen, monospace, metrics);
		impl = std::make_shared<Font_Impl>(font_family, desc);
	}

	Resource<Font> Font::resource(Canvas &canvas, const std::string &family_name, const FontDescription &desc, const ResourceManager &resources)
	{
		return DisplayCache::get(resources).get_font(canvas, family_name, desc);
	}

	void Font::throw_if_null() const
	{
		if (!impl)
			throw Exception("Font is null");
	}

	void Font::set_height(float value)
	{
		if (impl)
			impl->set_height(value);
	}

	void Font::set_weight(FontWeight value)
	{
		if (impl)
			impl->set_weight(value);
	}

	void Font::set_line_height(float height)
	{
		if (impl)
			impl->set_line_height(height);
	}
	void Font::set_style(FontStyle style)
	{
		if (impl)
			impl->set_style(style);
	}

	void Font::set_scalable(float height_threshold)
	{
		if (impl)
			impl->set_scalable(height_threshold);
	}

	GlyphMetrics Font::get_metrics(Canvas &canvas, unsigned int glyph)
	{
		if (impl)
			return impl->get_metrics(canvas, glyph);
		return GlyphMetrics();
	}

	GlyphMetrics Font::measure_text(Canvas &canvas, const std::string &string)
	{
		if (impl)
			return impl->measure_text(canvas, string);
		return GlyphMetrics();
	}

	size_t Font::clip_from_left(Canvas &canvas, const std::string &text, float width)
	{
		float x = 0.0f;
		UTF8_Reader reader(text.data(), text.length());
		while (!reader.is_end())
		{
			unsigned int glyph = reader.get_char();
			GlyphMetrics char_abc = get_metrics(canvas, glyph);

			if (x + char_abc.advance.width > width)
				return reader.get_position();

			x += char_abc.advance.width;
			reader.next();
		}

		return text.size();
	}

	size_t Font::clip_from_right(Canvas &canvas, const std::string &text, float width)
	{
		float x = 0.0f;
		UTF8_Reader reader(text.data(), text.length());
		reader.set_position(text.length());
		while (reader.get_position() != 0)
		{
			reader.prev();

			unsigned int glyph = reader.get_char();
			GlyphMetrics char_abc = get_metrics(canvas, glyph);

			if (x + char_abc.advance.width > width)
			{
				reader.next();
				return reader.get_position();
			}

			x += char_abc.advance.width;
		}

		return 0;
	}

	void Font::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color)
	{
		if (impl)
		{
			impl->draw_text(canvas, position, text, color);
		}
	}

	std::string Font::get_clipped_text(Canvas &canvas, const Sizef &box_size, const std::string &text, const std::string &ellipsis_text)
	{
		std::string out_string;
		out_string.reserve(text.length());

		if (impl)
		{
			Pointf pos;

			FontMetrics fm = get_font_metrics(canvas);
			float descent = fm.get_descent();
			float line_spacing = fm.get_line_height();
			std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
			for (std::vector<std::string>::size_type i = 0; i < lines.size(); i++)
			{
				if (i == 0 || pos.y + descent < box_size.height)
				{
					Sizef size = measure_text(canvas, lines[i]).bbox_size;
					if (pos.x + size.width <= box_size.width)
					{
						if (!out_string.empty())
							out_string += "\n";
						out_string += lines[i];
					}
					else
					{
						Sizef ellipsis = measure_text(canvas, ellipsis_text).bbox_size;

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
							if (seek_center == seek_end)
								break;

							utf8_reader.set_position(seek_start);
							utf8_reader.next();
							if (utf8_reader.get_position() == seek_end)
								break;

							Sizef text_size = measure_text(canvas, lines[i].substr(0, seek_center)).bbox_size;

							if (pos.x + text_size.width + ellipsis.width >= box_size.width)
								seek_end = seek_center;
							else
								seek_start = seek_center;
							seek_center = (seek_start + seek_end) / 2;
						}

						if (!out_string.empty())
							out_string += "\n";
						out_string += lines[i].substr(0, seek_center) + ellipsis_text;
					}
					pos.y += line_spacing;
				}
			}
		}
		return out_string;
	}

	FontMetrics Font::get_font_metrics(Canvas &canvas)
	{
		if (impl)
			return impl->get_font_metrics(canvas);
		return FontMetrics();
	}

	int Font::get_character_index(Canvas &canvas, const std::string &text, const Pointf &point)
	{
		if (impl)
			return impl->get_character_index(canvas, text, point);
		return 0;
	}

	std::vector<Rectf> Font::get_character_indices(Canvas &canvas, const std::string &text)
	{
		if (impl)
			return impl->get_character_indices(canvas, text);
		return std::vector<Rectf>();
	}

	FontHandle *Font::get_handle(Canvas &canvas)
	{
		if (impl)
			return impl->get_handle(canvas);
		return nullptr;
	}

	FontHandle::~FontHandle()
	{
	}
}
