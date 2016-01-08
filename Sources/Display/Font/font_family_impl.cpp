/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "font_family_impl.h"
#include "API/Display/Font/font_family.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Core/IOData/path_help.h"
#include "Display/2D/canvas_impl.h"
#include "Display/2D/sprite_impl.h"

#ifdef WIN32
#include "FontEngine/font_engine_win32.h"
#elif defined(__APPLE__)
#include "FontEngine/font_engine_cocoa.h"
#else
#include "FontEngine/font_engine_freetype.h"

#if !defined __ANDROID__
#include "../Platform/X11/font_config.h"
#endif

#endif

namespace clan
{
	class FontEngine_Sprite : public FontEngine
	{
	public:
		FontEngine_Sprite(const FontDescription &desc, FontMetrics &metrics) : font_metrics(metrics) { font_description = desc.clone(); }
		~FontEngine_Sprite() {}

		bool is_automatic_recreation_allowed() const override { return false; }
		const FontMetrics &get_metrics() const override { return font_metrics; }
		FontPixelBuffer get_font_glyph(int glyph) override { return FontPixelBuffer(); }
		void load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics) override {}
		const FontDescription &get_desc() const override { return font_description; }

	private:
		FontDescription font_description;
		FontMetrics font_metrics;
	};

	FontFamily_Impl::FontFamily_Impl(const std::string &family_name) : family_name(family_name), texture_group(Size(256, 256))
	{
	}

	FontFamily_Impl::~FontFamily_Impl()
	{
	}

	void FontFamily_Impl::add(const FontDescription &desc, DataBuffer &font_databuffer)
	{
		FontFamily_Definition definition;
		definition.desc = desc.clone();
		definition.font_databuffer = font_databuffer;
		font_definitions.push_back(definition);
	}

	void FontFamily_Impl::add(const FontDescription &desc, const std::string &typeface_name)
	{
		FontFamily_Definition definition;
		definition.desc = desc.clone();
		definition.typeface_name = typeface_name;
		font_definitions.push_back(definition);
	}

	void FontFamily_Impl::font_face_load(const FontDescription &desc, DataBuffer &font_databuffer, float pixel_ratio)
	{
#if defined(WIN32)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Win32>(desc, font_databuffer, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
#elif defined(__APPLE__)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Cocoa>(desc, font_databuffer, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
#else
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Freetype>(desc, font_databuffer, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
#endif
		font_cache.back().glyph_cache->set_texture_group(texture_group);
		font_cache.back().pixel_ratio = pixel_ratio;
	}

	void FontFamily_Impl::font_face_load(const FontDescription &desc, const std::string &typeface_name, float pixel_ratio)
	{
#if defined(WIN32)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Win32>(desc, typeface_name, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
		font_cache.back().glyph_cache->set_texture_group(texture_group);
		font_cache.back().pixel_ratio = pixel_ratio;
#elif defined(__APPLE__)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Cocoa>(desc, typeface_name, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
		font_cache.back().glyph_cache->set_texture_group(texture_group);
		font_cache.back().pixel_ratio = pixel_ratio;
#elif defined(__ANDROID__)
		throw Exception("automatic typeface to ttf file selection is not supported on android");
#else

		// Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		std::string font_file_path = fc.match_font(typeface_name, desc);
		std::string path = PathHelp::get_fullpath(font_file_path, PathHelp::path_type_file);
		auto filename = PathHelp::get_filename(font_file_path, PathHelp::path_type_file);
		auto fs = FileSystem(path);
		IODevice file = fs.open_file(filename);
		DataBuffer font_databuffer;
		font_databuffer.set_size(file.get_size());
		file.read(font_databuffer.get_data(), font_databuffer.get_size());
		font_face_load(desc, font_databuffer, pixel_ratio);
#endif
	}

	void FontFamily_Impl::font_face_load(Canvas &canvas, Sprite &sprite, const std::string &glyph_list, float spacelen, bool monospace, const FontMetrics &metrics)
	{
		FontMetrics font_metrics = metrics;

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
			for (int i = 0; i < length; ++i)
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
		for (int i = 0; i < length; ++i)
		{
			int glyph_height = sprite.get_frame_size(i).height;
			if (glyph_height > height)
				height = glyph_height;
		}

		if (font_metrics.get_height() == 0)
		{
			font_metrics = FontMetrics(
				height,
				font_metrics.get_ascent(),
				font_metrics.get_descent(),
				font_metrics.get_internal_leading(),
				font_metrics.get_external_leading(),
				font_metrics.get_line_height(),
				canvas.get_pixel_ratio());
		}

		FontDescription desc;
		desc.set_height(height);

		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Sprite>(desc, font_metrics);
		Font_Cache sprite_engine(engine);
		font_cache.push_back(sprite_engine);
		GlyphCache *glyph_cache = sprite_engine.glyph_cache.get();

		// Setup char to glyph map:

		UTF8_Reader reader(glyph_list.data(), glyph_list.length());
		int sprite_index = 0;
		while (!reader.is_end())
		{
			unsigned int glyph = reader.get_char();
			reader.next();

			const Sprite_Impl::SpriteFrame &sprite_frame = sprite.impl->frames[sprite_index];

			Point increment;
			if (fixed_width)
			{
				increment.x = fixed_width;
			}
			else
			{
				increment.x = sprite_frame.position.get_width();
			}
			Pointf offset(sprite_frame.offset);
			offset.y -= font_metrics.get_ascent();

			Sizef size = sprite_frame.position.get_size();

			Subtexture sub_texture(sprite_frame.texture, sprite_frame.position);
			glyph_cache->insert_glyph(canvas, glyph, sub_texture, offset, size, GlyphMetrics(Pointf(offset.x, offset.y), Sizef(increment.x, increment.y), Sizef(increment.x, increment.y)));

			sprite_index++;
		}

		// Did the glyphs not contain a space?
		std::string::size_type space_pos = glyph_list.find(' ');
		if (space_pos == std::string::npos)
		{
			FontPixelBuffer pb;
			pb.empty_buffer = true;
			pb.metrics.advance.width = spacelen;
			pb.metrics.bbox_size.width = spacelen;
			pb.glyph = ' ';
			glyph_cache->insert_glyph(canvas, pb);

		}

		float average_character_width = 0.0f;
		float max_character_width = 0.0f;

		if (monospace)
		{
			average_character_width = fixed_width;
			max_character_width = fixed_width;
		}
		else
		{
			for (int i = 0; i < length; ++i)
			{
				int glyph_width = sprite.get_frame_size(i).width;
				average_character_width += glyph_width;
				if (glyph_width > max_character_width)
					max_character_width = glyph_width;
			}
			if (length)
				average_character_width /= length;

		}

	}

	Font_Cache FontFamily_Impl::get_font(const FontDescription &desc, float pixel_ratio)
	{
		// Find cached version
		for (auto &cache : font_cache)
		{
			if (cache.pixel_ratio != pixel_ratio)
				continue;
			if (desc.get_style() != cache.engine->get_desc().get_style())
				continue;
			if (desc.get_weight() != cache.engine->get_desc().get_weight())
				continue;
			if (desc.get_subpixel() != cache.engine->get_desc().get_subpixel())
				continue;
			if (desc.get_anti_alias() != cache.engine->get_desc().get_anti_alias())
				continue;

			if (cache.engine->is_automatic_recreation_allowed())
			{
				if (desc.get_height() != cache.engine->get_desc().get_height())
					continue;
			}

			return cache;
		}
		return Font_Cache();
	}

	Font_Cache FontFamily_Impl::copy_font(const FontDescription &desc, float pixel_ratio)
	{
		// Find existing typeface, to obtain shared data that we can copy
		FontFamily_Definition font_definition;
		bool found = false;

		// Find find an exact match using style and weight
		for (auto &definitions : font_definitions)
		{
			if (desc.get_style() != definitions.desc.get_style())
				continue;
			if (desc.get_weight() != definitions.desc.get_weight())
				continue;

			font_definition = definitions;
			found = true;
			break;
		}

		// Else find the first font
		if (!found)
		{
			if (!font_definitions.empty())
			{
				font_definition = font_definitions[0];
				found = true;
			}
		}

		if (!found)
		{
			// Could not find a cached version of the font to use as reference
			font_face_load(desc, family_name, pixel_ratio);
		}
		else
		{
			if (!font_definition.font_databuffer.is_null())
			{
				// Cached font is allocated via a font databuffer
				font_face_load(desc, font_definition.font_databuffer, pixel_ratio);
			}
			else
			{
				// Cached font has allocated the typeface_name
				font_face_load(desc, font_definition.typeface_name, pixel_ratio);
			}
		}

		return font_cache.back();
	}
}
