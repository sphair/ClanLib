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
**    Magnus Norddahl
*/

#include "XML/precomp.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_family.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_description.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/XML/dom_element.h"
#include "API/Display/2D/canvas.h"
#include "API/XML/Resources/xml_resource_document.h"

namespace clan
{
	Font Font::load(Canvas &canvas, const std::string &family_name, const FontDescription &reference_desc, FontFamily &font_family, const XMLResourceDocument &doc, std::function<Resource<Sprite>(Canvas &, const std::string &)> cb_get_sprite)
	{
		DomElement font_element;
		XMLResourceNode resource;

		resource = doc.get_resource(family_name);
		font_element = resource.get_element();

		DomElement sprite_element = font_element.named_item("sprite").to_element();

		if (!sprite_element.is_null())
		{
			if (!sprite_element.has_attribute("glyphs"))
				throw Exception(string_format("Font resource %1 has no 'glyphs' attribute.", resource.get_name()));

			if (!sprite_element.has_attribute("letters"))
				throw Exception(string_format("Font resource %1 has no 'letters' attribute.", resource.get_name()));

			if (!cb_get_sprite)
				throw Exception(string_format("Font resource %1 requires a sprite loader callback specified.", resource.get_name()));
				
			Resource<Sprite> spr_glyphs = cb_get_sprite(canvas, sprite_element.get_attribute("glyphs"));

			const std::string &letters = sprite_element.get_attribute("letters");

			int spacelen = StringHelp::text_to_int(sprite_element.get_attribute("spacelen", "-1"));
			bool monospace = StringHelp::text_to_bool(sprite_element.get_attribute("monospace", "false"));

			// Modify the default font metrics, if specified

			float height = 0.0f;
			float line_height = 0.0f;
			float ascent = 0.0f;
			float descent = 0.0f;
			float internal_leading = 0.0f;
			float external_leading = 0.0f;

			if (sprite_element.has_attribute("height"))
				height = StringHelp::text_to_float(sprite_element.get_attribute("height", "0"));

			if (sprite_element.has_attribute("line_height"))
				line_height = StringHelp::text_to_float(sprite_element.get_attribute("line_height", "0"));

			if (sprite_element.has_attribute("ascent"))
				ascent = StringHelp::text_to_float(sprite_element.get_attribute("ascent", "0"));

			if (sprite_element.has_attribute("descent"))
				descent = StringHelp::text_to_float(sprite_element.get_attribute("descent", "0"));

			if (sprite_element.has_attribute("internal_leading"))
				internal_leading = StringHelp::text_to_float(sprite_element.get_attribute("internal_leading", "0"));

			if (sprite_element.has_attribute("external_leading"))
				external_leading = StringHelp::text_to_float(sprite_element.get_attribute("external_leading", "0"));

			FontMetrics font_metrics(height, ascent, descent, internal_leading, external_leading, line_height, canvas.get_pixel_ratio());

			font_family.add(canvas, spr_glyphs.get(), letters, spacelen, monospace, font_metrics);

			FontDescription desc = reference_desc.clone();
			return Font(font_family, desc);
		}

		DomElement ttf_element = font_element.named_item("ttf").to_element();
		if (ttf_element.is_null())
			ttf_element = font_element.named_item("freetype").to_element();

		if (!ttf_element.is_null())
		{
			FontDescription desc = reference_desc.clone();

			std::string filename;
			if (ttf_element.has_attribute("file"))
			{
				filename = PathHelp::combine(resource.get_base_path(), ttf_element.get_attribute("file"));
			}

			if (!ttf_element.has_attribute("typeface"))
				throw Exception(string_format("Font resource %1 has no 'typeface' attribute.", resource.get_name()));

			std::string font_typeface_name = ttf_element.get_attribute("typeface");

			if (ttf_element.has_attribute("height"))
				desc.set_height(ttf_element.get_attribute_int("height", 0));

			if (ttf_element.has_attribute("average_width"))
				desc.set_average_width(ttf_element.get_attribute_int("average_width", 0));

			if (ttf_element.has_attribute("anti_alias"))
				desc.set_anti_alias(ttf_element.get_attribute_bool("anti_alias", true));

			if (ttf_element.has_attribute("subpixel"))
				desc.set_subpixel(ttf_element.get_attribute_bool("subpixel", true));

			if (filename.empty())
			{
				font_family.add(font_typeface_name, desc);
				return Font(font_family, desc);
			}
			else
			{
				font_family.add(desc, filename, resource.get_file_system());
				return Font(font_family, desc);
			}
		}

		throw Exception(string_format("Font resource %1 did not have a <sprite> or <ttf> child element", resource.get_name()));
	}
}
